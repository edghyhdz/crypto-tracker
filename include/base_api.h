/*
 * Base class for Exchange API's
 */

#ifndef API_EXCHANGE_BASE
#define API_EXCHANGE_BASE

#include <map>

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
    
    // Handle by derived class - check whether requests can proceed or not
    virtual bool has_reached_request_limit() = 0;

    // Normal behavior methods
    virtual int get_id() { return _id; }
    virtual std::string get_api_key() { return _api_key; }

    virtual std::string get_api_name() { return _api_name; }
    virtual void set_id(int id) { this->_id = id; }

    // Setter & getter http code - used to check for Exchange API limits
    virtual void set_http_code(int http_code) { this->_http_code = http_code; }
    virtual int get_http_code() { return this->_http_code; }
    
    // http error counter - check if limit has been exceeded several times
    // consecutively
    virtual void increase_http_limit_counter() { this->_http_limit_counter++; }
    virtual void reset_http_limit_counter() { this->_http_limit_counter = 0; }
    virtual int get_http_limit_counter() { return this->_http_limit_counter; }
   
   protected:
    virtual void _get_request(long *http_code, std::string *read_buffer, std::string end_point) = 0;
    std::string _api_name;
    int _id;

   private:
    std::map<std::string, double> _processed_response;
    int _http_code{200};
    int _http_limit_counter{0};
    std::string _api_key;
};

#endif
