#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>

class Logger {
 public:
  enum Level { kDebug, kInfo, kWarn, kError, kCrit };

 private:
  static Level log_level_;
  static std::string log_path_;

  static const std::string kLogDebug;
  static const std::string kLogInfo;
  static const std::string kLogWarn;
  static const std::string kLogError;
  static const std::string kLogCrit;

  static std::string GetDateTime();

 private:
  std::ofstream log_stream_;
  bool log_open_;
  bool log_empty_;

 public:
  Logger(Level level);
  ~Logger();

  static int SetLogger(std::string log_path, std::string log_level);
  void Flush();

  template <typename T>
  Logger& operator<<(const T& log) {
    if (log_open_) {
      log_stream_ << log;
      log_empty_ = false;
    }
    return *this;
  }
};

#endif  // LOGGER_H_
