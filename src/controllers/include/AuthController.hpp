#pragma once
#include <drogon/HttpController.h>
#include "models/User.hpp"
#include <jwt-cpp/jwt.h>

class AuthController : public drogon::HttpController<AuthController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AuthController::registerUser, "/api/register", drogon::Post);
    ADD_METHOD_TO(AuthController::login, "/api/login", drogon::Post);
    METHOD_LIST_END

    void registerUser(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void login(const drogon::HttpRequestPtr& req,
              std::function<void(const drogon::HttpResponsePtr&)>&& callback);

private:
    const std::string JWT_SECRET = "your-secret-key"; // In production, load from config
    std::string generateToken(const std::string& username);
};