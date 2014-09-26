#ifndef DV2520_CSLIGHTING_FX
#define DV2520_CSLIGHTING_FX

#include <Common.fx>
#include <Lighting.fx>
#include <Intersection.fx>

Ray rayReflect(Ray p_ray, LightSurface p_sur, int p_primId) {
    Ray r = ConstructRay(p_sur.pos, normalize(reflect(p_ray.dir, p_sur.nor)));
    r.ref = p_ray.ref * 0.1f; //p_sur.specular.w;
    r.primId = p_primId;
    return r;
}

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
    if(gThreadId.x>=fovWidth || gThreadId.y>=fovHeight) {
        return;
    }
    const uint pixelIdx = gThreadId.y * fovWidth + gThreadId.x;
    Intersection i = uavIntersections[ pixelIdx ];
    if( i.primId<0) {
        return;
    }
    ObjInstance o = srvInstances[ i.instanceIdx ];
    
    Vertex v1 = srvVertices[ i.primVertexOffset + srvIndices[i.primId - 2]];
    Vertex v2 = srvVertices[ i.primVertexOffset + srvIndices[i.primId - 1]];
    Vertex v3 = srvVertices[ i.primVertexOffset + srvIndices[i.primId - 0]];

    float3 pPos = i.u * v2.pos + i.v * v3.pos + ( 1.0f - ( i.u + i.v ) ) * v1.pos;
    pPos = mul( o.world, float4( pPos, 1.0f ) ).xyz;
    float3 pNor = i.u * v2.nor + i.v * v3.nor + ( 1.0f - ( i.u + i.v ) ) * v1.nor;
    pNor = mul( o.world, float4( pNor, 0.0f ) ).xyz;
    pNor = normalize( pNor );
    float2 pTex = i.u * v2.tex + i.v * v3.tex + ( 1.0f - ( i.u + i.v ) ) * v1.tex;
    float3 toEye = normalize( pos - pPos );

    LightSurface pSurface = ConstructLightSurface(pPos, pNor,
                 /*Ambient*/ float4(1.0f, 1.0f, 1.0f, 1.0f),
                 /*Diffuse*/ texAlbedo.SampleLevel(ssDefault, pTex, 0),
                /*Spec+fac*/ float4(1.0f, 1.0f, 1.0f, 1.0f));

    float4 pAmbient = float4( 0.0f, 0.0f, 0.0f, 0.0f );
    float4 pDiffuse = float4( 0.0f, 0.0f, 0.0f, 0.0f );
    float4 pSpecular = float4( 0.0f, 0.0f, 0.0f, 0.0f );
    for( uint lightIdx = 0; lightIdx<lightsCnt; lightIdx++ ) {
        float3 lightPos = srvLights[lightIdx].pos;
        
        Ray rLight = ConstructRay(pPos, normalize(lightPos - pPos));
        rLight.distMax = length(lightPos - rLight.pos);
        rLight.primId = i.primId;
        Intersection iSha = intersectObjectInstances(rLight, rLight);
        if(iSha.primId>0) { // The point is in shadow.
            pAmbient += srvLights[lightIdx].ambient;
        } else { // Otherwise, apply point light.
            float4 ioAmbient, ioDiffuse, ioSpecular;
            LightingPoint(srvLights[ lightIdx ],
                          pSurface, 
                          toEye,
                          ioAmbient, ioDiffuse, ioSpecular );
            pAmbient += ioAmbient;
            pDiffuse += ioDiffuse;
            pSpecular += ioSpecular;
        }
    }
    pAmbient.xyz += pDiffuse.xyz + pSpecular.xyz;

    Ray r = uavRays[pixelIdx];
    uavColor[pixelIdx] += float4(pAmbient.xyz, 0.0f) * r.ref;
    uavRays[pixelIdx] = rayReflect(r, pSurface, i.primId);
    
    uavBackbuffer[gThreadId.xy] = uavColor[pixelIdx];
}

#endif // DV2520_CSLIGHTING_FX
