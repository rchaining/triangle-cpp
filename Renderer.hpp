#pragma once
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp> // For CA::MetalLayer
#include <vector>

class Renderer {
public:
    Renderer(MTL::Device* device);
    ~Renderer();
    
    void draw(CA::MetalLayer* layer);

private:
    MTL::Device* _device;
    MTL::CommandQueue* _commandQueue;
    MTL::RenderPipelineState* _pipelineState;

    MTL::Buffer* _vertexBuffer;
    int _vertexCount;

    float _angle;
    
    void buildShaders();
    void buildBuffers();
};