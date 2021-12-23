/// Kucoin Exchange API class definition

#include "kucoin_api.h"
#include <curl/curl.h>
#include <boost/algorithm/string.hpp>
#include <sstream>

// Callback to curl function
// Reference https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
// size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp) {
//    ((std::string *)userp)->append((char *)contents, size * nmemb);
//    return size * nmemb;
// }

yact::KucoinAPI::KucoinAPI(int id) {
    this->set_id(id);
    this->_mtx = new std::mutex();
}

/*
 * Class destructor
 */
yact::KucoinAPI::~KucoinAPI(){
    delete this->_mtx;
}


/**
 * Process get request response into json format
 *
 * @params[in] response
 *  Get request response
 */
std::map<std::string, double> yact::KucoinAPI::process_token_price_response(
    std::string &read_buffer) {
    // Change keys to symbol name

    yact::KuCoin::Response res = spotify::json::decode<yact::KuCoin::Response>(read_buffer);

    std::map<std::string, double> processed_response;
    for (auto item : res.response) {
        processed_response.insert(
            std::make_pair(item.at("symbol"), std::stod(item.at("price"))));
    }

    return processed_response;
}

/**/
std::map<std::string, double> yact::KucoinAPI::get_data(){
    long http_code;
    std::string read_buffer;
    
    std::map<std::string, double> delete_map; 
    // TODO: delete ^

    // Request token price data
    this->_get_request(&http_code, &read_buffer, yact::KuCoin::KUCOIN_TICKER_ENDPOINT);
    std::cout << "KuCoin response:\n" << read_buffer << std::endl;
    
    return delete_map;
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
void yact::KucoinAPI::_get_request(long *http_code, std::string *read_buffer, std::string end_point){
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
}


