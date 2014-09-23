#ifndef DV2520_INTERSECTION_FX
#define DV2520_INTERSECTION_FX

struct Intersection {
    float u;
    float v;
    float t; // Consider removing. May easily be calculated.

    int primId; // primitive id, currently index of the last primitive
                // index plus index offset.
    uint primVertexOffset;
    uint instanceIdx;

    float dist; // distance from origin to intersection.
};
Intersection ConstructIntersection() {
    Intersection i;
    i.u = i.v = i.t = i.dist = -1;
    i.primId = -1;
    i.primVertexOffset = 0;

    return i;
}

#define REAL_TIME_RENDERING_EPSILON 0.00001f
Intersection intersectRayTriangleRealTimeRendering(float3 p_rOrigin,
                                                   float3 p_rDir,
                                                   float3 p_p0,
                                                   float3 p_p1,
                                                   float3 p_p2,
                                                   uint p_primId ) {
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

//#define MOLLER_TROMBORE_CULL_BACKFACE
#define MOLLER_TROMBORE_EPSILON 0.000001f
Intersection intersectRayTriangleMollerTrumbore(float3 p_rOrigin,
                                                float3 p_rDir,
                                                float3 p_p0,
                                                float3 p_p1,
                                                float3 p_p2,
                                                uint p_primId ) {
    // Obs. Erronous implementation. Only works half-heartedly. Obs.
    Intersection i = ConstructIntersection();
    float3 e1, e2, tvec, pvec, qvec;
    float det, inv_det;

    e1 = p_p1 - p_p0; // Find vectors for two edges sharing p0.
    e2 = p_p2 - p_p0;
    pvec = cross( p_rDir, e2 ); // Calculate determinant. Also used to
                                // establish U.
    det = dot( e1, pvec ); // If determinant is near zero, then ray
                           // lies in the plane of the triangle.

#if defined( MOLLER_TROMBORE_CULL_BACKFACE )
    if( det<MOLLER_TROMBORE_EPSILON ) {
        return i;
    }

    tvec = p_rOrigin - p_p0; // Calculate distance from p0 to ray origin.
    i.u = dot( tvec, pvec ); // Establish U and test bounds.
    if( i.u<0.0f || i.u>det ) { 
        return i;
    }

    qvec = cross( tvec, e1 ); // Test V.
    i.v = dot( p_rDir, qvec );
    if( i.v<0.0 || ( i.u + i.v )>det ) {
        return i;
    }

    i.t = dot( e2, qvec ); // If the execution path has got this far,
                           // then the ray does intersect the
                           // triangle. Calculate T and scale
                           // parameters.
    inv_det = 1.0f / det;
    i.t *= inv_det;
    i.u *= inv_det;
    i.v *= inv_det;
#else
    if( det>-MOLLER_TROMBORE_EPSILON && det<MOLLER_TROMBORE_EPSILON ) {
        return i;
    }
    inv_det = 1.0f / det;

    tvec = p_rOrigin - p_p0; // Calculate distance from p0 to ray origin.
    i.u = dot( tvec, pvec ) * inv_det; // Establish U and test bounds.
    if( i.u<0.0f || i.u>1.0f ) {
        return i;
    }

    qvec = cross( tvec, e1 ); // Calculate V and test bounds.
    i.v = dot( p_rDir, qvec ) * inv_det;
    if( i.v<0.0f || ( i.u + i.v )>1.0f ) {
        return i;
    }

    i.t = dot( e2, qvec ) * inv_det; // If the execution path has got
                                     // this far, then the ray does
                                     // intersect the
                                     // triangle. Calculate T.
#endif // MOLLER_TROMBORE_CULL_BACKFACE
    i.dist = length( p_rOrigin - ( i.u * p_p0 + i.v * p_p1 + i.t * p_p2 ) );
    i.primId = p_primId; // Signifies intersection.
    return i;
}
// Implemented in accordance to Fast, Minimum Storage Ray/Triangle
// Intersection by Möller and Trumbore.

// float2 intersectBvhBBox(float3 p_rOrigin, float3 p_rDirInv, uint p_nodeIdx) {
//     float2 T;
// 
//     float3 diffMin = srvNodes[p_nodeIdx].min - p_rOrigin;
//     diffMin *= p_rDirInv;
//     float3 diffMax = srvNodes[p_nodeIdx].max - p_rOrigin;
//     diffMax *= p_rDirInv;
// 
//     T[0] = min(diffMin.x, diffMax.x);
//     T[1] = max(diffMin.x, diffMax.x);
//     
//     T[0] = max(T[0], min(diffMin.y, diffMax.y));
//     T[1] = min(T[1], max(diffMin.y, diffMax.y));
// 
//     T[0] = max(T[0], min(diffMin.z, diffMax.z));
//     T[1] = min(T[1], max(diffMin.z, diffMax.z));
// 
//     if(T[0]>T[1]) {
//         T[0] = T[1] = -1.0f;
//     }
//     return T;
// }
// 
// Intersection intersectBvh(Ray p_ray) {
//     float3 rDirInv = float3(1.0f / p_ray.dir);
// 
//     int stack[35];
//     int stackIdx = 0;
//     int nodeIdx = 0;
// 
//     Intersection iCur = ConstructIntersection();
//     Intersection iClo = ConstructIntersection();
//     iClo.dist = p_ray.distMax;
//     
//     while(true) {
//         float2 T = intersectBvhBBox(p_ray.pos, rDirInv, nodeIdx);
//         if(T[0]>iClo.dist || T[1]<0.0f) { // No intersection of the node bbox.
//             if(stackIdx==0) { // Traversal ends if the stack is empty.
//                 break;
//             }
//             nodeIdx = stack[--stackIdx]; // Pop new node from the stack.
//         } else if(srvNodes[nodeIdx].primitivesCnt>0) { // A leaf node bbox is intersected.
//             // bbox test:
//             //iClo.primId = 3;
//             //break;
// 
//             for(int i = 0; i<srvNodes[nodeIdx].primitivesCnt; i++) {
//                 int offset = (srvNodes[nodeIdx].primitivesIdx * 3) + (i * 3);
//                 float3 A = srvVertices[srvIndices[offset+0]].pos;
//                 float3 B = srvVertices[srvIndices[offset+1]].pos;
//                 float3 C = srvVertices[srvIndices[offset+2]].pos;
//                 iCur = intersectRayTriangleRealTimeRendering(p_ray.pos,
//                                                              p_ray.dir, A, B, C,
//                                                              offset + 2);
//                 if(iCur.primId>0
//                    && p_ray.primId!=iCur.primId
//                    && iCur.dist<iClo.dist) {
//                     iClo = iCur;
//                     iClo.primVertexOffset = 0; // Must be updated if more than 1 object.
//                     iClo.instanceIdx = 0;
//                 }
//             }
//             if(stackIdx==0) {
//                 break;
//             }
//             nodeIdx = stack[--stackIdx];
//         } else { // A branch node bbox is intersected.
//             stack[stackIdx++] = nodeIdx + 1; // Add first child to stack.
//             stack[stackIdx++] = srvNodes[nodeIdx].primitivesIdx; // ...and the secod child.
//             nodeIdx = stack[--stackIdx];
//         }
//     }
//     return iClo;
// }

// Depending on the ray direction and the split-axis,
// the order of the children changes on the stack.
//int dirIsNeg[3] = { rDirInv < 0 };
// -g_sNodes[nodeIdx].nPrimitives is the split axis: 0-1-2 (x-y-z)
//const int aux = dirIsNeg[-srvNodes[nodeIdx].primitivesCnt];
// aux replaces an if/else statement which improves traversal a little bit
//stack[stackIdx++] = (nodeIdx+1)*aux + (1-aux)*srvNodes[nodeIdx].primitivesIdx;
//nodeIdx = srvNodes[nodeIdx].primitivesIdx*aux + (1-aux)*(nodeIdx+1);

#endif // DV2520_INTERSECTION_FX
