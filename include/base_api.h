/*
 * Base class for Exchange API's
 */

#ifndef API_EXCHANGE_BASE
#define API_EXCHANGE_BASE

#include <map>

// Forward struct declaration
struct Response;

class BaseAPIExchange {
   public:
    virtual ~BaseAPIExchange(){};

    // Do get request and get Token/coin price data
    virtual std::string get_data() = 0;
    // Process get response
    virtual std::map<std::string, double> process_token_price_response(Response &response) = 0;

    // Normal behavior methods
    virtual int get_id() { return _id; }
    virtual std::string get_api_key() { return _api_key; }

    virtual void set_id(int id) { this->_id = id; }

   protected:
    int _id;

   private:
    std::map<std::string, double> _processed_response;
    std::string _api_key;
};

#endif
