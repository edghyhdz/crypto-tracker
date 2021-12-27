/**
 * Check orders class declaration
 *  Check for TPs, SL, Buy orders or Sell orders on holder's crypto currencies
 *  Multiple users are possible.
 */

#ifndef ORDER_H
#define ORDER_H

#include <boost/algorithm/string/join.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <spotify/json.hpp>
#include <spotify/json/decode.hpp>
#include <spotify/json/decode_exception.hpp>
#include <spotify/json/encode.hpp>
#include <vector>
#include "mongo_db.h"
#include "mongocxx/instance.hpp"

namespace yact {

enum class Exchange;  // Forward declaration

class CheckOrders {
   public:
    CheckOrders(Exchange ex, std::string uri, std::string db_name);
    ~CheckOrders();
    void get_exchange_crypto_symbols();  // Get all symbol names from exchange
    std::vector<std::string> get_user_cryptos();




   private:
    std::shared_ptr<yact::MongoDB> _db_h;
    mongocxx::instance _instance;
};
}  // namespace yact
#endif

