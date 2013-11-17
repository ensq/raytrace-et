#include <stdafx.h>

#include <Dx.h>
#include <Win.h>
#include <Ray.h>
#include <CogFx.h>
#include <CogCb.h>
#include <CogD3d.h>
#include <BufUav.h>
#include <Intersection.h>

#include <Vertex.h> // Remove me.

Dx::Dx( Win& p_win ) {
	m_win = &p_win;

	m_uavRays = nullptr;

	m_uavIntersections = nullptr;
	m_cogD3d = nullptr;
	m_cogFx = nullptr;
	m_cogCb = nullptr;

	m_srvTempVertices = nullptr;
	m_srvTempIndices = nullptr;
}
Dx::~Dx() {
	ASSERT_DELETE( m_uavRays );
	ASSERT_DELETE( m_uavIntersections );

	ASSERT_DELETE( m_cogD3d );
	ASSERT_DELETE( m_cogFx );
	ASSERT_DELETE( m_cogCb );

	ASSERT_RELEASE( m_srvTempVertices );
	ASSERT_RELEASE( m_srvTempIndices );
}

HRESULT Dx::init() {
	HRESULT hr = S_FALSE;
	
	// Initialize D3D first-of-all:
	m_cogD3d = new CogD3d( *m_win );
	hr = m_cogD3d->init();

	ID3D11Device* device = m_cogD3d->getDevice();
	ID3D11DeviceContext* devcon = m_cogD3d->getDevcon();

	// ...then continue initializing rest of the cogs:
	if( SUCCEEDED( hr ) ) {
		m_cogFx = new CogFx();
		hr = m_cogFx->init( device );
	}
	if( SUCCEEDED( hr ) ) {
		m_cogCb = new CogCb();
		hr = m_cogCb->init( device );
	}

	// Initialize buffers:
	unsigned numPixels = m_win->getWidth() * m_win->getHeight();
	if( SUCCEEDED( hr ) ) {
		m_uavRays = new BufUav( numPixels, sizeof( Ray ), DXGI_FORMAT_UNKNOWN );
		hr = m_uavRays->init( device );
	}
	if( SUCCEEDED( hr ) ) {
		m_uavIntersections = new BufUav( numPixels, sizeof( Intersection ), DXGI_FORMAT_UNKNOWN );
		hr = m_uavIntersections->init( device );
	}

	// Initialize temporary vertex- and index-buffers:
	hr = initTempVertexBuffer( device );
	if( SUCCEEDED( hr ) ) {
		hr = initTempIndexBuffer( device );
	}

	return hr;
}
HRESULT Dx::render( double p_delta, Mat4F& p_view, Mat4F& p_proj ) {
	ID3D11Device* device = m_cogD3d->getDevice();
	ID3D11DeviceContext* devcon = m_cogD3d->getDevcon();
	IDXGISwapChain* swapchain = m_cogD3d->getSwapChain();

	// Rubbish:
	float aspect = ( (float)m_win->getWidth() ) / ( (float)m_win->getHeight() );
	float fov = (float)PI/4;
	float zFar = 1000;
	float zNear = 1;

	// Update constant buffers:
	CbPerInstance cbPerInstance;
	cbPerInstance.screenWidth = m_win->getWidth();
	cbPerInstance.screenHeight = m_win->getHeight();
	cbPerInstance.aspect = aspect;
	cbPerInstance.fov = fov;
	m_cogCb->mapCbPerInstance( devcon, cbPerInstance );

	Mat4F viewInv = p_view; 
	viewInv.inverse();
	Mat4F projInv = p_proj;
	projInv.inverse();

	CbPerFrame cbPerFrame;
	cbPerFrame.view = p_view;
	cbPerFrame.viewInv = viewInv;
	cbPerFrame.proj = p_proj;
	cbPerFrame.projInv = projInv;
	m_cogCb->mapCbPerFrame( devcon, cbPerFrame );

	CbPerObject cbPerObject;
	cbPerObject.numVertices = 8;
	cbPerObject.numIndices = 36;
	m_cogCb->mapCbPerObject( devcon, cbPerObject );

	m_cogCb->setCbs( devcon );

	// Set SRVs:
	ID3D11ShaderResourceView* srvs[] = {
		m_srvTempVertices,
		m_srvTempIndices
	};
	devcon->CSSetShaderResources( 0, 2, srvs );

	// Set UAVs:
	ID3D11UnorderedAccessView* uavs[] = { 
		m_uavRays->getUav(), 
		m_uavIntersections->getUav(),
		m_cogD3d->getBackbuffer()
	};
	devcon->CSSetUnorderedAccessViews( 0, 3, uavs, NULL );

	// Temp:
	#define BLOCK_SIZE 16
	unsigned dX = (unsigned)ceil( (float)m_win->getWidth() / (float)BLOCK_SIZE );
	unsigned dY = (unsigned)ceil( (float)m_win->getHeight() / (float)BLOCK_SIZE );
	m_cogFx->fxSet( devcon, Fxs_CS_RAYSGENERATE );
	devcon->Dispatch( dX, dY, 1 );
	m_cogFx->fxUnset( devcon, Fxs_CS_RAYSGENERATE );

	m_cogFx->fxSet( devcon, Fxs_CS_RAYSINTERSECT );
	devcon->Dispatch( dX, dY, 1 );
	m_cogFx->fxUnset( devcon, Fxs_CS_RAYSINTERSECT );

	m_cogFx->fxSet( devcon, Fxs_CS_LIGHTING );
	devcon->Dispatch( dX, dY, 1 );
	m_cogFx->fxUnset( devcon, Fxs_CS_LIGHTING );

	return swapchain->Present( 0, 0 );
}

