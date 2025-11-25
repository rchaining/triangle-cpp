#pragma once
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp> // For CA::MetalLayer

class Renderer {
public:
    Renderer(MTL::Device* device);
    ~Renderer();
    
    void draw(CA::MetalLayer* layer);

private:
    MTL::Device* _device;
    MTL::CommandQueue* _commandQueue;
    MTL::RenderPipelineState* _pipelineState;
    float _angle;
    
    void buildShaders();
};