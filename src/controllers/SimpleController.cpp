// src/controllers/SimpleController.cpp
#include "include/SimpleController.hpp"
#include <ctime>
void SimpleController::hello(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) 
{
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("Hello, World!");
    callback(resp);
}

void SimpleController::jsonEndpoint(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) 
{
    Json::Value response;
    response["message"] = "Hello from JSON endpoint";
    response["timestamp"] = std::time(nullptr);
    response["status"] = "success";

    auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

void SimpleController::echo(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) 
{
    auto jsonBody = req->getJsonObject();
    if (!jsonBody) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse({
            {"error", "Invalid JSON"}
        });
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    auto resp = drogon::HttpResponse::newHttpJsonResponse(*jsonBody);
    callback(resp);
}