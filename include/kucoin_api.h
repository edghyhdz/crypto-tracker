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
    std::string symbol_name;
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
class KucoinAPI : public BaseAPIExchange {
   public:
    KucoinAPI();
    ~KucoinAPI() override;
    std::map<std::string, double> get_data() override;
    std::map<std::string, double> process_token_price_response(
        std::string &response) override;
    bool has_reached_request_limit() override;

    // Typical behavioral methods
    void _get_request(long *http_code, std::string *read_buffer, std::string end_point) override;

   private:
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
        codec.required("symbol", &yact::KuCoin::Ticker::symbol);
        codec.required("symbolName", &yact::KuCoin::Ticker::symbol_name);
        codec.optional("buy", &yact::KuCoin::Ticker::buy);
        codec.optional("sell", &yact::KuCoin::Ticker::sell);
        codec.optional("changeRate", &yact::KuCoin::Ticker::change_rate);
        codec.optional("changePrice", &yact::KuCoin::Ticker::change_price);
        codec.optional("high", &yact::KuCoin::Ticker::high);
        codec.optional("low", &yact::KuCoin::Ticker::low);
        codec.optional("vol", &yact::KuCoin::Ticker::vol);
        codec.optional("volValue", &yact::KuCoin::Ticker::vol_value);
        codec.optional("last", &yact::KuCoin::Ticker::last);
        // codec.optional("averagePrice", &yact::KuCoin::Ticker::average_price);
        codec.optional("takerFeeRate", &yact::KuCoin::Ticker::taker_fee_rate);
        codec.optional("makerFeeRate", &yact::KuCoin::Ticker::maker_fee_rate);
        codec.optional("takerCoefficient", &yact::KuCoin::Ticker::taker_coefficient);
        codec.optional("makerCoefficient", &yact::KuCoin::Ticker::maker_coefficient);
        return codec;
    }
};

}  // namespace json
}  // namespace spotify
#endif

