#include <stdafx.h>

#include <Dx.h>
#include <Ant.h>
#include <Win.h>
#include <Ray.h>
#include <Obj.h>
#include <CogFx.h>
#include <CogCb.h>
#include <CogD3d.h>
#include <BufUav.h>
#include <BufSrv.h>
#include <TimerD3d.h>
#include <Intersection.h>

#include <RdrObj.h> // Remove us.
#include <Vertex.h> 

Dx::Dx( Win& p_win ) {
	m_win = &p_win;

	m_uavBackbuffer = nullptr;
	m_rtvBackbuffer = nullptr;
	m_uavRays = nullptr;
	m_uavIntersections = nullptr;
	m_objTest = nullptr;
	m_cogD3d = nullptr;
	m_cogFx = nullptr;
	m_cogCb = nullptr;
	m_timer = nullptr;
}
Dx::~Dx() {
	ASSERT_RELEASE( m_uavBackbuffer );
	ASSERT_RELEASE( m_rtvBackbuffer );

	ASSERT_DELETE( m_uavRays );
	ASSERT_DELETE( m_uavIntersections );

	ASSERT_DELETE( m_objTest );

	ASSERT_DELETE( m_cogD3d );
	ASSERT_DELETE( m_cogFx );
	ASSERT_DELETE( m_cogCb );

	ASSERT_DELETE( m_timer );
}

