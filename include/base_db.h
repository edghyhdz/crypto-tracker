//  Base class for DB

// Mongo db installation based on evanugarte's repository
// https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/setup-mongocxx#

#ifndef DATABASE_H
#define DATABASE_H
#include <map>
#include <string>

// Forward struct declaration
struct Data;

class BaseDB {
   public:
    virtual ~BaseDB(){};
    virtual bool add_record(std::string table, Data data) = 0;
    virtual bool update_record() = 0;
    virtual bool delete_record(std::string id) = 0;
};
#endif
