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

struct Ticker {
    std::string symbol;
    std::string buy; 
    std::string sell;
    std::string change_rate;
    std::string change_price;
    std::string high;
    std::string low;
    std::string vol;
    std::string vol_value;
    std::string last;
    std::string average_price;
    std::string taker_fee_rate;
    std::string maker_fee_rate;
    std::string taker_coefficient;
    std::string maker_coefficient;
};

struct ResponseData {
    long time;
    std::vector<yact::KuCoin::Ticker> ticker;
};

struct Response {
    std::string code;
    ResponseData data;
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
        codec.required("code", &yact::KuCoin::Response::code);
        codec.required("data", &yact::KuCoin::Response::data);
        return codec;
    }
};

template <>
struct default_codec_t<yact::KuCoin::ResponseData> {
    static codec::object_t<yact::KuCoin::ResponseData> codec() {
        auto codec = codec::object<yact::KuCoin::ResponseData>();
        codec.required("time", &yact::KuCoin::ResponseData::time);
        codec.required("ticker", &yact::KuCoin::ResponseData::ticker);
        return codec;
    }
};

template <>
struct default_codec_t<yact::KuCoin::Ticker> {
    static codec::object_t<yact::KuCoin::Ticker> codec() {
        auto codec = codec::object<yact::KuCoin::Ticker>();
        codec.required("symbolName", &yact::KuCoin::Ticker::symbol);
        codec.required("buy", &yact::KuCoin::Ticker::buy);
        codec.required("sell", &yact::KuCoin::Ticker::sell);
        codec.required("changeRate", &yact::KuCoin::Ticker::change_rate);
        codec.required("changePrice", &yact::KuCoin::Ticker::change_price);
        codec.required("high", &yact::KuCoin::Ticker::high);
        codec.required("low", &yact::KuCoin::Ticker::low);
        codec.required("vol", &yact::KuCoin::Ticker::vol);
        codec.required("volValue", &yact::KuCoin::Ticker::vol_value);
        codec.required("last", &yact::KuCoin::Ticker::last);
        codec.required("averagePrice", &yact::KuCoin::Ticker::average_price);
        codec.required("takerFeeRate", &yact::KuCoin::Ticker::taker_fee_rate);
        codec.required("makerFeeRate", &yact::KuCoin::Ticker::maker_fee_rate);
        codec.required("takerCoefficient", &yact::KuCoin::Ticker::taker_coefficient);
        codec.required("makerCoefficient", &yact::KuCoin::Ticker::maker_coefficient);
        return codec;
    }
};

}  // namespace json
}  // namespace spotify
#endif

