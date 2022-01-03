#include "mongo_db.h"
#include <iostream>

/**
 * MongoDB class constructor
 */
yact::MongoDB::MongoDB(std::string uri, std::string db_name)
    : _uri(mongocxx::uri(uri)),
      _client(mongocxx::client(_uri)),
      _db(_client[db_name]) {
    std::cout << "Constructor MongoDB" << std::endl;
}

/**
 * Udates record
 */
bool yact::MongoDB::update_record() {
    std::cout << "Updating record " << std::endl;
    return true;
}

/**
 * Deletes all records with `timeStamp` value below given `data.time_stamp`
 *
 * @params[in] coll_name
 *  collection name
 * @params[in] data
 *  Data struct containing time_stamp attribute
 */
bool yact::MongoDB::delete_record(std::string coll_name, Data data) {
    // Find ids
    mongocxx::collection collection = this->_db[coll_name];

    bsoncxx::stdx::optional<mongocxx::result::delete_result> result =
        collection.delete_many(
            bsoncxx::builder::stream::document{}
            << "timeStamp" << bsoncxx::builder::stream::open_document << "$lte"
            << data.time_stamp << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::finalize);

    return true;
}

/**
 * Adds crypto currency price and timestamp as a new record in the given
 * collection name `coll_name`
 *
 * @param[in] coll_name
 *  Collection name where data will be added to
 * @param data[in]
 *  Data struct containing a `dictionary` map attribute that contains all
 * currencies and prices to be added
 * @return true if transaction was performed successfully, else false
 */
bool yact::MongoDB::add_record(std::string coll_name, Data data) {
    // Get current time stamp from system
    auto system_clock = std::chrono::system_clock::now();
    long time_stamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                          system_clock.time_since_epoch())
                          .count();
    try {
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
    } catch (...) {
        return false;
    }
}

/**
 * Return last added item from collection
 *
 * @param[in] coll_name
 *  collection name
 * @param[in, out] buffer_data
 *  queried data from db
 *
 */
bool yact::MongoDB::find_last(std::string coll_name, std::string *buffer_data) {
    try {
        mongocxx::collection collection = this->_db[coll_name];

        // Filter collection to get last inserted doc
        auto order = bsoncxx::builder::stream::document{}
                     << "$natural" << -1 << bsoncxx::builder::stream::finalize;

        auto opts = mongocxx::options::find{};
        opts.sort(order.view());

        // We just need the latest crypto currency data
        auto test = collection.find_one({}, opts);

        // Pass db data to pointer
        *buffer_data = bsoncxx::to_json(*test);

        return true;
    } catch (...) {
        return false;
    }
}

/**
 * Gets record based on given query on `data.document`
 *
 * @param[in] coll_name
 *  collection name
 * @param[in] data
 *  data containing query for db
 * @param[in, out] buffer_data
 *  queried data from db
 */
bool yact::MongoDB::get_record(std::string coll_name, Data data,
                               std::string *buffer_data) {
    try {
        mongocxx::collection collection = this->_db[coll_name];
        mongocxx::cursor cursor = collection.find(data.document->view());

        for (auto doc : cursor) {
            std::string temp_doc;
            if (data.filters != "") {
                temp_doc = bsoncxx::to_json(doc[data.filters].get_document());
            } else {
                temp_doc = bsoncxx::to_json(doc);
            }

            *buffer_data += "[" + temp_doc + "],";
        }
        // Get rid of comma from last item
        buffer_data->pop_back();
        std::cout << "ALL DATA\n" << *buffer_data << std::endl;

        return true;
    } catch (...) {
        return false;
    }
}
