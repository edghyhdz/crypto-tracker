/**
 * Binance API class handler
 */
#ifndef BINANCE_API
#define BINANCE_API

#include <iostream>
#include "base_api.h"

constexpr char BINANCE_ENDPOINT[] = "https://api.binance.com/api/v3/ticker/price";
constexpr int REQUEST_RATE = 500;

class BinanceAPI : public BaseAPIExchange {
   public:
    BinanceAPI(int id);
    ~BinanceAPI() override { std::cout << "Called BinanceAPI destructor\n"; };
    std::string get_data() override;
    std::string process_response(std::string &response) override;

   private:
    void _get_request(long *http_code, std::string *read_buffer);
};
#endif
