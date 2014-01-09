#ifndef DV2520_INTERSECTION_H
#define DV2520_INTERSECTION_H

struct Intersection {
	//Vec3F world;
	float u;
	float v;
	float t;
	
	int primId;
	unsigned primVertexOffset;

	float dist;
};

#endif // DV2520_INTERSECTION_H