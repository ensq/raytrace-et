#ifndef DV2520_CSLIGHTING_FX
#define DV2520_CSLIGHTING_FX

#include <Common.fx>

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
	unsigned int pixelIndex = gThreadId.y * screenWidth + gThreadId.x;

	//Ray ray = rays[pixel];
	Intersection intersection = uavIntersections[ pixelIndex ];

	if( intersection.primId!=-1 ) {
		uavBackbuffer[ gThreadId.xy ] = float4( 0.0f, 1.0f, 0.0f, 1.0f );
	} else {
		uavBackbuffer[ gThreadId.xy ] = float4( 1.0f, 0.0f, 0.0f, 1.0f );
	}
}

#endif // DV2520_CSLIGHTING_FX