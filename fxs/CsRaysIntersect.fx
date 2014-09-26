#ifndef DV2520_CSRAYSINTERSECT_FX
#define DV2520_CSRAYSINTERSECT_FX

#include <Common.fx>
#include <Intersection.fx>

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
    if(gThreadId.x>=fovWidth || gThreadId.y>=fovHeight) {
        return;
    }
    const uint pixelIdx = gThreadId.y * fovWidth + gThreadId.x;

    Ray ray = uavRays[ pixelIdx ];
    uavIntersections[ pixelIdx ] = intersectObjectInstances(ray, ray);
    uavRays[ pixelIdx ] = ray;
}

// Bvh test:
// Ray r = ray;
// r.dir = rayDir;
// r.pos = rayPos;
// curIntersection = intersectBvh(r);
// closestIntersection = curIntersection;
// ray.primId = closestIntersection.primId;

#endif // DV2520_CSRAYSINTERSECT_FX
