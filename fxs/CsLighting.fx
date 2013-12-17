#ifndef DV2520_CSLIGHTING_FX
#define DV2520_CSLIGHTING_FX

#include <Common.fx>

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
	unsigned int pixelIndex = gThreadId.y * screenWidth + gThreadId.x;

	Intersection intersection = uavIntersections[ pixelIndex ];
	if( intersection.primId!=-1 ) {
		const uint offset = intersection.primId * 3;
		Vertex v1 = srvVertices[ srvIndices[ intersection.primId + 0 ] ];
		Vertex v2 = srvVertices[ srvIndices[ intersection.primId + 1 ] ];
		Vertex v3 = srvVertices[ srvIndices[ intersection.primId + 2 ] ];

		float2 uv = intersection.u * v2.tex + intersection.v * v3.tex + ( 1.0f - ( intersection.u + intersection.v ) ) * v1.tex;
		uavBackbuffer[ gThreadId.xy ] = texAlbedo.SampleLevel( ssDefault, uv, 0 );
	} else {
		uavBackbuffer[ gThreadId.xy ] = float4( 1.0f, 0.0f, 0.0f, 1.0f );
	}
}

#endif // DV2520_CSLIGHTING_FX