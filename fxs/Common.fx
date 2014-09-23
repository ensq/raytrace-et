#ifndef DV2520_COMMON_FX
#define DV2520_COMMON_FX

#pragma pack_matrix( row_major )
#define BLOCK_SIZE 16

#include <structs.fx>

// t
StructuredBuffer< Vertex > srvVertices : register( t0 );
StructuredBuffer< uint > srvIndices : register( t1 );
StructuredBuffer< ObjInstance > srvInstances : register( t2 );
StructuredBuffer< LightPoint > srvLights : register( t3 );
StructuredBuffer< ObjNode > srvNodes : register( t4 );
Texture2D texAlbedo : register( t5 ); // In the future, texture arrays
                                      // may be added to support
                                      // materials.

// u
RWStructuredBuffer< Ray > uavRays : register( u0 );
RWStructuredBuffer< Intersection > uavIntersections : register( u1 );
RWTexture2D< float4 > uavBackbuffer : register( u2 );

// s
SamplerState ssDefault : register( s0 );

// b
cbuffer CbPerInstance : register( b0 ) {
    uint screenWidth;
    uint screenHeight;
    float aspect;
    float fov;
};
cbuffer CbPerFrame : register( b1 ) {
    row_major float4x4 view;
    row_major float4x4 viewInv;
    row_major float4x4 proj;
    row_major float4x4 projInv;

    float3 pos;
    uint instancesCnt;

    uint lightsCnt;
    float3 pad;
};

#endif // DV2520_COMMON_FX
