#ifndef DV2520_LIGHT_H
#define DV2520_LIGHT_H

#include <MathSimple.h> // eww.

struct LightPoint {
	Vec3F pos;
	float dist; // range

	Vec4F ambient;
	Vec4F diffuse;
	Vec4F specular;

	Vec3F attenuation;
	float pad;
};

#endif // DV2520_LIGHT_H