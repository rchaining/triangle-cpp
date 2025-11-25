#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Metal/Metal.hpp>

// Vertex struct matching our shaders'
struct Vertex {
    float position[4]; // x, y, z, w
    float color[4];    // r, g, b, a
};

// Open the obj as text file
// Read lines starting with v (vertex posns) and f (faces).
// Convert into a buffer to send to Metal

class MeshLoader {
public:
    // Only load positions for now
    static std::vector<Vertex> loadObj(const std::string& filename) {
        std::vector<float> rawPositions; // store the 'v' lines
        std::vector<Vertex> finalVertices; // store the 'f' assembled triangles

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return {};
        }

        std::string line;
        while ((std::getline)file, line) {
            std::istringstream s(line);
            std::string type;
            s >> type;

            // Parse vert positions (v x y z)
            if (type == "v") {
                float x, y, z;
                s >> x >> y >> z;
                // Add to list
                rawPositions.push_back(x);
                rawPositions.push_back(y);
                rawPositions.push_back(z);
            }
            // Parse faces (f v1, v2, v3)
            // Note obj indices start at 1
            else if (type == "f") {
                std::string v1s, v2s, v3s;
                s >> v1s >> v2s >> v3s;

                // OBJ faces can look like "1/1/1" or just 1.
                // We only care about the first number for now (position index)
                int idx1 = std::stoi(v1s) -1;
                int idx2 = std::stoi(v2s) -1;
                int idx3 = std::stoi(v3s) -1;

                // Create 3 vertices for this triangle
                // Mult by .5 to shrink it a bit so it fits on screen
                // ???
                // I'm guess here gemini has already assumed the monkey
                // has the default size (presumably 2m tall like the cube)
                // And knows the distance from the camera will be too small.

                Vertex v1 = {{rawPositions[idx1*3]*0.5f, rawPositions[idx1*3+1]*0.5f, rawPositions[idx1*3+2]*0.5f, 1.0f}, {1,1,1,1}};
                Vertex v2 = {{rawPositions[idx2*3]*0.5f, rawPositions[idx2*3+1]*0.5f, rawPositions[idx2*3+2]*0.5f, 1.0f}, {1,1,1,1}};
                Vertex v3 = {{rawPositions[idx3*3]*0.5f, rawPositions[idx3*3+1]*0.5f, rawPositions[idx3*3+2]*0.5f, 1.0f}, {1,1,1,1}};

                finalVertices.push_back(v1);
                finalVertices.push_back(v2);
                finalVertices.push_back(v3);
            }
        }
        std::cout << "Loaded " << finalVertices.size() << " vertices" << std::endl;
        return finalVertices;
    }
}