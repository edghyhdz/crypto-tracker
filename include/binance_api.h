/**
 * Binance API class handler
 */
#ifndef BINANCE_API
#define BINANCE_API

#include "base_api.h"

class BinanceAPI : public BaseAPIExchange {
   public:
    BinanceAPI(int id);
    std::string get_data() override;
    std::string process_response(std::string &response) override;
    
   private:
};
#endif
