#ifndef DV2520_STRUCTS_FX
#define DV2520_STRUCTS_FX

// Intersection and geometry
struct Intersection {
    float u;
    float v;
    float t; // Consider removing. May easily be calculated.

    int primId; // primitive id, currently index of the last primitive
                // index plus index offset.
    uint primVertexOffset;
    uint instanceIdx;

    float dist; // distance from origin to intersection.
};
Intersection ConstructIntersection() {
    Intersection i;
    i.u = i.v = i.t = i.dist = -1;
    i.primId = -1;
    i.primVertexOffset = i.instanceIdx = 0;
    return i;
}

// The ray struct will need a reflect factor variable.
struct Ray {
    float3 pos; // Origin of ray.
    float distMin; // Minimum distance of ray.

    float3 dir; // Direction of ray.
    float distMax; // Maximum distance of ray.

    float3 light; // Light intensity accumulated by ray.
    int primId; // Identifier signifying the last triangle intersected by ray.
};
Ray ConstructRay( float3 p_pos, float3 p_dir ) {
    Ray r;
    r.pos = p_pos;
    r.dir = p_dir;

    r.light = float3( 0.0f, 0.0f, 0.0f ); // not yet used.
    r.distMin = 0.0f; // not yet used.
    r.distMax = 9999999999.0f;
    r.primId = -1;

    return r;
}

// Lighting
struct LightPoint {
    float3 pos;
    float dist; // range
    
    float4 ambient;
    float4 diffuse;
    float4 specular;

    float3 attenuation;
    float pad;
};

struct LightSurface {
    float4 ambient;
    float4 diffuse;
    float4 specular; // .w is treated as specular power.

    float3 pos;
    float3 nor;
};
LightSurface ConstructLightSurface(float3 p_pos,
                                   float3 p_nor,
                                   float4 p_ambient,
                                   float4 p_diffuse,
                                   float4 p_specular ) {
    LightSurface ls;
    ls.pos = p_pos;
    ls.nor = p_nor;
    ls.ambient = p_ambient;
    ls.diffuse = p_diffuse;
    ls.specular = p_specular;
    return ls;
}

// Geometry
struct ObjInstance {
    row_major float4x4 world;
    row_major float4x4 worldInv;

    uint indexStart;
    uint indexCnt;
    uint vertexCnt;
    uint pad;
};

struct Vertex {
    float3 pos;
    float3 nor;
    float2 tex;
};

// BVH
struct ObjNode {
       float3 min;
       float3 max;
       int primitivesCnt;
       uint primitivesIdx;
};

#endif // DV2520_STRUCTS_FX
