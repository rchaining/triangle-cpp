#include <metal_stdlib>
using namespace metal;

struct Uniforms {
    float4x4 rotationMatrix;
};

// Vertex shader
// Runs once for every corner of the triangle (3 times total)
vertex float4 vertex_main(uint vertex_id [[vertex_id]],
                          constant Uniforms &uniforms [[buffer(1)]]) {
    // Define triangle positions
    float4 positions[] = {
        float4(0.0, 0.5, 0.0, 1.0), // Top Center
        float4(-0.5, -0.5, 0.0, 1.0), // Bottom Left
        float4(0.5, -0.5, 0.0, 1.0) // Bottom Right
    };
    // Return position that matches current ID (0, 1, or 2)
    return uniforms.rotationMatrix * positions[vertex_id];
}

// Fragment shader
// Runs for every pixel in the triangle.
fragment float4 fragment_main(){
    // Solid red (RGBA -> 1,0,0,1)
    return float4(1.0, 0.0, 0.0, 1.0);
}