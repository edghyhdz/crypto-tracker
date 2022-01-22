#include <iostream>
#include <thread>
#include "manager.h"
#include "orders.h"

int main(int argc, char** argv) {
    std::string uri{"mongodb://0.0.0.0:27017"};
    std::string db_name{"cryptoDB"};

    yact::DataManager data_handler = yact::DataManager(uri, db_name);
    // yact::Exchange bi = yact::Exchange::BINANCE;
    yact::CheckOrders order = yact::CheckOrders("kucoin", uri, db_name);
    std::string buf_data;
    order.open_orders(yact::OrderType::BUY, &buf_data);

    std::thread t =
        std::thread(&yact::DataManager::run_all_exchange_apis, &data_handler);

    t.join();
}
