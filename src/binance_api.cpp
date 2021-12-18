#include "binance_api.h"

BinanceAPI::BinanceAPI(int id) { this->set_id(id); }

std::string BinanceAPI::process_response(std::string &response) {
    return response + " extra text";
}

std::string BinanceAPI::get_data() { return "{'result': 'x'}"; }

