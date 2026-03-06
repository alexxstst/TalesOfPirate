//
#pragma once

#include "lwHeader.h"


LW_BEGIN

//#ifdef LW_USE_LWMATH_LIB

// _lwVector2
LW_INLINE _lwVector2::_lwVector2() 
{}

LW_INLINE _lwVector2::_lwVector2(float xo, float yo) 
: x(xo), y(yo) 
{}

LW_INLINE _lwVector2::_lwVector2(const float* v) 
: x(v[0]), y(v[1])
{
}

LW_INLINE _lwVector2::_lwVector2(const _lwVector2& v)
: x(v.x), y(v.y) 
{}

// casting
LW_INLINE _lwVector2::operator float*()
{
    return (float *)&x;
}

LW_INLINE _lwVector2::operator const float*() const
{
    return (const float *)&x;
}

// unary operators
LW_INLINE _lwVector2 _lwVector2::operator + () const
{
    return *this;
}

LW_INLINE _lwVector2 _lwVector2::operator - () const
{
    return _lwVector2(-x, -y);
}

LW_INLINE _lwVector2& _lwVector2::operator+=(const _lwVector2& v) 
{ 
    x += v.x; 
    y += v.y; 
    return *this; 
}
LW_INLINE _lwVector2& _lwVector2::operator-=(const _lwVector2& v) 
{ 
    x -= v.x; 
    y -= v.y; 
    return *this; 
}

LW_INLINE _lwVector2& _lwVector2::operator*=(float f) 
{ 
    x *= f; 
    y *= f; 
    return *this; 
}

LW_INLINE _lwVector2& _lwVector2::operator/=(float f) 
{ 
    float inv = 1.0f / f;
    x *= inv;
    y *= inv;
    return *this;
}


LW_INLINE float _lwVector2::operator[](int i) const 
{ 
    return ((float*)&x)[i]; 
}
LW_INLINE float& _lwVector2::operator[](int i) 
{ 
    return ((float*)&x)[i]; 
}

// binary operators
LW_INLINE _lwVector2 _lwVector2::operator+(const _lwVector2& v) const
{
    return _lwVector2(x + v.x, y + v.y);
}

LW_INLINE _lwVector2 _lwVector2::operator-(const _lwVector2& v) const
{
    return _lwVector2(x - v.x, y - v.y);
}

LW_INLINE _lwVector2 _lwVector2::operator*(float v) const
{
    return _lwVector2(x * v, y * v);
}

LW_INLINE _lwVector2 _lwVector2::operator/(float v) const
{
    float inv = 1.0f / v;
    return _lwVector2(x * inv, y * inv);
}


LW_INLINE _lwVector2 operator*(float f, const _lwVector2& v)
{
    return _lwVector2(f * v.x, f * v.y);
}

LW_INLINE BOOL _lwVector2::operator==(const _lwVector2& v) const
{
    return x == v.x && y == v.y;
}

LW_INLINE BOOL _lwVector2::operator!=(const _lwVector2& v) const
{
    return x != v.x || y != v.y;
}

// _lwVector2i
LW_INLINE _lwVector2i::_lwVector2i() 
{}

LW_INLINE _lwVector2i::_lwVector2i(int xo, int yo) 
: x(xo), y(yo) 
{}

LW_INLINE _lwVector2i::_lwVector2i(const int* v) 
: x(v[0]), y(v[1])
{
}

LW_INLINE _lwVector2i::_lwVector2i(const _lwVector2i& v)
: x(v.x), y(v.y) 
{}

// casting
LW_INLINE _lwVector2i::operator int*()
{
    return (int *)&x;
}

LW_INLINE _lwVector2i::operator const int*() const
{
    return (const int *)&x;
}

// unary operators
LW_INLINE _lwVector2i _lwVector2i::operator + () const
{
    return *this;
}

LW_INLINE _lwVector2i _lwVector2i::operator - () const
{
    return _lwVector2i(-x, -y);
}

LW_INLINE _lwVector2i& _lwVector2i::operator+=(const _lwVector2i& v) 
{ 
    x += v.x; 
    y += v.y; 
    return *this; 
}
LW_INLINE _lwVector2i& _lwVector2i::operator-=(const _lwVector2i& v) 
{ 
    x -= v.x; 
    y -= v.y; 
    return *this; 
}

LW_INLINE _lwVector2i& _lwVector2i::operator*=(int f) 
{ 
    x *= f; 
    y *= f; 
    return *this; 
}

LW_INLINE _lwVector2i& _lwVector2i::operator/=(int f) 
{ 
    x /= f;
    y /= f;
    return *this;
}


LW_INLINE int _lwVector2i::operator[](int i) const 
{ 
    return ((int*)&x)[i]; 
}
LW_INLINE int& _lwVector2i::operator[](int i) 
{ 
    return ((int*)&x)[i]; 
}

// binary operators
LW_INLINE _lwVector2i _lwVector2i::operator+(const _lwVector2i& v) const
{
    return _lwVector2i(x + v.x, y + v.y);
}

LW_INLINE _lwVector2i _lwVector2i::operator-(const _lwVector2i& v) const
{
    return _lwVector2i(x - v.x, y - v.y);
}

LW_INLINE _lwVector2i _lwVector2i::operator*(int v) const
{
    return _lwVector2i(x * v, y * v);
}

LW_INLINE _lwVector2i _lwVector2i::operator/(int v) const
{
    return _lwVector2i(x / v, y / v);
}


LW_INLINE _lwVector2i operator*(int f, const _lwVector2i& v)
{
    return _lwVector2i(f * v.x, f * v.y);
}

LW_INLINE BOOL _lwVector2i::operator==(const _lwVector2i& v) const
{
    return x == v.x && y == v.y;
}

LW_INLINE BOOL _lwVector2i::operator!=(const _lwVector2i& v) const
{
    return x != v.x || y != v.y;
}

// lwVector3i
LW_INLINE _lwVector3i::_lwVector3i()
{}

LW_INLINE _lwVector3i::_lwVector3i(int fx, int fy, int fz)
: x(fx), y(fy), z(fz)
{
}
LW_INLINE _lwVector3i::_lwVector3i(const int *v)
: x(v[0]), y(v[1]), z(v[2])
{
}

