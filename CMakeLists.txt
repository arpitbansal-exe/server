cmake_minimum_required(VERSION 3.5)
project(simple_server CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

# Find required packages
find_package(Drogon REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(jwt-cpp REQUIRED)

# Set bcrypt directory - replace with your actual path to bcrypt-cpp
set(BCRYPT_INCLUDE_DIR "/usr/local/include/bcrypt")
include_directories(${BCRYPT_INCLUDE_DIR})

# Create executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Drogon::Drogon
    jwt-cpp::jwt-cpp
    bcrypt
)

configure_file(${PROJECT_SOURCE_DIR}/config/config.json 
              ${PROJECT_BINARY_DIR}/config.json COPYONLY)