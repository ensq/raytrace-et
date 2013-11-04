RWTexture2D< float4 > backbuffer : register( u0 );

#define BLOCK_SIZE 16

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID )
{
	backbuffer[ gThreadId.xy ] = float4( 0.0f, 1.0f, 0.0f, 1.0f );
}