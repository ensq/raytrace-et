#include <stdafx.h>
#include <algorithm>
#include <DirectXMath.h>

#include <MathSimple.h>

Vec2F::Vec2F() {
    x = y = 0.0f;
}
Vec2F::Vec2F(float p_x, float p_y) {
    x = p_x;
    y = p_y;
}
Vec2F::~Vec2F() {
}

Vec3F::Vec3F() {
    x = y = z = 0.0f;
}
Vec3F::Vec3F(float p_x, float p_y, float p_z) {
    x = p_x;
    y = p_y;
    z = p_z;
}
Vec3F::~Vec3F() {
}

Vec3F& Vec3F::operator+=(const Vec3F& p_r) {
    x += p_r.x;
    y += p_r.y;
    z += p_r.z;
    return *this;
}
Vec3F& Vec3F::operator-=(const Vec3F& p_r) {
    x -= p_r.x;
    y -= p_r.y;
    z -= p_r.z;
    return *this;
}
Vec3F& Vec3F::operator*=(float p_r) {
    x *= p_r;
    y *= p_r;
    z *= p_r;
    return *this;
}

float Vec3F::length() const {
    return sqrt(x * x + y * y + z * z);
}
Vec3F Vec3F::cross(const Vec3F& p_r) const {
    const float _x = y * p_r.z - z * p_r.y;
    const float _y = z * p_r.x - x * p_r.z;
    const float _z = x * p_r.y - y * p_r.x;
    return Vec3F(_x, _y, _z);

}
Vec3F Vec3F::min(const Vec3F& p_r) const {
    return Vec3F(std::min(x, p_r.x),
                 std::min(y, p_r.y),
                 std::min(z, p_r.z));
}
Vec3F Vec3F::max(const Vec3F& p_r) const {
    return Vec3F(
               std::max(x, p_r.x),
               std::max(y, p_r.y),
               std::max(z, p_r.z));
}
Vec3F& Vec3F::normalize() {
    const float length = this->length();
    x /= length;
    y /= length;
    z /= length;
    return *this;
}
float Vec3F::dot(Vec3F p_r) const {
    return
        x * p_r.x +
        y * p_r.y +
        z * p_r.z;
}
void Vec3F::lerp(Vec3F& p_r, float p_factor) {
    x = x * (1.0f - p_factor) + p_r.x * p_factor;
    y = y * (1.0f - p_factor) + p_r.y * p_factor;
    z = z * (1.0f - p_factor) + p_r.z * p_factor;
}
float Vec3F::distance(Vec3F& p_r) const {
    Vec3F v(
        x - p_r.x,
        y - p_r.y,
        p_r.z - z);
    return v.length();
}
void Vec3F::rotate(float p_angle, const Vec3F& p_axis) {
    const float sinHalfAngle = sinf((float)RADIAN(p_angle / 2));
    const float cosHalfAngle = cosf((float)RADIAN(p_angle / 2));

    const float rX = p_axis.x * sinHalfAngle;
    const float rY = p_axis.y * sinHalfAngle;
    const float rZ = p_axis.z * sinHalfAngle;
    const float rW = cosHalfAngle;

    Quaternion rotationQ(rX, rY, rZ, rW);
    Quaternion conjugateQ = rotationQ.conjugate();
    Quaternion w = rotationQ * (*this) * conjugateQ;

    x = w.x;
    y = w.y;
    z = w.z;
}

Vec4F::Vec4F() {
    x = y = z = a = 0.0f;
}
Vec4F::Vec4F(float p_x, float p_y, float p_z, float p_a) {
    x = p_x;
    y = p_y;
    z = p_z;
    a = p_a;
}
Vec4F::~Vec4F() {
}

Mat4F::Mat4F() {
    _[0][0] = 1.0f;
    _[0][1] = 0.0f;
    _[0][2] = 0.0f;
    _[0][3] = 0.0f;
    _[1][0] = 0.0f;
    _[1][1] = 1.0f;
    _[1][2] = 0.0f;
    _[1][3] = 0.0f;
    _[2][0] = 0.0f;
    _[2][1] = 0.0f;
    _[2][2] = 1.0f;
    _[2][3] = 0.0f;
    _[3][0] = 0.0f;
    _[3][1] = 0.0f;
    _[3][2] = 0.0f;
    _[3][3] = 1.0f;
}
Mat4F::~Mat4F() {
}

