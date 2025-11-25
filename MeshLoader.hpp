#pragma once
#include <Metal/Metal.hpp>
#include <iostream>
#include <string>
#include <vector>

// Define the implementation in ONE cpp file (we'll do this in Renderer.cpp)
// #define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

struct Vertex {
  float position[4];
  float normal[4];
  float color[4];
};

class MeshLoader {
public:
  static std::vector<Vertex> loadObj(const std::string &filename) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename, reader_config)) {
      if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
      }
      return {};
    }
    
  }
};