#include "binance_api.h"
#include <curl/curl.h>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include "log.h"

/**
 * Class constructor
 */
yact::BinanceAPI::BinanceAPI() {
    this->_api_name = "binance";
    LOG_MESSAGE(level::Info, "Binance constructor");
}

/**
 * Class destructor
 */
yact::BinanceAPI::~BinanceAPI() {}

/**
 * Process get request response into json format
 *
 * @params[in] response
 *  Get request response
 */
std::map<std::string, double> yact::BinanceAPI::process_token_price_response(
    std::string &read_buffer) {
    // Change keys to symbol name

    Response res = spotify::json::decode<Response>(read_buffer);

    std::map<std::string, double> processed_response;
    for (auto item : res.response) {
        processed_response.insert(
            std::make_pair(item.at("symbol"), std::stod(item.at("price"))));
    }

    return processed_response;
}

/**
 * Parses resonse into a format that can be used as an input by spotify-json
 * decoder. Retrieves used weight from Binance API response in the following
 * form
 *
 * x-mbx-used-weight -> current transaction used weight
 * x-mbx-used-weight-1m -> cummulative used weight in 1 minute interval
 *
 * Separates response headers from response body
 * Response body has the following form
 * [{"symbol":"BTCUSD","price":"0.00003289"}, ..., {"symbol": "x", "price":"y"}]
 *
 * @params[in] response
 *  get-request's response to parse
 */
void yact::BinanceAPI::parse_response(std::string &response) {
    std::istringstream resp(response);
    std::string header, complete_header, response_body;
    std::size_t index;

    // Max request per minute in case request does not include that information
    this->set_current_weight(yact::WEIGHT_LIMIT);

    // Find used weight on the response headers
    while (std::getline(resp, header) && header != "\r") {
        complete_header += header + "\n";
        index = header.find(':', 0);
        if (index != std::string::npos) {
            if (boost::algorithm::trim_copy(header.substr(0, index)) ==
                "x-mbx-used-weight-1m") {
                this->set_current_weight(stoi(
                    boost::algorithm::trim_copy(header.substr(index + 1))));
            }
        }
    }

    // Get response headers out of the readBuffer string response
    size_t p = -1;
    std::string temp_word = complete_header + " \n";
    while ((p = response.find(complete_header)) != std::string::npos) {
        response.replace(p, temp_word.length(), "");
    }

    // Add `response` key
    response = "{\"response\": " + response + "}";
}

/*
 * Get symbol price data of all tokens on Exchange
 */
std::map<std::string, double> yact::BinanceAPI::get_data() {
    long http_code;
    std::string read_buffer;

    // request API data
    this->_get_request(&http_code, &read_buffer, BINANCE_ENDPOINT);

    this->set_http_code(http_code);
    
    // Check if request limit has not been exceeded
    if (this->has_reached_request_limit()) {
        LOG_MESSAGE(level::Error, "Request code: " + std::to_string(http_code) +
                                      ". Response:" + read_buffer);
        return std::map<std::string, double>();
    }

    // Parse response into json-spotify readable format
    this->parse_response(read_buffer);

    LOG(level::Info) << "CURRENT USED WEIGHT: " << this->_current_weight << std::endl;

    // Return a key/value pair map where the key is the token symbol and the
    // value is the token price. It will later saved on the db at the
    // DataManager class level
    return this->process_token_price_response(read_buffer);
}

/**
 * Checks if requests to the API have exceeded its limit
 *
 * @returns true if limit has been exceeded
 */
bool yact::BinanceAPI::has_reached_request_limit() {
    // If http_limit_counter has been exceeded 3 times in a row
    // API has to be checked manually
    if (this->get_http_limit_counter() == 3) return true;

    // Check if weight limit is about to be exceeded, if so sleep thread for a
    // minute
    if (this->get_current_weight() >= yact::WEIGHT_LIMIT) {
        LOG_MESSAGE(level::Warning, "Request weight about to be exceeded");
        std::this_thread::sleep_for(std::chrono::minutes(1));
        return false;
    }

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

    return true;
}

/**
 * Get request to unique Binance endpoint
 *
 * @params[in, out] http_code
 *  request response code, 200 successfull request etc
 * @params[in, out] read_buffer
 *  Response text
 * @return none
 */
void yact::BinanceAPI::_get_request(long *http_code, std::string *read_buffer,
                                    std::string end_point) {
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, end_point.c_str());
    // Used to write response header data back - debug purposes
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, read_buffer);
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_code);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

/**
 * Set Binance API current response weight.
 *
 * @params[in] c_weight
 *  current weight to set to `_current_weight`
 */
void yact::BinanceAPI::set_current_weight(int c_weight) {
    this->_current_weight = c_weight;
}

/**
 * Get Binance API's current response weight
 */
int yact::BinanceAPI::get_current_weight() { return this->_current_weight; }
