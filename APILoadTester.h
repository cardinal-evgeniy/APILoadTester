#pragma once

#include <unordered_map>
#include <memory>

#include <curl/curl.h>

#include "MultiCurlHandler.h"


struct APILoadTestResult
{
    std::unordered_map<size_t, size_t> codesMap; // code -> count
    double time { 0. };
};

class APILoadTester
{
public:
    APILoadTester() = delete;
    APILoadTester( const std::string& url, const size_t requestsCount, const size_t maxParallelRequestsCount );
    APILoadTester( const APILoadTester& ) = delete;
    APILoadTester& operator=( const APILoadTester& ) = delete;
    APILoadTester( APILoadTester&& ) = delete;
    APILoadTester& operator=( APILoadTester&& ) = delete;
    ~APILoadTester() noexcept { curl_global_cleanup(); }

    void run() const;
    auto getResult() const { return result_; };

private:
    const std::string& url_;
    const size_t requestsCount_;
    const size_t maxParallelRequestsCount_;

    std::unique_ptr<MultiCurlHandler> mcHandler_;
    mutable APILoadTestResult result_;

    void checkInput_() const;
};
