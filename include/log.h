/**
 * Modified log script from taylorconor
 * Original source: 
 * https://github.com/taylorconor/p2psc/blob/master/include/p2psc/log.h 
 * 
 * Change log: 
 *  - Added LOG_MESSAGE Macro
 */

#pragma once

#include <boost/filesystem/path.hpp>
#include <iostream>
#include <mutex>
#include <sstream>
#include <fstream>
#include <thread>

namespace yact {
namespace {
const char *level_strings[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
const char date_time_format[] = "%Y.%m.%d %H:%M:%S";

std::string current_time() {
  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);
  const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  const auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds);

  char time_str[] = "yyyy.mm.dd HH:MM:SS";
  strftime(time_str, strlen(time_str), date_time_format, localtime(&time));
  return std::string(time_str) + "." + std::to_string(milliseconds.count());
}
} // namespace

enum class level { Error, Warning, Info, Debug };

#define REPORTING_LEVEL level::Debug

#define LOG(level)                                                             \
  if (level > REPORTING_LEVEL)                                                 \
    ;                                                                          \
  else                                                                         \
    Log(level, __FILE__, __LINE__).get()                                       

#define LOG_MESSAGE(level, MESSAGE)                                            \
  if (level > REPORTING_LEVEL)                                                 \
    ;                                                                          \
  else                                                                         \
    Log(level, __FILE__, __LINE__).get(MESSAGE)                                       

class Log {
public:
  Log(enum level l, const char *file, long line)
      : _level(l), _file(file), _line(line) {}
  ~Log() {
    std::string stream_string = _stream.str();
    const auto end = stream_string.find_last_not_of("\n");
    stream_string.erase(end + 1);
    std::lock_guard<std::mutex> guard(_mutex());
    std::cout << stream_string << std::endl;
  }
  std::ostringstream &get() {
    _stream << current_time() << " " << level_strings[static_cast<int>(_level)]
            << " (" << std::this_thread::get_id() << ")\t"
            << _file.filename().string() << ":" << _line << ":\t";
    return _stream;
  }

    void get(std::string &message) {
   
    std::ofstream logFile(".log", std::ios_base::app);
    logFile << current_time() << " " << level_strings[static_cast<int>(_level)]
            << " (" << std::this_thread::get_id() << ")\t"
            << _file.filename().string() << ":" << _line << ":\t" << message
            << "\n";
    logFile.close();
  }

private:
  Log(const Log &);
  Log &operator=(const Log &);
  static std::mutex &_mutex() {
    static std::mutex m;
    return m;
  }

  level _level;
  boost::filesystem::path _file;
  long _line;
  std::ostringstream _stream;
};
} // namespace yact
