cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 17)

project(APILoadTestApp)

add_compile_options(-lcurl -Wextra -Wall -Werror -pedantic-errors)

if(WIN32)
    add_definitions(-DCURL_STATICLIB)
    link_directories("${CMAKE_SOURCE_DIR}/thirdparty/openssl/" "${CMAKE_SOURCE_DIR}/thirdparty/curl_win32")
else()
    link_directories("${CMAKE_SOURCE_DIR}/thirdparty/curl2")
endif()

add_executable(${PROJECT_NAME} main.cpp APILoadTester.cpp MultiCurlHandler.cpp)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} openssl curl)
else()
    target_link_libraries(${PROJECT_NAME} curl)
endif()
