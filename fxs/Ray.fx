#ifndef DV2520_RAY_FX
#define DV2520_RAY_FX

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

#endif // DV2520_RAY_FX