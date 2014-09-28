#ifndef DV2520_COMMON_FX
#define DV2520_COMMON_FX

#pragma pack_matrix( row_major )
#define BLOCK_SIZE 16

#include <structs.fx>

// t
StructuredBuffer<Vertex> srvVertices : register(t0);
StructuredBuffer<uint> srvIndices : register(t1);
StructuredBuffer<ObjInstance> srvInstances : register(t2);
StructuredBuffer<LightPoint> srvLights : register(t3);
StructuredBuffer<ObjNode> srvNodes : register(t4);
Texture2D texAlbedo : register( t5 ); // Make into texture array.
Texture2D src : register(t6);

// u
RWStructuredBuffer<Ray> uavRays : register(u0);
RWStructuredBuffer<Intersection> uavIntersections : register(u1);
RWStructuredBuffer<float4> uavColor : register(u2);
RWTexture2D<float4> uavBackbuffer : register(u3);
RWTexture2D<float4> dest : register(u4);

// s
SamplerState ssDefault : register(s0);

// b
cbuffer CbPerInstance : register(b0) {
    uint screenWidth;
    uint screenHeight;
    float2 pad0;
};
cbuffer CbPerFrame : register(b1) {
    row_major float4x4 view;
    row_major float4x4 viewInv;

    float3 pos;
    uint instancesCnt;

    uint lightsCnt;
    float3 pad1;
};
cbuffer CbPerFov : register(b2) {
    row_major float4x4 proj;
    row_major float4x4 projInv;
    
    uint fovWidth;
    uint fovHeight;
    uint fovOfsX;
    uint fovOfsY;

    float fov;
    float aspect;
    float2 pad2;
};

#endif // DV2520_COMMON_FX
