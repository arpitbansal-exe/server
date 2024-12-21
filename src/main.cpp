#include <drogon/drogon.h>
#include "controllers/include/SimpleController.hpp"

int main() {
    // Load config file
    drogon::app().loadConfigFile("config.json");

    // Enable CORS
    drogon::app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr&, const drogon::HttpResponsePtr& resp) {
            resp->addHeader("Access-Control-Allow-Origin", "*");
            resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
        });

    LOG_INFO << "Server starting...";
    drogon::app().run();
    return 0;
}