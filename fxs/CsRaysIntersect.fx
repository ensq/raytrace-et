#ifndef DV2520_CSRAYSINTERSECT_FX
#define DV2520_CSRAYSINTERSECT_FX

#include <Common.fx>

float2 intersectBvhBBox(float3 p_rOrigin, float3 p_rDirInv, uint p_nodeIdx) {
    float2 T;

    float3 diffMin = srvNodes[p_nodeIdx].min - p_rOrigin;
    diffMin *= p_rDirInv;
    float3 diffMax = srvNodes[p_nodeIdx].max - p_rOrigin;
    diffMax *= p_rDirInv;

    T[0] = min(diffMin.x, diffMax.x);
    T[1] = max(diffMin.x, diffMax.x);
    
    T[0] = max(T[0], min(diffMin.y, diffMax.y));
    T[1] = min(T[1], max(diffMin.y, diffMax.y));

    T[0] = max(T[0], min(diffMin.z, diffMax.z));
    T[1] = min(T[1], max(diffMin.z, diffMax.z));

    if(T[0]>T[1]) {
        T[0] = T[1] = -1.0f;
    }
    return T;
}

Intersection intersectBvh(Ray p_ray) {
    float3 rDirInv = float3(1.0f / p_ray.dir);

    int stack[35];
    int stackIdx = 0;
    int nodeIdx = 0;

    Intersection iCur = ConstructIntersection();
    Intersection iClo = ConstructIntersection();
    iClo.dist = p_ray.distMax;
    
    while(true) {
        float2 T = intersectBvhBBox(p_ray.pos, rDirInv, nodeIdx);
        if(T[0]>iClo.dist || T[1]<0.0f) { // No intersection of the node bbox.
            if(stackIdx==0) { // Traversal ends if the stack is empty.
                break;
            }
            nodeIdx = stack[--stackIdx]; // Pop new node from the stack.
        } else if(srvNodes[nodeIdx].primitivesCnt>0) { // A leaf node bbox is intersected.
            for(int i = 0; i<srvNodes[nodeIdx].primitivesCnt; i++) {
                int offset = (srvNodes[nodeIdx].primitivesIdx * 3) + (i * 3);
                float3 A = srvVertices[srvIndices[offset+0]].pos;
                float3 B = srvVertices[srvIndices[offset+1]].pos;
                float3 C = srvVertices[srvIndices[offset+2]].pos;
                iCur = intersectRayTriangleRealTimeRendering(p_ray.pos,
                                                             p_ray.dir, A, B, C,
                                                             offset + 2);
                if(iCur.primId>0
                   && p_ray.primId!=iCur.primId
                   && iCur.dist<iClo.dist) {
                    iClo = iCur;
                    iClo.primVertexOffset = 0; // Must be updated if more than 1 object.
                    iClo.instanceIdx = 0;
                }
            }
            if(stackIdx==0) {
                break;
            }                                    
            nodeIdx = stack[--stackIdx];
        } else { // A branch node bbox is intersected.
            stack[stackIdx++] = nodeIdx + 1; // Add first child to stack.
            stack[stackIdx++] = srvNodes[nodeIdx].primitivesIdx; // ...and the secod child.
            nodeIdx = stack[--stackIdx];
        }
    }
    return iClo;
}

// Depending on the ray direction and the split-axis,
// the order of the children changes on the stack.
//int dirIsNeg[3] = { rDirInv < 0 };
// -g_sNodes[nodeIdx].nPrimitives is the split axis: 0-1-2 (x-y-z)
//const int aux = dirIsNeg[-srvNodes[nodeIdx].primitivesCnt];
// aux replaces an if/else statement which improves traversal a little bit
//stack[stackIdx++] = (nodeIdx+1)*aux + (1-aux)*srvNodes[nodeIdx].primitivesIdx;
//nodeIdx = srvNodes[nodeIdx].primitivesIdx*aux + (1-aux)*(nodeIdx+1);

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
    const uint pixelIdx = gThreadId.y * screenWidth + gThreadId.x;
    Ray ray = uavRays[ pixelIdx ];
    
    Intersection curIntersection = ConstructIntersection();
    Intersection closestIntersection = ConstructIntersection();
    closestIntersection.dist = ray.distMax;

    uint indicesOffset = 0;
    uint verticesOffset = 0;
    for( uint instancesIdx = 0; instancesIdx<instancesCnt; instancesIdx++ ) {
        float3 p0, p1, p2;
        uint indicesCnt = srvInstances[ instancesIdx ].indexCnt;
        float3 rayDir = mul( srvInstances[ instancesIdx ].worldInv,
                             float4( ray.dir, 0.0f ) ).xyz;
        float3 rayPos = mul( srvInstances[ instancesIdx ].worldInv,
                             float4( ray.pos, 1.0f ) ).xyz;

        Ray r = ray;
        r.dir = rayDir;
        r.pos = rayPos;
        curIntersection = intersectBvh(r);
        closestIntersection = curIntersection;
        ray.primId = closestIntersection.primId;
                
        /* for( uint vertexIdx = 2; vertexIdx<indicesCnt; vertexIdx += 3 ) { */
        /*     uint indexId = indicesOffset + vertexIdx; */
        /*     p0 = srvVertices[ verticesOffset + srvIndices[ indexId - 2 ]    ].pos; */
        /*     p1 = srvVertices[ verticesOffset + srvIndices[ indexId - 1 ]    ].pos; */
        /*     p2 = srvVertices[ verticesOffset + srvIndices[ indexId ]        ].pos; */
        /*     curIntersection = intersectRayTriangleRealTimeRendering( rayPos, rayDir, p0, p1, p2, indexId ); */
            
        /*     if( curIntersection.primId>=0 && */
        /*         ray.primId!=curIntersection.primId && */
        /*         curIntersection.dist<closestIntersection.dist ) { */
        /*         // Update closest intersection with new data: */
        /*         closestIntersection = curIntersection; */
        /*         closestIntersection.primVertexOffset = verticesOffset; */
        /*         closestIntersection.instanceIdx = instancesIdx; */

        /*         ray.primId = curIntersection.primId; */
        /*     } */
        /* } */
        indicesOffset += srvInstances[ instancesIdx ].indexCnt;
        verticesOffset += srvInstances[ instancesIdx ].vertexCnt;
    }

    uavRays[ pixelIdx ] = ray;
    uavIntersections[ pixelIdx ] = closestIntersection;
}

#endif // DV2520_CSRAYSINTERSECT_FX
