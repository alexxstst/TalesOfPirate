//
#include "stdafx.h"
#include "lwViewFrustum.h"

LW_BEGIN

LW_STD_IMPLEMENTATION(lwViewFrustum)

void lwViewFrustum::Update(const lwMatrix44* mat_viewproj)
{
    lwPlane* p;

	// top plane  
    p = &_plane_seq[VIEWFRUSTUM_TOP];
    p->a = mat_viewproj->_14 - mat_viewproj->_12;
    p->b = mat_viewproj->_24 - mat_viewproj->_22;
    p->c = mat_viewproj->_34 - mat_viewproj->_32;
    p->d = mat_viewproj->_44 - mat_viewproj->_42;
	lwPlaneNormalize(p, p);

    // bottom plane  
    p = &_plane_seq[VIEWFRUSTUM_BOTTOM];
    p->a = mat_viewproj->_14 + mat_viewproj->_12;
	p->b = mat_viewproj->_24 + mat_viewproj->_22;
	p->c = mat_viewproj->_34 + mat_viewproj->_32;
	p->d = mat_viewproj->_44 + mat_viewproj->_42;
	lwPlaneNormalize(p, p);

    // left plane  
    p = &_plane_seq[VIEWFRUSTUM_LEFT];
    p->a = mat_viewproj->_14 + mat_viewproj->_11;
    p->b = mat_viewproj->_24 + mat_viewproj->_21;
    p->c = mat_viewproj->_34 + mat_viewproj->_31;
    p->d = mat_viewproj->_44 + mat_viewproj->_41;
	lwPlaneNormalize(p, p);

    // right plane  
    p = &_plane_seq[VIEWFRUSTUM_RIGHT];
    p->a = mat_viewproj->_14 - mat_viewproj->_11;
    p->b = mat_viewproj->_24 - mat_viewproj->_21;
    p->c = mat_viewproj->_34 - mat_viewproj->_31;
    p->d = mat_viewproj->_44 - mat_viewproj->_41;
	lwPlaneNormalize(p, p);

    // near plane
    p = &_plane_seq[VIEWFRUSTUM_FRONT];
    p->a = mat_viewproj->_13;
    p->b = mat_viewproj->_23;
    p->c = mat_viewproj->_33;
    p->d = mat_viewproj->_43;
	lwPlaneNormalize(p, p);

    // far plane
    p = &_plane_seq[VIEWFRUSTUM_BACK];
    p->a = mat_viewproj->_14 - mat_viewproj->_13;
    p->b = mat_viewproj->_24 - mat_viewproj->_23;
    p->c = mat_viewproj->_34 - mat_viewproj->_33;
    p->d = mat_viewproj->_44 - mat_viewproj->_43;
	lwPlaneNormalize(p, p);

}

BOOL lwViewFrustum::IsPointInFrustum(const lwVector3* v)
{
	// If you remember the plane equation (A*x + B*y + C*z + D = 0), then the rest
	// of this code should be quite obvious and easy to figure out yourself.
	// In case don't know the plane equation, it might be a good idea to look
	// at our Plane Collision tutorial at www.GameTutorials.com in OpenGL Tutorials.
	// I will briefly go over it here.  (A,B,C) is the (X,Y,Z) of the normal to the plane.
	// They are the same thing... but just called ABC because you don't want to say:
	// (x*x + y*y + z*z + d = 0).  That would be wrong, so they substitute them.
	// the (x, y, z) in the equation is the point that you are testing.  The D is
	// The distance the plane is from the origin.  The equation ends with "= 0" because
	// that is true when the point (x, y, z) is ON the plane.  When the point is NOT on
	// the plane, it is either a negative number (the point is behind the plane) or a
	// positive number (the point is in front of the plane).  We want to check if the point
	// is in front of the plane, so all we have to do is go through each point and make
	// sure the plane equation goes out to a positive number on each side of the frustum.
	// The result (be it positive or negative) is the distance the point is front the plane.

    for(int i = 0; i < 6; i++)
    {
        if(lwPlaneClassify(&_plane_seq[i], v) <= 0.0f)
            return 0;
    }
    return 1;
}
BOOL lwViewFrustum::IsSphereInFrustum(const lwSphere* s)
{
	// Now this function is almost identical to the IsPointInFrustum(), except we
	// now have to deal with a radius around the point.  The point is the center of
	// the radius.  So, the point might be outside of the frustum, but it doesn't
	// mean that the rest of the sphere is.  It could be half and half.  So instead of
	// checking if it's less than 0, we need to add on the radius to that.  Say the
	// equation produced -2, which means the center of the sphere is the distance of
	// 2 behind the plane.  Well, what if the radius was 5?  The sphere is still inside,
	// so we would say, if(-2 < -5) then we are outside.  In that case it's false,
	// so we are inside of the frustum, but a distance of 3.  This is reflected below.

    for(int i = 0; i < 6; i++)
    {
        if(lwPlaneClassify(&_plane_seq[i], &s->c) <= -s->r)
            return 0;
    }
    return 1;
}

BOOL lwViewFrustum::IsSphereInFrustum(const lwSphere* s, DWORD* last_check_plane)
{
    BOOL ret = 0;

    if(last_check_plane && (*last_check_plane) != LW_INVALID_INDEX)
    {
        if(lwPlaneClassify(&_plane_seq[*last_check_plane], &s->c) <= -s->r)
            goto __ret;
    }

    for(DWORD i = 0; i < 6; i++)
    {
        if(last_check_plane && (i == *last_check_plane))
            continue;

        if(lwPlaneClassify(&_plane_seq[i], &s->c) <= -s->r)
        {
            if(last_check_plane)
            {
                *last_check_plane = i;
            }

            goto __ret;
        }
    }

    if(last_check_plane)
    {
        *last_check_plane = LW_INVALID_INDEX;
    }

    ret = 1;
__ret:
    return ret;
}

BOOL lwViewFrustum::IsBoxInFrustum(const lwBox* b, const lwMatrix44* b_mat, DWORD* last_check_plane)
{
    DWORD t;

    if(last_check_plane && (*last_check_plane) != LW_INVALID_INDEX)
    {
        t = lwPlaneClassifyBox(&_plane_seq[*last_check_plane], b, b_mat);

        if(t != PLANE_FRONT_SIDE)
            goto __ret;
    }

    for(DWORD i = 0; i < 6; i++)
    {
        if(last_check_plane && (i == *last_check_plane))
            continue;

        t = lwPlaneClassifyBox(&_plane_seq[i], b, b_mat);

        if(t != PLANE_FRONT_SIDE)
        {
            if(last_check_plane)
            {
                *last_check_plane = i;
            }

            goto __ret;
        }
    }


__ret:
    return (t == PLANE_BACK_SIDE) ? 0 : 1;
}

float lwViewFrustum::GetDistanceFromNearPlane(const lwVector3* v)
{
    return lwPlaneClassify(&_plane_seq[VIEWFRUSTUM_FRONT], v);
}

LW_END
