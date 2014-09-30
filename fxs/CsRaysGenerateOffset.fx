#ifndef DV2520_CSRAYSGENERATEOFFSET_FX
#define DV2520_CSRAYSGENERATEOFFSET_FX

#include <Common.fx>

[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void main(uint3 gThreadId : SV_DispatchThreadID) {
    RETURN_BOUNDS(gThreadId.x, gThreadId.y, fovWidth, fovHeight)
    const uint pixelIdx = gThreadId.y * fovWidth + gThreadId.x;
    
    float3 pixelPos = getNormalizedScreenCoordinates(gThreadId.xy,
                                                     screenWidth, screenHeight);
    pixelPos = mul(float4(pixelPos, 1.0f), viewInv).xyz;
    float4 aux = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), viewInv);

    float3 rayPos = aux.xyz / aux.w;
    uavRays[pixelIdx] = ConstructRay(rayPos, normalize(pixelPos - rayPos));
    uavColor[pixelIdx] = float4(0.0f, 0.0f, 0.0f, 1.0f);
}

#endif // DV2520_CSRAYSGENERATEOFFSET_FX
