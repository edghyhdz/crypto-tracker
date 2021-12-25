#include "orders.h"
#include "log.h"

yact::CheckOrders::CheckOrders(Exchange ex, std::string uri, std::string db){
   this->_db_h = std::make_shared<MongoDB>(uri, db);
   LOG(level::Info) << "CheckOrders constructor" << std::endl;
}

yact::CheckOrders::~CheckOrders() {}

void yact::CheckOrders::get_exchange_crypto_symbols() {
    return;
}
