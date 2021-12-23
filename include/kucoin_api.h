/**
 * KuCoin API class declaration
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

namespace yact {

namespace KuCoin {

constexpr char KUCOIN_TICKER_ENDPOINT[] = "https://api.kucoin.com/api/v1/market/allTickers";
constexpr int REQUEST_LIMIT_CODE = 403;  // Error code to expect if request limit is surpassed
constexpr int REQUEST_RATE = 1000;  // Request new data (in milliseconds)

struct Response {
    std::vector<std::map<std::string, std::string>> response;
};

}  // namespace KuCoin
class KucoinAPI : BaseAPIExchange {
   public:
    KucoinAPI(int id);
    ~KucoinAPI() override;
    std::map<std::string, double> get_data() override;
    std::map<std::string, double> process_token_price_response(
        std::string &response) override;

    // Typical behavioral methods
    void set_current_request_code(int r_code);
    int get_current_request_code();

    void _get_request(long *http_code, std::string *read_buffer, std::string end_point) override;
   
   private:
    int _current_request_code;
    std::mutex *_mtx;
};
}  // namespace yact

namespace spotify {
namespace json {

// Specialize spotify::json::default_codec_t to specify default behavior when
// encoding and decoding objects of certain types.
template <>
struct default_codec_t<yact::KuCoin::Response> {
    static codec::object_t<yact::KuCoin::Response> codec() {
        auto codec = codec::object<yact::KuCoin::Response>();
        codec.required("response", &yact::KuCoin::Response::response);
        return codec;
    }
};
}  // namespace json
}  // namespace spotify
#endif

