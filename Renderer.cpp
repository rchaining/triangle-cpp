// We need to define these implementations in EXACTLY one .cpp file
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Renderer.hpp"
#include "MeshLoader.hpp"
#include <iostream>
#include <cmath>

// /10 to slow it way down while I'm fiddling.
const float angleChange = 0.05f/10.0f;

struct Uniforms {
    float rotationMatrix[4][4];
};

Renderer::Renderer(MTL::Device* device) : _device(device), _angle(0.0f), _angleDelta(angleChange) {
    // In C++, we need to retain objects we keep around
    _device->retain(); 
    _commandQueue = _device->newCommandQueue();
    buildShaders();
    buildBuffers();
}

Renderer::~Renderer() {
    _vertexBuffer->release();
    _commandQueue->release();
    _pipelineState->release();
    _device->release();
}

void Renderer::buildShaders() {
    // Load the library
    NS::Error* pError = nullptr;
    // Update path to match the Makefile's build directory
    MTL::Library* pLibrary = _device->newLibrary( NS::String::string("./build/default.metallib", NS::UTF8StringEncoding), &pError );
    if ( !pLibrary )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }
    
    // We need to wrap C-strings in NS::String for Metal
    NS::String* vertexName = NS::String::string("vertex_main", NS::UTF8StringEncoding);
    NS::String* fragName = NS::String::string("fragment_main", NS::UTF8StringEncoding);
    
    MTL::Function* vertexFn = pLibrary->newFunction(vertexName);
    MTL::Function* fragFn = pLibrary->newFunction(fragName);
    
    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(vertexFn);
    desc->setFragmentFunction(fragFn);
    desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);
    
    NS::Error* error = nullptr;
    _pipelineState = _device->newRenderPipelineState(desc, &error);
    if (!_pipelineState) {
        std::cerr << "Failed to create pipeline state: " << error->localizedDescription()->utf8String() << std::endl;
    }

    vertexFn->release();
    fragFn->release();
    desc->release();
    pLibrary->release();
    vertexName->release(); // NS::String must be released
    fragName->release();
}

void Renderer::buildBuffers() {
    // monke.obj should be in the same folder as the executable
    std::vector<Vertex> mesh = MeshLoader::loadObj("monke.obj");
    _vertexCount = mesh.size();
    size_t dataSize = mesh.size() * sizeof(Vertex);
    // Create GPU buffer
    // MTLResourceStorageModeShared = CPU writes, GPU reads
    _vertexBuffer = _device->newBuffer(dataSize, MTL::ResourceStorageModeShared);
    // Copy data from C++ Vector to Metal Buffer
    memcpy(_vertexBuffer->contents(), mesh.data(), dataSize);
}

// Helper for math
Uniforms makeRotation(float angleRadians) {
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    Uniforms u;

    // Initialize identity matrix
    // For loop will output:
    // [[1, 0, 0, 0],
    //  [0, 1, 0, 0],
    //  [0, 0, 1, 0],
    //  [0, 0, 0, 1]]
    for(int i=0; i<4; i++) for(int j=0; j<4; j++) u.rotationMatrix[i][j] = (i==j ? 1.0f : 0.0f);
    // Then make it into a rotation matrix:
    u.rotationMatrix[0][0] = c;
    u.rotationMatrix[0][1] = s;
    u.rotationMatrix[1][0] = -s;
    u.rotationMatrix[1][1] = c;
    // This leaves it as:
    // [[c, -s, 0, 0],
    //  [s, c, 0, 0],
    //  [0, 0, 1, 0],
    //  [0, 0, 0, 1]]
    return u;
}

void Renderer::draw(CA::MetalLayer* layer) {
    // Entry into the C++ layer, called by the main loop in ObjC.
    CA::MetalDrawable* drawable = layer->nextDrawable();
    if (!drawable) return;

    MTL::CommandBuffer* cmdBuf = _commandQueue->commandBuffer();
    
    MTL::RenderPassDescriptor* passDesc = MTL::RenderPassDescriptor::renderPassDescriptor();
    passDesc->colorAttachments()->object(0)->setTexture(drawable->texture());
    passDesc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    passDesc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0, 0, 0, 1));
    
    MTL::RenderCommandEncoder* enc = cmdBuf->renderCommandEncoder(passDesc);
    enc->setRenderPipelineState(_pipelineState);
    
    _angle += _angleDelta; // Simplified for brevity
    Uniforms u = makeRotation(_angle);
    
    // Bind buffer 0 -> object vertices
    enc->setVertexBuffer(_vertexBuffer, 0, 0);

    // Bind buffer 1 -> Rotation matrix
    enc->setVertexBytes(&u, sizeof(u), 1);

    // Draw
    enc->drawPrimitives(MTL::PrimitiveTypeTriangle, (NS::UInteger)0, (NS::UInteger)_vertexCount);
    
    enc->endEncoding();
    cmdBuf->presentDrawable(drawable);
    cmdBuf->commit();
}