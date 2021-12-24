#ifndef MANAGER_H
#define MANAGER_H

#include <mongocxx/instance.hpp>
#include <thread>
#include "binance_api.h"
#include "kucoin_api.h"
#include "mongo_db.h"

namespace yact {

enum class Exchange { BINANCE, KUCOIN };

class DataManager {
   public:
    DataManager(std::string uri, std::string db_name);
    ~DataManager();
    void run_all_exchange_apis();
    void get_token_price_data(BaseAPIExchange *api);

   private:
    mongocxx::instance _instance;
    std::vector<BaseAPIExchange *> _apis;
    std::vector<std::thread> _threads;
    std::shared_ptr<MongoDB> _db_h;
};
}  // namespace yact
#endif
