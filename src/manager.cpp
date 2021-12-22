#include "manager.h"
#include <iostream>
#include <thread>

/**
 * Class constructor
 */
yact::DataManager::DataManager(std::string uri, std::string db_name) {
    this->_db_handler = new yact::MongoDB(uri, db_name);
    this->_binance_handler = new yact::BinanceAPI(1);

    // Assign a lower api request weight to perform the first binance api's
    // requests
    this->_binance_handler->set_current_weight(0);

    std::cout << "DataManager constructor\n";
}

/**
 * DataManager class destructor
 */
yact::DataManager::~DataManager() {
    delete this->_db_handler;
    delete this->_binance_handler;
    std::cout << "DataManager Destructor\n";
}

/**
 * Requests token price data from given Exchange (Binance, Kucoin, etc ...)
 *
 * @param[in] ex
 *  Exchange to get currency info from (i.e., Kucoin, Binance, etc)
 * @return void
 */
void yact::DataManager::get_token_price_data(Exchange ex) {
    std::cout << "Started getting Exchange data" << std::endl;

    Data data;
    std::string coll_name{""};
    while (true) {
        // Get data from corresponding exchange
        switch (ex) {
            case Exchange::BINANCE:
                std::cout << "Case BINANCE\n";
                coll_name = "testBinance";
                data.dictionary = this->_binance_handler->get_data();
                break;
            case Exchange::KUCOIN:
                std::cout << "Case Kucoin\n";
                coll_name = "testKucoin";
                // TODO: Kucoin API
                break;
            default:
                std::cout << "Default case\n";
                continue;
        }

        // Save data into db
        this->_db_handler->add_record(coll_name, data);

        // Delete records that have exceeded MAX_TIME_DB
        // we are only interested in records from the past MAX_TIME_DB
        // seconds
        auto system_clock = std::chrono::system_clock::now();
        data.time_stamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                              system_clock.time_since_epoch())
                              .count();

        // Delete all records from one hour in the past
        data.time_stamp = data.time_stamp - 3600000;
        this->_db_handler->delete_record(coll_name, data);

        // Sleep until next REQUEST_RATE miliseconds have pased by
        std::this_thread::sleep_for(std::chrono::milliseconds(yact::REQUEST_RATE));
    }
}

