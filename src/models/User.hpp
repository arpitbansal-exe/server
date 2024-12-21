#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <string>
#include <json/json.h>

class User {
public:
    User() = default;
    User(const std::string& username, const std::string& password);

    std::string getUsername() const { return username_; }
    std::string getPasswordHash() const { return password_hash_; }
    Json::Value toJson() const;

private:
    std::string username_;
    std::string password_hash_;
};