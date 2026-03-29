//
#pragma once

#include <math.h>
#include "lwHeader.h"
#include "lwStdInc.h"

#include "lwFrontAPI.h"
//

// =============================
#define LW_USE_DXMATH_LIB
// =============================


#ifdef LW_USE_DXMATH_LIB

#include "lwDirectX.h"

// dll-interface to be used by clients
#pragma warning(disable : 4251)

#define LWVECTOR2_DECL
#define LWVECTOR3_DECL
#define LWVECTOR4_DECL
#define LWMATRIX44_DECL
#define LWQUATERNION_DECL

typedef D3DXVECTOR2 lwVector2;
typedef D3DXVECTOR3 lwVector3;
typedef D3DXVECTOR4 lwVector4;
typedef D3DXMATRIX lwMatrix44;
typedef D3DXQUATERNION lwQuaternion;

#define lwMatrix44Multiply D3DXMatrixMultiply
#define lwMatrix44Inverse D3DXMatrixInverse
#define lwMatrix44Transpose D3DXMatrixTranspose
#define lwQuaternionMultiply(ret, q1, q2) \
    D3DXQuaternionMultiply(ret, q2, q1)


#endif

LW_BEGIN


/////////

#define OP_MATRIX_REPLACE		0
#define OP_MATRIX_LOCAL			1
#define OP_MATRIX_GLOBAL		2

// axes ID
enum lwAxisDef 
{ 
	LW_AXIS_X = 0,
	LW_AXIS_Y = 1,
	LW_AXIS_Z = 2,
	LW_AXIS_INVALID = 0xffffffff
};


// define pi value
const float LW_PI = 3.1415926535897f;
const float LW_HALF_PI = LW_PI * 0.5f;
const float LW_2_PI = LW_PI * 2.0f;
const float LW_ANGLE_TO_RADIAN = (LW_PI / 180.0f);
const float LW_RADIAN_TO_ANGLE = (180.0f / LW_PI);

const float epsilon = 1e-6f;
inline int lwFloatZero(float f,float e) { return fabsf(f)<e; }
inline int lwFloatEqual(float f1,float f2,float e) { return lwFloatZero(f1-f2,e); }
inline int lwFloatZero(float f) { return lwFloatZero(f,epsilon); }
inline int lwFloatEqual(float f1,float f2) { return lwFloatZero(f1-f2,epsilon); }
inline int lwFloatZero(double f, double e) { return fabs(f) < e; }
inline int lwFloatEqual(double f1, double f2, double e) { return lwFloatZero(f1 - f2, e); }

enum lwPlaneClassifyType
{
    PLANE_ON_SIDE =          0,
    PLANE_FRONT_SIDE =       1,
    PLANE_BACK_SIDE =        2,
};
///////////
//////////////////



//#ifdef LW_USE_LWMATH_LIB

//class lwVector3;
//class lwMatrix33;
//class lwMatrix44;
//class lwPlane;

class _lwVector2
{
public:
  float x, y;

public:
    _lwVector2();
    _lwVector2(float x, float y);
    _lwVector2(const float* v);
    _lwVector2(const _lwVector2& v);

    // casting
    operator float*();
    operator const float*() const;

    // assignment operators
    _lwVector2& operator+=(const _lwVector2& v);
    _lwVector2& operator-=(const _lwVector2& v);
    _lwVector2& operator*=(float v);
    _lwVector2& operator/=(float v);

    // unary operators
    _lwVector2 operator+() const;
    _lwVector2 operator-() const;

    // binary operators
    _lwVector2 operator+(const _lwVector2& v) const;
    _lwVector2 operator-(const _lwVector2& v) const;
    _lwVector2 operator*(float v) const;
    _lwVector2 operator/(float v) const;

    friend _lwVector2 operator * (float, const _lwVector2& v);

    BOOL operator == (const _lwVector2& v) const;
    BOOL operator != (const _lwVector2& v) const;

    float operator[](int i) const;
    float& operator[](int i);
};

#ifndef LWVECTOR2_DECL
#define LWVECTOR2_DECL
typedef _lwVector2 lwVector2;
#endif


class _lwVector2i
{
public:
    int x, y;

public:
    _lwVector2i();
    _lwVector2i(int x, int y);
    _lwVector2i(const int* v);
    _lwVector2i(const _lwVector2i& v);

    // casting
    operator int*();
    operator const int*() const;

    // assignment operators
    _lwVector2i& operator+=(const _lwVector2i& v);
    _lwVector2i& operator-=(const _lwVector2i& v);
    _lwVector2i& operator*=(int v);
    _lwVector2i& operator/=(int v);

    // unary operators
    _lwVector2i operator+() const;
    _lwVector2i operator-() const;

    // binary operators
    _lwVector2i operator+(const _lwVector2i& v) const;
    _lwVector2i operator-(const _lwVector2i& v) const;
    _lwVector2i operator*(int v) const;
    _lwVector2i operator/(int v) const;

