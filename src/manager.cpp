#include "manager.h"
#include <iostream>
#include <thread>

/**
 * Class constructor
 */
DataManager::DataManager(std::string uri, std::string db_name) {
    this->_db_handler = new MongoDB(uri, db_name);
    this->_binance_handler = new BinanceAPI(1);
    std::cout << "DataManager constructor\n";
}

/**
 * DataManager class destructor
 */
DataManager::~DataManager() {
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
void DataManager::get_token_price_data(Exchange ex) {
    std::cout << "Started getting Exchange data" << std::endl;
    while (true) {
        if (ex == Exchange::BINANCE) {
            std::cout << "Save binance data" << std::endl;
            auto read_buffer = this->_binance_handler->get_data();
            std::cout << "Read buffer:\n" << read_buffer << std::endl;

        } else {
            std::cout << "Saving kucoin data\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

