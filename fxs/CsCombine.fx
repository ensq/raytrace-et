#ifndef DV2520_CSCOMBINE_FX
#define DV2520_CSCOMBINE_FX

#define BLOCK_SIZE 16

// t
Texture2D old : register(t6);

// u
RWTexture2D<float4> com : register(u4); 

[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void main(uint3 gThreadId : SV_DispatchThreadID) {
    //const uint pixelIdx = gThreadId.y * screenWidth + gThreadId.x;

    com[gThreadId.xy] = old[gThreadId.xy];
}

#endif // DV2520_CSCOMBINE_FX