    friend _lwVector2i operator * (int, const _lwVector2i& v);

    BOOL operator == (const _lwVector2i& v) const;
    BOOL operator != (const _lwVector2i& v) const;

    int operator[](int i) const;
    int& operator[](int i);

};
#ifndef LWVECTOR2I_DECL
#define LWVECTOR2I_DECL
typedef _lwVector2i lwVector2i;
#endif

class _lwVector3i
{
public:
    int x, y, z;

public:
    _lwVector3i();
    _lwVector3i(const int * v);
    _lwVector3i(const _lwVector3i& v);
    _lwVector3i(int x, int y, int z);

    // casting
    operator int* ();
    operator const int* () const;

    // assignment operators
    _lwVector3i& operator+=(const _lwVector3i& v);
    _lwVector3i& operator-=(const _lwVector3i& v);
    _lwVector3i& operator*=(int v);
    _lwVector3i& operator/=(int v);

    // unary operators
    _lwVector3i operator+() const;
    _lwVector3i operator-() const;

    // binary operators
    _lwVector3i operator+(const _lwVector3i& v) const;
    _lwVector3i operator-(const _lwVector3i& v) const;
    _lwVector3i operator*(const _lwVector3i& v) const;
    _lwVector3i operator/(const _lwVector3i& v) const;
    _lwVector3i operator*(int v) const;
    _lwVector3i operator/(int v) const;

    friend _lwVector3i operator*(int f, const _lwVector3i& v);

    BOOL operator==(const _lwVector3i& v) const;
    BOOL operator!=(const _lwVector3i& v) const;

    int operator[](int i) const;
    int& operator[](int i);
};

#ifndef LWVECTOR3I_DECL
#define LWVECTOR3I_DECL
typedef _lwVector3i lwVector3i;
#endif

// lwVector3
/////////////////////////////////////
class _lwVector3
{
public:
    float x, y, z;

public:
    _lwVector3();
    _lwVector3(const float * v);
    _lwVector3(const _lwVector3& v);
    _lwVector3(float x, float y, float z);

    // casting
    operator float* ();
    operator const float* () const;

    // assignment operators
    _lwVector3& operator+=(const _lwVector3& v);
    _lwVector3& operator-=(const _lwVector3& v);
    _lwVector3& operator*=(float v);
    _lwVector3& operator/=(float v);

    // unary operators
    _lwVector3 operator+() const;
    _lwVector3 operator-() const;

    // binary operators
    _lwVector3 operator+(const _lwVector3& v) const;
    _lwVector3 operator-(const _lwVector3& v) const;
    _lwVector3 operator*(const _lwVector3& v) const;
    _lwVector3 operator/(const _lwVector3& v) const;
    _lwVector3 operator*(float v) const;
    _lwVector3 operator/(float v) const;

    friend _lwVector3 operator*(float f, const _lwVector3& v);

    BOOL operator==(const _lwVector3& v) const;
    BOOL operator!=(const _lwVector3& v) const;

    float operator[](int i) const;
    float& operator[](int i);
};

#ifndef LWVECTOR3_DECL
#define LWVECTOR3_DECL
typedef _lwVector3 lwVector3;
#endif


// lwVector4
/////////////////////////////////
class _lwVector4
{
public:
  float x, y, z, w;


public:
    _lwVector4();
    _lwVector4(float xo, float yo, float zo, float wo);
    _lwVector4(const float* v);
    _lwVector4(const lwVector3& v);
    _lwVector4(const _lwVector4& v);

    // casting
    operator float* ();
    operator const float* () const;

    // assignment operators
    _lwVector4& operator+=(const _lwVector4& v);
    _lwVector4& operator-=(const _lwVector4& v);
    _lwVector4& operator*=(float v);
    _lwVector4& operator/=(float v);

    // unary operators
    _lwVector4 operator+() const;
    _lwVector4 operator-() const;

    // binary operators
    _lwVector4 operator+(const _lwVector4& v) const;
    _lwVector4 operator-(const _lwVector4& v) const;
    _lwVector4 operator*(float v) const;
    _lwVector4 operator/(float v) const;

    friend _lwVector4 operator*(float f, const _lwVector4& v);

    BOOL operator==(const _lwVector4& v) const;
    BOOL operator!=(const _lwVector4& v) const;

    float operator[](int i) const;
    float& operator[](int i);
};

#ifndef LWVECTOR4_DECL
#define LWVECTOR4_DECL
typedef _lwVector4 lwVector4;
#endif

// lwMatrix44
///////////////////////
class _lwMatrix44
{
public:
    union 
    {
        struct 
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };

public:
    _lwMatrix44();
    _lwMatrix44(const float *v);
    _lwMatrix44(const _lwMatrix44& v);
    _lwMatrix44(float _11, float _12, float _13, float _14,
                float _21, float _22, float _23, float _24,
                float _31, float _32, float _33, float _34,
                float _41, float _42, float _43, float _44);


