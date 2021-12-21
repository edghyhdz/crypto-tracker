#ifndef MANAGER_H
#define MANAGER_H

#include <mongocxx/instance.hpp>
#include "binance_api.h"
#include "mongo_db.h"

enum class Exchange { BINANCE, KUCOIN };

class DataManager {
   public:
    DataManager(std::string uri, std::string db_name);
    void get_token_price_data(Exchange ex);
    void save_data();

   private:
    mongocxx::instance _instance;
    MongoDB *_db_handler;
    BinanceAPI *_binance_handler;
};

#endif
