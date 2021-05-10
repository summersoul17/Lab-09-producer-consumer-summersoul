// Copyright 2020 lamp
// Created by lamp on 5/10/21.
//

#include <fstream>
#include <image_url_queue.hh>

image_url_queue* image_url_queue::get_instance(){
  static image_url_queue instance{};
  return &instance;
}

bool image_url_queue::pop(url& u) {
  std::lock_guard lock(_mutex);

  if (_queue.empty()) {
    return false;
  }

  u = std::move(_queue.front());
  _queue.pop();
  return true;
}

void image_url_queue::push(url&& u) {
  std::lock_guard lock(_mutex);
  _queue.emplace(std::move(u));
}

void image_url_queue::push(const url& u) {
  std::lock_guard lock(_mutex);
  _queue.emplace(u);
}

void image_url_queue::push(std::vector<url>&& urls) {
  std::lock_guard lock(_mutex);
  for (auto url : urls){
    _queue.emplace(std::move(url));
  }
}
void image_url_queue::update_filename(std::string_view filename) {
  std::lock_guard _lock(_file_mutex);
  _filename = filename;

  std::ofstream output(_filename);
  if (!output.is_open()) {
    throw std::runtime_error{"Error loading file: " + _filename};
  }
  output << "Images:\n";
  output.close();
}
bool image_url_queue::dump() {
  std::scoped_lock lock(_file_mutex, _mutex);

  if (_queue.empty()) {
    return false;
  }

  std::ofstream file(_filename, std::ios::app);

  if (!file.is_open()) {
    throw std::runtime_error{"Error loading file: " + _filename};
  }
  while (!_queue.empty()) {
    auto item = std::move(_queue.front());
    _queue.pop();
    file << item._url_str << '\n';
  }

  file.close();

  return true;
}
