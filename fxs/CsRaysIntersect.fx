#ifndef DV2520_CSRAYSINTERSECT_FX
#define DV2520_CSRAYSINTERSECT_FX

#include <Common.fx>

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
    const uint pixelIdx = gThreadId.y * screenWidth + gThreadId.x;
    Ray    ray    = uavRays[ pixelIdx ];
    
    Intersection curIntersection = ConstructIntersection();
    Intersection closestIntersection = ConstructIntersection();
    closestIntersection.dist = ray.distMax;

    uint indicesOffset = 0;
    uint verticesOffset = 0;
    for( uint instancesIdx = 0; instancesIdx<instancesCnt; instancesIdx++ ) {
        float3 p0, p1, p2;
        uint indicesCnt = srvInstances[ instancesIdx ].indexCnt;
        for( uint vertexIdx = 2; vertexIdx<indicesCnt; vertexIdx += 3 ) {
            float3 rayDir = mul( srvInstances[ instancesIdx ].worldInv, float4( ray.dir, 0.0f ) ).xyz;
            float3 rayPos = mul( srvInstances[ instancesIdx ].worldInv, float4( ray.pos, 1.0f ) ).xyz;

            uint indexId = indicesOffset + vertexIdx;
            p0 = srvVertices[ verticesOffset + srvIndices[ indexId - 2 ]    ].pos;
            p1 = srvVertices[ verticesOffset + srvIndices[ indexId - 1 ]    ].pos;
            p2 = srvVertices[ verticesOffset + srvIndices[ indexId ]        ].pos;
            curIntersection = intersectRayTriangleRealTimeRendering( rayPos, rayDir, p0, p1, p2, indexId );
            
            if( curIntersection.primId>=0 && 
                ray.primId!=curIntersection.primId &&
                curIntersection.dist<closestIntersection.dist ) {
                // Update closest intersection with new data:
                closestIntersection = curIntersection;
                closestIntersection.primVertexOffset = verticesOffset;
                closestIntersection.instanceIdx = instancesIdx;

                ray.primId = curIntersection.primId;
            }
        }
        indicesOffset += srvInstances[ instancesIdx ].indexCnt;
        verticesOffset += srvInstances[ instancesIdx ].vertexCnt;
    }

    uavRays[ pixelIdx ] = ray;
    uavIntersections[ pixelIdx ] = closestIntersection;
}

#endif // DV2520_CSRAYSINTERSECT_FX