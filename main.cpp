#include <iostream>
#include <thread>
#include "binance_api.h"
#include "manager.h"
#include "mongo_db.h"

int main(int argc, char** argv) {
    std::string uri{"mongodb://0.0.0.0:27017"};
    std::string db_name{"testDB"};

    yact::DataManager data_handler = yact::DataManager(uri, db_name);

    std::thread t = std::thread(&yact::DataManager::run_all_exchange_apis, &data_handler);

    t.join();
}
