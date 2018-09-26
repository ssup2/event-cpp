#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "logger.h"

// Set Default log path and log level.
std::string Logger::log_path_ = "./log.txt";
Logger::Level Logger::log_level_ = Logger::kInfo;

const std::string Logger::kLogDebug = "Debg";
const std::string Logger::kLogInfo = "Info";
const std::string Logger::kLogWarn = "Warn";
const std::string Logger::kLogError = "Erro";
const std::string Logger::kLogCrit = "Crit";

std::string Logger::GetDateTime() {
  using namespace std::chrono;

  auto now = system_clock::now();
  auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
  auto timer = system_clock::to_time_t(now);
  auto tp = *std::localtime(&timer);

  char date[sizeof("2018-01-01 12:00:00")];
  std::strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &tp);

  std::ostringstream oss;
  oss << date << '.' << std::setfill('0') << std::setw(3) << ms.count();
  return oss.str();
}

Logger::Logger(Level level) : log_open_(false), log_empty_(true) {
  if (log_level_ <= level) {
    log_stream_.open(log_path_, std::ofstream::app);
    log_open_ = true;

    log_stream_ << "[" << GetDateTime() << "]";

    log_stream_ << "[";
    if (level == Level::kDebug)
      log_stream_ << kLogDebug;
    else if (level == Level::kInfo)
      log_stream_ << kLogInfo;
    else if (level == Level::kWarn)
      log_stream_ << kLogWarn;
    else if (level == Level::kError)
      log_stream_ << kLogWarn;
    else if (level == Level::kCrit)
      log_stream_ << kLogCrit;
    log_stream_ << "] ";
  }
}

Logger::~Logger() {
  if (log_open_ && !log_empty_) {
    log_empty_ = true;
    log_stream_ << std::endl;
    log_stream_.close();
  }
}

int Logger::SetLogger(std::string log_path, std::string log_level) {
  log_path_ = log_path;

  if (!log_level.compare(kLogDebug))
    log_level_ = Level::kDebug;
  else if (!log_level.compare(kLogInfo))
    log_level_ = Level::kInfo;
  else if (!log_level.compare(kLogWarn))
    log_level_ = Level::kWarn;
  else if (!log_level.compare(kLogError))
    log_level_ = Level::kError;
  else if (!log_level.compare(kLogCrit))
    log_level_ = Level::kCrit;
  else
    return -1;

  return 0;
}

void Logger::Flush() {
  if (log_open_ && !log_empty_) {
    log_empty_ = true;
    log_stream_ << std::endl;
    log_stream_.flush();
  }
}
