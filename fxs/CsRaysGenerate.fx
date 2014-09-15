#ifndef DV2520_CSRAYSGENERATE_FX
#define DV2520_CSRAYSGENERATE_FX

#include <Common.fx>

float3 getNormalizedScreenCoordinates( uint3 screenPos ) {
    float x = screenPos.x;
    float y = screenPos.y;
    float halfWidth = screenWidth / 2.0f;
    float halfHeight = screenHeight / 2.0f;

    float dx = tan( fov * 0.5f ) * ( x / halfWidth - 1.0f ) / aspect;
    float dy = tan( fov * 0.5f ) * ( 1.0f - y / halfHeight );
    float dz = 1.0f;

    return float3( dx, dy, dz );
}

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
    const uint pixelIndex = gThreadId.y * screenWidth + gThreadId.x;
    
    float3 pixelPos    = getNormalizedScreenCoordinates( gThreadId );
    pixelPos = mul( float4( pixelPos, 1.0f ), viewInv ).xyz;
    float4 aux = mul( float4( 0.0f, 0.0f, 0.0f, 1.0f ), viewInv );

    float3 rayPos = aux.xyz / aux.w;
    uavRays[ pixelIndex ] = ConstructRay( rayPos, normalize( pixelPos - rayPos ) );;
}

#endif // DV2520_CSRAYSGENERATE_FX