#pragma once
#include <drogon/HttpController.h>

class SimpleController : public drogon::HttpController<SimpleController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SimpleController::hello, "/hello", drogon::Get);
    ADD_METHOD_TO(SimpleController::jsonEndpoint, "/json", drogon::Get);
    ADD_METHOD_TO(SimpleController::echo, "/echo", drogon::Post);
    METHOD_LIST_END

    void hello(const drogon::HttpRequestPtr& req,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback);
               
    void jsonEndpoint(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback);
                     
    void echo(const drogon::HttpRequestPtr& req,
              std::function<void(const drogon::HttpResponsePtr&)>&& callback);
};
