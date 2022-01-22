#include "orders.h"
#include "log.h"

yact::CheckOrders::CheckOrders(std::string exchange_name, std::string uri,
                               std::string db) {
    this->_exchange_name = exchange_name;
    this->_db_h = std::make_shared<MongoDB>(uri, db);

    LOG(level::Info) << "CheckOrders constructor" << std::endl;
}

yact::CheckOrders::~CheckOrders() {}

/**
 * Search for BUY, SELL or BUY & SELL open orders.
 * @param[in] o_type
 *  Order Type to search on database
 * @param[in, out] buffer_data
 *  returned data from db to be parsed as json
 */
void yact::CheckOrders::open_orders(OrderType o_type,
                                    std::string *buffer_data) {
    Data data;
    data.filters = "";

    std::string buffer;
    bsoncxx::builder::stream::document doc{};

    auto doc_view = doc << "exchange" << this->_exchange_name
                        << bsoncxx::builder::stream::finalize;

    // Pass it as a document view otherwise object will get destroyed after
    // leaving this scope
    data.document_view = doc_view.view();
    this->_db_h->get_record("userOrders", data, buffer_data);

    yact::Users users = spotify::json::decode<yact::Users>(*buffer_data);

    // for (yact::UserOrderrs item

    for (yact::UserOrders item : users.all_orders) {
        std::cout << "\nUser: " << item._uid << " -------------------- \n\n";

        for (auto b_order : item.buy_orders) {
            std::cout << "Crypto on " << item.exchange << ": " << b_order.first
                      << ". Buy order: " << b_order.second[0].price
                      << std::endl;
        }
        for (auto s_order : item.sell_orders) {
            std::cout << "Crypto: " << s_order.first
                      << ". Sell order: " << s_order.second[0].price
                      << std::endl;
        }

        // std::cout << "Size buy orders: " << item.buy_orders.size() << std::endl;
        // std::cout << "Size sell orders: " << item.sell_orders.size()
        //          << std::endl;
    }
}

void yact::CheckOrders::get_exchange_crypto_symbols() { return; }
