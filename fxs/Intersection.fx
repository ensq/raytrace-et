#ifndef DV2520_INTERSECTION_FX
#define DV2520_INTERSECTION_FX

struct Intersection {
	float u;
	float v;
	float t; // Consider removing. May easily be calculated.

	int primId; // primitive id, currently index of the last primitive index plus index offset.
	uint primVertexOffset;
	uint instanceIdx;

	float dist; // distance from origin to intersection.
};
Intersection ConstructIntersection() {
	Intersection i;
	i.u = i.v = i.t = i.dist = -1;
	i.primId = -1;
	i.primVertexOffset = 0;

	return i;
}

#define REAL_TIME_RENDERING_EPSILON 0.00001f
Intersection intersectRayTriangleRealTimeRendering( float3 p_rOrigin, float3 p_rDir, float3 p_p0, float3 p_p1, float3 p_p2, uint p_primId ) {
	Intersection i = ConstructIntersection();
	
	float3 e1 = p_p1 - p_p0;
	float3 e2 = p_p2 - p_p0;
	float3 q = cross( p_rDir, e2 );
	float a = dot( e1, q );
	if( a>-REAL_TIME_RENDERING_EPSILON && a<REAL_TIME_RENDERING_EPSILON ) {
		return i; // No intersection.
	}
	float f = 1.0f / a;
	float3 s = p_rOrigin - p_p0;
	i.u = f * dot( s, q );
	if( i.u<0.0f ) {
		return i; // No intersection.
	}
	float3 r = cross( s, e1 );
	i.v = f * dot( p_rDir, r );
	if( i.v<0.0f || ( i.u+i.v )>1.0f ) {
		return i; // No intersection.
	}
	i.t = f * dot( e2, r );
	if( i.t>=0 ) {
		i.dist = i.t;
		i.primId = p_primId;
	} // False intersection.
	
	return i;
}
// Implemented in accordance to the ray/triangle-algorithm in Real-Time Rendering, which is in-and-of-it's-own derived from Möller & Trumbore's algorithm.

//#define MOLLER_TROMBORE_CULL_BACKFACE
#define MOLLER_TROMBORE_EPSILON 0.000001f
Intersection intersectRayTriangleMollerTrumbore( float3 p_rOrigin, float3 p_rDir, float3 p_p0, float3 p_p1, float3 p_p2, uint p_primId ) {
	// Obs. Erronous implementation. Only works half-heartedly. Obs.
	Intersection i = ConstructIntersection();
	float3 e1, e2, tvec, pvec, qvec;
	float det, inv_det;

	e1 = p_p1 - p_p0; // Find vectors for two edges sharing p0.
	e2 = p_p2 - p_p0;
	pvec = cross( p_rDir, e2 ); // Calculate determinant. Also used to establish U.
	det = dot( e1, pvec ); // If determinant is near zero, then ray lies in the plane of the triangle.

#if defined( MOLLER_TROMBORE_CULL_BACKFACE )
	if( det<MOLLER_TROMBORE_EPSILON ) {
		return i;
	}

	tvec = p_rOrigin - p_p0; // Calculate distance from p0 to ray origin.
	i.u = dot( tvec, pvec ); // Establish U and test bounds.
	if( i.u<0.0f || i.u>det ) { 
		return i;
	}

	qvec = cross( tvec, e1 ); // Test V.
	i.v = dot( p_rDir, qvec );
	if( i.v<0.0 || ( i.u + i.v )>det ) {
		return i;
	}

	i.t = dot( e2, qvec ); // If the execution path has got this far, then the ray does intersect the triangle. Calculate T and scale parameters.
	inv_det = 1.0f / det;
	i.t *= inv_det;
	i.u *= inv_det;
	i.v *= inv_det;
#else
	if( det>-MOLLER_TROMBORE_EPSILON && det<MOLLER_TROMBORE_EPSILON ) {
		return i;
	}
	inv_det = 1.0f / det;

	tvec = p_rOrigin - p_p0; // Calculate distance from p0 to ray origin.
	i.u = dot( tvec, pvec ) * inv_det; // Establish U and test bounds.
	if( i.u<0.0f || i.u>1.0f ) {
		return i;
	}

	qvec = cross( tvec, e1 ); // Calculate V and test bounds.
	i.v = dot( p_rDir, qvec ) * inv_det;
	if( i.v<0.0f || ( i.u + i.v )>1.0f ) {
		return i;
	}

	i.t = dot( e2, qvec ) * inv_det; // If the execution path has got this far, then the ray does intersect the triangle. Calculate T.
#endif // MOLLER_TROMBORE_CULL_BACKFACE
	i.dist = length( p_rOrigin - ( i.u * p_p0 + i.v * p_p1 + i.t * p_p2 ) );
	i.primId = p_primId; // Signifies intersection.
	return i;
}
// Implemented in accordance to Fast, Minimum Storage Ray/Triangle Intersection by Möller and Trumbore.

#endif // DV2520_INTERSECTION_FX