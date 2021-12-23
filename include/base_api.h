/*
 * Base class for Exchange API's
 */

#ifndef API_EXCHANGE_BASE
#define API_EXCHANGE_BASE

#include <map>

// Forward struct declaration
// struct Response;

// Callback to curl function
// Reference https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
inline size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

class BaseAPIExchange {
   public:
    virtual ~BaseAPIExchange(){};

    // Do get request and get Token/coin price data
    virtual std::map<std::string, double> get_data() = 0;
    // Process get response
    virtual std::map<std::string, double> process_token_price_response(std::string &response) = 0;

    // Normal behavior methods
    virtual int get_id() { return _id; }
    virtual std::string get_api_key() { return _api_key; }

    virtual void set_id(int id) { this->_id = id; }
    // virtual void _get_request(long *http_code, std::string *read_buffer, std::string end_point);
   protected:
    virtual void _get_request(long *http_code, std::string *read_buffer, std::string end_point) = 0;
    int _id;

   private:
    std::map<std::string, double> _processed_response;
    std::string _api_key;
};

#endif
