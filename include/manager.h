#ifndef MANAGER_H
#define MANAGER_H

#include "binance_api.h"
#include "mongo_db.h"

enum class API { BINANCE, KUCOIN };

class ApiManager {
   public:
    ApiManager();
    void get_token_price_data(API api);
    void save_data();

   private:
    MongoDB _db_handler;
    BinanceAPI _binance_handler;
};

#endif
