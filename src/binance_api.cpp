#include "binance_api.h"
#include <curl/curl.h>
#include <boost/algorithm/string.hpp>
#include <sstream>

// Callback to curl function
// Reference https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

yact::BinanceAPI::BinanceAPI(int id) {
    this->set_id(id); 
    this->_mtx = new std::mutex();
}

/**
 * Process get request response into json format
 *
 * @params[in] response
 *  Get request response
 */
std::map<std::string, double> yact::BinanceAPI::process_token_price_response(
    Response &res) {
    // Change keys to symbol name

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
    this->_current_weight = 1200;

    // Find used weight on the response headers
    while (std::getline(resp, header) && header != "\r") {
        complete_header += header + "\n";
        index = header.find(':', 0);
        if (index != std::string::npos) {
            if (boost::algorithm::trim_copy(header.substr(0, index)) ==
                "x-mbx-used-weight-1m") {
                this->set_current_weight(                    
                    stoi(boost::algorithm::trim_copy(header.substr(index + 1))));
 //               this->_current_weight =
                // this->_requestWeight->setCurrentWeight(_currentWeight);
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

std::map<std::string, double> yact::BinanceAPI::get_data() {
    long http_code;
    std::string read_buffer;
    // request API data
    this->_get_request(&http_code, &read_buffer);

    // Parse response into json-spotify readable format
    this->parse_response(read_buffer);

    // Parse it as a Response struct
    Response res = spotify::json::decode<Response>(read_buffer);

    // It will later saved on the db at the DataManager class level
    std::cout << "CURRENT USED WEIGHT: " << this->_current_weight << std::endl;

    std::cout << "This is the request code: " << std::to_string(http_code)
              << std::endl;
    // Return a key/value pair map where key is the token symbol and the value
    // is the token price
    return this->process_token_price_response(res);
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
void yact::BinanceAPI::_get_request(long *http_code, std::string *read_buffer) {
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, BINANCE_ENDPOINT);
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
void yact::BinanceAPI::set_current_weight(int c_weight){
    std::lock_guard<std::mutex>(*this->_mtx);
    this->_current_weight = c_weight;
}

/**
 * Get Binance API's current response weight
 */
int yact::BinanceAPI::get_current_weight(){
    std::lock_guard<std::mutex>(*this->_mtx);
    return this->_current_weight;
}
