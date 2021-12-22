/**
 * Binance API class handler
 */
#ifndef KUCOIN_API
#define KUCOIN_API

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

constexpr char KUCOIN_TICKER_ENDPOINT[] = "https://api.kucoin.com/api/v1/market/allTickers";
constexpr int REQUEST_LIMIT_CODE = 403;  // Error code to expect if request limit is surpassed
constexpr int REQUEST_RATE = 1000;  // Request new data (in milliseconds)

class KucoinAPI : BaseAPIExchange {
   public:
    KucoinAPI(int id);
    ~KucoinAPI() override { std::cout << "Called KucoinAPI destructor\n"; }
    std::map<std::string, double> get_data() override;
    std::map<std::string, double> process_token_price_response(
        std::string &response) override;

    // Typical behavioral methods
    void set_current_request_code(int r_code);
    int get_current_request_code();

   private:
    void _get_request(long *http_code, std::string *read_buffer);
    int _current_request_code;
    std::mutex *_mtx;

};

}  // namespace yact

#endif

