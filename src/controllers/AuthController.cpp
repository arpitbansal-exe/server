#include "include/AuthController.hpp"
#include <bcrypt/BCrypt.hpp>

void AuthController::registerUser(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) 
{
    auto json = req->getJsonObject();
    if (!json) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse({
            {"error", "Invalid JSON"}
        });
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string username = (*json)["username"].asString();
    std::string password = (*json)["password"].asString();

    if (username.empty() || password.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse({
            {"error", "Username and password required"}
        });
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    try {
        auto clientPtr = drogon::app().getDbClient();
        auto user = User(username, password);

        clientPtr->execSqlSync(
            "INSERT INTO users (username, password_hash) VALUES (?, ?)",
            user.getUsername(),
            user.getPasswordHash()
        );

        auto resp = drogon::HttpResponse::newHttpJsonResponse({
            {"message", "User registered successfully"}
        });
        resp->setStatusCode(drogon::k201Created);
        callback(resp);
    } catch (const drogon::orm::DrogonDbException& e) {
        if (std::string(e.base().what()).find("UNIQUE constraint failed") != std::string::npos) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse({
                {"error", "Username already exists"}
            });
            resp->setStatusCode(drogon::k409Conflict);
            callback(resp);
        } else {
            auto resp = drogon::HttpResponse::newHttpJsonResponse({
                {"error", "Database error"}
            });
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }
    }
}

void AuthController::login(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) 
{
    auto json = req->getJsonObject();
    if (!json) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse({
            {"error", "Invalid JSON"}
        });
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string username = (*json)["username"].asString();
    std::string password = (*json)["password"].asString();

    try {
        auto clientPtr = drogon::app().getDbClient();
        auto result = clientPtr->execSqlSync(
            "SELECT password_hash FROM users WHERE username = ?",
            username
        );

        if (result.empty()) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse({
                {"error", "Invalid credentials"}
            });
            resp->setStatusCode(drogon::k401Unauthorized);
            callback(resp);
            return;
        }

        std::string storedHash = result[0]["password_hash"].as<std::string>();

        if (BCrypt::validatePassword(password, storedHash)) {
            std::string token = generateToken(username);
            auto resp = drogon::HttpResponse::newHttpJsonResponse({
                {"token", token}
            });
            callback(resp);
        } else {
            auto resp = drogon::HttpResponse::newHttpJsonResponse({
                {"error", "Invalid credentials"}
            });
            resp->setStatusCode(drogon::k401Unauthorized);
            callback(resp);
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse({
            {"error", "Database error"}
        });
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }
}

std::string AuthController::generateToken(const std::string& username) {
    return jwt::create()
        .set_issuer("auth_server")
        .set_type("JWS")
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
        .set_payload_claim("username", jwt::claim(username))
        .sign(jwt::algorithm::hs256{JWT_SECRET});
}