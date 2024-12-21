#include "User.hpp"
#include <bcrypt/BCrypt.hpp>

User::User(const std::string& username, const std::string& password) 
    : username_(username) {
    password_hash_ = BCrypt::generateHash(password);
}

Json::Value User::toJson() const {
    Json::Value json;
    json["username"] = username_;
    return json;
}