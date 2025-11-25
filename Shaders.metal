#include <metal_stdlib>
using namespace metal;

// 1. Define the incoming data structure
// This matches the "Vertex" struct in our C++ code exactly.
struct VertexIn {
    float4 position;
    float4 color;
};

// 2. Define the data passing from Vertex -> Fragment
struct VertexOut {
    float4 position [[position]]; // Tag with position for the GPU (Why is this necessary?)
    float4 color;
};

struct Uniforms {
    float4x4 rotationMatrix;
};

vertex VertexOut vertex_main(device const VertexIn* vertices [[buffer(0)]], // Read array from Buffer 0
                             constant Uniforms &uniforms   [[buffer(1)]], // Read matrix from Buffer 1
                             uint vertexId                 [[vertex_id]]) // Current index
{
    VertexOut out;
    
    // Grab the vertex from the array using the index
    float4 pos = vertices[vertexId].position;
    
    // Apply rotation
    out.position = uniforms.rotationMatrix * pos;
    
    // Pass the color through to the fragment shader
    out.color = vertices[vertexId].color;
    
    return out;
}

fragment float4 fragment_main(VertexOut in [[stage_in]]) {
    // Just return the color we read from the file (or defaults)
    return in.color;
}