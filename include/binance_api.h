/**
 * Binance API class handler
 */
#ifndef BINANCE_API
#define BINANCE_API

#include <iostream>
#include <map>
#include <mutex>
#include <spotify/json.hpp>
#include <spotify/json/decode.hpp>
#include <spotify/json/decode_exception.hpp>
#include <spotify/json/encode.hpp>
#include "base_api.h"

struct Response {
    std::vector<std::map<std::string, std::string>> response;
};

namespace spotify {
namespace json {

// Specialize spotify::json::default_codec_t to specify default behavior when
// encoding and decoding objects of certain types.
template <>
struct default_codec_t<Response> {
    static codec::object_t<Response> codec() {
        auto codec = codec::object<Response>();
        codec.required("response", &Response::response);
        return codec;
    }
};
}  // namespace json
}  // namespace spotify

namespace yact {

constexpr char BINANCE_ENDPOINT[] = "https://api.binance.com/api/v3/ticker/price";
constexpr int REQUEST_RATE = 500;   // Request new data (in miliseconds)
constexpr int WEIGHT_LIMIT = 1000;  // Binance limit per minute

class BinanceAPI : public BaseAPIExchange {
   public:
    BinanceAPI(int id);
    ~BinanceAPI() override { std::cout << "Called BinanceAPI destructor\n"; };
    std::map<std::string, double> get_data() override;
    std::map<std::string, double> process_token_price_response(
        std::string &response) override;
    void parse_response(std::string &response);
    void set_current_weight(int c_weight);
    int get_current_weight();
    void _get_request(long *http_code, std::string *read_buffer, std::string end_point) override;

   private:
    int _current_weight;
    std::mutex *_mtx;
};
}  // namespace yact
#endif
