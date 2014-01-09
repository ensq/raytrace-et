#ifndef DV2520_CSLIGHTING_FX
#define DV2520_CSLIGHTING_FX

#include <Common.fx>

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
	unsigned int pixelIndex = gThreadId.y * screenWidth + gThreadId.x;

	Intersection intersection = uavIntersections[ pixelIndex ];
	if( intersection.primId>=0 ) {
		Vertex v1 = srvVertices[ intersection.primVertexOffset + srvIndices[ intersection.primId - 2 ] ];
		Vertex v2 = srvVertices[ intersection.primVertexOffset + srvIndices[ intersection.primId - 1 ] ];
		Vertex v3 = srvVertices[ intersection.primVertexOffset + srvIndices[ intersection.primId ] ];

		float2 uv = intersection.u * v2.tex + intersection.v * v3.tex + ( 1.0f - ( intersection.u + intersection.v ) ) * v1.tex;
		uavBackbuffer[ gThreadId.xy ] = texAlbedo.SampleLevel( ssDefault, uv, 0 );
	}
}

#endif // DV2520_CSLIGHTING_FX

// Temp:
//float test = intersection.primId / 36.0f;
//if( test <= 1.0f ) {
//	uavBackbuffer[ gThreadId.xy ] = float4( 1.0f, 0.0f, 0.0f, 1.0f );
//} else if( test <= 2.0f ) {
//	uavBackbuffer[ gThreadId.xy ] = float4( 0.0f, 1.0f, 0.0f, 1.0f );
//} else if( test <= 3.0f ) {
//	uavBackbuffer[ gThreadId.xy ] = float4( 0.0f, 0.0f, 1.0f, 1.0f );
//} else if( test <= 4.0f ) {
//	uavBackbuffer[ gThreadId.xy ] = float4( 1.0f, 0.0f, 0.0f, 1.0f );
//} else if( test <= 5.0f ) {
//	uavBackbuffer[ gThreadId.xy ] = float4( 0.0f, 1.0f, 0.0f, 1.0f );
//}