HRESULT Dx::initTempVertexBuffer( ID3D11Device* p_device ) {
	HRESULT hr = S_OK;

	float width = 20.0f;
	float offset = width / 2.0f;

	std::vector< Vertex > vertices;
	Vec3F p1 = Vec3F( -offset, offset, -offset );
	Vec3F p2 = Vec3F( -offset, offset, offset );
	Vec3F p3 = Vec3F( offset, offset, offset );
	Vec3F p4 = Vec3F( offset, offset, -offset );
	Vec3F p5 = Vec3F( -offset, -offset, -offset );
	Vec3F p6 = Vec3F( -offset, -offset, offset );
	Vec3F p7 = Vec3F( offset, -offset, offset );
	Vec3F p8 = Vec3F( offset, -offset, -offset );

	Vec3F nor = Vec3F( 1.0f, 1.0f, 1.0f );
	Vec2F tex = Vec2F( 1.0f, 1.0f );

	Vertex pos; UINT noVertices = 8;
	pos.nor = nor; pos.tex = tex;
	pos.pos = p1; vertices.push_back( pos );
	pos.pos = p2; vertices.push_back( pos );
	pos.pos = p3; vertices.push_back( pos );
	pos.pos = p4; vertices.push_back( pos );
	pos.pos = p5; vertices.push_back( pos );
	pos.pos = p6; vertices.push_back( pos );
	pos.pos = p7; vertices.push_back( pos );
	pos.pos = p8; vertices.push_back( pos );

	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = sizeof( Vertex ) * noVertices;
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof( Vertex );
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices.at( 0 );

	ID3D11Buffer* vertexBuffer = NULL;
	hr = p_device->CreateBuffer( &inputDesc, &vinitData, &vertexBuffer );
	if( SUCCEEDED( hr ) ) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = noVertices;

		hr = p_device->CreateShaderResourceView( vertexBuffer, &srvDesc, &m_srvTempVertices );
	}
	ASSERT_RELEASE( vertexBuffer );

	return hr;
}
HRESULT Dx::initTempIndexBuffer( ID3D11Device* p_device ) {
	HRESULT hr = S_OK;

	std::vector< unsigned > indices;

	unsigned numIndices = 36;
	indices.push_back( 0 ); indices.push_back( 1 ); indices.push_back( 2 );
	indices.push_back( 0 ); indices.push_back( 2 ); indices.push_back( 3 );
	indices.push_back( 4 ); indices.push_back( 6 ); indices.push_back( 5 );
	indices.push_back( 4 ); indices.push_back( 7 ); indices.push_back( 6 );
	indices.push_back( 4 ); indices.push_back( 5 ); indices.push_back( 1 );
	indices.push_back( 4 ); indices.push_back( 1 ); indices.push_back( 0 );
	indices.push_back( 3 ); indices.push_back( 2 ); indices.push_back( 6 );
	indices.push_back( 3 ); indices.push_back( 6 ); indices.push_back( 7 );
	indices.push_back( 1 ); indices.push_back( 5 ); indices.push_back( 6 );
	indices.push_back( 1 ); indices.push_back( 6 ); indices.push_back( 2 );
	indices.push_back( 4 ); indices.push_back( 0 ); indices.push_back( 3 );
	indices.push_back( 4 ); indices.push_back( 3 ); indices.push_back( 7 );

	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = sizeof( unsigned ) * indices.size();
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof( unsigned );
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &indices.at( 0 );

	ID3D11Buffer* indexBuffer = NULL;
	hr = p_device->CreateBuffer( &inputDesc, &vinitData, &indexBuffer );

	if( SUCCEEDED( hr ) ) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = indices.size();

		hr = p_device->CreateShaderResourceView( indexBuffer, &srvDesc, &m_srvTempIndices );
	}
	ASSERT_RELEASE( indexBuffer );

	return hr;
}