    // access grants
    float& operator()(int row, int col);
    float operator()(int row, int col) const;

    float operator[] (int i) const;
    float& operator[] (int i);

    // casting operators
    operator float*();
    operator const float*() const;

    // assignment operators
    _lwMatrix44& operator*=(const _lwMatrix44& v);
    _lwMatrix44& operator+=(const _lwMatrix44& v);
    _lwMatrix44& operator-=(const _lwMatrix44& v);
    _lwMatrix44& operator*=(float v);
    _lwMatrix44& operator/=(float v);

    // unary operators
    _lwMatrix44 operator+() const;
    _lwMatrix44 operator-() const;

    // binary operators
    _lwMatrix44 operator*(const _lwMatrix44& v) const;
    _lwMatrix44 operator+(const _lwMatrix44& v) const;
    _lwMatrix44 operator-(const _lwMatrix44& v) const;
    _lwMatrix44 operator*(float v) const;
    _lwMatrix44 operator/(float v) const;

    friend _lwMatrix44 operator*(float f, const _lwMatrix44& v);

    BOOL operator==(const _lwMatrix44& v) const;
    BOOL operator!=(const _lwMatrix44& v) const;
};

#ifndef LWMATRIX44_DECL
#define LWMATRIX44_DECL
typedef _lwMatrix44 lwMatrix44;
#endif


//--------------------------
// Quaternion
//--------------------------

class _lwQuaternion
{
public:
    float x, y, z, w;
    
public:
    _lwQuaternion();
    _lwQuaternion(const float * v);
    _lwQuaternion(float x, float y, float z, float w);

    // casting
    operator float*();
    operator const float*() const;

    // assignment operators
    _lwQuaternion& operator+=(const _lwQuaternion& q);
    _lwQuaternion& operator-=(const _lwQuaternion& q);
    _lwQuaternion& operator*=(const _lwQuaternion& q);
    _lwQuaternion& operator*=(float f);
    _lwQuaternion& operator/=(float f);

    // unary operators
    _lwQuaternion operator+() const;
    _lwQuaternion operator-() const;

    // binary operators
    _lwQuaternion operator+(const _lwQuaternion& q) const;
    _lwQuaternion operator-(const _lwQuaternion& q) const;
    _lwQuaternion operator*(const _lwQuaternion& q) const;
    _lwQuaternion operator*(float f) const;
    _lwQuaternion operator/(float f) const;

    friend _lwQuaternion operator*(float f, const _lwQuaternion& q);

    BOOL operator==(const _lwQuaternion& q) const;
    BOOL operator!=(const _lwQuaternion& q) const;

};
#ifndef LWQUATERNION_DECL
#define LWQUATERNION_DECL
typedef _lwQuaternion lwQuaternion;
#endif



//#endif

// lwMatrix33
///////////////////////
class lwMatrix33
{
public:
    union 
    {
        struct 
        {
            float _11, _12, _13;
            float _21, _22, _23;
            float _31, _32, _33;
        };
        float m[3][3];
    };

public:
    lwMatrix33();
    lwMatrix33(const float *v);
    lwMatrix33(const lwMatrix33& v);
    lwMatrix33(float _11, float _12, float _13, 
                float _21, float _22, float _23, 
                float _31, float _32, float _33);


    // access grants
    float& operator()(int row, int col);
    float operator()(int row, int col) const;

    float operator[](int i) const;
    float& operator[](int i);

    // casting operators
    operator float*();
    operator const float*() const;

    // assignment operators
    lwMatrix33& operator*=(const lwMatrix33& v);
    lwMatrix33& operator+=(const lwMatrix33& v);
    lwMatrix33& operator-=(const lwMatrix33& v);
    lwMatrix33& operator*=(float v);
    lwMatrix33& operator/=(float v);

    // unary operators
    lwMatrix33 operator+() const;
    lwMatrix33 operator-() const;

    // binary operators
    lwMatrix33 operator*(const lwMatrix33& v) const;
    lwMatrix33 operator+(const lwMatrix33& v) const;
    lwMatrix33 operator-(const lwMatrix33& v) const;
    lwMatrix33 operator*(float v) const;
    lwMatrix33 operator/(float v) const;

    friend lwMatrix33 operator*(float f, const lwMatrix33& v);

    BOOL operator==(const lwMatrix33& v) const;
    BOOL operator!=(const lwMatrix33& v) const;

};

// lwMatrix43
///////////////////////
class lwMatrix43
{
public:
    union {
        struct {
            float _11, _12, _13;
            float _21, _22, _23;
            float _31, _32, _33;
            float _41, _42, _43;
        };
        float m[4][3];
    };

public:
  //static const lwMatrix43 Identity;

  lwMatrix43() {}

