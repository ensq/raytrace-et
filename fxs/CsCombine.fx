#ifndef DV2520_CSCOMBINE_FX
#define DV2520_CSCOMBINE_FX

#include <Common.fx>

[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void main(uint3 gThreadId : SV_DispatchThreadID) {
    float x = (float)gThreadId.x + fovOfsX;
    float y = (float)gThreadId.y + fovOfsY;
    float2 co = float2(gThreadId.x / (float)fovWidth,
                       gThreadId.y / (float)fovHeight);
    com[float2(x,y)] = old.SampleLevel(ssDefault, co, 0);
}

#endif // DV2520_CSCOMBINE_FX
