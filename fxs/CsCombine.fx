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
cbuffer CbPerFov : register( b2 ) {
    row_major float4x4 proj;
    row_major float4x4 projInv;
    
    uint fovWidth;
    uint fovHeight;
    uint fovOfsX;
    uint fovOfsY;

    float fov;
    float aspect;
    float2 pad2;
};

[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void main(uint3 gThreadId : SV_DispatchThreadID) {
    float x = (float)gThreadId.x + fovOfsX;
    float y = (float)gThreadId.y + fovOfsY;
    float2 co = float2(gThreadId.x / (float)fovWidth,
                       gThreadId.y / (float)fovHeight);
    com[float2(x,y)] = old.SampleLevel(ssDefault, co, 0);
}

#endif // DV2520_CSCOMBINE_FX