  lwMatrix43(float f11, float f12, float f13,
             float f21, float f22, float f23,
             float f31, float f32, float f33,
             float f41, float f42, float f43)
  {
    _11=f11; _12=f12; _13=f13;
    _21=f21; _22=f22; _23=f23;
    _31=f31; _32=f32; _33=f33;
    _41=f41; _42=f42; _43=f43;
  }

  lwMatrix43(const lwMatrix43& m)
  {
      *this = m;
  }
  //lwMatrix43(const lwMatrix44& m)
  //{
  //    _11 = m._11; _12 = m._12; _13 = m._13;
  //    _21 = m._21; _22 = m._22; _23 = m._23;
  //    _31 = m._31; _32 = m._32; _33 = m._33;
  //    _41 = m._41; _42 = m._42; _43 = m._43;
  //}

  inline float operator() (int i, int j) const { return m[i][j]; }
  inline float& operator() (int i, int j) { return m[i][j]; }

  inline float operator[] (int i) const { return ((float*)m)[i]; }
  inline float& operator[] (int i) { return ((float*)m)[i]; }

};


// plane
class _lwPlane
{
public:
    float a, b, c, d;

    _lwPlane();
    _lwPlane(const float* v);
    _lwPlane(float fa, float fb, float fc, float fd);

    // casting
    operator float*();
    operator const float*() const;

    // unary operators
    _lwPlane operator+() const;
    _lwPlane operator-() const;

    // binary operators
    BOOL operator==(const _lwPlane& p) const;
    BOOL operator!=(const _lwPlane& p) const;
};
#ifndef LWPLANE_DECL
#define LWPLANE_DECL
typedef _lwPlane lwPlane;
#endif


// triangle
class lwTriangle
{
public:
    union {
        struct {
            lwVector3 v0, v1, v2; // vector
        };
        struct {
            lwVector3 v[3];
        };
    };

  lwVector3 c; // center
  lwVector3 n; // normal

public:
  lwTriangle() {}
  lwTriangle(const lwVector3& v0_, const lwVector3& v1_, const lwVector3& v2_)
	  : v0(v0_), v1(v1_), v2(v2_)
  {
      // do not use  outside funciton, expand vector cross
	  //lwVector3Cross(&n, &lwVector3(v1_ - v0_), &lwVector3(v2_ - v0_));
      lwVector3 c0(v1_ - v0_);
      lwVector3 c1(v2_ - v0_);
      n.x = c0.y * c1.z - c0.z * c1.y;
      n.y = c0.z * c1.x - c0.x * c1.z;
      n.z = c0.x * c1.y - c0.y * c1.x;

	  c = (v0 + v1 + v2);
      c *= (1.0f / 3);
  }
};

class lwBox
{
public:
    lwVector3 c;
    lwVector3 r;

public:
    lwBox() {}
    lwBox(float cx,float cy,float cz,float rx,float ry,float rz)
        : c(cx,cy,cz), r(rx,ry,rz) 
    {}
    lwBox(const lwVector3& center,const lwVector3& radius)
        : c(center), r(radius) 
    {}
    lwBox(const lwBox& box)
        : c(box.c),r(box.r) 
    {}

    lwVector3* GetPointWithMask(lwVector3* out_v, DWORD mask) const
    {
        out_v->x = (mask & 0x100) ? (c.x + r.x) : (c.x - r.x);
        out_v->y = (mask & 0x010) ? (c.y + r.y) : (c.y - r.y);
        out_v->z = (mask & 0x001) ? (c.z + r.z) : (c.z - r.z);

        return out_v;
    }
};


class lwRound
{
public:
    lwVector2 c;
    float r;

public:
    lwRound() {}
    lwRound(const lwVector2& c_, float r_) : c(c_), r(r_) {}
};

class lwSphere
{
public:
    lwVector3 c;
    float r;

    lwSphere() {}
    lwSphere(const lwVector3& c_, float r_) : c(c_), r(r_) {}
};

class _lwRect
{
public:
    int left;
    int top;
    int right;
    int bottom;

public:
    _lwRect()
        :left(0), top(0), right(0), bottom(0)
    {}
    _lwRect(int l, int t, int r, int b)
        : left(l), top(t), right(r), bottom(b)
    {}
    _lwRect(const lwVector2i& lt, const lwVector2i& rb)
        : left(lt.x), top(lt.y), right(rb.x), bottom(rb.y)
    {}
    _lwRect(const _lwRect& s)
        : left(s.left), top(s.top), right(s.right), bottom(s.bottom)
    {}

    // 0: not in, 1: in rect area, 2: on the side, 3: on the corner
    DWORD PointInRect(const lwVector2i* p) const
    {
        DWORD ret;
        BOOL mask = 0;

        if((p->x < left || p->x > right) || (p->y < top || p->y > bottom))
            ret = 0;
        else
        {
            if(p->x == left || p->x == right)
                mask = 0x0001;
            if(p->y == top || p->y == bottom)
                mask |= 0x0002;

            if(mask == 0)
                ret = 1;
            else if(mask == 3)
                ret = 3;
            else
                ret = 2;
        }

        return ret;
    }
    BOOL PointInRect2(const lwVector2i* p) const
    {
        return (p->x >= left && p->x < right) && (p->y >= top && p->y < bottom);
    }

