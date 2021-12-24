/// Kucoin Exchange API class definition

#include "kucoin_api.h"
#include <curl/curl.h>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include "log.h"

/**
 * Class constructor
 */
yact::KucoinAPI::KucoinAPI() { this->_api_name = "kucoin"; }

/*
 * Class destructor
 */
yact::KucoinAPI::~KucoinAPI() {}

/**
 * Process get request response into json format
 *
 * @params[in] read_buffer
 *  Get request response
 */
std::map<std::string, double> yact::KucoinAPI::process_token_price_response(
    std::string &read_buffer) {
    // Change keys to symbol name
    yact::KuCoin::Response res =
        spotify::json::decode<yact::KuCoin::Response>(read_buffer);

    std::map<std::string, double> processed_response;

    for (auto item : res.data.ticker) {
        processed_response.insert(
            std::make_pair(item.symbol, std::stod(item.last)));
    }

    return processed_response;
}

/**
 * Get symbol price data of all tokens on Exchange
 */
std::map<std::string, double> yact::KucoinAPI::get_data() {
    long http_code;
    std::string read_buffer;

    // Request token price data
    this->_get_request(&http_code, &read_buffer,
                       yact::KuCoin::KUCOIN_TICKER_ENDPOINT);

    this->set_http_code(http_code);
    LOG(level::Info) << "Current request code KuCoin: " << this->get_http_code()
                     << std::endl;

    if (this->has_reached_request_limit()) {
        LOG_MESSAGE(level::Error, "Request code: " + std::to_string(http_code) +
                                      ". Response: " + read_buffer);
        return std::map<std::string, double>();
    }

    return this->process_token_price_response(read_buffer);
}

/**
 * Check whether request limit has been exceeded.
 * @returns true if request has been exceeded.
 */
bool yact::KucoinAPI::has_reached_request_limit() { 
    
    // If http_limit_counter has been exceeded 3 times in a row
    // API has to be checked manually
    if (this->get_http_limit_counter() == 3) return true;

    // Check first if http code is 200 else sleep for 5 min
    if (this->get_http_code() == 200) {
        this->reset_http_limit_counter();
        return false;
    }

    // We have a different 200 code
    this->increase_http_limit_counter();
    
    // Should log only once
    if (this->get_http_limit_counter() == 3){
        LOG_MESSAGE(level::Error, "Error counter exceeded, needs manual check");
    }

    LOG_MESSAGE(level::Info, "Waiting 6 minutes to enable API back");
    // Sleep for 6 minutes to stop doing request to API
    std::this_thread::sleep_for(std::chrono::minutes(6));

    return false; 
}

/**
 * Get request to unique KuCoin endpoint
 *
 * @params[in, out] http_code
 *  request response code, 200 successfull request etc
 * @params[in, out] read_buffer
 *  Response text
 * @return none
 */
void yact::KucoinAPI::_get_request(long *http_code, std::string *read_buffer,
                                   std::string end_point) {
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, end_point.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, read_buffer);
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_code);
    curl_easy_cleanup(curl);
}

