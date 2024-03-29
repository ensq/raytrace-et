#ifndef DV2520_LIGHT_FX
#define DV2520_LIGHT_FX

#include <structs.fx>

void LightingPoint( 
    LightPoint p_light,
    LightSurface p_surface,
    float3 p_toEye,
    out float4 io_ambient,
    out float4 io_diffuse,
    out float4 io_specular ) {
    io_ambient = io_diffuse = io_specular = float4( 0.0f, 0.0f, 0.0f, 0.0f );

    float3 lightVec = p_light.pos - p_surface.pos;
    const float d = length( lightVec );
    if( d>p_light.dist ) {
        return; // Out of range.
    }
    lightVec /= d;

    // Ambient term:
    io_ambient = p_surface.ambient * p_light.ambient;
    
    // Diffuse & specular term:
    const float diffuseFac = dot( lightVec, p_surface.nor );
    if( diffuseFac>0.0f ) {
        float3 v = reflect( -lightVec, p_surface.nor );
        float specularFac = pow( max( dot( v, p_toEye ), 0.0f ), p_surface.specular.w );

        io_diffuse = diffuseFac * p_surface.diffuse * p_light.diffuse;
        io_specular = specularFac * p_surface.specular * p_light.specular;

        // Attenuate:
        const float attenuation = 1.0f / dot( p_light.attenuation, float3( 1.0f, d, d * d ) );
        io_diffuse *= attenuation;
        io_specular *= attenuation;
    }
}

#endif // DV2520_LIGHT_FX
