#ifndef MANAGER_H
#define MANAGER_H

#include <mongocxx/instance.hpp>
#include "binance_api.h"
#include "kucoin_api.h"
#include "mongo_db.h"

namespace yact {

enum class Exchange { BINANCE, KUCOIN };

class DataManager {
   public:
    DataManager(std::string uri, std::string db_name);
    ~DataManager();
    void get_token_price_data(Exchange ex);
    void save_data();
    bool check_request_limit(Exchange ex);  // Check if we can proceed with API request

   private:
    mongocxx::instance _instance;
    MongoDB *_db_handler;
    BinanceAPI *_binance_handler;
    KucoinAPI *_kucoin_handler;
};
}  // namespace yact
#endif