LW_INLINE _lwVector3i::_lwVector3i(const _lwVector3i& v)
: x(v.x), y(v.y), z(v.z)
{
}



// casting
LW_INLINE _lwVector3i::operator int*()
{
    return (int *)&x;
}

LW_INLINE _lwVector3i::operator const int*() const
{
    return (const int *)&x;
}


// assignment operators
LW_INLINE _lwVector3i& _lwVector3i::operator+=(const _lwVector3i& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

LW_INLINE _lwVector3i& _lwVector3i::operator-=(const _lwVector3i& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

LW_INLINE _lwVector3i& _lwVector3i::operator*=(int v)
{
    x *= v;
    y *= v;
    z *= v;
    return *this;
}

LW_INLINE _lwVector3i& _lwVector3i::operator /= (int v)
{
    int inv = (int)(1.0f / v);
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}


// unary operators
LW_INLINE _lwVector3i _lwVector3i::operator+() const
{
    return *this;
}

LW_INLINE _lwVector3i _lwVector3i::operator-() const
{
    return _lwVector3i(-x, -y, -z);
}


// binary operators
LW_INLINE _lwVector3i _lwVector3i::operator+(const _lwVector3i& v) const
{
    return _lwVector3i(x + v.x, y + v.y, z + v.z);
}

LW_INLINE _lwVector3i _lwVector3i::operator-(const _lwVector3i& v) const
{
    return _lwVector3i(x - v.x, y - v.y, z - v.z);
}
LW_INLINE _lwVector3i _lwVector3i::operator*(const _lwVector3i& v) const
{
    return _lwVector3i(x * v.x, y * v.y, z * v.z);
}

LW_INLINE _lwVector3i _lwVector3i::operator/(const _lwVector3i& v) const
{
    return _lwVector3i(x / v.x, y / v.y, z / v.z);
}

LW_INLINE _lwVector3i _lwVector3i::operator*(int v) const
{
    return _lwVector3i(x * v, y * v, z * v);
}

LW_INLINE _lwVector3i _lwVector3i::operator/(int v) const
{
    int inv = (int)(1.0f / v);
    return _lwVector3i(x * inv, y * inv, z * inv);
}


LW_INLINE _lwVector3i operator*(int f, const _lwVector3i& v)
{
    return _lwVector3i(f * v.x, f * v.y, f * v.z);
}


LW_INLINE BOOL _lwVector3i::operator==(const _lwVector3i& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

LW_INLINE BOOL _lwVector3i::operator!=(const _lwVector3i& v) const
{
    return x != v.x || y != v.y || z != v.z;
}
LW_INLINE int _lwVector3i::operator[](int i) const 
{ 
    return ((int*)&x)[i]; 
}
LW_INLINE int& _lwVector3i::operator[](int i) 
{ 
    return ((int*)&x)[i]; 
}


// lwVector3
LW_INLINE _lwVector3::_lwVector3()
{}

LW_INLINE _lwVector3::_lwVector3(float fx, float fy, float fz)
: x(fx), y(fy), z(fz)
{
}
LW_INLINE _lwVector3::_lwVector3(const float *v)
: x(v[0]), y(v[1]), z(v[2])
{
}

LW_INLINE _lwVector3::_lwVector3(const _lwVector3& v)
: x(v.x), y(v.y), z(v.z)
{
}



// casting
LW_INLINE _lwVector3::operator float*()
{
    return (float *)&x;
}

LW_INLINE _lwVector3::operator const float*() const
{
    return (const float *)&x;
}


// assignment operators
LW_INLINE _lwVector3& _lwVector3::operator+=(const _lwVector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

LW_INLINE _lwVector3& _lwVector3::operator-=(const _lwVector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

LW_INLINE _lwVector3& _lwVector3::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
    return *this;
}

LW_INLINE _lwVector3& _lwVector3::operator /= (float v)
{
    float inv = 1.0f / v;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}


// unary operators
LW_INLINE _lwVector3 _lwVector3::operator+() const
{
    return *this;
}

LW_INLINE _lwVector3 _lwVector3::operator-() const
{
    return _lwVector3(-x, -y, -z);
}


// binary operators
LW_INLINE _lwVector3 _lwVector3::operator+(const _lwVector3& v) const
{
    return _lwVector3(x + v.x, y + v.y, z + v.z);
}

LW_INLINE _lwVector3 _lwVector3::operator-(const _lwVector3& v) const
{
    return _lwVector3(x - v.x, y - v.y, z - v.z);
}
LW_INLINE _lwVector3 _lwVector3::operator*(const _lwVector3& v) const
{
    return _lwVector3(x * v.x, y * v.y, z * v.z);
}

LW_INLINE _lwVector3 _lwVector3::operator/(const _lwVector3& v) const
{
    return _lwVector3(x / v.x, y / v.y, z / v.z);
}

LW_INLINE _lwVector3 _lwVector3::operator*(float v) const
{
    return _lwVector3(x * v, y * v, z * v);
}

LW_INLINE _lwVector3 _lwVector3::operator/(float v) const
{
    float inv = 1.0f / v;
    return _lwVector3(x * inv, y * inv, z * inv);
}


LW_INLINE _lwVector3 operator*(float f, const _lwVector3& v)
{
    return _lwVector3(f * v.x, f * v.y, f * v.z);
}


LW_INLINE BOOL _lwVector3::operator==(const _lwVector3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

LW_INLINE BOOL _lwVector3::operator!=(const _lwVector3& v) const
{
    return x != v.x || y != v.y || z != v.z;
}
LW_INLINE float _lwVector3::operator[](int i) const 
{ 
    return ((float*)&x)[i]; 
}
LW_INLINE float& _lwVector3::operator[](int i) 
{ 
    return ((float*)&x)[i]; 
}


// lwVector4
LW_INLINE _lwVector4::_lwVector4()
{}

LW_INLINE _lwVector4::_lwVector4(float fx, float fy, float fz, float fw)
: x(fx), y(fy), z(fz), w(fw)
{
}

LW_INLINE _lwVector4::_lwVector4(const float *v)
: x(v[0]), y(v[1]), z(v[2]), w(v[3])
{
}
LW_INLINE _lwVector4::_lwVector4(const lwVector3& v)
: x(v.x), y(v.y), z(v.z), w(1.0f)
{
}
LW_INLINE _lwVector4::_lwVector4(const _lwVector4& v)
: x(v.x), y(v.y), z(v.z), w(v.w)
{
}


// casting
LW_INLINE _lwVector4::operator float*()
{
    return (float *)&x;
}

LW_INLINE _lwVector4::operator const float*() const
{
    return (const float *)&x;
}


// assignment operators
LW_INLINE _lwVector4& _lwVector4::operator+=(const _lwVector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

LW_INLINE _lwVector4& _lwVector4::operator-=(const _lwVector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

LW_INLINE _lwVector4& _lwVector4::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
}

LW_INLINE _lwVector4& _lwVector4::operator /= (float v)
{
    float inv = 1.0f / v;
    x *= inv;
    y *= inv;
    z *= inv;
    w *= inv;
    return *this;
}


// unary operators
LW_INLINE _lwVector4 _lwVector4::operator+() const
{
    return *this;
}

LW_INLINE _lwVector4 _lwVector4::operator-() const
{
    return _lwVector4(-x, -y, -z, -w);
}


// binary operators
LW_INLINE _lwVector4 _lwVector4::operator+(const _lwVector4& v) const
{
    return _lwVector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

LW_INLINE _lwVector4 _lwVector4::operator-(const _lwVector4& v) const
{
    return _lwVector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

LW_INLINE _lwVector4 _lwVector4::operator*(float v) const
{
    return _lwVector4(x * v, y * v, z * v, w * v);
}

LW_INLINE _lwVector4 _lwVector4::operator/(float v) const
{
    float inv = 1.0f / v;
    return _lwVector4(x * inv, y * inv, z * inv, w * inv);
}


LW_INLINE _lwVector4 operator*(float f, const _lwVector4& v)
{
    return _lwVector4(f * v.x, f * v.y, f * v.z, f * v.w);
}


LW_INLINE BOOL _lwVector4::operator==(const _lwVector4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

LW_INLINE BOOL _lwVector4::operator!=(const _lwVector4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}
LW_INLINE float _lwVector4::operator[](int i) const 
{ 
    return ((float*)&x)[i]; 
}
LW_INLINE float& _lwVector4::operator[](int i) 
{ 
    return ((float*)&x)[i]; 
}

//--------------------------
// lwMatrix44
//--------------------------
LW_INLINE _lwMatrix44::_lwMatrix44()
{}

LW_INLINE _lwMatrix44::_lwMatrix44(const float* v)
{
    memcpy(&_11, v, sizeof(_lwMatrix44));
}

LW_INLINE _lwMatrix44::_lwMatrix44(const _lwMatrix44& mat)
{
    memcpy(&_11, &mat, sizeof(_lwMatrix44));
}

LW_INLINE _lwMatrix44::_lwMatrix44(float f11, float f12, float f13, float f14,
                        float f21, float f22, float f23, float f24,
                        float f31, float f32, float f33, float f34,
                        float f41, float f42, float f43, float f44)
{
    _11 = f11; _12 = f12; _13 = f13; _14 = f14;
    _21 = f21; _22 = f22; _23 = f23; _24 = f24;
    _31 = f31; _32 = f32; _33 = f33; _34 = f34;
    _41 = f41; _42 = f42; _43 = f43; _44 = f44;
}



// access grants
LW_INLINE float& _lwMatrix44::operator()(int row, int col)
{
    return m[row][col];
}

LW_INLINE float _lwMatrix44::operator()(int row, int col) const
{
    return m[row][col];
}


// casting operators
LW_INLINE _lwMatrix44::operator float*()
{
    return (float *)&_11;
}

LW_INLINE _lwMatrix44::operator const float*() const
{
    return (const float *)&_11;
}

LW_INLINE float _lwMatrix44::operator[](int i) const 
{ 
    return ((float*)m)[i]; 
}
LW_INLINE float& _lwMatrix44::operator[](int i) 
{ 
    return ((float*)m)[i]; 
}

// assignment operators
LW_INLINE _lwMatrix44& _lwMatrix44::operator*=(const _lwMatrix44& mat)
{
    _lwMatrix44Multiply((lwMatrix44*)this, (lwMatrix44*)this, (lwMatrix44*)&mat);
    return *this;
}

LW_INLINE _lwMatrix44& _lwMatrix44::operator+=(const _lwMatrix44& mat)
{
    _11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
    _21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
    _31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
    _41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
    return *this;
}

LW_INLINE _lwMatrix44& _lwMatrix44::operator-=(const _lwMatrix44& mat)
{
    _11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
    _21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
    _31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
    _41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
    return *this;
}

LW_INLINE _lwMatrix44& _lwMatrix44::operator*=(float v)
{
    _11 *= v; _12 *= v; _13 *= v; _14 *= v;
    _21 *= v; _22 *= v; _23 *= v; _24 *= v;
    _31 *= v; _32 *= v; _33 *= v; _34 *= v;
    _41 *= v; _42 *= v; _43 *= v; _44 *= v;
    return *this;
}

LW_INLINE _lwMatrix44& _lwMatrix44::operator /= (float v)
{
    float inv = 1.0f / v;
    _11 *= inv; _12 *= inv; _13 *= inv; _14 *= inv;
    _21 *= inv; _22 *= inv; _23 *= inv; _24 *= inv;
    _31 *= inv; _32 *= inv; _33 *= inv; _34 *= inv;
    _41 *= inv; _42 *= inv; _43 *= inv; _44 *= inv;
    return *this;
}


// unary operators
LW_INLINE _lwMatrix44 _lwMatrix44::operator+() const
{
    return *this;
}

LW_INLINE _lwMatrix44 _lwMatrix44::operator-() const
{
    return _lwMatrix44(-_11, -_12, -_13, -_14,
                      -_21, -_22, -_23, -_24,
                      -_31, -_32, -_33, -_34,
                      -_41, -_42, -_43, -_44);
}


// binary operators
LW_INLINE _lwMatrix44 _lwMatrix44::operator*(const _lwMatrix44& mat) const
{
    _lwMatrix44 ret;
    _lwMatrix44Multiply((lwMatrix44*)&ret, (lwMatrix44*)this, (lwMatrix44*)&mat);
    return ret;
}

LW_INLINE _lwMatrix44 _lwMatrix44::operator+(const _lwMatrix44& mat) const
{
    return _lwMatrix44(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
                      _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
                      _31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
                      _41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

LW_INLINE _lwMatrix44 _lwMatrix44::operator-(const _lwMatrix44& mat) const
{
    return _lwMatrix44(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
                      _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
                      _31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
                      _41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

LW_INLINE _lwMatrix44 _lwMatrix44::operator*(float v) const
{
    return _lwMatrix44(_11 * v, _12 * v, _13 * v, _14 * v,
                      _21 * v, _22 * v, _23 * v, _24 * v,
                      _31 * v, _32 * v, _33 * v, _34 * v,
                      _41 * v, _42 * v, _43 * v, _44 * v);
}

LW_INLINE _lwMatrix44 _lwMatrix44::operator/(float v) const
{
    float inv = 1.0f / v;
    return _lwMatrix44(_11 * inv, _12 * inv, _13 * inv, _14 * inv,
                      _21 * inv, _22 * inv, _23 * inv, _24 * inv,
                      _31 * inv, _32 * inv, _33 * inv, _34 * inv,
                      _41 * inv, _42 * inv, _43 * inv, _44 * inv);
}


LW_INLINE _lwMatrix44 operator*(float f, const _lwMatrix44& mat)
{
    return _lwMatrix44(f * mat._11, f * mat._12, f * mat._13, f * mat._14,
                      f * mat._21, f * mat._22, f * mat._23, f * mat._24,
                      f * mat._31, f * mat._32, f * mat._33, f * mat._34,
                      f * mat._41, f * mat._42, f * mat._43, f * mat._44);
}


LW_INLINE BOOL _lwMatrix44::operator == (const _lwMatrix44& mat) const
{
    return 0 == memcmp(this, &mat, sizeof(_lwMatrix44));
}

LW_INLINE BOOL _lwMatrix44::operator!=(const _lwMatrix44& mat) const
{
    return 0 != memcmp(this, &mat, sizeof(_lwMatrix44));
}

//--------------------------
// Quaternion
//--------------------------
LW_INLINE _lwQuaternion::_lwQuaternion()
{}

LW_INLINE _lwQuaternion::_lwQuaternion(const float* v)
: x(v[0]), y(v[1]), z(v[2]), w(v[3])
{
}

LW_INLINE _lwQuaternion::_lwQuaternion(float fx, float fy, float fz, float fw)
: x(fx), y(fy), z(fz), w(fw)
{
}


// casting
LW_INLINE _lwQuaternion::operator float*()
{
    return (float *)&x;
}

LW_INLINE _lwQuaternion::operator const float*() const
{
    return (const float *)&x;
}


// assignment operators
LW_INLINE _lwQuaternion& _lwQuaternion::operator+=(const _lwQuaternion& q)
{
    x += q.x;
    y += q.y;
    z += q.z;
    w += q.w;
    return *this;
}

LW_INLINE _lwQuaternion& _lwQuaternion::operator-=(const _lwQuaternion& q)
{
    x -= q.x;
    y -= q.y;
    z -= q.z;
    w -= q.w;
    return *this;
}

LW_INLINE _lwQuaternion& _lwQuaternion::operator*=(const _lwQuaternion& q)
{
    lwQuaternionMultiply((lwQuaternion*)this, (lwQuaternion*)this, (lwQuaternion*)&q);
    return *this;
}

LW_INLINE _lwQuaternion& _lwQuaternion::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
}

LW_INLINE _lwQuaternion& _lwQuaternion::operator/=(float f)
{
    float inv = 1.0f / f;
    x *= inv;
    y *= inv;
    z *= inv;
    w *= inv;
    return *this;
}


// unary operators
LW_INLINE _lwQuaternion _lwQuaternion::operator+() const
{
    return *this;
}

LW_INLINE _lwQuaternion _lwQuaternion::operator-() const
{
    return _lwQuaternion(-x, -y, -z, -w);
}

// binary operators
LW_INLINE _lwQuaternion _lwQuaternion::operator+(const _lwQuaternion& q) const
{
    return _lwQuaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

LW_INLINE _lwQuaternion _lwQuaternion::operator-(const _lwQuaternion& q) const
{
    return _lwQuaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

LW_INLINE _lwQuaternion _lwQuaternion::operator*(const _lwQuaternion& q) const
{
    _lwQuaternion ret;
    _lwQuaternionMultiply((lwQuaternion*)&ret, (lwQuaternion*)this, (lwQuaternion*)&q);
    return ret;
}

LW_INLINE _lwQuaternion _lwQuaternion::operator*(float f) const
{
    return _lwQuaternion(x * f, y * f, z * f, w * f);
}

LW_INLINE _lwQuaternion _lwQuaternion::operator/(float f) const
{
    float inv = 1.0f / f;
    return _lwQuaternion(x * inv, y * inv, z * inv, w * inv);
}


LW_INLINE _lwQuaternion operator*(float f, const _lwQuaternion& q)
{
    return _lwQuaternion(f * q.x, f * q.y, f * q.z, f * q.w);
}


LW_INLINE BOOL _lwQuaternion::operator==(const _lwQuaternion& q) const
{
    return x == q.x && y == q.y && z == q.z && w == q.w;
}

LW_INLINE BOOL _lwQuaternion::operator!=(const _lwQuaternion& q) const
{
    return x != q.x || y != q.y || z != q.z || w != q.w;
}


//#endif
//--------------------------
// lwMatrix33
//--------------------------
LW_INLINE lwMatrix33::lwMatrix33()
{}

LW_INLINE lwMatrix33::lwMatrix33(const float* v)
{
    memcpy(&_11, v, sizeof(lwMatrix33));
}

LW_INLINE lwMatrix33::lwMatrix33(const lwMatrix33& mat)
{
    memcpy(&_11, &mat, sizeof(lwMatrix33));
}

LW_INLINE lwMatrix33::lwMatrix33(float f11, float f12, float f13,
                                  float f21, float f22, float f23,
                                  float f31, float f32, float f33)
{
    _11 = f11; _12 = f12; _13 = f13; 
    _21 = f21; _22 = f22; _23 = f23; 
    _31 = f31; _32 = f32; _33 = f33;
}



// access grants
LW_INLINE float& lwMatrix33::operator()(int row, int col)
{
    return m[row][col];
}

LW_INLINE float lwMatrix33::operator()(int row, int col) const
{
    return m[row][col];
}
LW_INLINE float lwMatrix33::operator[](int i) const 
{ 
    return ((float*)m)[i]; 
}
LW_INLINE float& lwMatrix33::operator[](int i) 
{ 
    return ((float*)m)[i]; 
}

// casting operators
LW_INLINE lwMatrix33::operator float*()
{
    return (float *)&_11;
}

LW_INLINE lwMatrix33::operator const float*() const
{
    return (const float *)&_11;
}


// assignment operators
LW_INLINE lwMatrix33& lwMatrix33::operator*=(const lwMatrix33& mat)
{
    lwMatrix33Multiply(this, this, &mat);
    return *this;
}

LW_INLINE lwMatrix33& lwMatrix33::operator+=(const lwMatrix33& mat)
{
    _11 += mat._11; _12 += mat._12; _13 += mat._13;
    _21 += mat._21; _22 += mat._22; _23 += mat._23;
    _31 += mat._31; _32 += mat._32; _33 += mat._33;
    return *this;
}

LW_INLINE lwMatrix33& lwMatrix33::operator-=(const lwMatrix33& mat)
{
    _11 -= mat._11; _12 -= mat._12; _13 -= mat._13;
    _21 -= mat._21; _22 -= mat._22; _23 -= mat._23;
    _31 -= mat._31; _32 -= mat._32; _33 -= mat._33;
    return *this;
}

LW_INLINE lwMatrix33& lwMatrix33::operator*=(float v)
{
    _11 *= v; _12 *= v; _13 *= v;
    _21 *= v; _22 *= v; _23 *= v;
    _31 *= v; _32 *= v; _33 *= v;
    return *this;
}

LW_INLINE lwMatrix33& lwMatrix33::operator /= (float v)
{
    float inv = 1.0f / v;
    _11 *= inv; _12 *= inv; _13 *= inv;
    _21 *= inv; _22 *= inv; _23 *= inv;
    _31 *= inv; _32 *= inv; _33 *= inv;
    return *this;
}


// unary operators
LW_INLINE lwMatrix33 lwMatrix33::operator+() const
{
    return *this;
}

LW_INLINE lwMatrix33 lwMatrix33::operator-() const
{
    return lwMatrix33(-_11, -_12, -_13,
                      -_21, -_22, -_23,
                      -_31, -_32, -_33);
}


// binary operators
LW_INLINE lwMatrix33 lwMatrix33::operator*(const lwMatrix33& mat) const
{
    lwMatrix33 ret;
    lwMatrix33Multiply(&ret, this, &mat);
    return ret;
}

LW_INLINE lwMatrix33 lwMatrix33::operator+(const lwMatrix33& mat) const
{
    return lwMatrix33(_11 + mat._11, _12 + mat._12, _13 + mat._13,
                      _21 + mat._21, _22 + mat._22, _23 + mat._23, 
                      _31 + mat._31, _32 + mat._32, _33 + mat._33); 
}

LW_INLINE lwMatrix33 lwMatrix33::operator-(const lwMatrix33& mat) const
{
    return lwMatrix33(_11 - mat._11, _12 - mat._12, _13 - mat._13, 
                      _21 - mat._21, _22 - mat._22, _23 - mat._23, 
                      _31 - mat._31, _32 - mat._32, _33 - mat._33);
}

LW_INLINE lwMatrix33 lwMatrix33::operator*(float v) const
{
    return lwMatrix33(_11 * v, _12 * v, _13 * v, 
                      _21 * v, _22 * v, _23 * v, 
                      _31 * v, _32 * v, _33 * v);
}

LW_INLINE lwMatrix33 lwMatrix33::operator/(float v) const
{
    float inv = 1.0f / v;
    return lwMatrix33(_11 * inv, _12 * inv, _13 * inv,
                      _21 * inv, _22 * inv, _23 * inv,
                      _31 * inv, _32 * inv, _33 * inv);
}


LW_INLINE lwMatrix33 operator*(float f, const lwMatrix33& mat)
{
    return lwMatrix33(f * mat._11, f * mat._12, f * mat._13,
                      f * mat._21, f * mat._22, f * mat._23,
                      f * mat._31, f * mat._32, f * mat._33);
}


LW_INLINE BOOL lwMatrix33::operator == (const lwMatrix33& mat) const
{
    return 0 == memcmp(this, &mat, sizeof(lwMatrix33));
}

LW_INLINE BOOL lwMatrix33::operator!=(const lwMatrix33& mat) const
{
    return 0 != memcmp(this, &mat, sizeof(lwMatrix33));
}

//--------------------------
// Plane
//--------------------------

LW_INLINE _lwPlane::_lwPlane()
{}

LW_INLINE _lwPlane::_lwPlane(const float* v)
{
    a = v[0];
    b = v[1];
    c = v[2];
    d = v[3];
}

LW_INLINE _lwPlane::_lwPlane(float fa, float fb, float fc, float fd)
{
    a = fa;
    b = fb;
    c = fc;
    d = fd;
}


// casting
LW_INLINE _lwPlane::operator float*()
{
    return (float *)&a;
}

LW_INLINE _lwPlane::operator const float*() const
{
    return (const float *)&a;
}


// unary operators
LW_INLINE _lwPlane _lwPlane::operator+() const
{
    return *this;
}

LW_INLINE _lwPlane _lwPlane::operator-() const
{
    return _lwPlane(-a, -b, -c, -d);
}


// binary operators
LW_INLINE BOOL _lwPlane::operator==(const _lwPlane& p) const
{
    return a == p.a && b == p.b && c == p.c && d == p.d;
}

LW_INLINE BOOL _lwPlane::operator!=(const _lwPlane& p) const
{
    return a != p.a || b != p.b || c != p.c || d != p.d;
}


// ----------------------------------------
// LW_INLINE method
// ----------------------------------------

// lwVector2
LW_INLINE lwVector2* lwVec2Add(lwVector2* dst, const lwVector2* v1, const lwVector2* v2)
{
	dst->x = v1->x + v2->x;
	dst->y = v1->y + v2->y;

	return dst;	
}
LW_INLINE lwVector2* lwVec2Sub(lwVector2* dst, const lwVector2* v1, const lwVector2* v2)
{
	dst->x = v1->x - v2->x;
	dst->y = v1->y - v2->y;

	return dst;	
}
LW_INLINE lwVector2* lwVec2Scale(lwVector2* dst, const lwVector2* vec, float s)
{
	dst->x = vec->x * s;
	dst->y = vec->y * s;
	return dst;	
}
LW_INLINE float lwVec2Cross(const lwVector2* v1, const lwVector2* v2)
{
    return v1->x * v2->y - v1->y * v2->x;
}

LW_INLINE float lwVec2Dot(const lwVector2* v1, const lwVector2* v2)
{
	return (v1->x*v2->x) + (v1->y*v2->y);
}

LW_INLINE lwVector2* lwVec2Normalize(lwVector2* dst, const lwVector2* src)
{
	float len = 1.0f / (float)sqrt(src->x * src->x + src->y * src->y);

	dst->x = src->x * len;
	dst->y = src->y * len;

	return dst;
}
LW_INLINE lwVector2* lwVec2Normalize(lwVector2* vec)
{
    return lwVec2Normalize(vec, vec);
}

LW_INLINE float lwVec2SquareMagnitude(const lwVector2* vec)
{
	return vec->x * vec->x + vec->y * vec->y;
}
LW_INLINE float lwVec2Magnitude(const lwVector2* vec)
{
	return (float)sqrt(lwVec2SquareMagnitude(vec));
}
LW_INLINE float lwVec2SquareMagnitude(const lwVector2* v1, const lwVector2* v2)
{
	float x = v2->x - v1->x;
	float y = v2->y - v1->y;
	return x * x + y * y;
}
LW_INLINE float lwVec2Magnitude(const lwVector2* v1, const lwVector2* v2)
{
	return (float)sqrt(lwVec2SquareMagnitude(v1,v2));
}
LW_INLINE int lwVec2Equal(const lwVector2* v1, const lwVector2* v2)
{
    return (lwFloatEqual(v1->x,v2->x) && lwFloatEqual(v1->y,v2->y));
}
LW_INLINE int lwVec2Equal(const lwVector2* v1,const lwVector2* v2, float esp)
{
    return (lwFloatEqual(v1->x,v2->x, esp) && lwFloatEqual(v1->y,v2->y, esp));
}

LW_INLINE lwVector2* lwConvertAngleToVector2(lwVector2* v, float radian)
{
    v->x = cosf(radian);
    v->y = sinf(radian);
    return v;
}

///////////////////////////////////////////
// lwVector3
LW_INLINE lwVector3* lwVector3Add(lwVector3* v1, const lwVector3* v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
	v1->z += v2->z;
	return v1;
}
LW_INLINE lwVector3* lwVector3Add(lwVector3* ret,const lwVector3* v1,const lwVector3* v2)
{
	ret->x = v1->x + v2->x;
	ret->y = v1->y + v2->y;
	ret->z = v1->z + v2->z;

	return ret;	
}
LW_INLINE lwVector3* lwVector3Sub(lwVector3* v1,const lwVector3* v2)
{
	v1->x -= v2->x;
	v1->y -= v2->y;
	v1->z -= v2->z;
	return v1;
}

LW_INLINE lwVector3* lwVector3Sub(lwVector3* ret,const lwVector3* v1,const lwVector3* v2)
{
	ret->x = v1->x - v2->x;
	ret->y = v1->y - v2->y;
	ret->z = v1->z - v2->z;

	return ret;	
}
LW_INLINE lwVector3* lwVector3Scale(lwVector3* vec, float s)
{
	vec->x *= s;
	vec->y *= s;
	vec->z *= s;

	return vec;	
}
LW_INLINE lwVector3* lwVector3Scale(lwVector3* v, const lwVector3* s)
{
    v->x *= s->x;
    v->y *= s->y;
    v->z *= s->z;

    return v;
}
LW_INLINE lwVector3* lwVector3Cross(lwVector3* ret,const lwVector3* v1,const lwVector3* v2)
{
    ret->x = v1->y * v2->z - v1->z * v2->y;
    ret->y = v1->z * v2->x - v1->x * v2->z;
    ret->z = v1->x * v2->y - v1->y * v2->x;
	return ret;
}

LW_INLINE float lwVector3Dot(const lwVector3* v1,const lwVector3* v2)
{
	return (v1->x*v2->x) + (v1->y*v2->y) + (v1->z*v2->z);
}
LW_INLINE float lwVector3SquareMagnitude(const lwVector3* vec) 
{ 
	return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z; 
}
LW_INLINE float lwVector3SquareMagnitude(const lwVector3* v1,const lwVector3* v2)
{
    auto vdif = lwVector3(*v1 - *v2);
    return lwVector3SquareMagnitude(&vdif);
}
LW_INLINE float lwVector3Magnitude(const lwVector3* vec)
{ 
	return (float)sqrt(lwVector3SquareMagnitude(vec)); 
}
LW_INLINE float lwVector3Magnitude(const lwVector3* v1,const lwVector3* v2)
{
    return (float)sqrt(lwVector3SquareMagnitude(v1,v2));
}
LW_INLINE lwVector3* lwVector3Normalize(lwVector3* vec)
{
	return lwVector3Scale(vec, 1.0f / lwVector3Magnitude(vec));
}

LW_INLINE int lwVector3Zero(const lwVector3* vec,float eps=1e-3f)
{
    return (lwFloatZero(vec->x,eps) 
        && lwFloatZero(vec->y,eps) 
        && lwFloatZero(vec->z,eps));
}
LW_INLINE int lwVector3Equal(const lwVector3* v1,const lwVector3* v2,float eps=1e-3f)
{
    return (lwFloatZero(v1->x - v2->x,eps)
        && lwFloatZero(v1->y - v2->y,eps)
        && lwFloatZero(v1->z - v2->z,eps));
}

LW_INLINE lwVector3* lwVector3Slerp(lwVector3* ret, const lwVector3* v0, const lwVector3* v1, float t)
{
    *ret = (1.0f - t) * (*v0) + t * (*v1);
    return ret;
}
// lwVector4
//////////////////////////////////////
LW_INLINE lwVector4* lwVec4Add(lwVector4* ret, const lwVector4* v1, const lwVector4* v2)
{
	ret->x = v1->x + v2->x;
	ret->y = v1->y + v2->y;
	ret->z = v1->z + v2->z;
    ret->w = v1->w + v2->w;

	return ret;	
}

LW_INLINE lwVector4* lwVec4Sub(lwVector4* ret,const lwVector4* v1,const lwVector4* v2)
{
	ret->x = v1->x - v2->x;
	ret->y = v1->y - v2->y;
	ret->z = v1->z - v2->z;
    ret->w = v1->w - v2->w;
	return ret;	
}
LW_INLINE lwVector4* lwVec4Scale(lwVector4* v, float s)
{
	v->x *= s;
	v->y *= s;
	v->z *= s;
    v->w *= s;
	return v;	
}
LW_INLINE lwVector4* lwVec4Scale(lwVector4* v, const lwVector4* s)
{
    v->x *= s->x;
    v->y *= s->y;
    v->z *= s->z;
    v->w *= s->w;
    return v;
}

LW_INLINE int lwVec4Equal(const lwVector4* v1, const lwVector4* v2,float eps=1e-3f)
{
    return (lwFloatZero(v1->x - v2->x,eps)
        && lwFloatZero(v1->y - v2->y,eps)
        && lwFloatZero(v1->z - v2->z,eps)
        && lwFloatZero(v1->w - v2->w,eps));
}

// lwMatrix33
////////////////////////////////////////
LW_INLINE lwMatrix33* lwMatrix33Identity(lwMatrix33* m)
{
    return &(*m = lwMatrix33(1.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 1.0f));

}


LW_INLINE lwMatrix33 lwMatrix33Translate(float x,float y)
{
  return lwMatrix33(1.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f,
                        x,   y,  1.0f);
}
LW_INLINE lwMatrix33 lwMatrix33Translate(const lwVector2& v)
{
	return lwMatrix33Translate(v.x, v.y);
}
LW_INLINE lwMatrix33* lwMatrix33Translate(lwMatrix33* mat, float x, float y) 
{
    mat->_31 = x;
	mat->_32 = y;
	return mat;
}

LW_INLINE lwMatrix33 lwMatrix33Scale(float x, float y)
{
   return lwMatrix33(   x, 0.0f, 0.0f,
                      0.0f,    y, 0.0f,
                      0.0f, 0.0f, 1.0f);
}


LW_INLINE lwMatrix33 lwMatrix33Scale(const float s)
{
   return lwMatrix33Scale(s, s);
}
LW_INLINE lwMatrix33* lwMatrix33Scale(lwMatrix33* mat, float x, float y)
{
	mat->_11 = x;
	mat->_22 = y;
	return mat;
}
LW_INLINE lwMatrix33* lwMatrix33Scale(lwMatrix33* mat, float s)
{
	return lwMatrix33Scale(mat, s, s);
}

// @@ All rotate operation is in the Right-Hand-Coordinates System
LW_INLINE lwMatrix33 lwMatrix33Rotate(const float c, const float s)
{
   return lwMatrix33(   c,    s, 0.0f,
					    -s,    c, 0.0f,
					  0.0f, 0.0f, 1.0f);
}
LW_INLINE lwMatrix33 lwMatrix33Rotate(const float theta)
{
   return lwMatrix33Rotate(cosf(theta), sinf(theta));
}

LW_INLINE lwVector2* lwMatrix33GetRow(const lwMatrix33* mat, int row)
{
    return (lwVector2*)mat->m[row];
}

LW_INLINE lwMatrix33* lwMatrix33Rotate(lwMatrix33* mat, float radian)
{
	auto r = lwMatrix33Rotate(radian);
	return lwMatrix33Multiply(mat, mat, &r);
}

// lwMatrix44
LW_INLINE lwMatrix44* lwMatrix44Identity(lwMatrix44* m)
{
	return &(*m = lwMatrix44_Identity);
}

LW_INLINE lwMatrix44 lwMatrix44Translate(float x,float y,float z)
{
  return lwMatrix44(1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                       x,   y,   z,   1.0f);
}
LW_INLINE lwMatrix44 lwMatrix44Translate(const lwVector3& v)
{
	return lwMatrix44Translate(v.x,v.y,v.z);
}
LW_INLINE lwMatrix44* lwMatrix44Translate(lwMatrix44* matrix,float x,float y,float z) 
{
	matrix->_41 = x;
	matrix->_42 = y;
	matrix->_43 = z;
	return matrix;
}

LW_INLINE lwMatrix44 lwMatrix44Scale(float x, float y, float z)
{
   return lwMatrix44(
       x,    0.0f, 0.0f, 0.0f,
       0.0f, y,    0.0f, 0.0f,
       0.0f, 0.0f, z,    0.0f,
       0.0f, 0.0f, 0.0f, 1.0f
       );
}

LW_INLINE lwMatrix44 lwMatrix44Scale(const lwVector3& v)
{
    return lwMatrix44Scale(v.x, v.y, v.z);
}

LW_INLINE lwMatrix44* lwMatrix44Scale(lwMatrix44* matrix,float x,float y,float z)
{
	matrix->_11 = x;
	matrix->_22 = y;
	matrix->_33 = z;
	return matrix;
}
LW_INLINE lwMatrix44* lwMatrix44Scale(lwMatrix44* matrix,float s)
{
    matrix->_11 *= s;
    matrix->_12 *= s;
    matrix->_13 *= s;
    matrix->_14 *= s;

    matrix->_21 *= s;
    matrix->_22 *= s;
    matrix->_23 *= s;
    matrix->_24 *= s;

    matrix->_31 *= s;
    matrix->_32 *= s;
    matrix->_33 *= s;
    matrix->_34 *= s;

    return matrix;
}

// @@ All ratate operation is in the Right-Hand-Coordinates System
LW_INLINE lwMatrix44 lwMatrix44RotateX(const float c, const float s)
{
   return lwMatrix44(1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f,    c,   -s, 0.0f,
					 0.0f,    s,    c, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}
LW_INLINE lwMatrix44 lwMatrix44RotateX(const float theta)
{
   return lwMatrix44RotateX((float) cos(theta), (float) sin(theta));
}


LW_INLINE lwMatrix44 lwMatrix44RotateY(const float c, const float s)
{
   return lwMatrix44(c,    0.0f,    s,    0.0f,
                     0.0f, 1.0f,    0.0f, 0.0f,
                     -s,   0.0f,    c,    0.0f,
                     0.0f, 0.0f,    0.0f, 1.0f);
}

LW_INLINE lwMatrix44 lwMatrix44RotateY(const float theta)
{
   return lwMatrix44RotateY((float) cos(theta), (float) sin(theta));
}

LW_INLINE lwMatrix44 lwMatrix44RotateZ(const float c, const float s)
{
   return lwMatrix44(c,   -s,    0.0f, 0.0f,
                     s,    c,    0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);
}

LW_INLINE lwMatrix44 lwMatrix44RotateZ(const float theta)
{
   return lwMatrix44RotateZ((float) cos(theta), (float) sin(theta));
}
LW_INLINE lwMatrix44* lwMatrix44RotateX(lwMatrix44* matrix, float angle)
{
	auto r = lwMatrix44RotateX(angle);
	return lwMatrix44Multiply(matrix, matrix, &r);
}
LW_INLINE lwMatrix44* lwMatrix44RotateY(lwMatrix44* matrix, float angle)
{
	auto r = lwMatrix44RotateY(angle);
	return lwMatrix44Multiply(matrix, matrix, &r);
}
LW_INLINE lwMatrix44* lwMatrix44RotateZ(lwMatrix44* matrix, float angle)
{
	auto r = lwMatrix44RotateZ(angle);
	return lwMatrix44Multiply(matrix, matrix, &r);
}

LW_INLINE lwVector3* lwMat44GetRow(const lwMatrix44* mat, int row)
{
    return (lwVector3*)mat->m[row];
}

LW_INLINE lwMatrix44* lwMat44SetRow(lwMatrix44* mat, const lwVector3* v, int row)
{
    mat->m[row][0] = v->x;
    mat->m[row][1] = v->y;
    mat->m[row][2] = v->z;
	return mat;
}

// convert matrix
LW_INLINE lwMatrix44* lwConvertMat43ToMat44(lwMatrix44* mat44, const lwMatrix43* mat43)
{
    mat44->_11 = mat43->_11; mat44->_12 = mat43->_12; mat44->_13 = mat43->_13;
    mat44->_21 = mat43->_21; mat44->_22 = mat43->_22; mat44->_23 = mat43->_23;
    mat44->_31 = mat43->_31; mat44->_32 = mat43->_32; mat44->_33 = mat43->_33;
    mat44->_41 = mat43->_41; mat44->_42 = mat43->_42; mat44->_43 = mat43->_43;

    mat44->_14 = mat44->_24 = mat44->_34 = 0.0f;
    mat44->_44 = 1.0f;

    return mat44;
}

LW_INLINE lwMatrix43* lwConvertMat44ToMat43(lwMatrix43* mat43, const lwMatrix44* mat44)
{
    mat43->_11 = mat44->_11; mat43->_12 = mat44->_12; mat43->_13 = mat44->_13;
    mat43->_21 = mat44->_21; mat43->_22 = mat44->_22; mat43->_23 = mat44->_23;
    mat43->_31 = mat44->_31; mat43->_32 = mat44->_32; mat43->_33 = mat44->_33;
    mat43->_41 = mat44->_41; mat43->_42 = mat44->_42; mat43->_43 = mat44->_43;

    return mat43;
}

// ----------------------
// lwQuaternion
// ----------------------
LW_INLINE lwQuaternion* lwQuaternionIdentity(lwQuaternion* q)
{
    q->x = q->y = q->z = 0.0f;
    q->w = 1.0f;
    return q;
}
LW_INLINE lwQuaternion* lwQuaternionConjugate(lwQuaternion* ret, const lwQuaternion* q)
{
    ret->x = -q->x;
    ret->y = -q->y;
    ret->z = -q->z;
    ret->w = q->w;
    return ret;
}

LW_INLINE float lwQuaternionSquareMagnitude(const lwQuaternion* q)
{
    return q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w;
}
LW_INLINE float lwQuaternionMagnitude(const lwQuaternion* q)
{
    return sqrtf(lwQuaternionSquareMagnitude(q));
}
LW_INLINE lwQuaternion* lwQuaternionNormalize(lwQuaternion* ret, const lwQuaternion* q)
{
    // q / magnitude
    float inv_m = 1.0f / lwQuaternionMagnitude(q);
    ret->x = q->x * inv_m;
    ret->y = q->y * inv_m;
    ret->z = q->z * inv_m;
    ret->w = q->w * inv_m;

    return ret;
}
LW_INLINE lwQuaternion* lwQuaternionInverse(lwQuaternion* ret, const lwQuaternion* q)
{
    // inverse q^ = conjugate q / normalize q
    // if q is a unit quaternion then q^ = conjugate q
    lwQuaternionNormalize(ret, q);
    lwQuaternionConjugate(ret, ret);

    return ret;
}
// ----------------------
// lwPlane
// ----------------------

// ax + by + cz + dw
LW_INLINE float lwPlaneDot(const lwPlane* p, const lwVector4* v)
{
    return p->a * v->x + p->b * v->y + p->c * v->z + p->d * v->w;
}

// ax + by + cz + d
LW_INLINE float lwPlaneDotCoord(const lwPlane *p, const lwVector3 *v)
{
    return p->a * v->x + p->b * v->y + p->c * v->z + p->d;
}

// ax + by + cz
LW_INLINE float lwPlaneDotNormal(const lwPlane *p, const lwVector3 *v)
{
    return p->a * v->x + p->b * v->y + p->c * v->z;
}

LW_INLINE DWORD lwPlaneClassifyVertex(const lwPlane* p, const lwVector3* v)
{
    float d = lwPlaneDotCoord(p, v);
    return (d == 0.0f) ? PLANE_ON_SIDE : ((d > 0.0f) ? PLANE_FRONT_SIDE : PLANE_BACK_SIDE);
}
// > 0.0f: up
// == 0.0f: on
// < 0.0f: below
#define lwPlaneClassify lwPlaneDotCoord

LW_INLINE int lwPlaneGetVectorWeight(lwVector3* v, int flag, const lwPlane* p)
{
	(*v)[flag] = 0.0f;

	float w = lwPlaneClassify(p, v);
    float k = ((float*)&p->a)[flag];

    if(k != 0.0f) 
    {
		(*v)[flag] = -w / k;		
		return 1;
	}

	return (w == 0.0f);
}


LW_END