void Mat4F::scale(float p_x, float p_y, float p_z) {
    _[0][0] = p_x;
    _[0][1] = 0.0f;
    _[0][2] = 0.0f;
    _[0][3] = 0.0f;
    _[1][0] = 0.0f;
    _[1][1] = p_y;
    _[1][2] = 0.0f;
    _[1][3] = 0.0f;
    _[2][0] = 0.0f;
    _[2][1] = 0.0f;
    _[2][2] = p_z;
    _[2][3] = 0.0f;
    _[3][0] = 0.0f;
    _[3][1] = 0.0f;
    _[3][2] = 0.0f;
    _[3][3] = 1.0f;
}
void Mat4F::rotate(float p_x, float p_y, float p_z) {
    float radX = (float)RADIAN(p_x);
    float radXSin = sinf(radX);
    float radXCos = cosf(radX);

    float radY = (float)RADIAN(p_y);
    float radYSin = sinf(radY);
    float radYCos = cosf(radY);

    float radZ = (float)RADIAN(p_z);
    float radZSin = sinf(radZ);
    float radZCos = cosf(radZ);

    Mat4F rX, rY, rZ;
    rX._[0][0] = 1.0f;
    rX._[0][1] = 0.0f;
    rX._[0][2] = 0.0f;
    rX._[0][3] = 0.0f;
    rX._[1][0] = 0.0f;
    rX._[1][1] = radXCos;
    rX._[1][2] = -radXSin;
    rX._[1][3] = 0.0f;
    rX._[2][0] = 0.0f;
    rX._[2][1] = radXSin;
    rX._[2][2] = radXCos;
    rX._[2][3] = 0.0f;
    rX._[3][0] = 0.0f;
    rX._[3][1] = 0.0f;
    rX._[3][2] = 0.0f;
    rX._[3][3] = 1.0f;

    rY._[0][0] = radYCos;
    rY._[0][1] = 0.0f;
    rY._[0][2] = -radYSin;
    rY._[0][3] = 0.0f;
    rY._[1][0] = 0.0f;
    rY._[1][1] = 1.0f;
    rY._[1][2] = 0.0f;
    rY._[1][3] = 0.0f;
    rY._[2][0] = radYSin;
    rY._[2][1] = 0.0f;
    rY._[2][2] = radYCos;
    rY._[2][3] = 0.0f;
    rY._[3][0] = 0.0f;
    rY._[3][1] = 0.0f;
    rY._[3][2] = 0.0f;
    rY._[3][3] = 1.0f;

    rZ._[0][0] = radZCos;
    rZ._[0][1] = -radZSin;
    rZ._[0][2] = 0.0f;
    rZ._[0][3] = 0.0f;
    rZ._[1][0] = radZSin;
    rZ._[1][1] = radZCos;
    rZ._[1][2] = 0.0f;
    rZ._[1][3] = 0.0f;
    rZ._[2][0] = 0.0f;
    rZ._[2][1] = 0.0f;
    rZ._[2][2] = 1.0f;
    rZ._[2][3] = 0.0f;
    rZ._[3][0] = 0.0f;
    rZ._[3][1] = 0.0f;
    rZ._[3][2] = 0.0f;
    rZ._[3][3] = 1.0f;

    *this = rZ * rY * rX;
}
void Mat4F::translate(float p_x, float p_y, float p_z) {
    _[0][0] = 1.0f;
    _[0][1] = 0.0f;
    _[0][2] = 0.0f;
    _[0][3] = p_x;
    _[1][0] = 0.0f;
    _[1][1] = 1.0f;
    _[1][2] = 0.0f;
    _[1][3] = p_y;
    _[2][0] = 0.0f;
    _[2][1] = 0.0f;
    _[2][2] = 1.0f;
    _[2][3] = p_z;
    _[3][0] = 0.0f;
    _[3][1] = 0.0f;
    _[3][2] = 0.0f;
    _[3][3] = 1.0f;
}
void Mat4F::transpose() {
    float m[4][4];
    m[0][0] = _[0][0];
    m[1][0] = _[0][1];
    m[2][0] = _[0][2];
    m[3][0] = _[0][3];

    m[0][1] = _[1][0];
    m[1][1] = _[1][1];
    m[2][1] = _[1][2];
    m[3][1] = _[1][3];

    m[0][2] = _[2][0];
    m[1][2] = _[2][1];
    m[2][2] = _[2][2];
    m[3][2] = _[2][3];

    m[0][3] = _[3][0];
    m[1][3] = _[3][1];
    m[2][3] = _[3][2];
    m[3][3] = _[3][3];

    for(unsigned i = 0; i<4; i++) {
        for(unsigned j = 0; j<4; j++) {
            _[i][j] = m[i][j];
        }
    }
}
void Mat4F::inverse() {
    DirectX::XMFLOAT4X4 xm4x4(_[0]);
    DirectX::XMMATRIX xmMat = DirectX::XMLoadFloat4x4(&xm4x4);
    DirectX::XMVECTOR xmDet = DirectX::XMMatrixDeterminant(xmMat);
    DirectX::XMMATRIX xmMatInv = DirectX::XMMatrixInverse(&xmDet, xmMat);

    DirectX::XMFLOAT4X4 xm4x4Inv;
    DirectX::XMStoreFloat4x4(&xm4x4Inv, xmMatInv);
    memcpy(_[0], xm4x4Inv.m[0], sizeof(Mat4F));
}
void Mat4F::lookAtLH(Vec3F p_eye, Vec3F p_at, Vec3F p_up) {
    Vec3F axisZ = (p_at - p_eye).normalize();
    Vec3F axisX = (p_up.cross(axisZ)).normalize();
    Vec3F axisY = axisZ.cross(axisX);

    float xDotEye = -axisX.dot(p_eye);
    float yDotEye = -axisY.dot(p_eye);
    float zDotEye = -axisZ.dot(p_eye);
    _[0][0] = axisX.x;
    _[0][1] = axisY.x;
    _[0][2] = axisZ.x;
    _[0][3] = 0.0f;
    _[1][0] = axisX.y;
    _[1][1] = axisY.y;
    _[1][2] = axisZ.y;
    _[1][3] = 0.0f;
    _[2][0] = axisX.z;
    _[2][1] = axisY.z;
    _[2][2] = axisZ.z;
    _[2][3] = 0.0f;
    _[3][0] = xDotEye;
    _[3][1] = yDotEye;
    _[3][3] = zDotEye;
    _[3][3] = 1.0f;
}