    void Extend(const _lwRect* rc)
    {
        if(left > rc->left)
            left = rc->left;
        if(right < rc->right)
            right = rc->right;
        if(top > rc->top)
            top = rc->top;
        if(bottom < rc->bottom)
            bottom = rc->bottom;
    }
};

#ifndef LWRECT_DECL
#define LWRECT_DECL
typedef _lwRect lwRect;
#endif





// ====================
const static lwMatrix33 lwMatrix33_Identity(1.0f, 0.0f, 0.0f,
                                            0.0f, 1.0f, 0.0f,
                                            0.0f, 0.0f, 1.0f);

const static lwMatrix43 lwMatrix43_Identity(1.0f, 0.0f, 0.0f,
                                            0.0f, 1.0f, 0.0f,
                                            0.0f, 0.0f, 1.0f,
                                            0.0f, 0.0f, 0.0f);

const static lwMatrix44 lwMatrix44_Identity(1.0f, 0.0f, 0.0f, 0.0f,
                                            0.0f, 1.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 1.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 1.0f);
// ---------------------------------------
// inline method
// ---------------------------------------
LW_INLINE float lwFloatSlerp(float v0, float v1, float t)
{
    return (1.0f - t) * v0 + t * v1;
}
LW_INLINE int lwVector2iCross(const lwVector2i* v1, const lwVector2i* v2)
{
    return (v1->x * v2->y - v1->y * v2->x);
}

// ---------------------------------------
// non-inline method
// ---------------------------------------
LW_FRONT_API lwVector2* lwVec2Mat33Mul(lwVector2* vec, const lwMatrix33* mat);

// lwMatrix33 method
LW_FRONT_API lwMatrix33* _lwMatrix33Multiply(lwMatrix33* out, const lwMatrix33* m1,const lwMatrix33* m2);
LW_FRONT_API lwMatrix33* _lwMatrix33Transpose( lwMatrix33* out, const lwMatrix33* m );
LW_FRONT_API lwMatrix33* _lwMatrix33Inverse(lwMatrix33* out, float* determinant, const lwMatrix33* m);
LW_FRONT_API lwMatrix33* lwMatrix33Adjoint(lwMatrix33* out, const lwMatrix33* m);


LW_FRONT_API lwMatrix33* lwMatrix33Translate(lwMatrix33* mat, float x, float y, DWORD op);
LW_FRONT_API lwMatrix33* lwMatrix33Rotate(lwMatrix33* mat, float radian, DWORD op);
LW_FRONT_API lwMatrix33* lwMatrix33Scale(lwMatrix33* mat, float x, float y, DWORD op);

LW_FRONT_API lwMatrix33* lwMatrix33ReflectWithPoint(lwMatrix33* mat, const lwVector2* pt);
LW_FRONT_API lwMatrix33* lwMatrix33ReflectWithAxis(lwMatrix33* mat, const lwVector2* axis);
LW_FRONT_API lwMatrix33* lwMatrix33ReflectWithAxis(lwMatrix33* mat, const lwVector2* v1, const lwVector2* v2);

// lwMatrix44
LW_FRONT_API lwMatrix44* _lwMatrix44Multiply(lwMatrix44* out,const lwMatrix44* m1,const lwMatrix44* m2);
LW_FRONT_API lwMatrix44* lwMatrix44MultiplyScale(lwMatrix44* ret,const lwMatrix44* mat_scale,const lwMatrix44* mat);

LW_FRONT_API lwMatrix44* _lwMatrix44Inverse(lwMatrix44* ret, float* determinant, const lwMatrix44* m);
LW_FRONT_API lwMatrix44* _lwMatrix44InverseNoScaleFactor(lwMatrix44* ret,const lwMatrix44* m);
LW_FRONT_API lwMatrix44* _lwMatrix44Transpose(lwMatrix44* ret, const lwMatrix44* m);

LW_FRONT_API lwMatrix44* lwMatrix44Translate(lwMatrix44* matrix,float x,float y,float z,DWORD op);
LW_FRONT_API lwMatrix44* lwMatrix44RotateAxis(lwMatrix44* matrix,DWORD axis,float angle,DWORD op);
LW_FRONT_API lwMatrix44* lwMatrix44Scale(lwMatrix44* matrix,float x,float y,float z,DWORD op);

LW_FRONT_API lwMatrix44* lwMatrix44Adjoint(lwMatrix44* out, const lwMatrix44* m);

