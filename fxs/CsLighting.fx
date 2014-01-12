#ifndef DV2520_CSLIGHTING_FX
#define DV2520_CSLIGHTING_FX

#include <Common.fx>

[ numthreads( BLOCK_SIZE, BLOCK_SIZE, 1 ) ]
void main( uint3 gThreadId : SV_DispatchThreadID ) {
	const uint pixelIndex = gThreadId.y * screenWidth + gThreadId.x;
	Intersection i = uavIntersections[ pixelIndex ];
	if( i.primId<0 ) {
		return;
	}
	ObjInstance o = srvInstances[ i.instanceIdx ];
	
	Vertex v1 = srvVertices[ i.primVertexOffset + srvIndices[ i.primId - 2 ] ];
	Vertex v2 = srvVertices[ i.primVertexOffset + srvIndices[ i.primId - 1 ] ];
	Vertex v3 = srvVertices[ i.primVertexOffset + srvIndices[ i.primId ] ];

	float3 pPos = i.u * v2.pos + i.v * v3.pos + ( 1.0f - ( i.u + i.v ) ) * v1.pos;
	pPos = mul( o.world, float4( pPos, 1.0f ) ).xyz;
	float3 pNor = i.u * v2.nor + i.v * v3.nor + ( 1.0f - ( i.u + i.v ) ) * v1.nor;
	pNor = mul( o.world, float4( pNor, 0.0f ) ).xyz;
	pNor = normalize( pNor );
	float2 pTex = i.u * v2.tex + i.v * v3.tex + ( 1.0f - ( i.u + i.v ) ) * v1.tex;
	
	float3 toEye = pPos - pos;

	LightSurface pSurface = ConstructLightSurface( 
		pPos,
		pNor,
		/*Ambient*/		float4( 1.0f, 1.0f, 1.0f, 1.0f ),
		/*Diffuse*/		texAlbedo.SampleLevel( ssDefault, pTex, 0 ),
		/*Specular*/	float4( 1.0f, 1.0f, 1.0f, 1.0f ) );

	float4 pAmbient = float4( 0.0f, 0.0f, 0.0f, 0.0f );
	float4 pDiffuse = float4( 0.0f, 0.0f, 0.0f, 0.0f );
	float4 pSpecular = float4( 0.0f, 0.0f, 0.0f, 0.0f );
	for( uint lightIdx = 0; lightIdx<lightsCnt; lightIdx++ ) {
		float4 ioAmbient, ioDiffuse, ioSpecular;
		LightingPoint( 
			srvLights[ lightIdx ],
			pSurface, 
			toEye,
			ioAmbient, ioDiffuse, ioSpecular );
		pAmbient += ioAmbient;
		pDiffuse += ioDiffuse;
		pSpecular += ioSpecular;
	}
	pAmbient.xyz += pDiffuse.xyz + pSpecular.xyz;

	uavBackbuffer[ gThreadId.xy ] = float4( pAmbient.xyz, 1.0f );
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