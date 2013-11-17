#ifndef DV2520_CSRAYSINTERSECT_FX
#define DV2520_CSRAYSINTERSECT_FX

#include <Common.fx>

float epsilon = pow( 10, -5 ); // wtf
Intersection intersectRayTriangle( float3 rOrigin, float3 rDir, float3 p0, float3 p1, float3 p2, unsigned int primId ) {
	Intersection intersection;
	intersection.primId = -1;

	float3 e1 = p1 - p0;
	float3 e2 = p2 - p0;
	float3 q = cross( rDir, e2 );
	float a	= dot( e1, q );
	if( a>-epsilon && a<epsilon) {
		return intersection;
	}

	float f	= 1.0f / a;
	float3 s = rOrigin - p0;
	float u	= f * dot( s, q );
	if( u<0.0f ) {
		return intersection;
	}

	float3 r = cross( s, e1 );
	float v = f * dot( rDir, r );
	if( v<0.0f || (u + v)>1.0f) {
		return intersection;
	}

	// Barycentric coordinates: u, v, t
	float t = f * dot( e2, r );

	intersection.world = u * p0 + v * p1 + t * p2;
	intersection.dist = length( rOrigin - intersection.world );
	intersection.primId = primId;

	return intersection;
}

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
	unsigned int pixelIndex = gThreadId.y * screenWidth + gThreadId.x;
	
	Ray	ray	= uavRays[ pixelIndex ];
	
	Intersection closestIntersection;
	closestIntersection.dist = ray.distMax;
	closestIntersection.primId = -1;

	float3	p0, p1, p2;
	Intersection curIntersection;
	for( uint i = 2; i<numIndices; i += 3 ) {
		p0 = srvVertices[ srvIndices[ i - 2 ]	].pos;
		p1 = srvVertices[ srvIndices[ i - 1 ]	].pos;
		p2 = srvVertices[ srvIndices[ i ]		].pos;

		curIntersection = intersectRayTriangle( ray.pos, ray.dir, p0, p1, p2, i );
		if( curIntersection.primId != -1 ) { // Intersection
			if( ray.primID!=curIntersection.primId ) { // Not self-intersection
				if( curIntersection.dist<closestIntersection.dist ) { // Closer than last intersection
					closestIntersection = curIntersection;
				}
			}
		}
	}

	uavIntersections[ pixelIndex ] = closestIntersection;
}

#endif // DV2520_CSRAYSINTERSECT_FX