Mat4F Mat4F::Identity() {
    Mat4F m;
    m._[0][0] = 1.0f;
    m._[0][1] = 0.0f;
    m._[0][2] = 0.0f;
    m._[0][3] = 0.0f;
    m._[1][0] = 0.0f;
    m._[1][1] = 1.0f;
    m._[1][2] = 0.0f;
    m._[1][3] = 0.0f;
    m._[2][0] = 0.0f;
    m._[2][1] = 0.0f;
    m._[2][2] = 1.0f;
    m._[2][3] = 0.0f;
    m._[3][0] = 0.0f;
    m._[3][1] = 0.0f;
    m._[3][2] = 0.0f;
    m._[3][3] = 1.0f;
    return m;
}

Quaternion::Quaternion() {
    *this = Identity();
}
Quaternion::Quaternion(float p_x, float p_y, float p_z, float p_w) {
    x = p_x;
    y = p_y;
    z = p_z;
    w = p_w;
}
Quaternion::~Quaternion() {
}

float Quaternion::length() const {
    return sqrtf(x * x + y * y + z * z + w * w);
}
void Quaternion::normalize() {
    float length = this->length();
    x /= length;
    y /= length;
    z /= length;
    w /= length;
}
Quaternion Quaternion::conjugate() {
    return Quaternion(-x, -y, -z, w);
}
Mat4F Quaternion::toMatrix() {
    Mat4F m;
    m._[0][0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
    m._[0][1] = 2.0f * x * y - 2.0f * z * w;
    m._[0][2] = 2.0f * x * z + 2.0f * y * w;
    m._[0][3] = 0.0f;

    m._[1][0] = 2.0f * x * y + 2.0f * z * w;
    m._[1][1] = 1.0f - 2.0f * x * x - 2.0f * z * z;
    m._[1][2] = 2.0f * y * z - 2.0f * x * w;
    m._[1][3] = 0.0f;

    m._[2][0] = 2.0f * x * z - 2.0f * y * w;
    m._[2][1] = 2.0f * y * z + 2.0f * x * w;
    m._[2][2] = 1.0f - 2.0f * x * x - 2.0f * y * y;
    m._[2][3] = 0.0f;

    m._[3][0] = 0.0f;
    m._[3][1] = 0.0f;
    m._[3][2] = 0.0f;
    m._[3][3] = 1.0f;
    return m;
}
Quaternion Quaternion::Identity() {
    return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

Quaternion operator*(const Quaternion& p_l, const Quaternion& p_r) {
    const float x = (p_l.x * p_r.w) + (p_l.w * p_r.x) + (p_l.y * p_r.z) - (p_l.z * p_r.y);
    const float y = (p_l.y * p_r.w) + (p_l.w * p_r.y) + (p_l.z * p_r.x) - (p_l.x * p_r.z);
    const float z = (p_l.z * p_r.w) + (p_l.w * p_r.z) + (p_l.x * p_r.y) - (p_l.y * p_r.x);
    const float w = (p_l.w * p_r.w) - (p_l.x * p_r.x) - (p_l.y * p_r.y) - (p_l.z * p_r.z);
    return Quaternion(x, y, z, w);
}
Quaternion operator*(const Quaternion& p_q, const Vec3F& p_v) {
    const float x = (p_q.w * p_v.x) + (p_q.y * p_v.z) - (p_q.z * p_v.y);
    const float y = (p_q.w * p_v.y) + (p_q.z * p_v.x) - (p_q.x * p_v.z);
    const float z = (p_q.w * p_v.z) + (p_q.x * p_v.y) - (p_q.y * p_v.x);
    const float w = -(p_q.x * p_v.x) - (p_q.y * p_v.y) - (p_q.z * p_v.z);
    return Quaternion(x, y, z, w);
}
