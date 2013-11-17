#ifndef DV2520_RAY_FX
#define DV2520_RAY_FX

struct Ray {
	float3 pos; // Origin of ray.
	float distMin; // Minimum distance of ray.

	float3 dir; // Direction of ray.
	float distMax; // Maximum distance of ray.

	float3 light; // Light intensity accumulated by ray.
	int primID; // Identifier signifying the last triangle intersected by ray.
};

#endif // DV2520_RAY_FX