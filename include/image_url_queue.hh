// Copyright 2020 lamp
// Created by lamp on 5/10/21.
//

#ifndef INCLUDE_IMAGE_URL_QUEUE_HH_
#define INCLUDE_IMAGE_URL_QUEUE_HH_

#include <url.hh>

#include <string_view>
#include <string>
#include <vector>
#include <queue>
#include <mutex>

class image_url_queue {
 public:
  static image_url_queue* get_instance();
  void update_filename(std::string_view filename);

  bool pop(url&);
  void push(const url&);
  void push(url&&);
  void push(std::vector<url>&&);

  bool dump();

  image_url_queue(const image_url_queue&) = delete;
  image_url_queue(image_url_queue&&) = delete;
  image_url_queue& operator=(const image_url_queue&) = delete;
  image_url_queue& operator=(image_url_queue&) = delete;

 private:
  image_url_queue() = default;

  mutable std::mutex _mutex;
  mutable std::mutex _file_mutex;
  std::queue<url> _queue;
  std::string _filename{"images.txt"};
};


#endif  // INCLUDE_IMAGE_URL_QUEUE_HH_
