#include "binance_api.h"
#include <curl/curl.h>

// Callback to curl function
// Reference https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}
BinanceAPI::BinanceAPI(int id) { this->set_id(id); }

std::string BinanceAPI::process_response(std::string &response) {
    return response + " extra text";
}

std::string BinanceAPI::get_data() { 
    long http_code;
    std::string read_buffer;
    this->_get_request(&http_code, &read_buffer);    

    std::cout << "This is the exit code: " << std::to_string(http_code) << std::endl;
    
    return read_buffer; 
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
void BinanceAPI::_get_request(long *http_code, std::string *read_buffer) {
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

