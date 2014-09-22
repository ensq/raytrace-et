#include <stdafx.h>

#include <Dx.h>
#include <Ant.h>
#include <Win.h>
#include <Obj.h>
#include <CogFx.h>
#include <CogCb.h>
#include <CogSS.h>
#include <CogD3d.h>
#include <CogTex.h>
#include <BufUav.h>
#include <structs.h>
#include <TimerD3d.h>

#include <lbo.h>
#include <Bvh.h>
#include <geometry.h> 

Dx::Dx( Win& p_win ) {
    m_win = &p_win;

    m_uavBackbuffer = nullptr;
    m_rtvBackbuffer = nullptr;
    m_srvStreamVertices = nullptr;
    m_srvStreamIndices = nullptr;
    m_srvStreamInstances = nullptr;
    m_srvStreamLights = nullptr;
    m_srvStreamNodes = nullptr;
    m_uavRays = nullptr;
    m_uavIntersections = nullptr;
    m_cogD3d = nullptr;
    m_cogFx = nullptr;
    m_cogCb = nullptr;
    m_cogSS = nullptr;
    m_cogTex = nullptr;
    m_timer = nullptr;
}
Dx::~Dx() {
    ASSERT_RELEASE( m_uavBackbuffer );
    ASSERT_RELEASE( m_rtvBackbuffer );
    
    ASSERT_DELETE( m_srvStreamVertices );
    ASSERT_DELETE( m_srvStreamIndices );
    ASSERT_DELETE( m_srvStreamInstances );
    ASSERT_DELETE( m_srvStreamLights );
    ASSERT_DELETE( m_srvStreamNodes );
    ASSERT_DELETE( m_uavRays );
    ASSERT_DELETE( m_uavIntersections );
    ASSERT_DELETE( m_cogD3d );
    ASSERT_DELETE( m_cogFx );
    ASSERT_DELETE( m_cogCb );
    ASSERT_DELETE( m_cogSS );
    ASSERT_DELETE( m_cogTex );
    ASSERT_DELETE( m_timer );

    for( unsigned i = 0; i<m_objects.size(); i++ ) {
        ASSERT_DELETE( m_objects.at( i ) );
    }

    assert(m_nodes!=nullptr);
    delete[] m_nodes;
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

        float aspect = ( (float)m_win->getWidth() ) / ( (float)m_win->getHeight() ); // Rubbish, get rid of this:
        float fov = (float)PI/4;
        float zFar = 1000;
        float zNear = 1;
        // Per instance CB only needs to be updated once:
        CbPerInstance cbPerInstance;
        cbPerInstance.screenWidth = m_win->getWidth();
        cbPerInstance.screenHeight = m_win->getHeight();
        cbPerInstance.aspect = aspect;
        cbPerInstance.fov = fov;
        m_cogCb->mapCbPerInstance( d3d.devcon, cbPerInstance );
    }
    if( SUCCEEDED( hr ) ) {
        m_cogSS = new CogSS();
        hr = m_cogSS->init( d3d.device );
    }
    if( SUCCEEDED( hr ) ) {
        m_cogTex = new CogTex();
        hr = m_cogTex->init( d3d.device );
    }

    // Initialize UAVs:
    unsigned numPixels = m_win->getWidth() * m_win->getHeight();
    if( SUCCEEDED( hr ) ) {
        m_uavRays = new BufUav( numPixels, sizeof( Ray ), DXGI_FORMAT_UNKNOWN );
        hr = m_uavRays->init( d3d.device );
    }
    if( SUCCEEDED( hr ) ) {
        m_uavIntersections = new BufUav( numPixels, sizeof( Intersection ), DXGI_FORMAT_UNKNOWN );
        hr = m_uavIntersections->init( d3d.device );
    }

    // Initialize SRVs:
    if( SUCCEEDED( hr ) ) {
        m_srvStreamVertices = new BufStreamSrv< Vertex >();
        m_srvStreamIndices = new BufStreamSrv< unsigned >();
        m_srvStreamInstances = new BufStreamSrv< ObjInstance >();
        m_srvStreamLights = new BufStreamSrv< LightPoint >();
        m_srvStreamNodes = new BufStreamSrv< Bvh_Node_Flat >();
    }

    // Initialize demo objects:
    initObjects( d3d.device );

    // Initialize timer
    if( SUCCEEDED( hr ) ) {
        m_timer = new TimerD3d();
        hr = m_timer->init( d3d.device );
    }

    return hr;
}
HRESULT Dx::render( double p_delta, Vec3F& p_pos, Mat4F& p_view, Mat4F& p_proj ) {
    D3d d3d = m_cogD3d->getD3d();
    d3d.devcon->ClearRenderTargetView( m_rtvBackbuffer, DxClearColor::Black );

    // Prepare render by clearing streams and updating them with new data:
    m_srvStreamVertices->reset();
    m_srvStreamIndices->reset();
    m_srvStreamInstances->reset();
    m_srvStreamLights->reset();
    m_srvStreamNodes->reset();

    // Add a lone pointlight to the scene:
    static float osc = 0.0f;
    static bool up = true;
    if( up==true ) {
        osc += 0.01f;
        if( osc>1.0f ) {
            up = false;
        }
    } else {
        osc -= 0.01f;
        if( osc<0.0f ) {
            up = true;
        }
    }

    unsigned lightsCnt = 0;
    LightPoint light;
    light.pos = Vec3F( -10.0f * cos(osc), 0.5f, 0.0f );
    light.ambient = Vec4F( 0.2f, 0.2f, 0.2f, 1.0f );
    light.diffuse = Vec4F( 0.0f, 0.6f, 0.0f, 1.0f );
    light.specular = Vec4F( 0.0f, 0.8f, 0.0f, 1.0f );
    light.attenuation = Vec3F( 0.5f, 1.0f, 0.0f );
    light.dist = 100.0f;
    m_srvStreamLights->pushElement( light ); lightsCnt++;

    light.pos = Vec3F( 0.0f, 0.5f, 10.0f * sin(osc) );
    light.ambient = Vec4F( 0.2f, 0.2f, 0.2f, 1.0f );
    light.diffuse = Vec4F( 0.6f, 0.0f, 0.0f, 1.0f );
    light.specular = Vec4F( 0.8f, 0.0f, 0.0f, 1.0f );
    light.attenuation = Vec3F( 0.5f, 1.0f, 0.0f );
    light.dist = 100.0f;
    m_srvStreamLights->pushElement( light ); lightsCnt++;

    light.pos = Vec3F( 0.0f, 0.5f, -10.0f * cos(osc) );
    light.ambient = Vec4F( 0.2f, 0.2f, 0.2f, 1.0f );
    light.diffuse = Vec4F( 0.0f, 0.0f, 0.6f, 1.0f );
    light.specular = Vec4F( 0.0f, 0.0f, 0.8f, 1.0f );
    light.attenuation = Vec3F( 0.5f, 1.0f, 0.0f );
    light.dist = 100.0f;
    m_srvStreamLights->pushElement( light ); lightsCnt++;

    // Add objects to the scene:
    unsigned indexOffset = 0;
    unsigned instancesCnt = 0;
    for( unsigned i = 0; i<m_objects.size(); i++ ) {
        Obj* obj = m_objects.at( i );

        static float rotate = 0.0f;
        obj->getRotation().rotate( 0.0f, rotate, 0.0f );
        rotate += 1.0f;

        ObjInstance instance;
        instance.indexStart = indexOffset;
        instance.indexCnt = obj->getIndicesCnt();
        instance.vertexCnt = obj->getVerticesCnt();
        instance.world = obj->getWorldTransform();
        instance.worldInv = obj->getWorldTransform();
        instance.worldInv.inverse();
        indexOffset += instance.indexCnt;

        m_srvStreamVertices->pushElements( obj->getVertices(), obj->getVerticesCnt() );
        m_srvStreamIndices->pushElements( obj->getIndices(), obj->getIndicesCnt() );
        m_srvStreamInstances->pushElement( instance );
        m_srvStreamNodes->pushElements( m_nodes, numNodes );

        instancesCnt++;
    }

    // Update streams:
    m_srvStreamVertices->updateBufStream( d3d.device, d3d.devcon );
    m_srvStreamIndices->updateBufStream( d3d.device, d3d.devcon );
    m_srvStreamInstances->updateBufStream( d3d.device, d3d.devcon );
    m_srvStreamLights->updateBufStream( d3d.device, d3d.devcon );
    m_srvStreamNodes->updateBufStream( d3d.device, d3d.devcon );

    // Update per-frame CB:
    Mat4F viewInv = p_view; viewInv.inverse();
    Mat4F projInv = p_proj; projInv.inverse();

    CbPerFrame cbPerFrame;
    cbPerFrame.view = p_view;
    cbPerFrame.viewInv = viewInv;
    cbPerFrame.proj = p_proj;
    cbPerFrame.projInv = projInv;
    cbPerFrame.pos = p_pos;
    cbPerFrame.instancesCnt = instancesCnt;
    cbPerFrame.lightsCnt = lightsCnt;
    m_cogCb->mapCbPerFrame( d3d.devcon, cbPerFrame );

    // Set CBs:
    m_cogCb->setCbs( d3d.devcon );

    // Set SRVs:
    ID3D11ShaderResourceView* srvs[] = {
        m_srvStreamVertices->getSrv(),
        m_srvStreamIndices->getSrv(),
        m_srvStreamInstances->getSrv(),
        m_srvStreamLights->getSrv(),
        m_srvStreamNodes->getSrv(),
        m_cogTex->getTex( d3d.device, Texs_default )->getSrv() // Make this more general.
    };
    d3d.devcon->CSSetShaderResources( 0, 6, srvs );

    // Set UAVs:
    ID3D11UnorderedAccessView* uavs[] = { 
        m_uavRays->getUav(), 
        m_uavIntersections->getUav(),
        m_uavBackbuffer
    };
    d3d.devcon->CSSetUnorderedAccessViews( 0, 3, uavs, NULL );

    // Set Samplerstates:
    ID3D11SamplerState* sss[] = { m_cogSS->getSamplerState( SSs_default ) };
    d3d.devcon->CSSetSamplers( 0, 1, sss );

    Singleton< Ant >::get().setTimeRaysGenerate( dispatch( d3d.devcon, Fxs_CS_RAYSGENERATE ) );
    Singleton< Ant >::get().setTimeRaysInterect( dispatch( d3d.devcon, Fxs_CS_RAYSINTERSECT ) );
    Singleton< Ant >::get().setTimeLighting( dispatch( d3d.devcon, Fxs_CS_LIGHTING ) );
    
    // Unset SRVs:
    ID3D11ShaderResourceView* srvsUnset[] = { NULL, NULL, NULL, NULL, NULL, NULL };
    d3d.devcon->CSSetShaderResources( 0, 6, srvsUnset );

    // Unset UAVs:
    ID3D11UnorderedAccessView* uavsUnset[] = { NULL, NULL, NULL };
    d3d.devcon->CSSetUnorderedAccessViews( 0, 3, uavsUnset, NULL );

    // Unset Samplerstates:
    ID3D11SamplerState* sssUnset[] = { NULL };
    d3d.devcon->CSSetSamplers( 0, 1, sssUnset );

    // Render GUI
    d3d.devcon->OMSetRenderTargets( 1, &m_rtvBackbuffer, NULL);
    Singleton< Ant >::get().render();
    d3d.devcon->OMSetRenderTargets( 0, NULL, NULL);

    return d3d.swapChain->Present( 0, 0 );
}
double Dx::dispatch( ID3D11DeviceContext* p_devcon, Fxs p_fx ) {
    unsigned dX = (unsigned)ceil( (float)m_win->getWidth() / (float)BLOCK_SIZE );
    unsigned dY = (unsigned)ceil( (float)m_win->getHeight() / (float)BLOCK_SIZE );

    m_cogFx->fxSet( p_devcon, p_fx );
    m_timer->start( p_devcon );
    p_devcon->Dispatch( dX, dY, 1 );
    m_timer->stop( p_devcon );
    m_cogFx->fxUnset( p_devcon, p_fx );

    return m_timer->time( p_devcon );
}

bool Dx::initObjects( ID3D11Device* p_device ) {
    bool success = true;
    for( unsigned i = 0; i<1 && success==true; i++ ) {
        std::vector<float> vertices;
        std::vector<unsigned> indices;
        success = lbo::parse("../../../obj/dv2520/box2.obj", vertices, indices); // "../../../obj/dv2520/bth.obj"
        assert(success==true);

        std::vector<Vertex> vertices_struct;
        for(size_t i = 0; i<vertices.size(); i+=8) {
            float* p = &vertices.at(i);
            Vertex v(Vec3F(p[0], p[1], p[2]),
                    Vec3F(p[5], p[6], p[7]),    
                    Vec2F(p[3], p[4]));
            vertices_struct.push_back(v);
        }
        Obj* obj = new Obj(vertices_struct.size(), indices.size(), &vertices_struct.at(0), &indices.at(0));

        Bvh bvh(obj, 1);
        bvh.init();
        numNodes = bvh.getNodesCnt();
        m_nodes = new Bvh_Node_Flat[numNodes];
        memcpy(m_nodes, bvh.getNodes(), sizeof(Bvh_Node_Flat) * numNodes);

        m_objects.push_back( obj );
    }

    return success;
}