#include "manager.h"
#include <iostream>
#include <memory>
#include <thread>

/**
 * Class constructor
 */
yact::DataManager::DataManager(std::string uri, std::string db_name) {
    this->_db_h = std::make_shared<MongoDB>(uri, db_name);
    
    // ALL EXCHANGE APIs GO HERE
    this->_apis.push_back(new yact::BinanceAPI());
    this->_apis.push_back(new yact::KucoinAPI());

    std::cout << "DataManager constructor\n";
}

/**
 * DataManager class destructor
 */
yact::DataManager::~DataManager() {
    // Join all API threads
    this->_threads.front().join();
    std::for_each(this->_threads.begin(), this->_threads.end(),
                  [](std::thread& t) { t.join(); });

    // All api objects were instantiated as pointers
    for (auto api : this->_apis) {
        delete api;
    }

    std::cout << "DataManager Destructor\n";
}

/**
 * Launch all available Exchange APIs
 */
void yact::DataManager::run_all_exchange_apis() {
    for (auto api : this->_apis) {
        this->_threads.emplace_back(&yact::DataManager::get_token_price_data,
                                    this, api);
    }
}

/**
 * Requests token price data from given Exchange (Binance, Kucoin, etc ...)
 *
 * @param[in] api
 *  Exchange api to get currency info from (i.e., Kucoin, Binance, etc)
 * @return void
 */
void yact::DataManager::get_token_price_data(BaseAPIExchange* api) {
    Data data;
    while (true) {
        // Check if API has not exceeded any limit
        if (api->has_reached_request_limit()) continue;

        std::cout << "Saving data for: " << api->get_api_name() << std::endl;
        data.dictionary = api->get_data();
        this->_db_h->add_record(api->get_api_name(), data);

        // Delete records that have exceeded MAX_TIME_DB
        // we are only interested in records from the past MAX_TIME_DB
        // seconds
        auto system_clock = std::chrono::system_clock::now();
        data.time_stamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                              system_clock.time_since_epoch())
                              .count();

        // Delete all records from one hour in the past
        data.time_stamp = data.time_stamp - 3600000;

        this->_db_h->delete_record(api->get_api_name(), data);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(yact::REQUEST_RATE));
    }

    return;
}

