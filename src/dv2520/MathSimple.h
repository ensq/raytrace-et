#ifndef DV2520_MATHSIMPLE_H
#define DV2520_MATHSIMPLE_H

/*
Simple math lib used to demo raytracing. 
Be careful with virtual methods, as methods declared as virtual allocates a reference table increasing the size of the object.
*/

#define PI 3.141592654
#define RADIAN( x ) ( x * PI / 180.0f )
#define DEGREE( x ) ( x * 180.0f / PI )

struct Vec2F {
	float x;
	float y;

	Vec2F();
	Vec2F( float p_x, float p_y );
	~Vec2F();
};

struct Vec3F {
	float x;
	float y;
	float z;

	Vec3F();
	Vec3F( float p_x, float p_y, float p_z );
	~Vec3F();

	Vec3F& operator+=( const Vec3F& p_r );
	Vec3F& operator-=( const Vec3F& p_r );
	Vec3F& operator*=( float p_r );

	float length() const;
	Vec3F cross( const Vec3F& p_r ) const;
	Vec3F& normalize();
	float dot( Vec3F p_r ) const;
	void lerp( Vec3F& p_r, float p_factor );
	float distance( Vec3F& p_r ) const;

	void rotate( float p_angle, const Vec3F& p_axis );
};
inline Vec3F operator+( const Vec3F& p_l, const Vec3F& p_r ) {
	return Vec3F(
		p_l.x + p_r.x,
		p_l.y + p_r.y,
		p_l.z + p_r.z);
}
inline Vec3F operator-( const Vec3F& p_l, const Vec3F& p_r ) {
	return Vec3F(
		p_l.x - p_r.x,
		p_l.y - p_r.y,
		p_l.z - p_r.z);
}
inline Vec3F operator*( const Vec3F& p_l, float p_s ) {
	return Vec3F(
		p_l.x * p_s,
		p_l.y * p_s,
		p_l.z * p_s );
}

struct Mat4F {
	float _[4][4];

	Mat4F();
	~Mat4F();

	void scale( float p_x, float p_y, float p_z );
	void rotate( float p_x, float p_y, float p_z );
	void translate( float p_x, float p_y, float p_z );
	void transpose();
	void inverse();
	void lookAtLH( Vec3F p_eye, Vec3F p_at, Vec3F p_up );

	static Mat4F Identity();
	inline Mat4F operator*( const Mat4F& p_r ) const {
		Mat4F m;
		for( unsigned i = 0 ; i < 4 ; i++ ) {
			for( unsigned j = 0 ; j < 4 ; j++ ) {
				m._[ i ][ j ] =
					_[ i ][ 0 ] * p_r._[ 0 ][ j ] +
					_[ i ][ 1 ] * p_r._[ 1 ][ j ] +
					_[ i ][ 2 ] * p_r._[ 2 ][ j ] +
					_[ i ][ 3 ] * p_r._[ 3 ][ j ];
			}
		}
		return m;
	}
};

struct Quaternion {
	float x;
	float y;
	float z;
	float w;

	Quaternion();
	Quaternion( float p_x, float p_y, float p_z, float p_w );
	~Quaternion();

	float length() const;
	void normalize();
	Quaternion conjugate();
	Mat4F toMatrix();

	static Quaternion Identity();

};
Quaternion operator*( const Quaternion& p_l, const Quaternion& p_r );
Quaternion operator*( const Quaternion& p_q, const Vec3F& p_v );

#endif // DV2520_MATHSIMPLE_H

// Devilry:
/*
Quaternion(MatF4* pm)
{
int i, maxi;
float maxdiag, S, trace;

trace = pm->m[0][0] + pm->m[1][1] + pm->m[2][2] + 1.0f;
if (trace > 0.0f)
{
x = (pm->m[1][2] - pm->m[2][1]) / (2.0f * sqrt(trace));
y = (pm->m[2][0] - pm->m[0][2]) / (2.0f * sqrt(trace));
z = (pm->m[0][1] - pm->m[1][0]) / (2.0f * sqrt(trace));
w = sqrt(trace) / 2.0f;
return;
}
maxi = 0;
maxdiag = pm->m[0][0];
for (i=1; i<3; i++)
{
if ( pm->m[i][i] > maxdiag )
{
maxi = i;
maxdiag = pm->m[i][i];
}
}
switch( maxi )
{
case 0:
S = 2.0f * sqrt(1.0f + pm->m[0][0] - pm->m[1][1] - pm->m[2][2]);
x = 0.25f * S;
y = ( pm->m[0][1] + pm->m[1][0] ) / S;
z = ( pm->m[0][2] + pm->m[2][0] ) / S;
w = ( pm->m[1][2] - pm->m[2][1] ) / S;
break;
case 1:
S = 2.0f * sqrt(1.0f + pm->m[1][1] - pm->m[0][0] - pm->m[2][2]);
x = ( pm->m[0][1] + pm->m[1][0] ) / S;
y = 0.25f * S;
z = ( pm->m[1][2] + pm->m[2][1] ) / S;
w = ( pm->m[2][0] - pm->m[0][2] ) / S;
break;
case 2:
S = 2.0f * sqrt(1.0f + pm->m[2][2] - pm->m[0][0] - pm->m[1][1]);
x = ( pm->m[0][2] + pm->m[2][0] ) / S;
y = ( pm->m[1][2] + pm->m[2][1] ) / S;
z = 0.25f * S;
w = ( pm->m[0][1] - pm->m[1][0] ) / S;
break;
}
};
*/