#ifndef DV2520_RAY_H
#define DV2520_RAY_H

#include <MathSimple.h>

struct Ray {
	Vec3F pos;
	float distMin;

	Vec3F dir;
	float distMax;

	Vec3F light;
	int primID;
};

#endif // DV2520_RAY_H