#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Metal/Metal.hpp>

struct Vertex {
    float position[4]; // x, y, z, w
    float color[4];    // r, g, b, a
};

class MeshLoader {
public:
    // Only loads positions for now
    static std::vector<Vertex> loadObj(const std::string& filename) {
        std::vector<float> rawPositions; 
        std::vector<Vertex> finalVertices;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open OBJ file: " << filename << std::endl;
            return {};
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream s(line);
            std::string type;
            s >> type;

            // Parse Vertex Positions
            if (type == "v") {
                float x, y, z;
                s >> x >> y >> z;
                // Add to our raw list
                rawPositions.push_back(x);
                rawPositions.push_back(y);
                rawPositions.push_back(z);
            }
            // Parse Faces (Mesh should be triangularized b4 export)
            // Note: OBJ indices start at 1
            else if (type == "f") {
                std::string v1Str, v2Str, v3Str;
                s >> v1Str >> v2Str >> v3Str;

                int idx1 = std::stoi(v1Str) - 1;
                int idx2 = std::stoi(v2Str) - 1;
                int idx3 = std::stoi(v3Str) - 1;

                // Create the 3 vertices for this triangle
                // (Multiplying by 0.5 to shrink it a bit so it fits on screen)
                Vertex v1 = {{rawPositions[idx1*3]*0.5f, rawPositions[idx1*3+1]*0.5f, rawPositions[idx1*3+2]*0.5f, 1.0f}, {1,1,1,1}};
                Vertex v2 = {{rawPositions[idx2*3]*0.5f, rawPositions[idx2*3+1]*0.5f, rawPositions[idx2*3+2]*0.5f, 1.0f}, {1,1,1,1}};
                Vertex v3 = {{rawPositions[idx3*3]*0.5f, rawPositions[idx3*3+1]*0.5f, rawPositions[idx3*3+2]*0.5f, 1.0f}, {1,1,1,1}};

                finalVertices.push_back(v1);
                finalVertices.push_back(v2);
                finalVertices.push_back(v3);
            }
        }
        
        std::cout << "Loaded " << finalVertices.size() << " vertices." << std::endl;
        return finalVertices;
    }
};