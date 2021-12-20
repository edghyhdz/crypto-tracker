#include "mongo_db.h"
#include <iostream>

MongoDB::MongoDB(std::string uri, std::string db_name)
    : _uri(mongocxx::uri(uri)),
      _client(mongocxx::client(_uri)),
      _db(_client[db_name]) {

    std::cout << "Constructor MongoDB" << std::endl;
}

bool MongoDB::update_record() {
    std::cout << "Updating record " << std::endl;
    return true;
}

bool MongoDB::delete_record(std::string id) {
    std::cout << "Deleted record id: " << id << std::endl;
    return true;
}

///
/// Adds currency price and current time stamp as a record into given collection
/// name `coll_name`
///
/// @param[in] coll_name
///   Collection name where data will be added to
/// @param data
///   Data struct containing map `dictionary` with currencies and prices to add
/// @return True if transaction was performed successfully
///
bool MongoDB::add_record(std::string coll_name, Data data) {
    // Get current time stamp from system
    auto system_clock = std::chrono::system_clock::now();
    long time_stamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                          system_clock.time_since_epoch())
                          .count();

    mongocxx::collection collection = this->_db[coll_name];
    bsoncxx::builder::stream::document builder{};

    auto init_doc = builder << "timeStamp" << time_stamp << "results"
                            << bsoncxx::builder::stream::open_document;

    // Add all records to builder
    for (auto &&cd : data.dictionary) {
        init_doc << cd.first << cd.second;
    }
    auto close_doc = init_doc << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value doc = close_doc
                                   << bsoncxx::builder::stream::finalize;

    collection.insert_one(doc.view());

    return true;
}
