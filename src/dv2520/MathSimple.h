#ifndef DV2520_MATHSIMPLE_H
#define DV2520_MATHSIMPLE_H

/*
Simple math lib used to demo raytracing.  Be careful with virtual
methods, as methods declared as virtual allocates a reference table
increasing the size of the object.
*/

#include <limits>

#define PI 3.141592654
#define RADIAN(x) (x * PI / 180.0f)
#define DEGREE(x) (x * 180.0f / PI)
#define INFI_FLOAT std::numeric_limits<float>::infinity()
#define INFI_DOUBL std::numeric_limits<double>::infinity()

template <typename T>
T clip(const T& n, const T& min, const T& max) {
    return std::max(min, std::min(n, max));
}

struct Vec2F {
    float x;
    float y;

    Vec2F();
    Vec2F(float p_x, float p_y);
    ~Vec2F();
};

struct Vec3F {
    float x;
    float y;
    float z;

    Vec3F();
    Vec3F(float p_x, float p_y, float p_z);
    ~Vec3F();

    Vec3F& operator+=(const Vec3F& p_r);
    Vec3F& operator-=(const Vec3F& p_r);
    Vec3F& operator*=(float p_r);
    float& operator[](int i) {
        assert(i==0||i==1||i==2);
        float* p = &x;
        return *(p + i);
    };
    
    float length() const;
    Vec3F cross(const Vec3F& p_r) const;
    Vec3F min(const Vec3F& p_r) const;
    Vec3F max(const Vec3F& p_r) const;
    Vec3F& normalize();
    float dot(Vec3F p_r) const;
    void lerp(Vec3F& p_r, float p_factor);
    float distance(Vec3F& p_r) const;

    void rotate(float p_angle, const Vec3F& p_axis);
};
inline Vec3F operator+(const Vec3F& p_l, const Vec3F& p_r) {
    return Vec3F(
        p_l.x + p_r.x,
        p_l.y + p_r.y,
        p_l.z + p_r.z);
}
inline Vec3F operator-(const Vec3F& p_l, const Vec3F& p_r) {
    return Vec3F(
        p_l.x - p_r.x,
        p_l.y - p_r.y,
        p_l.z - p_r.z);
}
inline Vec3F operator*(const Vec3F& p_l, float p_s) {
    return Vec3F(
        p_l.x * p_s,
        p_l.y * p_s,
        p_l.z * p_s );
}

struct Vec4F {
    float x;
    float y;
    float z;
    float a;

    Vec4F();
    Vec4F(float p_x, float p_y, float p_z, float p_a);
    ~Vec4F();
};

struct Mat4F {
    float _[4][4];

    Mat4F();
    ~Mat4F();

    void scale(float p_x, float p_y, float p_z);
    void rotate(float p_x, float p_y, float p_z);
    void translate(float p_x, float p_y, float p_z);
    void transpose();
    void inverse();
    void lookAtLH(Vec3F p_eye, Vec3F p_at, Vec3F p_up);

    static Mat4F Identity();
    inline Mat4F operator*(const Mat4F& p_r) const {
        Mat4F m;
        for(unsigned i = 0 ; i<4 ; i++) {
            for(unsigned j = 0 ; j<4 ; j++) {
                m._[i][j] =
                    _[i][0] * p_r._[0][j] +
                    _[i][1] * p_r._[1][j] +
                    _[i][2] * p_r._[2][j] +
                    _[i][3] * p_r._[3][j];
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
    Quaternion(float p_x, float p_y, float p_z, float p_w);
    ~Quaternion();

    float length() const;
    void normalize();
    Quaternion conjugate();
    Mat4F toMatrix();

    static Quaternion Identity();
};
Quaternion operator*(const Quaternion& p_l, const Quaternion& p_r);
Quaternion operator*(const Quaternion& p_q, const Vec3F& p_v);

#endif // DV2520_MATHSIMPLE_H