HRESULT Dx::init() {
	HRESULT hr = S_FALSE;
	
	// Initialize D3D first-of-all:
	m_cogD3d = new CogD3d( *m_win );
	hr = m_cogD3d->init();
	D3d d3d = m_cogD3d->getD3d();

	// Initialize backbuffer
	if( SUCCEEDED( hr ) ) {
		ID3D11Texture2D* backbufferTex;
		HRESULT hr = d3d.swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&backbufferTex );
		if( SUCCEEDED( hr ) ) {
			hr = d3d.device->CreateUnorderedAccessView( backbufferTex, NULL, &m_uavBackbuffer );
		}
		if( SUCCEEDED( hr ) ) {
			hr = d3d.device->CreateRenderTargetView( backbufferTex, NULL, &m_rtvBackbuffer );
		}
		ASSERT_RELEASE( backbufferTex );
	}

	// Initialize AntTweakBar
	if( SUCCEEDED( hr ) ) {
		Singleton< Ant >::get().init( d3d.device, m_win->getWidth(), m_win->getHeight() );
	}

	// ...then continue initializing rest of the cogs:
	if( SUCCEEDED( hr ) ) {
		m_cogFx = new CogFx();
		hr = m_cogFx->init( d3d.device );
	}
	if( SUCCEEDED( hr ) ) {
		m_cogCb = new CogCb();
		hr = m_cogCb->init( d3d.device );
	}

	// Initialize buffers:
	unsigned numPixels = m_win->getWidth() * m_win->getHeight();
	if( SUCCEEDED( hr ) ) {
		m_uavRays = new BufUav( numPixels, sizeof( Ray ), DXGI_FORMAT_UNKNOWN );
		hr = m_uavRays->init( d3d.device );
	}
	if( SUCCEEDED( hr ) ) {
		m_uavIntersections = new BufUav( numPixels, sizeof( Intersection ), DXGI_FORMAT_UNKNOWN );
		hr = m_uavIntersections->init( d3d.device );
	}

	// Initialize temporary vertex- and index-buffers:
	if( SUCCEEDED( hr ) ) {
		hr = initObjTest( d3d.device );
	}

	// Initialize timer
	if( SUCCEEDED( hr ) ) {
		m_timer = new TimerD3d();
		hr = m_timer->init( d3d.device );
	}

	return hr;
}
HRESULT Dx::render( double p_delta, Mat4F& p_view, Mat4F& p_proj ) {
	D3d d3d = m_cogD3d->getD3d();

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
	m_cogCb->mapCbPerInstance( d3d.devcon, cbPerInstance );

	Mat4F viewInv = p_view; 
	viewInv.inverse();
	Mat4F projInv = p_proj;
	projInv.inverse();

	CbPerFrame cbPerFrame;
	cbPerFrame.view = p_view;
	cbPerFrame.viewInv = viewInv;
	cbPerFrame.proj = p_proj;
	cbPerFrame.projInv = projInv;
	m_cogCb->mapCbPerFrame( d3d.devcon, cbPerFrame );

	CbPerObject cbPerObject;
	cbPerObject.numVertices = 8;
	cbPerObject.numIndices = 36;
	m_cogCb->mapCbPerObject( d3d.devcon, cbPerObject );

	m_cogCb->setCbs( d3d.devcon );

	// Set SRVs:
	ID3D11ShaderResourceView* srvs[] = {
		m_objTest->getBufferVertex()->getSrv(),
		m_objTest->getBufferIndex()->getSrv()
	};
	d3d.devcon->CSSetShaderResources( 0, 2, srvs );

	// Set UAVs:
	ID3D11UnorderedAccessView* uavs[] = { 
		m_uavRays->getUav(), 
		m_uavIntersections->getUav(),
		m_uavBackbuffer
	};
	d3d.devcon->CSSetUnorderedAccessViews( 0, 3, uavs, NULL );

	Singleton< Ant >::get().setTimeRaysGenerate( dispatch( d3d.devcon, Fxs_CS_RAYSGENERATE ) );
	Singleton< Ant >::get().setTimeRaysInterect( dispatch( d3d.devcon, Fxs_CS_RAYSINTERSECT ) );
	Singleton< Ant >::get().setTimeLighting( dispatch( d3d.devcon, Fxs_CS_LIGHTING ) );

	// Unset UAVs:
	ID3D11UnorderedAccessView* uavsUnset[] = { NULL, NULL, NULL };
	d3d.devcon->CSSetUnorderedAccessViews( 0, 3, uavsUnset, NULL );

	// Render GUI
	d3d.devcon->OMSetRenderTargets( 1, &m_rtvBackbuffer, NULL);
	Singleton< Ant >::get().render();
	d3d.devcon->OMSetRenderTargets( 0, NULL, NULL);

	return d3d.swapChain->Present( 0, 0 );
}
double Dx::dispatch( ID3D11DeviceContext* p_devcon, Fxs p_fx ) {
	// Temp:
#define BLOCK_SIZE 16
	unsigned dX = (unsigned)ceil( (float)m_win->getWidth() / (float)BLOCK_SIZE );
	unsigned dY = (unsigned)ceil( (float)m_win->getHeight() / (float)BLOCK_SIZE );

	m_cogFx->fxSet( p_devcon, p_fx );
	m_timer->start( p_devcon );
	p_devcon->Dispatch( dX, dY, 1 );
	m_timer->stop( p_devcon );
	m_cogFx->fxUnset( p_devcon, p_fx );

	return m_timer->time( p_devcon );
}

HRESULT Dx::initObjTest( ID3D11Device* p_device ) {
	//RdrObj rdr( "../../../res/", "bunny.obj" );
	//bool sucess = true;
	//m_objTest = rdr.read( sucess );
	//return m_objTest->init( p_device );

	// Vertices
	float width = 20.0f;
	float offset = width / 2.0f;

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
	Vertex* vertices = new Vertex[ noVertices ];
	pos.nor = nor; pos.tex = tex;
	pos.pos = p1; vertices[0] = pos;
	pos.pos = p2; vertices[1] = pos;
	pos.pos = p3; vertices[2] = pos;
	pos.pos = p4; vertices[3] = pos;
	pos.pos = p5; vertices[4] = pos;
	pos.pos = p6; vertices[5] = pos;
	pos.pos = p7; vertices[6] = pos;
	pos.pos = p8; vertices[7] = pos;

	// Indices
	unsigned numIndices = 36;
	unsigned* indices = new unsigned[ numIndices ];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	// Obj
	m_objTest = new Obj( noVertices, numIndices, vertices, indices );
	return m_objTest->init( p_device );
}