LW_FRONT_API lwMatrix44* lwMatrix44Compose(lwMatrix44* matrix,const lwVector3* pos,const lwVector3* face,const lwVector3* up);
LW_FRONT_API lwMatrix44* lwMatrix44MetathesisYZ(lwMatrix44* mat_lh, const lwMatrix44* mat_rh);
LW_FRONT_API lwMatrix44* lwMatrix44MetathesisMindPower(lwMatrix44* dst, const lwMatrix44* src);

LW_FRONT_API lwMatrix43* lwMatrix43MetathesisYZ(lwMatrix43* mat_lh, const lwMatrix43* mat_rh);
LW_FRONT_API lwMatrix43* lwMatrix43MetathesisMindPower(lwMatrix43* dst, const lwMatrix43* src);

LW_FRONT_API lwVector3* lwVector3Transform(lwVector3* out, const lwVector3* v, const lwMatrix33* mat);
LW_FRONT_API lwVector3* lwVector3Transform(lwVector3* out, const lwVector3* v, const lwQuaternion* nrm_q);
LW_FRONT_API lwVector3* lwVector3Transform(lwVector3* out, const lwVector3* v, const lwVector3* nrm_axis, float angle);
LW_FRONT_API lwVector3* lwVector3Transform(lwVector3* out, const lwVector3* v, const lwMatrix44* mat);
LW_FRONT_API lwVector3* lwVector3RotateAxisAngle(lwVector3* out, const lwVector3* v, const lwVector3* axis_v0, const lwVector3* axis_v1, float angle);
LW_FRONT_API lwVector3* lwVec3Mat44Mul(lwVector3* vec,const lwMatrix44* mat);
LW_FRONT_API lwVector3* lwVec3Mat44MulNormal(lwVector3* vec,const lwMatrix44* mat);

LW_FRONT_API lwMatrix44* lwMat44ReflectWithPoint(lwMatrix44* mat, const lwVector3* pt);
LW_FRONT_API lwMatrix44* lwMat44ReflectWithAxis(lwMatrix44* mat, const lwVector3* axis);
LW_FRONT_API lwMatrix44* lwMat44ReflectWithPlane(lwMatrix44* mat, const lwPlane* plane);
LW_FRONT_API lwMatrix44* lwMat44Slerp(lwMatrix44* ret, const lwMatrix44* m1, const lwMatrix44* m2, float v);
LW_FRONT_API lwMatrix44* lwMatrix44Slerp(const lwMatrix44* m0, const lwMatrix44* m1, float t, lwMatrix44* mat);
LW_FRONT_API lwMatrix44* lwMatrix44Slerp(const lwVector3* p0, const lwVector3* p1, const lwVector3* s0, const lwVector3* s1, const lwQuaternion* r0, const lwQuaternion* r1, float t, lwMatrix44* mat);
LW_FRONT_API LW_RESULT lwMatrix44ConvertPRS(const lwMatrix44* mat, lwVector3* pos, lwQuaternion* rotation, lwVector3* scale);

// quaternion
LW_FRONT_API lwMatrix44* lwQuaternionToMatrix44(lwMatrix44* matrix,lwQuaternion* quat);
LW_FRONT_API lwMatrix44* lwQuaternionToMatrix44Inv(lwMatrix44* matrix,lwQuaternion* quat);
LW_FRONT_API lwQuaternion* lwMatrix44ToQuaternion(lwQuaternion* quat,const lwMatrix44* matrix);
LW_FRONT_API lwQuaternion* lwEulerAngleToQuaternion(lwQuaternion* quat,float roll,float pitch,float yaw);
LW_FRONT_API lwQuaternion* _lwQuaternionMultiply(lwQuaternion* ret, const lwQuaternion* q1, const lwQuaternion* q2);
LW_FRONT_API lwQuaternion* lwQuaternionSlerp(lwQuaternion* res, const lwQuaternion* from, const lwQuaternion* to, float t);
LW_FRONT_API lwQuaternion* lwQuaternionRotationAxis(lwQuaternion* quat, const lwVector3* std_axis, float angle);
LW_FRONT_API void lwQuaternionToAxisAngle(lwVector3* axis, float* angle, const lwQuaternion* quat);
LW_FRONT_API lwQuaternion* lwQuaternionRotationSpherical(lwQuaternion* quat, float longitude, float latitude, float angle);

LW_FRONT_API int lwCheckVectorInBox(const lwVector3* v, const lwBox* b);
// mat_box[in]: scale factor forbidden
LW_FRONT_API int lwCheckVectorInBox(const lwVector3* v, const lwBox* b, const lwMatrix44* mat_box);

// vp[in]: checked vector;
// v1, v2, v3[in]: triangle vector
LW_FRONT_API int lwCheckVectorInTriangle2D(const lwVector2* vp,const lwVector2* v1,const lwVector2* v2,const lwVector2* v3);

