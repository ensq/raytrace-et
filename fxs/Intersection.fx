#ifndef DV2520_INTERSECTION_FX
#define DV2520_INTERSECTION_FX

#include <structs.fx>
#include <Common.fx>

#define REAL_TIME_RENDERING_EPSILON 0.00001f
Intersection intersectRayTriangleRTR(float3 p_rOrigin,
                                     float3 p_rDir,
                                     float3 p_p0,
                                     float3 p_p1,
                                     float3 p_p2,
                                     uint p_primId) {
    Intersection i = ConstructIntersection();
    
    float3 e1 = p_p1 - p_p0;
    float3 e2 = p_p2 - p_p0;
    float3 q = cross( p_rDir, e2 );
    float a = dot( e1, q );
    if( a>-REAL_TIME_RENDERING_EPSILON && a<REAL_TIME_RENDERING_EPSILON ) {
        return i; // No intersection.
    }
    float f = 1.0f / a;
    float3 s = p_rOrigin - p_p0;
    i.u = f * dot( s, q );
    if( i.u<0.0f ) {
        return i; // No intersection.
    }
    float3 r = cross( s, e1 );
    i.v = f * dot( p_rDir, r );
    if( i.v<0.0f || ( i.u+i.v )>1.0f ) {
        return i; // No intersection.
    }
    i.t = f * dot( e2, r );
    if( i.t>=0 ) {
        i.dist = i.t;
        i.primId = p_primId;
    } // False intersection.
    
    return i;
}
// Implemented in accordance to the ray/triangle-algorithm in
// Real-Time Rendering, which is in-and-of-it's-own derived from
// Möller & Trumbore's algorithm.

// Consider returning lone primId number rather than entire Ray structure.
Intersection intersectObjectInstances(Ray p_ray, out Ray io_ray) {
    Intersection iCur = ConstructIntersection();
    Intersection iClo = ConstructIntersection();
    iClo.dist = p_ray.distMax;

    uint indicesOfs = 0;
    uint verticesOfs = 0;
    for(uint instancesIdx = 0; instancesIdx<instancesCnt; instancesIdx++) {
        uint indicesCnt = srvInstances[instancesIdx].indexCnt;
        float3 rayDir = mul(srvInstances[instancesIdx].worldInv,
                            float4(p_ray.dir, 0.0f)).xyz;
        float3 rayPos = mul(srvInstances[instancesIdx].worldInv,
                            float4(p_ray.pos, 1.0f)).xyz;
        for(uint vertexIdx = 2; vertexIdx<indicesCnt; vertexIdx += 3) {
            uint indexId = indicesOfs + vertexIdx;
            float3 p0 = srvVertices[verticesOfs + srvIndices[indexId - 2]].pos;
            float3 p1 = srvVertices[verticesOfs + srvIndices[indexId - 1]].pos;
            float3 p2 = srvVertices[verticesOfs + srvIndices[indexId - 0]].pos;
            iCur = intersectRayTriangleRTR(rayPos, rayDir, p0, p1, p2, indexId);
            if(iCur.primId>=0
               && p_ray.primId!=iCur.primId
               && iCur.dist<iClo.dist) {
                iClo = iCur;
                iClo.primVertexOffset = verticesOfs;
                iClo.instanceIdx = instancesIdx;
                p_ray.primId = iClo.primId;
            }
        }
        indicesOfs += srvInstances[instancesIdx].indexCnt;
        verticesOfs += srvInstances[instancesIdx].vertexCnt;
    }
    io_ray = p_ray;
    return iClo;
}

#endif // DV2520_INTERSECTION_FX
