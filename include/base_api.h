/*
 * Base class for Exchange API's
 */

#ifndef API_EXCHANGE_BASE
#define API_EXCHANGE_BASE

#include <map>
#include <string>

class BaseAPIExchange {
   public:
    // Do get request and get Token/coin price data
    virtual std::string get_data() = 0;
    // Process get response 
    virtual std::string process_response(std::string &response) = 0;

    // Normal behavior methods
    virtual int get_id() { return _id; }
    virtual std::string get_api_key() { return _api_key; }

   private:
    std::map<std::string, double> _processed_response;
    int _id;
    std::string _api_key;
};

#endif