// extension above functoin 
// return: 3: the same vertices; 2: in the sides; 1: in it; 0:outside it
// flag represents index of vertices or sides
LW_FRONT_API int lwCheckVectorInTriangle2D2(int* flag, const lwVector2* vp, const lwVector2* v1, const lwVector2* v2, const lwVector2* v3);

// check vector in triangle
LW_FRONT_API int lwCheckVectorInTriangle3D(const lwVector3* v, const lwVector3* v0, const lwVector3* v1, const lwVector3* v2, const lwVector3* n);
inline int lwCheckVectorInTriangle3D(const lwVector3* v, const lwTriangle* t)
{
	return lwCheckVectorInTriangle3D(v, &t->v0, &t->v1, &t->v2, &t->n);
}

// return: 2: in vertices; 3: in sides; 1: in it; 0: outside
// flag: which vertix or side vp is in
LW_FRONT_API int lwCheckVectorInTriangle3D2(int* flag, const lwVector3* v, const lwVector3* v0, const lwVector3* v1, const lwVector3* v2, const lwVector3* n);
inline int lwCheckVectorInTriangle3D2(int* flag, const lwVector3* v, const lwTriangle* t)
{
	return lwCheckVectorInTriangle3D2(flag, v, &t->v0, &t->v1, &t->v2, &t->n);
}

inline int lwCheckVectorInRound(const lwVector2* v, const lwRound* r)
{
    float xo = (v->x - r->c.x);
    float yo = (v->y - r->c.y);

    return ((xo * xo + yo * yo) < (r->r * r->r));
}

inline int lwCheckVectorInSphere(const lwVector3* v, const lwSphere* s)
{
    float xo = (v->x - s->c.x);
    float yo = (v->y - s->c.y);
    float zo = (v->z - s->c.z);

    return ((xo * xo + yo * yo + zo * zo) < (s->r * s->r));
}

LW_FRONT_API int lwIntersectLineLine2D(lwVector2* v, const lwVector2* u1, const lwVector2* u2, const lwVector2* v1, const lwVector2* v2, int be_ray);
LW_FRONT_API int lwIntersectLineLine3D(lwVector3* v, const lwVector3* u1, const lwVector3* u2, const lwVector3* v1, const lwVector3* v2, int be_ray);

LW_FRONT_API int lwIntersectBoxBoxAABB(const lwBox* b1, const lwBox* b2);
LW_FRONT_API int lwIntersectBoxBoxOBB(const lwBox* b1, const lwBox* b2, const lwMatrix44* mat);
LW_FRONT_API LW_RESULT lwIntersectLineRect(lwVector2* o_v, int* o_n, const lwVector2* u1, const lwVector2* u2, const lwVector2* rc_v1, const lwVector2* rc_v2, int is_segment);
LW_FRONT_API int lwIntersectRaySphere(lwVector3* ret, const lwSphere* sphere, const lwVector3* org, const lwVector3* ray);
LW_FRONT_API int lwIntersectSphereSphere(lwVector3* ret, const lwSphere* s1, const lwSphere* s2);

// ray intersect with triangle
// v[out]: intersect point
// d[out]: distance from org to v
// ray[in]: ray dir
// org[in]: orginal point
// t[in]: triangle 
LW_FRONT_API int lwIntersectRayTriangle(lwVector3* v, float* d, const lwVector3* org,const lwVector3* ray, const lwTriangle* tri);

////////////
//////////////////////////////////
LW_FRONT_API lwVector3* lwGetTriangleNormal(lwVector3* n,const lwVector3* v0,const lwVector3* v1,const lwVector3* v2);

//
// i1,i2: mapping axis, ret: exclude axis , vec: donot need normalized
LW_FRONT_API int lwGetVecNormalAxisMapping(int* i1,int* i2,const lwVector3* vec); 


// get a vector 's weight by another normal vector
//            ^ n    ^ up
// face\     /      /
//      \   /      /
// v<---/--/------/---> v
//     /          \
//  up/            \
//                  \ face
// understand? ^_^
// return value: if vec-normal angle < 90 return 1, 90-180 return 0
// remarks: vec and normal must be normalized first
LW_FRONT_API int lwGetVec3WeightWithNormal(lwVector3* face,lwVector3* up,const lwVector3* vec,const lwVector3* normal);

// lwGetEquation2  lwGetEquation2f
// ret[out]: x, y
// e1,e2[in]: coefficient x, y and equation result
LW_FRONT_API void lwGetEquation2(float* ret, const float* e1, const float* e2);

// ret[out]: x1, x2
// equation: a1 * x1 + b1 * x2 = c1
// e1: (a1, b1, c1); e2: (a2, b2, c2)
LW_FRONT_API void lwGetEquation2f(float* ret, const float* e1, const float* e2);

// ret[out]: x, y, z
// e1,e2,e3[in]: coefficient x, y, z and equation result
LW_FRONT_API void lwGetEquation3(float* ret, const float* e1, const float* e2, const float* e3);

