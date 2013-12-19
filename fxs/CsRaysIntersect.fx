#ifndef DV2520_CSRAYSINTERSECT_FX
#define DV2520_CSRAYSINTERSECT_FX

#include <Common.fx>

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
	uint pixelIdx = gThreadId.y * screenWidth + gThreadId.x;
	Ray	ray	= uavRays[ pixelIdx ];
	
	Intersection closestIntersection;
	closestIntersection.dist = ray.distMax;
	closestIntersection.primId = -1;

	uint indicesOffset = 0;
	uint verticesOffset = 0;
	for( uint instancesIdx = 0; instancesIdx<instancesCnt; instancesIdx++ ) {
		float3 p0, p1, p2;
		Intersection curIntersection;
		for( uint vertexIdx = 2; vertexIdx<srvInstances[ instancesIdx ].indexCnt; vertexIdx += 3 ) {
			float3 rayDir = mul( srvInstances[ instancesIdx ].worldInv, float4( ray.dir, 0.0f ) ).xyz;
			float3 rayPos = mul( srvInstances[ instancesIdx ].worldInv, float4( ray.pos, 1.0f ) ).xyz;

			p0 = srvVertices[ verticesOffset + srvIndices[ indicesOffset + vertexIdx - 2 ]	].pos;
			p1 = srvVertices[ verticesOffset + srvIndices[ indicesOffset + vertexIdx - 1 ]	].pos;
			p2 = srvVertices[ verticesOffset + srvIndices[ indicesOffset + vertexIdx ]		].pos;
			curIntersection = intersectRayTriangle( rayPos, rayDir, p0, p1, p2, indicesOffset + vertexIdx );
			
			if( curIntersection.primId<0 || ray.primID==curIntersection.primId || curIntersection.dist>=closestIntersection.dist ) {
				continue;
			}
			closestIntersection = curIntersection;
		}
		indicesOffset += srvInstances[ instancesIdx ].indexCnt;
		verticesOffset += srvInstances[ instancesIdx ].vertexCnt;
	}

	uavIntersections[ pixelIdx ] = closestIntersection;
}

#endif // DV2520_CSRAYSINTERSECT_FX