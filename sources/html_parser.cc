// Copyright 2020 lamp
//
// Created by lamp on 17.04.2021.
//

#include <html_parser.hh>

#include <url_queue.hh>
#include <image_url_queue.hh>

#include <gumbo.h>

#include <vector>
#include <algorithm>

static std::vector<std::string> image_ext{
    "jpg", "jpeg", "gif", "png", "svg",  "tiff", "ico", "bmp"
};

void add_url(std::vector<url>& vector, const char* value, const page& page);
bool image_url(const std::string& url) {
  auto ex_first = url.find_last_of('.');
  const std::string extension = url.substr(ex_first + 1);

  return std::any_of(image_ext.cbegin(), image_ext.cend(),
                     [&](const std::string& s) { return extension == s; });
}
void search_urls(GumboNode* node, std::vector<url>& images,
                 std::vector<url>& pages, size_t depth,
                 const page& p){
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }

  GumboAttribute* href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    if (image_url(href->value))
      add_url(images, href->value, p);
    else if (p.url_._depth < depth)
      add_url(pages, href->value, p);
  }

  GumboAttribute* src;
  if ((node->v.element.tag == GUMBO_TAG_IMG ||
       node->v.element.tag == GUMBO_TAG_IMAGE) &&
      (src = gumbo_get_attribute(&node->v.element.attributes, "src"))) {
    if (src->value[0] != '\0') // Ignore empty
      add_url(images, src->value, p);
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    search_urls(static_cast<GumboNode*>(children->data[i]), images,
                  pages, depth, p);
  }
}
void add_url(std::vector<url>& vector, const char* value, const page& page) {
  if (value[0] == '#'){ return; }
  if (value[0] == '\0') { return; }
  if (value[0] == '/' || value[0] == '?'){
    if (value[1] == '\0' || value[1] == '#'){ return; }
  }

  vector.emplace_back(page.url_._url_str + value,
                      page.url_._depth + 1);
}

void html_parser::parse(const page & p) {
  std::vector<url> images{};
  std::vector<url> pages{};

  GumboOutput * output = gumbo_parse(p.body_.c_str());
  search_urls(output->root, images, pages, _depth, p);
  gumbo_destroy_output(&kGumboDefaultOptions, output);

  url_queue::get_instance()->push(std::move(pages));
  image_url_queue::get_instance()->push(std::move(images));
}
