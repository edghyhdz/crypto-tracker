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

    while (true) {
        if (ex == Exchange::BINANCE) {
            // Do not perform get request in case request is about to exceed
            // binance's api rate limit
            if (this->_binance_handler->get_current_weight() >= yact::WEIGHT_LIMIT)
                continue;

            std::cout << "Save binance data" << std::endl;

            data.dictionary = this->_binance_handler->get_data();
            std::cout << "Data: " << data.dictionary.size() << std::endl;

            // Save data into db
            this->_db_handler->add_record("testCrypto", data);

            // Parse it into json format
            // Response res = spotify::json::decode<Response>(read_buffer);

            // std::cout << "ALl tokens: " << res.response.size() << std::endl;

            //  std::cout << "Read buffer:\n" << read_buffer << std::endl;

        } else {
            std::cout << "Saving kucoin data\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

