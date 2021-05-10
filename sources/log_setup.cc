//
// Created by lamp on 16.04.2021.
//

#include <log_setup.hh>

void crawler::init() {
  boost::log::add_common_attributes();

  boost::log::add_console_log(
      std::cout, boost::log::keywords::format =
                     "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");
  boost::log::add_file_log(
      boost::log::keywords::file_name = "log_%N.log",
      boost::log::keywords::rotation_size = 10 * 1024 * 1024,
      boost::log::keywords::time_based_rotation =
          boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
      boost::log::keywords::format =
          "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");
}