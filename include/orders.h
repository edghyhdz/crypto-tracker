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

struct OrderData {
   double price;
   double qty;
   bool executed;
};

struct UserOrders {
   std::string _uid;
   std::string exchange;
   bool has_orders;
   std::map<std::string, std::vector<OrderData>> sell_orders;
   std::map<std::string, std::vector<OrderData>> buy_orders;
};

 struct AllUsers {
    std::vector<UserOrders> all_orders;
 };

enum class Exchange;  // Forward declaration

enum class OrderType { BUY, SELL, BUY_OR_SELL };

class CheckOrders {
   public:
    CheckOrders(std::string exchange_name, std::string uri, std::string db_name);
    ~CheckOrders();
    void open_orders(OrderType o_type, std::string *buffer_data);

    std::map<std::string, double> get_symbol_prices();
    void get_exchange_crypto_symbols();  // Get all symbol names from exchange
    std::vector<std::string> get_user_cryptos();

   private:
    std::string _exchange_name;
    std::shared_ptr<yact::MongoDB> _db_h;
};
}  // namespace yact

namespace spotify {
namespace json {

// Specialize spotify::json::default_codec_t to specify default behavior when
// encoding and decoding objects of certain types.
template <> struct default_codec_t<yact::AllUsers> {
  static codec::object_t<yact::AllUsers> codec() {
    auto codec = codec::object<yact::AllUsers>();
    codec.required("results", &yact::AllUsers::all_orders);
    return codec;
  }
};

// Specialize spotify::json::default_codec_t to specify default behavior when
// encoding and decoding objects of certain types.
template <> struct default_codec_t<yact::UserOrders> {
  static codec::object_t<yact::UserOrders> codec() {
    auto codec = codec::object<yact::UserOrders>();
    codec.required("uid", &yact::UserOrders::_uid);
    codec.required("exchange", &yact::UserOrders::exchange);
    codec.required("hasOrders", &yact::UserOrders::has_orders);
    codec.optional("sellOrders", &yact::UserOrders::sell_orders);
    codec.optional("buyOrders", &yact::UserOrders::buy_orders);
    return codec;
  }
};
// Specialize spotify::json::default_codec_t to specify default behavior when
// encoding and decoding objects of certain types.
template <> struct default_codec_t<yact::OrderData> {
  static codec::object_t<yact::OrderData> codec() {
    auto codec = codec::object<yact::OrderData>();
    codec.required("price", &yact::OrderData::price);
    codec.required("qty", &yact::OrderData::qty);
    codec.required("executed", &yact::OrderData::executed);
    return codec;
  }
};
} // namespace json
} // namespace spotify

#endif

