/**
 * Binance API class handler
 */
#ifndef BINANCE_API
#define BINANCE_API

#include <iostream>
#include <map>
#include <spotify/json.hpp>
#include <spotify/json/decode.hpp>
#include <spotify/json/decode_exception.hpp>
#include <spotify/json/encode.hpp>
#include "base_api.h"

constexpr char BINANCE_ENDPOINT[] =
    "https://api.binance.com/api/v3/ticker/price";
constexpr int REQUEST_RATE = 500;

struct Response {
    std::vector<std::map<std::string, std::string>> response;
};

namespace spotify {
namespace json {

// Specialize spotify::json::default_codec_t to specify default behavior when
// encoding and decoding objects of certain types.
template <> struct default_codec_t<Response> {
  static codec::object_t<Response> codec() {
    auto codec = codec::object<Response>();
    codec.required("response", &Response::response);
    return codec;
  }
};
} // namespace json
} // namespace spotify

class BinanceAPI : public BaseAPIExchange {
   public:
    BinanceAPI(int id);
    ~BinanceAPI() override { std::cout << "Called BinanceAPI destructor\n"; };
    std::string get_data() override;
    std::map<std::string, double> process_token_price_response(Response &response) override;
    void parse_response(std::string &response); 
   private:
    void _get_request(long *http_code, std::string *read_buffer);
    int _current_weight;
};
#endif
