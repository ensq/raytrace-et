#ifndef DV2520_CSCOMBINE_FX
#define DV2520_CSCOMBINE_FX

#define BLOCK_SIZE 16

// t
Texture2D old : register(t6);

// u
RWTexture2D<float4> com : register(u4);

// s
SamplerState ssDefault : register(s0);

// b
cbuffer CbPerInstance : register( b0 ) {
    uint screenWidth;
    uint screenHeight;
    float aspect;
    float fov;
};

[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void main(uint3 gThreadId : SV_DispatchThreadID) {
    //const uint pixelIdx = gThreadId.y * screenWidth + gThreadId.x;

    float2 co = float2((float)gThreadId.x / (float)screenWidth,
                             (float)gThreadId.y / (float)screenHeight);
    com[gThreadId.xy] = old.SampleLevel(ssDefault, co, 0); //old[gThreadId.xy];
}

#endif // DV2520_CSCOMBINE_FX
