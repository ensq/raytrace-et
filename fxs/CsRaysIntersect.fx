#ifndef DV2520_CSRAYSINTERSECT_FX
#define DV2520_CSRAYSINTERSECT_FX

#include <Common.fx>

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
	unsigned int pixelIndex = gThreadId.y * screenWidth + gThreadId.x;
	
	Ray	ray	= uavRays[ pixelIndex ];
	
	Intersection closestIntersection;
	closestIntersection.dist = ray.distMax;
	closestIntersection.primId = -1;

	float3 p0, p1, p2;
	Intersection curIntersection;
	for( uint i = 2; i<numIndices; i += 3 ) {
		p0 = srvVertices[ srvIndices[ i - 2 ]	].pos;
		p1 = srvVertices[ srvIndices[ i - 1 ]	].pos;
		p2 = srvVertices[ srvIndices[ i ]		].pos;

		curIntersection = intersectRayTriangle( ray.pos, ray.dir, p0, p1, p2, i );
		if( curIntersection.primId<0 || ray.primID==curIntersection.primId ) { // Not an intersection or self-intersection.
			continue;
		}
		if( curIntersection.dist<closestIntersection.dist ) { // Closer than last intersection
			closestIntersection = curIntersection;
		}
	}

	uavIntersections[ pixelIndex ] = closestIntersection;
}

#endif // DV2520_CSRAYSINTERSECT_FX