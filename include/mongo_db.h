#ifndef MONGO_DB
#define MONGO_DB

#include <map>
#include <set>
#include "base_db.h"
#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"

constexpr char kMongoDbUri[] = "mongodb://0.0.0.0:27017";
constexpr char kDatabaseName[] = "testDB";
constexpr char kCryptoCollection[] = "testCryptos";
constexpr char kUserCollection[] = "testCollection";
constexpr char kTargetPoints[] = "targetPoints";

constexpr char kUsersPortfolio[] = "userPortfolio";

// Data struct to pass to add_record method
struct Data {
    std::map<std::string, double> dictionary;
};

class MongoDB : public BaseDB {
   public:
    MongoDB(std::string uri, std::string db_name);
    bool add_record(std::string collection, Data data) override;
    bool update_record() override;
    bool delete_record(std::string id) override;

   private:
    mongocxx::uri _uri;
    mongocxx::client _client;
    mongocxx::database _db;
};

#endif
