#ifndef DV2520_COMMON_FX
#define DV2520_COMMON_FX

#pragma pack_matrix( row_major )
#define BLOCK_SIZE 16

#include <Ray.fx>
#include <Vertex.fx>
#include <Intersection.fx>

StructuredBuffer< Vertex > srvVertices : register( s0 );
StructuredBuffer< uint > srvIndices : register( s1 );

RWStructuredBuffer< Ray > uavRays : register( u0 );
RWStructuredBuffer< Intersection > uavIntersections : register( u1 );
RWTexture2D< float4 > uavBackbuffer : register( u2 );

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
	uint pad0;
};

cbuffer CbPerObject : register( b2 ) {
	uint numVertices;
	uint numIndices;
	uint pad1[ 2 ];
};

#endif // DV2520_COMMON_FX