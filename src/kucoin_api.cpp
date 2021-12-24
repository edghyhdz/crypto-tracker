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

    return this->process_token_price_response(read_buffer);
}

/**
 * Check whether request limit has been exceeded.
 * @returns true if request has been exceeded.
 */
bool yact::KucoinAPI::has_reached_request_limit() { return false; }

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