inline int lwFloatRoundDec(float f) { return static_cast<int>(f); }
inline int lwFloatRoundInc(float f) { return lwFloatRoundDec(f) + 1; }
inline float lwFloatDecimal(float f) { return (f - (float)lwFloatRoundDec(f)); }

inline DWORD lwConvertColor3fToDWORD(const float* c) 
{ 
    return (0xff000000 | (unsigned char)(c[0] * 255) | ((unsigned char)(c[1] * 255)<< 8) | ((unsigned char)(c[2] * 255)<<16)); 
}
inline lwVector3 lwConvertDWORDToColor3f(DWORD c) 
{
    float f = 1.0f / 255;
    return lwVector3((c & 0xff) * f, ((c>>8) & 0xff) * f, ((c>>16) & 0xff) * f);
}


LW_FRONT_API float lwGetLineOriginDistance2(const lwVector2* v1, const lwVector2* v2);

// [in]: src_dir, dst_dir must be unit-vector
LW_FRONT_API float lwGetDirTurnAngle(const lwVector2* src_dir, const lwVector2* dst_dir);

// linear transform: y = A * x + B
// x range: (x_var[0], x_var[1])
// y range: (y_var[0], y_var[1])
// ret_param[out]: coefficient A, B
// x_var, y_var,[in]: transform range
LW_FRONT_API void lwGetLinearTransformParam(float* ret_param, const float* x_var, const float* y_var);

// lwPlane
// Normalize plane (so that |a,b,c| == 1)
LW_FRONT_API lwPlane* WINAPI lwPlaneNormalize(lwPlane *p_out, const lwPlane *p);

// Find the intersection between a plane and a line.  If the line is
// parallel to the plane, NULL is returned.
LW_FRONT_API lwVector3* WINAPI lwPlaneIntersectLine(lwVector3 *p_out, const lwPlane *p, const lwVector3 *v1, const lwVector3 *v2);

// Construct a plane from a point and a normal
LW_FRONT_API lwPlane* WINAPI lwPlaneFromPointNormal(lwPlane *p_out, const lwVector3 *v, const lwVector3 *n);

// Construct a plane from 3 points
LW_FRONT_API lwPlane* WINAPI lwPlaneFromPoints(lwPlane *p_out, const lwVector3 *v1, const lwVector3 *v2, const lwVector3 *v3);

LW_FRONT_API DWORD WINAPI lwPlaneClassifyBox(const lwPlane* p, const lwBox* b);
LW_FRONT_API DWORD WINAPI lwPlaneClassifyBox(const lwPlane* p, const lwBox* b, const lwMatrix44* b_mat);
// extension class

//class lwMatrix44X : public lwMatrix44
//{
//public:
//
//	inline void Translate(float x, float y, float z, DWORD op) { lwMatrix44Translate(this, x, y, z, op); }
//	inline void RotateAxis(DWORD axis, float angle, DWORD op) { lwMatrix44RotateAxis(this, axis, angle, op); }
//	inline void Scale(float x, float y, float z, DWORD op) { lwMatrix44Scale(this, x, y, z, op); }
//    inline lwVector3* GetRow(int row) { return (lwVector3*)m[row]; }
//    inline void SetRow(const lwVector3* ver, int row) { lwMat44SetRow(this, ver, row); }
//	inline void Identity() { lwMatrix44Identity(this); }
//
//    inline lwMatrix44X& operator=(const lwMatrix44& mat) { *(static_cast<lwMatrix44*>(this)) = mat; return *this; }
//
//};


// compatiable old method
//#define lwMat44GetLinearValue lwMat44Slerp
//#define CheckVertexInPolygon2D lwCheckVectorInTriangle2D
//#define lwCheckVectorInTriangle2D2 lwCheckVectorInTriangle2D2
//#define lwLineIntersect2D lwIntersectLineLine2D
//#define lwLineIntersect3D lwIntersectLineLine3D
// end namesapce lsh
LW_END

#ifndef lwMatrix33Multiply
#define lwMatrix33Multiply _lwMatrix33Multiply
#endif

#ifndef lwMatrix33Inverse
#define lwMatrix33Inverse _lwMatrix33Inverse
#endif

#ifndef lwMatrix33Transpose
#define lwMatrix33Transpose _lwMatrix33Transpose
#endif

#ifndef lwMatrix33Multiply
#define lwMatrix33Multiply _lwMatrix33Multiply
#endif

#ifndef lwMatrix44Inverse
#define lwMatrix44Inverse _lwMatrix44Inverse
#endif

#ifndef lwMatrix44InverseNoScaleFactor
#define lwMatrix44InverseNoScaleFactor _lwMatrix44InverseNoScaleFactor
#endif

#ifndef lwMatrix44Transpose
#define lwMatrix44Transpose _lwMatrix44Transpose
#endif

#ifndef lwQuaternionMultiply
#define lwQuaternionMultiply _lwQuaternionMultiply
#endif 

#include "lwMath.inl"

