//
#include "stdafx.h"
#include "lwMath.h"

LW_BEGIN
////////////////
////////////////////////////////
//const lwVector2 lwVector2::Zero(0.0f,0.0f);
//
//const lwVector3 lwVector3::Zero(0.0f,0.0f,0.0f);
//
/////////////
/////////////////////////////
//const lwMatrix33 lwMatrix33::Identity( 1.0f, 0.0f, 0.0f,
//                                       0.0f, 1.0f, 0.0f,
//                                       0.0f, 0.0f, 1.0f );
//
//const lwMatrix43 lwMatrix43::Identity( 1.0f, 0.0f, 0.0f,
//                                       0.0f, 1.0f, 0.0f,
//                                       0.0f, 0.0f, 1.0f,
//                                       0.0f, 0.0f, 0.0f );
//
//const lwMatrix44 lwMatrix44::Identity( 1.0f, 0.0f, 0.0f, 0.0f,
//                                       0.0f, 1.0f, 0.0f, 0.0f,
//                                       0.0f, 0.0f, 1.0f, 0.0f,
//                                       0.0f, 0.0f, 0.0f, 1.0f );

// =========

// lwMatrix33
///////////////////////
lwMatrix33* _lwMatrix33Transpose( lwMatrix33* ret, const lwMatrix33* m )
{
    lwMatrix33 tmp( *m );

	tmp._12 = m->_21;
    tmp._21 = m->_12;

	tmp._13 = m->_31;
	tmp._31 = m->_13;

	tmp._23 = m->_32;
	tmp._32 = m->_23;

    *ret = tmp;

	return ret;
}


lwMatrix33* _lwMatrix33Multiply( lwMatrix33* ret, const lwMatrix33* m1, const lwMatrix33* m2 )
{
   lwMatrix33 tmp;

   tmp._11 = m1->_11*m2->_11 + m1->_12*m2->_21 + m1->_13*m2->_31;
   tmp._12 = m1->_11*m2->_12 + m1->_12*m2->_22 + m1->_13*m2->_32;
   tmp._13 = m1->_11*m2->_13 + m1->_12*m2->_23 + m1->_13*m2->_33;

   tmp._21 = m1->_21*m2->_11 + m1->_22*m2->_21 + m1->_23*m2->_31;
   tmp._22 = m1->_21*m2->_12 + m1->_22*m2->_22 + m1->_23*m2->_32;
   tmp._23 = m1->_21*m2->_13 + m1->_22*m2->_23 + m1->_23*m2->_33;

   tmp._31 = m1->_31*m2->_11 + m1->_32*m2->_21 + m1->_33*m2->_31;
   tmp._32 = m1->_31*m2->_12 + m1->_32*m2->_22 + m1->_33*m2->_32;
   tmp._33 = m1->_31*m2->_13 + m1->_32*m2->_23 + m1->_33*m2->_33;


   memcpy( ret, &tmp, sizeof(lwMatrix33) );

	return ret;

}

lwMatrix33* lwMatrix33Translate( lwMatrix33* mat, float x, float y, DWORD op )
{
   if (op == OP_MATRIX_REPLACE) {
       // replace the working one
       mat->_11 = 1.0f; mat->_12 = 0.0f; mat->_13 = 0.0f;
       mat->_21 = 0.0f; mat->_22 = 1.0f; mat->_23 = 0.0f;
       mat->_31 = x;    mat->_32 = y;    mat->_33 = 1.0f;
   }
   else if (op == OP_MATRIX_LOCAL) {
       // post-multiply the mat to the working one to 
       // perform the transformation to local coordinate system of the node
       mat->_31 += x * mat->_11 + y * mat->_21;
       mat->_32 += x * mat->_12 + y * mat->_22;
   }
   else if (op == OP_MATRIX_GLOBAL) {
       mat->_31 += x;
       mat->_32 += y;
   }

   return mat;
}


lwMatrix33* lwMatrix33Rotate( lwMatrix33* mat, float radian, DWORD op )
{
	// compute the rotation values
	float csv = cosf( radian );
	float snv = sinf( radian );

	if( op == OP_MATRIX_REPLACE ) {
		// replace the working one
        mat->_11 = csv;  mat->_12 = -snv; mat->_13 = 0.0f;
        mat->_21 = snv;  mat->_22 =  csv; mat->_23 = 0.0f;
        mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f;
	}
	else {
		// form the rotation matrix
        lwMatrix33 tmp( csv, -snv, 0.0f,
                        snv,  csv, 0.0f,
                        0.0f, 0.0f,1.0f );

		if (op == OP_MATRIX_LOCAL) {
			// pre-multiply the matrix to the working one to perform
			// the transformation to local coordinate system of the node
			lwMatrix33Multiply( mat, &tmp, mat );
		}
		else if (op == OP_MATRIX_GLOBAL) {
			// post-multiply the matrix to the working one to perform
			// the transformation to global coordinate system of the node
			lwMatrix33Multiply( mat, mat, &tmp );
		}
	}

	return mat;

}


lwMatrix33* lwMatrix33Scale( lwMatrix33* mat, float x, float y, DWORD op )
{
	
	if( op == OP_MATRIX_REPLACE ) {
		// replace the working one
        mat->_11 = x;    mat->_12 = 0.0f; mat->_13 = 0.0f;
        mat->_21 = 0.0f; mat->_22 = y;    mat->_23 = 0.0f;
        mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f;
	}
	else {
		// setup the matrix
        lwMatrix33 tmp( x,    0.0f, 0.0f,
                        0.0f,    y, 0.0f,
                        0.0f, 0.0f, 1.0f );

		if (op == OP_MATRIX_LOCAL) {

			// pre-multiply the matrix to the working one to perform
			// the transformation to local coordinate system of the node
			lwMatrix33Multiply( mat, &tmp, mat );
		}
		else if (op == OP_MATRIX_GLOBAL) {
			// post-multiply the matrix to the working one to perform
			// the transformation to global coordinate system of the node
			lwMatrix33Multiply( mat, mat, &tmp );
		}
	}

	return mat;
}

lwVector2* lwVec2Mat33Mul( lwVector2* vec, const lwMatrix33* mat )
{
    lwVector2 v(*vec);

    v.x = vec->x * mat->_11 + vec->y * mat->_21 + mat->_31;
    v.y = vec->x * mat->_12 + vec->y * mat->_22 + mat->_32;

    *vec = v;
	return vec;

}
lwVector3* lwVector3Transform(lwVector3* ret, const lwVector3* v, const lwMatrix33* mat)
{
    lwVector3 u(*v);

    ret->x = u.x * mat->_11 + u.y * mat->_21 + u.z * mat->_31;
    ret->y = u.x * mat->_12 + u.y * mat->_22 + u.z * mat->_32;
    ret->z = u.x * mat->_13 + u.y * mat->_23 + u.z * mat->_33;

    return ret;
}

lwMatrix33* lwMatrix33ReflectWithPoint( lwMatrix33* mat, const lwVector2* pt )
{
    // Mt(-pt) * Mt( -1 ) * Mt( pt )

    *mat = lwMatrix33( -1.0f,  0.0f,  0.0f, 
                        0.0f, -1.0f,  0.0f,
                        2 * pt->x, 2 * pt->y, 1.0f );
    return mat;

}
lwMatrix33* lwMatrix33ReflectWithAxis( lwMatrix33* mat, const lwVector2* axis )
{
    // Mr(-angle) * Mt(axis_x) * Mr(angle)
    const auto v = lwVector2(1.0f, 0.0f);
    float cosv = lwVec2Dot( axis, &v);
    float sinv = (cosv == 0.0f) ? 1.0f : axis->y * cosv / axis->x;
    float cosvcosv = cosv * cosv;
    float sinvsinv = sinv * sinv;
    float cosvsinv = cosv * sinv;

    *mat = lwMatrix33( cosvcosv - sinvsinv, 2 * cosvsinv,        0.0f,
                       2 * cosvsinv,        sinvsinv - cosvcosv, 0.0f,
                       0.0f,                0.0f,                1.0f );
    return mat;
}

// 1. translate axis to origin; 2. rotate axis; 3. untraslate axis with origin
// 
lwMatrix33* lwMatrix33ReflectWithAxis( lwMatrix33* mat, const lwVector2* v1, const lwVector2* v2 )
{
    lwVector2 axis( v2->x - v1->x, v2->y - v1->y );
    lwVec2Normalize( &axis );

    lwVector2 pt( -axis.y, axis.x );

    float s = lwGetLineOriginDistance2( v1, v2 );

    lwVec2Scale( &pt, &pt, s );

    lwMatrix33ReflectWithAxis( mat, &axis );

    mat->_31 = - pt.x * mat->_11 - pt.y * mat->_21 + pt.x;
    mat->_32 = - pt.x * mat->_12 - pt.y * mat->_22 + pt.y;

    return mat;
}

lwMatrix33* lwMatrix33Adjoint(lwMatrix33* out, const lwMatrix33* m)
{
    lwVector3* v = (lwVector3*)out;

    lwVector3Cross(&v[0], (lwVector3*)&m->_21, (lwVector3*)&m->_31);
    lwVector3Cross(&v[1], (lwVector3*)&m->_31, (lwVector3*)&m->_11);
    lwVector3Cross(&v[2], (lwVector3*)&m->_11, (lwVector3*)&m->_21);

    return out;
}

lwMatrix33* _lwMatrix33Inverse(lwMatrix33* ret, float* determinant, const lwMatrix33* m)
{
    lwMatrix33 mat;

    lwMatrix33Adjoint(&mat, m);

    float d = lwVector3Dot((lwVector3*)&mat._11, (lwVector3*)&m->_11);
    if(d != 0.0f)
    {
        mat /= d;
        _lwMatrix33Transpose(ret, &mat);
    }

    if(determinant)
    {
        *determinant = d;
    }

    return ret;
}
// lwMatrix44
///////////////////////
lwMatrix44* _lwMatrix44Multiply( lwMatrix44* ret,const lwMatrix44* m1,const lwMatrix44* m2)
{
   lwMatrix44 tmp;

   tmp._11 = m1->_11*m2->_11 + m1->_12*m2->_21 + m1->_13*m2->_31 + m1->_14*m2->_41;
   tmp._12 = m1->_11*m2->_12 + m1->_12*m2->_22 + m1->_13*m2->_32 + m1->_14*m2->_42;
   tmp._13 = m1->_11*m2->_13 + m1->_12*m2->_23 + m1->_13*m2->_33 + m1->_14*m2->_43;
   tmp._14 = m1->_11*m2->_14 + m1->_12*m2->_24 + m1->_13*m2->_34 + m1->_14*m2->_44;

   tmp._21 = m1->_21*m2->_11 + m1->_22*m2->_21 + m1->_23*m2->_31 + m1->_24*m2->_41;
   tmp._22 = m1->_21*m2->_12 + m1->_22*m2->_22 + m1->_23*m2->_32 + m1->_24*m2->_42;
   tmp._23 = m1->_21*m2->_13 + m1->_22*m2->_23 + m1->_23*m2->_33 + m1->_24*m2->_43;
   tmp._24 = m1->_21*m2->_14 + m1->_22*m2->_24 + m1->_23*m2->_34 + m1->_24*m2->_44;

   tmp._31 = m1->_31*m2->_11 + m1->_32*m2->_21 + m1->_33*m2->_31 + m1->_34*m2->_41;
   tmp._32 = m1->_31*m2->_12 + m1->_32*m2->_22 + m1->_33*m2->_32 + m1->_34*m2->_42;
   tmp._33 = m1->_31*m2->_13 + m1->_32*m2->_23 + m1->_33*m2->_33 + m1->_34*m2->_43;
   tmp._34 = m1->_31*m2->_14 + m1->_32*m2->_24 + m1->_33*m2->_34 + m1->_34*m2->_44;

   tmp._41 = m1->_41*m2->_11 + m1->_42*m2->_21 + m1->_43*m2->_31 + m1->_44*m2->_41;
   tmp._42 = m1->_41*m2->_12 + m1->_42*m2->_22 + m1->_43*m2->_32 + m1->_44*m2->_42;
   tmp._43 = m1->_41*m2->_13 + m1->_42*m2->_23 + m1->_43*m2->_33 + m1->_44*m2->_43;
   tmp._44 = m1->_41*m2->_14 + m1->_42*m2->_24 + m1->_43*m2->_34 + m1->_44*m2->_44;

	
	memcpy(ret, &tmp, sizeof(lwMatrix44));

	return ret;

}

lwMatrix44* lwMatrix44MultiplyScale(lwMatrix44* ret,const lwMatrix44* mat_scale,const lwMatrix44* mat)
{
   ret->_11 = mat_scale->_11 * mat->_11;
   ret->_12 = mat_scale->_11 * mat->_12;
   ret->_13 = mat_scale->_11 * mat->_13;
   ret->_14 = mat_scale->_11 * mat->_14;

   ret->_21 = mat_scale->_22 * mat->_21;
   ret->_22 = mat_scale->_22 * mat->_22;
   ret->_23 = mat_scale->_22 * mat->_23;
   ret->_24 = mat_scale->_22 * mat->_24;

   ret->_31 = mat_scale->_33 * mat->_31;
   ret->_32 = mat_scale->_33 * mat->_32;
   ret->_33 = mat_scale->_33 * mat->_33;
   ret->_34 = mat_scale->_33 * mat->_34;

	
	return ret;
}

///////////////
lwMatrix44* lwMatrix44Translate(lwMatrix44* matrix,float x,float y,float z,DWORD op)
{

   float *m = (float*)matrix->m;


   if (op == OP_MATRIX_REPLACE) {
      /* replace the working one */

      //lwMatrix44Identity(matrix);
	  
      m[12] = x;
      m[13] = y;
      m[14] = z;
   }
   else if (op == OP_MATRIX_LOCAL) {
      /* post-multiply the matrix to the working one to perform
         the transformation to local coordinate system of the node */
      m[12] = x * m[0] + y * m[4] + z * m[8] + m[12];
      m[13] = x * m[1] + y * m[5] + z * m[9] + m[13];
      m[14] = x * m[2] + y * m[6] + z * m[10] + m[14];
   }
   else if (op == OP_MATRIX_GLOBAL) {
      /* post-multiply the matrix to the working one to perform
         the transformation to global coordinate system of the node */
      m[12] += x;
      m[13] += y;
      m[14] += z;
   }

   return matrix;
}


lwMatrix44* lwMatrix44RotateAxis(lwMatrix44* matrix,DWORD axis,float angle,DWORD op)
{
	int next[3], nax1, nax2;

	float csv, snv, tmp[16], *m;


	// create next table
	next[0] = 1; next[1] = 2; next[2] = 0;

	// get the axes
	nax1 = next[axis];
	nax2 = next[nax1];

	// compute the rotation values
	csv = (float) cos(angle);
	snv = (float) sin(angle);

	// get the working transformation matrix
	m = (float*)matrix->m;

	if (op == OP_MATRIX_REPLACE) {
		// replace the working one
		//lwMatrix44Identity(matrix);

		m[4*nax1+nax1] = csv;
		m[4*nax2+nax2] = csv;
		m[4*nax1+nax2] = -snv;
		m[4*nax2+nax1] = snv;
	}
	else {
		// form the rotation matrix
		lwMatrix44Identity((lwMatrix44*)tmp);
		tmp[4*nax1+nax1] = csv;
		tmp[4*nax2+nax2] = csv;
		tmp[4*nax1+nax2] = -snv;
		tmp[4*nax2+nax1] = snv;

		if (op == OP_MATRIX_LOCAL) {
			// post-multiply the matrix to the working one to perform
			// the transformation to local coordinate system of the node
			lwMatrix44Multiply(matrix,(lwMatrix44*)tmp,matrix);
		}
		else if (op == OP_MATRIX_GLOBAL) {
			// post-multiply the matrix to the working one to perform
			// the transformation to global coordinate system of the node
			lwMatrix44Multiply(matrix, matrix,(lwMatrix44*)tmp);
		}
	}

	return matrix;

}


lwMatrix44* lwMatrix44Scale(lwMatrix44* matrix,float x,float y,float z,DWORD op)
{
	float *m, tmp[16];

	m = (float*)matrix->m;

	if( op == OP_MATRIX_REPLACE ) {
		// replace the working one
		//lwMatrix44Identity((lwMatrix44*)m);
		m[0] = x;
		m[5] = y;
		m[10] = z;
	}
	else {
		// setup the matrix
		lwMatrix44Identity((lwMatrix44*)tmp);
		tmp[0] = x;
		tmp[5] = y;
		tmp[10] = z;

		if (op == OP_MATRIX_LOCAL) {

			// pre-multiply the matrix to the working one to perform
			// the transformation to local coordinate system of the node
			lwMatrix44Multiply(matrix,(lwMatrix44*)tmp,matrix);
		}
		else if (op == OP_MATRIX_GLOBAL) {
			// post-multiply the matrix to the working one to perform
			// the transformation to global coordinate system of the node
			lwMatrix44Multiply(matrix,matrix,(lwMatrix44*)tmp);
		}
	}

	return matrix;
}


lwMatrix43* lwMatrix43MetathesisYZ( lwMatrix43* mat_lh, const lwMatrix43* mat_rh )
{
	lwMatrix43 mat( mat_rh->_11, mat_rh->_13, mat_rh->_12,
					mat_rh->_31, mat_rh->_33, mat_rh->_32,
					mat_rh->_21, mat_rh->_23, mat_rh->_22,
					mat_rh->_41, mat_rh->_43, mat_rh->_42 );

	memcpy( mat_lh, &mat, sizeof(lwMatrix43) );

	return mat_lh;
}

lwMatrix43* lwMatrix43MetathesisMindPower( lwMatrix43* dst, const lwMatrix43* src )
{
	lwMatrix43 mat( src->_11,-src->_13, src->_12,
				   -src->_31, src->_33,-src->_32,
					src->_21,-src->_23, src->_22,
					src->_41,-src->_43, src->_42 );

	memcpy( dst, &mat, sizeof(lwMatrix43) );

	return dst;
}

lwMatrix44* lwMatrix44MetathesisYZ( lwMatrix44* mat_lh, const lwMatrix44* mat_rh )
{
	lwMatrix44 mat( mat_rh->_11, mat_rh->_13, mat_rh->_12, mat_rh->_14,
					mat_rh->_31, mat_rh->_33, mat_rh->_32, mat_rh->_24,
					mat_rh->_21, mat_rh->_23, mat_rh->_22, mat_rh->_34,
					mat_rh->_41, mat_rh->_43, mat_rh->_42, mat_rh->_44 );

	memcpy( mat_lh, &mat, sizeof(lwMatrix44) );

	return mat_lh;
}

lwMatrix44* lwMatrix44MetathesisMindPower( lwMatrix44* dst, const lwMatrix44* src )
{
	lwMatrix44 mat( src->_11,-src->_13, src->_12, src->_14,
				   -src->_31, src->_33,-src->_32, src->_24,
					src->_21,-src->_23, src->_22, src->_34,
					src->_41,-src->_43, src->_42, src->_44 );

	memcpy( dst, &mat, sizeof(lwMatrix44) );

	return dst;
}

lwMatrix44* lwMatrix44Compose(lwMatrix44* matrix,const lwVector3* pos,const lwVector3* face,const lwVector3* up)
{
	// identity
	lwMatrix44Identity(matrix);

	// get right vector
	lwVector3Cross((lwVector3*)(&matrix->m[0][0]),up,face);

	// up vector
	matrix->m[1][0] = up->x;
	matrix->m[1][1] = up->y;
	matrix->m[1][2] = up->z;

	// face vector
	matrix->m[2][0] = face->x;
	matrix->m[2][1] = face->y;
	matrix->m[2][2] = face->z;

	// position
	matrix->m[3][0] = pos->x;
	matrix->m[3][1] = pos->y;
	matrix->m[3][2] = pos->z;


	return matrix;
}


inline float MINOR(const lwMatrix44& m, const int r0, const int r1, const int r2, const int c0, const int c1, const int c2)
{
   return m(r0,c0) * (m(r1,c1) * m(r2,c2) - m(r2,c1) * m(r1,c2)) -
          m(r0,c1) * (m(r1,c0) * m(r2,c2) - m(r2,c0) * m(r1,c2)) +
          m(r0,c2) * (m(r1,c0) * m(r2,c1) - m(r2,c0) * m(r1,c1));
}


lwMatrix44 Adjoint(const lwMatrix44* m)
{
   return lwMatrix44( MINOR(*m, 1, 2, 3, 1, 2, 3),
                   -MINOR(*m, 0, 2, 3, 1, 2, 3),
                    MINOR(*m, 0, 1, 3, 1, 2, 3),
                   -MINOR(*m, 0, 1, 2, 1, 2, 3),

                   -MINOR(*m, 1, 2, 3, 0, 2, 3),
                    MINOR(*m, 0, 2, 3, 0, 2, 3),
                   -MINOR(*m, 0, 1, 3, 0, 2, 3),
                    MINOR(*m, 0, 1, 2, 0, 2, 3),

                    MINOR(*m, 1, 2, 3, 0, 1, 3),
                   -MINOR(*m, 0, 2, 3, 0, 1, 3),
                    MINOR(*m, 0, 1, 3, 0, 1, 3),
                   -MINOR(*m, 0, 1, 2, 0, 1, 3),

                   -MINOR(*m, 1, 2, 3, 0, 1, 2),
                    MINOR(*m, 0, 2, 3, 0, 1, 2),
                   -MINOR(*m, 0, 1, 3, 0, 1, 2),
                    MINOR(*m, 0, 1, 2, 0, 1, 2));
}


inline float Determinant(const lwMatrix44* m)
{
	return m->_11 * MINOR(*m, 1, 2, 3, 1, 2, 3) -
		   m->_12 * MINOR(*m, 1, 2, 3, 0, 2, 3) +
		   m->_13 * MINOR(*m, 1, 2, 3, 0, 1, 3) -
		   m->_14 * MINOR(*m, 1, 2, 3, 0, 1, 2);
}

lwMatrix44* _lwMatrix44Inverse(lwMatrix44* ret, float* determinant, const lwMatrix44* m)
{
   //*ret = Adjoint(m);
   //return lwMatrix44Scale(ret, 1.0f / Determinant(m) );
	
    float d = Determinant( m );
	*ret = (1.0f / d) * Adjoint(m);
    if( determinant )
    {
        *determinant = d;
    }
	return ret;
}

lwMatrix44* _lwMatrix44InverseNoScaleFactor(lwMatrix44* ret,const lwMatrix44* m)
{
	lwMatrix44 tmp( *m );

	ret->m[0][0] = tmp.m[0][0];
	ret->m[1][0] = tmp.m[0][1];
	ret->m[2][0] = tmp.m[0][2];
	
	ret->m[0][1] = tmp.m[1][0];
	ret->m[1][1] = tmp.m[1][1];
	ret->m[2][1] = tmp.m[1][2];

	ret->m[0][2] = tmp.m[2][0];
	ret->m[1][2] = tmp.m[2][1];
	ret->m[2][2] = tmp.m[2][2];

	ret->m[3][0] = -lwVector3Dot((const lwVector3*)&tmp.m[0][0],(const lwVector3*)&tmp.m[3][0]);
	ret->m[3][1] = -lwVector3Dot((const lwVector3*)&tmp.m[1][0],(const lwVector3*)&tmp.m[3][0]);
	ret->m[3][2] = -lwVector3Dot((const lwVector3*)&tmp.m[2][0],(const lwVector3*)&tmp.m[3][0]);

	ret->m[0][3] = 0.0f;
	ret->m[1][3] = 0.0f;
	ret->m[2][3] = 0.0f;
	ret->m[3][3] = 1.0f;

	return ret;

}


lwMatrix44* _lwMatrix44Transpose( lwMatrix44* ret, const lwMatrix44* m )
{
    lwMatrix44 tmp( *m );

	tmp._12 = m->_21;
    tmp._21 = m->_12;

	tmp._13 = m->_31;
	tmp._31 = m->_13;

	tmp._14 = m->_41;
	tmp._41 = m->_14;

	tmp._23 = m->_32;
	tmp._32 = m->_23;

	tmp._24 = m->_42;
	tmp._42 = m->_24;

	tmp._34 = m->_43;
	tmp._43 = m->_34;


    *ret = tmp;

	return ret;
}

lwMatrix44* lwMatrix44Adjoint(lwMatrix44* out, const lwMatrix44* m)
{
    *out = Adjoint(m);

    return out;
}

lwVector3* lwVec3Mat44Mul(lwVector3* vec,const lwMatrix44* mat)
{
    lwVector3 v(*vec);

	v[0] = vec->x*mat->_11 + vec->y*mat->_21 + vec->z*mat->_31 + mat->_41;
    v[1] = vec->x*mat->_12 + vec->y*mat->_22 + vec->z*mat->_32 + mat->_42;
    v[2] = vec->x*mat->_13 + vec->y*mat->_23 + vec->z*mat->_33 + mat->_43;

    *vec = v;
	return vec;
}

lwVector3* lwVec3Mat44MulNormal(lwVector3* vec,const lwMatrix44* mat)
{
    lwVector3 v(*vec);

	v[0] = vec->x*mat->_11 + vec->y*mat->_21 + vec->z*mat->_31;
    v[1] = vec->x*mat->_12 + vec->y*mat->_22 + vec->z*mat->_32;
    v[2] = vec->x*mat->_13 + vec->y*mat->_23 + vec->z*mat->_33;

    *vec = v;
	return vec;
}

lwMatrix44* lwMat44ReflectWithPoint( lwMatrix44* mat, const lwVector3* pt )
{
    // Mt(-pt) * Mt( -1 ) * Mt( pt )

    *mat = lwMatrix44( -1.0f,  0.0f,  0.0f,  0.0f,
                        0.0f, -1.0f,  0.0f,  0.0f,
                        0.0f,  0.0f, -1.0f,  0.0f,
                        2 * pt->x, 2 * pt->y, 2 * pt->z, 1.0f );
    return mat;
}

lwMatrix44* lwMat44ReflectWithAxis( lwMatrix44* mat, const lwVector3* axis )
{
    // quaternion rotate axis with 180 degree
    //lwQuaternion quat = { axis->x, axis->y, axis->z, 0.0f };
    lwQuaternion quat;
    quat.x = axis->x;
    quat.y = axis->y;
    quat.z = axis->z;
    quat.w = 0.0f;

    return lwQuaternionToMatrix44( mat, &quat );
}

lwMatrix44* lwMat44ReflectWithPlane( lwMatrix44* mat, const lwPlane* p )
{
    *mat = lwMatrix44( -2 * p->a * p->a + 1,  -2 * p->b * p->a,     -2 * p->c * p->a,       0,
                       -2 * p->a * p->b,      -2 * p->b * p->b + 1, -2 * p->c * p->b,       0,
                       -2 * p->a * p->c,      -2 * p->b * p->c,     -2 * p->c * p->c + 1,   0,
                       -2 * p->a * p->d,      -2 * p->b * p->d,     -2 * p->c * p->d,       1.0f );
    return mat;
}
lwMatrix44* lwMat44Slerp( lwMatrix44* ret, const lwMatrix44* m1, const lwMatrix44* m2, float v )
{
    if( v == 0.0f )
    {
        *ret = *m1;
    }
    else if( v == 1.0f )
    {
        *ret = *m2;
    }
    else
    {
        float u = 1.0f - v;
        // use linear interpolate    
        for(int i = 0; i < 16; i++)
        {
            (*ret)[i] = u * (*m1)[i] + v * (*m2)[i];
        }
    }

    return ret;
}

lwMatrix44* lwMatrix44Slerp(const lwVector3* p0, const lwVector3* p1, const lwVector3* s0, const lwVector3* s1, const lwQuaternion* r0, const lwQuaternion* r1, float t, lwMatrix44* mat)
{
    const float e = 1e-5f;

    lwVector3 p;
    lwVector3 s;
    lwQuaternion r;
    lwMatrix44 m;

    if(mat == 0)
        goto __ret;

    lwMatrix44Identity(mat);

    if(r0 && r1)
    {
        lwQuaternionSlerp(&r, r0, r1, t);
        lwQuaternionToMatrix44(mat, &r);
    }

    if(s0 && s1)
    {
        lwVector3Slerp(&s, s0, s1, t);

        if(!lwFloatEqual(s.x, 1.0f, e)
            || !lwFloatEqual(s.y, 1.0f, e)
            || !lwFloatEqual(s.z, 1.0f, e))
        {
            m = lwMatrix44Scale(s);
            lwMatrix44Multiply(mat, &m, mat);
        }
    }

    if(p0 && p1)
    {
        lwVector3Slerp(&p, p0, p1, t);
        mat->_41 = p.x;
        mat->_42 = p.y;
        mat->_43 = p.z;
    }

__ret:
    return mat;
}

lwMatrix44* lwMatrix44Slerp(const lwMatrix44* m0, const lwMatrix44* m1, float t, lwMatrix44* mat)
{
    const float e = 1e-5f;

    lwVector3 p0, p1;
    lwVector3 s0, s1;
    lwQuaternion r0, r1;

    if(mat == 0 || m0 == 0 || m1 == 0)
        return 0;

    if(lwFloatEqual(t, 0.0f, e))
    {
        *mat = *m0;
    }
    else if(lwFloatEqual(t, 1.0f, e))
    {
        *mat = *m1;
    }
    else
    {
        lwMatrix44ConvertPRS(m0, &p0, &r0, &s0);
        lwMatrix44ConvertPRS(m1, &p1, &r1, &s1);

        lwMatrix44Slerp(&p0, &p1, &s0, &s1, &r0, &r1, t, mat);
    }

    return mat;
}

LW_RESULT lwMatrix44ConvertPRS(const lwMatrix44* mat, lwVector3* pos, lwQuaternion* rotation, lwVector3* scale)
{
    const float e = 1e-5f;
    lwVector3 s;
    lwMatrix44 m, m0;

    if(mat == 0)
        return LW_RET_FAILED;
    
    if(pos)
    {
        pos->x = mat->_41;
        pos->y = mat->_42;
        pos->z = mat->_43;
    }

    if(rotation || scale)
    {
        s.x = lwVector3Magnitude((lwVector3*)&mat->_11);
        s.y = lwVector3Magnitude((lwVector3*)&mat->_21);
        s.z = lwVector3Magnitude((lwVector3*)&mat->_31);
    }

    if(scale)
    {
        *scale = s;
    }

    if(rotation)
    {
        if(lwFloatEqual(s.x, 1.0f, e) 
            && lwFloatEqual(s.y, 1.0f, e)
            && lwFloatEqual(s.z, 1.0f, e))
        {
            lwMatrix44ToQuaternion(rotation, mat);
        }
        else
        {
            m0 = lwMatrix44Scale(1.0f/s.x, 1.0f/s.y, 1.0f/s.z);
            lwMatrix44Multiply(&m, &m0, mat);
            lwMatrix44ToQuaternion(rotation, &m);
        }
    }

    
    return LW_RET_OK;
}

lwVector3* lwGetTriangleNormal( lwVector3* n,const lwVector3* v0,const lwVector3* v1,const lwVector3* v2 )
{
    const lwVector3 v[] = { lwVector3(*v1 - *v0), lwVector3(*v2 - *v0) };
	lwVector3Cross( n, &v[0], &v[1]);
	lwVector3Normalize( n );
	return n;
}

// for triangle
int lwCheckVectorInTriangle2D(const lwVector2* vp,const lwVector2* v1,const lwVector2* v2,const lwVector2* v3)
{

#if 1
	lwVector2 vec_1, vec_2, vec_3;

	lwVec2Sub(&vec_1,v1,vp);
	lwVec2Sub(&vec_2,v2,vp);
	lwVec2Sub(&vec_3,v3,vp);

    float vz1 = lwVec2Cross(&vec_1,&vec_2);
    float vz2 = lwVec2Cross(&vec_2,&vec_3);
    float vz3 = lwVec2Cross(&vec_3,&vec_1);

	// if the vp is on the side of triangle then return 0
    if( (vz1<0.0f && vz2<0.0f && vz3<0.0f) || (vz1>0.0f && vz2>0.0f && vz3>0.0f) )
        return 1;
    else
        return 0;
#else
	lwVector2 vec_1, vec_2, vec_3, vec_4, vec_5;

	lwVec2Sub( &vec_1, vp, v1 );
	lwVec2Sub( &vec_2, vp, v2 );
	lwVec2Sub( &vec_3, v2, v1 );
	lwVec2Sub( &vec_4, v3, v1 );
	lwVec2Sub( &vec_5, v3, v2 );

	float s = lwVec2Cross( &vec_3, &vec_4 );
	float p1 = lwVec2Cross( &vec_1, &vec_3 );
	float p2 = lwVec2Cross( &vec_1, &vec_4 );
	float p3 = lwVec2Cross( &vec_2, &vec_5 );

	return lwFloatEqual( fabsf(s) , fabsf(p1) + fabsf(p2) + fabsf(p3) );
	

#endif

}
int lwCheckVectorInTriangle2D2(int* flag,const lwVector2* vp,const lwVector2* v1,const lwVector2* v2,const lwVector2* v3)
{
    float vz1,vz2,vz3;
	lwVector2 vec_1, vec_2, vec_3;

	lwVec2Sub(&vec_1,v1,vp);
	lwVec2Sub(&vec_2,v2,vp);
	lwVec2Sub(&vec_3,v3,vp);

	if( *vp == *v1 ) {		
		*flag = 0; // 
		return 3;
	}
	else if( *vp == *v2 ) {
		*flag = 1;
		return 3;
	}
	else if( *vp == *v3 ) {
		*flag = 2;
		return 3;
	}

    vz1 = lwVec2Cross(&vec_1,&vec_2);
    vz2 = lwVec2Cross(&vec_2,&vec_3);
    vz3 = lwVec2Cross(&vec_3,&vec_1);

    const float esp = 1e-3f;
	if( lwFloatZero(vz1,esp) )  {
		if( lwVec2Dot( &vec_1, &vec_2 ) > 0 )
			return 0;
		*flag = 0; // 
		return 2;
	}
	else if( lwFloatZero(vz2,esp) ) {
		if( lwVec2Dot( &vec_2, &vec_3 ) > 0 )
			return 0;
		*flag = 1;
		return 2;
	}
	else if( lwFloatZero(vz3,esp) ) {
		if( lwVec2Dot( &vec_3, &vec_1 ) > 0 )
			return 0;
		*flag = 2;
		return 2;
	}

	if( (vz1<0.0f && vz2<0.0f && vz3<0.0f) || (vz1>0.0f && vz2>0.0f && vz3>0.0f) ) {		 
        return 1;// 
	}
    else
        return 0;

}
/////////
/////////////////
lwQuaternion* lwMatrix44ToQuaternion(lwQuaternion* quat,const lwMatrix44* matrix)
{
    float  tr, s, q[4];
    int    i, j, k;

    int nxt[3] = {1, 2, 0};


    tr = matrix->m[0][0] + matrix->m[1][1] + matrix->m[2][2];


    // check the diagonal
    if ( tr > 0.0f ) {
        s = (float)sqrt(tr + 1.0f);
        quat->w = s / 2.0f;
        s = 0.5f / s;
        quat->x = (matrix->m[1][2] - matrix->m[2][1]) * s;
        quat->y = (matrix->m[2][0] - matrix->m[0][2]) * s;
        quat->z = (matrix->m[0][1] - matrix->m[1][0]) * s;
    } 
    else {		
        // diagonal is negative
        i = 0;
        if (matrix->m[1][1] > matrix->m[0][0]) i = 1;
        if (matrix->m[2][2] > matrix->m[i][i]) i = 2;
        j = nxt[i];
        k = nxt[j];


        s = (float)sqrt( (matrix->m[i][i] - (matrix->m[j][j] + matrix->m[k][k])) + 1.0f );

        q[i] = s * 0.5f;

        if( s != 0.0f )
            s = 0.5f / s;


        q[3] = (matrix->m[j][k] - matrix->m[k][j]) * s;
        q[j] = (matrix->m[i][j] + matrix->m[j][i]) * s;
        q[k] = (matrix->m[i][k] + matrix->m[k][i]) * s;


        quat->x = q[0];
        quat->y = q[1];
        quat->z = q[2];
        quat->w = q[3];
    }

    return quat;
}

//lwQuaternion* lwMatrix44ToQuaternionLH(lwQuaternion* quat,const lwMatrix44* matrix)
//{
//    lwMatrix44ToQuaternionRH(quat,matrix);
//    quat->x = - quat->x;
//    quat->y = - quat->y;
//    quat->z = - quat->z;
//
//    return quat;
//}
//
lwMatrix44* lwQuaternionToMatrix44(lwMatrix44* matrix,lwQuaternion* quat)
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    // calculate coefficients
    x2 = quat->x + quat->x; 
    y2 = quat->y + quat->y; 
    z2 = quat->z + quat->z;

    xx = quat->x * x2;   
    xy = quat->x * y2;   
    xz = quat->x * z2;

    yy = quat->y * y2;   
    yz = quat->y * z2;   
    zz = quat->z * z2;

    wx = quat->w * x2;   
    wy = quat->w * y2;   
    wz = quat->w * z2;


    matrix->m[0][0] = 1.0f - (yy + zz); 	
    matrix->m[0][1] = xy + wz;
    matrix->m[0][2] = xz - wy;		
    matrix->m[0][3] = 0.0;

    matrix->m[1][0] = xy - wz;		
    matrix->m[1][1] = 1.0f - (xx + zz);
    matrix->m[1][2] = yz + wx;		
    matrix->m[1][3] = 0.0f;


    matrix->m[2][0] = xz + wy;		
    matrix->m[2][1] = yz - wx;
    matrix->m[2][2] = 1.0f - (xx + yy);		
    matrix->m[2][3] = 0.0f;


    matrix->m[3][0] = 0;			
    matrix->m[3][1] = 0;
    matrix->m[3][2] = 0;			
    matrix->m[3][3] = 1.0f;

    return matrix;
}
lwMatrix44* lwQuaternionToMatrix44Inv(lwMatrix44* matrix,lwQuaternion* quat)
{
    lwQuaternion q;
    q.x = - quat->x;
    q.y = - quat->y;
    q.z = - quat->z;
    q.w = quat->w;
    return lwQuaternionToMatrix44(matrix,&q);

}

lwQuaternion* lwEulerAngleToQuaternion(lwQuaternion* quat,float roll,float pitch,float yaw)
{
    float cr, cp, cy, sr, sp, sy, cpcy, spsy;


    // calculate trig identities
    cr = cosf(roll/2);

    cp = cosf(pitch/2);
    cy = cosf(yaw/2);


    sr = sinf(roll/2);
    sp = sinf(pitch/2);
    sy = sinf(yaw/2);

    cpcy = cp * cy;
    spsy = sp * sy;


    quat->w = cr * cpcy + sr * spsy;
    quat->x = sr * cpcy - cr * spsy;
    quat->y = cr * sp * cy + sr * cp * sy;
    quat->z = cr * cp * sy - sr * sp * cy;

    return quat;
}
//lwQuaternion* lwEulerAngleToQuaternionLH(lwQuaternion* quat,float roll,float pitch,float yaw)
//{
//    lwEulerAngleToQuaternionRH(quat,roll,pitch,yaw);
//    quat->x = - quat->x;
//    quat->y = - quat->y;
//    quat->z = - quat->z;
//
//    return quat;
//}
lwQuaternion* _lwQuaternionMultiply( lwQuaternion* ret, const lwQuaternion* q1, const lwQuaternion* q2 )
{

    float A, B, C, D, E, F, G, H;


    A = (q1->w + q1->x) * (q2->w + q2->x);
    B = (q1->z - q1->y) * (q2->y - q2->z);
    C = (q1->w - q1->x) * (q2->y + q2->z); 
    D = (q1->y + q1->z) * (q2->w - q2->x);
    E = (q1->x + q1->z) * (q2->x + q2->y);
    F = (q1->x - q1->z) * (q2->x - q2->y);
    G = (q1->w + q1->y) * (q2->w - q2->z);
    H = (q1->w - q1->y) * (q2->w + q2->z);


    ret->w = B + (-E - F + G + H) / 2;
    ret->x = A - (E + F + G + H) / 2; 
    ret->y = C + (E - F + G - H) / 2; 
    ret->z = D + (E - F - G + H) / 2;

    return ret;
}

lwQuaternion* lwQuaternionSlerp(lwQuaternion* res, const lwQuaternion* from, const lwQuaternion* to, float t)
{
    float           to1[4];
    double        omega, cosom, sinom, scale0, scale1;


    // calc cosine
    cosom = from->x * to->x 
        + from->y * to->y 
        + from->z * to->z
        + from->w * to->w;


    // adjust signs (if necessary)
    if ( cosom <0.0f ) { 
        cosom = -cosom; 
        to1[0] = - to->x;
        to1[1] = - to->y;
        to1[2] = - to->z;
        to1[3] = - to->w;
    } 
    else {
        to1[0] = to->x;
        to1[1] = to->y;
        to1[2] = to->z;
        to1[3] = to->w;
    }


    // calculate coefficients


    if ( (1.0f - cosom) > 0.001f/*DELTA*/ ) {
        // standard case (slerp)
        omega = acos(cosom);
        sinom = sin(omega);
        scale0 = sin((1.0f - t) * omega) / sinom;
        scale1 = sin(t * omega) / sinom;


    } else {        
        // "from" and "to" quaternions are very close 
        //  ... so we can do a linear interpolation
        scale0 = 1.0f - t;
        scale1 = t;
    }

    // calculate final values
    res->x = (float)(scale0 * from->x + scale1 * to1[0]);
    res->y = (float)(scale0 * from->y + scale1 * to1[1]);
    res->z = (float)(scale0 * from->z + scale1 * to1[2]);
    res->w = (float)(scale0 * from->w + scale1 * to1[3]);

    return res;
}


lwQuaternion* lwQuaternionRotationAxis( lwQuaternion* quat, const lwVector3* std_axis, float angle )
{
    float cosv = cosf( angle * 0.5f );
    float sinv = sinf( angle * 0.5f );

    quat->w = cosv;
    quat->x = std_axis->x * sinv;
    quat->y = std_axis->y * sinv;
    quat->z = std_axis->z * sinv;


    return quat;
}

void lwQuaternionToAxisAngle( lwVector3* axis, float* angle, const lwQuaternion* quat )
{
    float acosv = acosf( quat->w );
    float count_sinv = 1.0f / sqrtf( 1.0f - quat->w * quat->w );

    axis->x = quat->x * count_sinv;
    axis->y = quat->y * count_sinv;
    axis->z = quat->z * count_sinv;

    *angle = acosv * 2.0f;
}

// s: s->x: longitude, s->y: latitude, s->z: angle
lwQuaternion* lwQuaternionRotationSpherical( lwQuaternion* quat, float longitude, float latitude, float angle )
{
    float sin_a = sinf( angle * 0.5f );
    float cos_a = sqrtf( 1.0f - sin_a * sin_a );
    float sin_lat = sinf( latitude );
    float cos_lat = sqrtf( 1.0f - sin_lat * sin_lat );
    float sin_long = sinf( longitude );
    float cos_long = sqrtf( 1.0f - sin_long * sin_long );

    quat->x = sin_a * sin_lat * cos_long;
    quat->y = sin_a * cos_lat;
    quat->z = sin_a * sin_lat * sin_long;
    quat->w = cos_a;

    return quat;
}

lwVector3* lwVector3Transform(lwVector3* out, const lwVector3* v, const lwQuaternion* nrm_q)
{
    // normalize N(q) = 
    // conjugate q* = [-v, w];
    // inverse q^ = q* / N(q);
    lwQuaternion inv_q(-nrm_q->x, -nrm_q->y, -nrm_q->z, nrm_q->w);
    lwQuaternion vec_q(v->x, v->y, v->z, 0);
    lwQuaternion t;

    lwQuaternionMultiply(&t, nrm_q, &vec_q);
    lwQuaternionMultiply(&t, &t, &inv_q);

    out->x = t.x;
    out->y = t.y;
    out->z = t.z;

    return out;
}
lwVector3* lwVector3Transform(lwVector3* out, const lwVector3* v, const lwVector3* nrm_axis, float angle)
{
    //
    float cosv = cosf(angle);
    float sinv = sinf(angle);
    float d = lwVector3Dot(v, nrm_axis);
    lwVector3 c;
    lwVector3Cross(&c, nrm_axis, v);

    *out = cosv * (*v) + (1-cosv) * d * (*nrm_axis) + sinv * c;

    return out;
}
lwVector3* lwVector3Transform(lwVector3* out, const lwVector3* v, const lwMatrix44* mat)
{

	out->x = v->x * mat->_11 + v->y * mat->_21 + v->z * mat->_31 + mat->_41;
    out->y = v->x * mat->_12 + v->y * mat->_22 + v->z * mat->_32 + mat->_42;
    out->z = v->x * mat->_13 + v->y * mat->_23 + v->z * mat->_33 + mat->_43;

	return out;
}

lwVector3* lwVector3RotateAxisAngle(lwVector3* out, const lwVector3* v, const lwVector3* axis_v0, const lwVector3* axis_v1, float angle)
{
    // offset to origion coord
    lwVector3 o_axis(*axis_v1 - *axis_v0);
    lwVector3 o_v(*v - *axis_v0);

    lwVector3Normalize(&o_axis);
    lwVector3Transform(out, &o_v, &o_axis, angle);
    // revert
    lwVector3Add(out, axis_v0);

    return out;
}

// ---------------------
// lwPlane
// ---------------------
lwPlane* WINAPI lwPlaneNormalize(lwPlane *p_out, const lwPlane *p)
{
    float m = lwVector3Magnitude((lwVector3*)&p->a);
    m = 1.0f / m;
    p_out->a = p->a * m;
    p_out->b = p->b * m;
    p_out->c = p->c * m;
    p_out->d = p->d * m;
    return p_out;
}

// Find the intersection between a plane and a line.  If the line is
// parallel to the plane, NULL is returned.
lwVector3* WINAPI lwPlaneIntersectLine(lwVector3 *p_out, const lwPlane *p, const lwVector3 *v1, const lwVector3 *v2)
{
    return NULL;
}
// Construct a plane from a point and a normal
lwPlane* WINAPI lwPlaneFromPointNormal(lwPlane *p_out, const lwVector3 *v, const lwVector3 *n)
{
    p_out->a = n->x;
    p_out->b = n->y;
    p_out->c = n->z;
    p_out->d = -lwVector3Dot(v, n);
    return p_out;
}

// Construct a plane from 3 points
lwPlane* WINAPI lwPlaneFromPoints(lwPlane *p_out, const lwVector3 *v1, const lwVector3 *v2, const lwVector3 *v3)
{
    lwVector3 normal;
    lwVector3 p1 = *v2 - *v1;
    lwVector3 p2 = *v3 - *v1;
    lwVector3Cross(&normal, &p1, &p2);
    lwVector3Normalize(&normal);
    return lwPlaneFromPointNormal(p_out, v1, &normal);
}

DWORD WINAPI lwPlaneClassifyBox(const lwPlane* p, const lwBox* b)
{
    DWORD id = 0;
    DWORD nor_tab[3] = { 0x01, 0x02, 0x04 }; // x, y, z mask
    DWORD axis_tab[8][2] =
    {
        {0x000, 0x111}, // ---
        {0x100, 0x011}, // +--
        {0x010, 0x101}, // -+-
        {0x110, 0x001}, // ++-

        {0x001, 0x110}, // --+
        {0x101, 0x010}, // +-+
        {0x011, 0x100}, // -++
        {0x111, 0x000}, // +++
    };

    if(p->a >= 0.0f)
        id |= nor_tab[0];

    if(p->b >= 0.0f)
        id |= nor_tab[1];

    if(p->c >= 0.0f)
        id |= nor_tab[2];

    lwVector3 v;

    b->GetPointWithMask(&v, axis_tab[id][0]);
    if(lwPlaneClassifyVertex(p, &v) == PLANE_BACK_SIDE)
        return PLANE_BACK_SIDE;

    b->GetPointWithMask(&v, axis_tab[id][1]);
    if(lwPlaneClassifyVertex(p, &v) == PLANE_FRONT_SIDE)
        return PLANE_FRONT_SIDE;

    return PLANE_ON_SIDE;
}

DWORD WINAPI lwPlaneClassifyBox(const lwPlane* p, const lwBox* b, const lwMatrix44* b_mat)
{
    lwVector3 p_n = lwVector3(p->a, p->b, p->c);
    lwVector3 p_p = lwVector3(0.0f, 0.0f, -p->d / p->c);

    lwPlane inv_p;
    lwMatrix44 inv_mat;
    lwMatrix44Inverse(&inv_mat, NULL, b_mat);

    lwVec3Mat44Mul(&p_p, &inv_mat);
    lwVec3Mat44MulNormal(&p_n, &inv_mat);
    
    lwPlaneFromPointNormal(&inv_p, &p_p, &p_n);

    return lwPlaneClassifyBox(&inv_p, b);
}

int lwCheckVectorInBox( const lwVector3* v, const lwBox* b )
{
    if((v->x < (b->c.x - b->r.x)) || (v->x > (b->c.x + b->r.x)))
        return 0;
    if((v->y < (b->c.y - b->r.y)) || (v->y > (b->c.y + b->r.y)))
        return 0;
    if((v->z < (b->c.z - b->r.z)) || (v->z > (b->c.z + b->r.z)))
        return 0;

    return 1;
}

int lwCheckVectorInBox( const lwVector3* v, const lwBox* b, const lwMatrix44* mat_box )
{
    lwMatrix44 mat_inv;
    lwMatrix44InverseNoScaleFactor( &mat_inv, mat_box );

    lwVector3 v_inv( *v );
    lwVec3Mat44Mul( &v_inv, &mat_inv );

    return lwCheckVectorInBox( &v_inv, b );

}

int lwIntersectBoxBoxAABB( const lwBox* b1, const lwBox* b2 )
{
    lwVector3 s1 = b1->c - b1->r;
	lwVector3 s2 = b1->c + b1->r;
	lwVector3 d1 = b2->c - b2->r;
	lwVector3 d2 = b2->c + b2->r;

    return ( max(s1.x,d1.x) <= min(s2.x,d2.x)
          && max(s1.y,d1.y) <= min(s2.y,d2.y)
          && max(s1.z,d1.z) <= min(s2.z,d2.z) );
}
int lwIntersectBoxBoxOBB( const lwBox* b1, const lwBox* b2, const lwMatrix44* mat )
{
    // 3
    // boxhalf_size
    //lwVector3 b_cen( b.GetCenter() );
    //lwVec3Mat44Mul( &b_cen, mat );
    //lwVector3 vec_cen;
    //lwVector3Sub( &vec_cen, &b_cen, &GetCenter() );

    //lwVector3 ea = 0.5f * GetSize();
    //lwVector3 eb = 0.5f * b.GetSize();

    //for( int i = 0; i < 3; i++ ) {
    //    float ro = fabsf( vec_cen[i] );
    //    lwVector3 vv( fabsf(mat->o
    //    float r1 = ea[i];
    //    float r2 = b.GetCenter()
    //}
    //// bmatbox
    //lwMatrix44 mat_inv;
    //lwMatrix44Inverse( &mat_inv, mat );

    //lwVector3& p( b.GetPosition() );
    //lwVector3& s( b.GetSize() );


    //lwVector3 pt[] = {

    //    lwVector3( p.x, p.y, p.z ) ,
    //    lwVector3( p.x, p.y, p.z + s.z ),
    //    lwVector3( p.x + s.x, p.y, p.z + s.z ),
    //    lwVector3( p.x + s.x, p.y, p.z ),

    //    lwVector3( p.x, p.y + s.y, p.z ),
    //    lwVector3( p.x, p.y + s.y, p.z + s.z ),
    //    lwVector3( p.x + s.x, p.y + s.y, p.z + s.z ),
    //    lwVector3( p.x + s.x, p.y + s.y, p.z )

    //};

    //int r = 0;

    //for( int i = 0; i < 8; i++ ) {
    //    lwVec3Mat44Mul( &pt[i], mat );
    //    r += PointInBox( &pt[i] );
    //}

    //if( r == 0 ) {
    //    lwVector3 cen( b.GetCenter() );
    //    lwVec3Mat44Mul( &cen, &mat );
    //    if( PointInBox( &cen ) )
    //        return 1;
    //    cen = GetCenter();
    //    return 1;
    //}
    //else {
    //    return 1;
    //}

    return 1;
    
}
// vector, line equation API
//////////////////////////////////
int lwIntersectLineLine2D( lwVector2* v, const lwVector2* u1, const lwVector2* u2, const lwVector2* v1, const lwVector2* v2, int be_ray )
{
    float t1, t2;

    float a1, a2, b1, b2, c1, c2, d1, d2;


    a1 = (*u2)[0] - (*u1)[0];
    b1 = (*u1)[0];
    c1 = (*u2)[1] - (*u1)[1];
    d1 = (*u1)[1];

    a2 = (*v2)[0] - (*v1)[0];
    b2 = (*v1)[0];
    c2 = (*v2)[1] - (*v1)[1];
    d2 = (*v1)[1];

    // t1, t2 range [0,1]
    // l1 :  x = a1 * t1 + b1;
    //       y = c1 * t1 + d1;
    // l2 :  x = a2 * t2 + b2;
    //       y = c2 * t2 + d2;

    if( a1*c2 == a2*c1 )
		return 0;


    t2 = (b1*c1 + a1*d2 - a1*d1 - b2*c1) / (a2*c1 - a1*c2);

    if( fabsf(a1)>1e-3f )
        t1 = (a2*t2 + b2 - b1) / a1;
    else if( c1 )
        t1 = (c2*t2 + d2 - d1) / c1;
    else
        return 0;

    if( !be_ray ) {

		if( t1<0.0f || t1>1.0f || t2<0.0f || t2>1.0f )
			return 0;
    }

	if( v ) {
		v->x = a1*t1 + b1;
		v->y = c1*t1 + d1;
	}

	return 1;

}

int lwIntersectLineLine3D( lwVector3* v, const lwVector3* u1, const lwVector3* u2, const lwVector3* v1, const lwVector3* v2, int be_ray )
{
    int i1, i2, i3;

    lwVector3 nor;
    
    const lwVector3 vecs[] = { (*u2 - *u1), (*v2 - *v1) };

	lwVector3Cross(&nor, &(vecs[0]), &(vecs[1]));

    i3 = lwGetVecNormalAxisMapping( &i1, &i2, &nor );

    float t1, t2;

    float a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2;


    a1 = (*u2)[i1] - (*u1)[i1];
    b1 = (*u1)[i1];
    c1 = (*u2)[i2] - (*u1)[i2];
    d1 = (*u1)[i2];
    e1 = (*u2)[i3] - (*u1)[i3];
    f1 = (*u1)[i3];

    a2 = (*v2)[i1] - (*v1)[i1];
    b2 = (*v1)[i1];
    c2 = (*v2)[i2] - (*v1)[i2];
    d2 = (*v1)[i2];
    e2 = (*v2)[i3] - (*v1)[i3];
    f2 = (*v1)[i3];

    // t1, t2 range [0,1]
    // l1 :  x = a1 * t1 + b1;
    //       y = c1 * t1 + d1;
    // l2 :  x = a2 * t2 + b2;
    //       y = c2 * t2 + d2;

    if( a1*c2 == a2*c1 )
		return 0;


    t2 = (b1*c1 + a1*d2 - a1*d1 - b2*c1) / (a2*c1 - a1*c2);

    //if( fabsf(a1)>1e-3f )
    //    t1 = (a2*t2 + b2 - b1) / a1;
    //else if( c1 )
    //    t1 = (c2*t2 + d2 - d1) / c1;
    //else
    //    return 0;
    if( fabsf( a1 ) > fabsf( c1 ) )
        t1 = (a2*t2 + b2 - b1) / a1;
    else 
        t1 = (c2*t2 + d2 - d1) / c1;

    if( !be_ray ) {

		if( t1<0.0f || t1>1.0f || t2<0.0f || t2>1.0f )
			return 0;
    }

    float z1 = e1 * t1 + f1;
    float z2 = e2 * t2 + f2;

    if( !lwFloatEqual(z1,z2,1e-2f) )
        return 0;

	if( v ) {
		(*v)[i1] = a1*t1 + b1;
		(*v)[i2] = c1*t1 + d1;
        (*v)[i3] = e1*t1 + f1;
	}

	return 1;
}
LW_RESULT lwIntersectLineRect(lwVector2* o_v, int* o_n, const lwVector2* u1, const lwVector2* u2, const lwVector2* rc_v1, const lwVector2* rc_v2, int is_segment)
{
    LW_RESULT ret = LW_RET_OK;

    // line u: (u2 - u1) * t + u1
    // line rc_horizon: (rc_v2->x - rc_v1->x) * th + (rc_v1->x)
    // line rc_vertical: (rc_v2->y - rc_v1->y) * tv + (rc_v1->y)
    
    float ta, tb;
    float ux, uy, vx, vy;
    float t1, t2;
    float a, b;
    float m;
   
    int i = 0;

    ux = (u2->x - u1->x);
    uy = (u2->y - u1->y);
    vx = (rc_v2->x - rc_v1->x);
    vy = (rc_v2->y - rc_v1->y);

    // horizon intersect
    a = rc_v1->y - u1->y;
    b = rc_v2->y - u1->y;

    if(!lwFloatZero(uy, 1e-3f))
    {
        ta = a / uy;

        if(is_segment && (ta < 0.0f || ta > 1.0f))
            goto __i_1;

        m = ux * ta + u1->x;
        t1 = (m - rc_v1->x) / vx;

        if(t1 >= 0.0f && t1 <= 1.0f)
        {
            if(o_v == 0)
                goto __ret_ok;

            o_v[i].x = m;
            o_v[i].y = rc_v1->y;
            i++;

            if(i >= *o_n)
                goto __ret_ok;
        }

__i_1:
        tb = b / uy;

        if(is_segment && (tb < 0.0f || tb > 1.0f))
            goto __i_2;

        m = ux * tb + u1->x;
        t2 = (m - rc_v1->x) / vx;

        if(t2 >= 0.0f && t2 <= 1.0f)
        {
            if(o_v == 0)
                goto __ret_ok;

            o_v[i].x = m;
            o_v[i].y = rc_v2->y;
            i++;

            if(i >= *o_n)
                goto __ret_ok;
        }
__i_2:
        ;
    }


    // vertical intersect

    a = rc_v1->x - u1->x;
    b = rc_v2->x - u1->x;

    if(!lwFloatZero(ux, 1e-3f))
    {
        ta = a / ux;

        if(is_segment && (ta < 0.0f || ta > 1.0f))
            goto __i_3;

        m = uy * ta + u1->y;
        t1 = (m - rc_v1->y) / vy;

        if(t1 >= 0.0f && t1 <= 1.0f)
        {
            if(o_v == 0)
                goto __ret_ok;

            o_v[i].x = rc_v1->x;
            o_v[i].y = m;
            i++;

            if(i >= *o_n)
                goto __ret_ok;

        }
__i_3:
        tb = b / ux;

        if(is_segment && (tb < 0.0f || tb > 1.0f))
            goto __i_4;

        m = uy * tb + u1->y;
        t2 = (m - rc_v1->y) / vy;

        if(t2 >= 0.0f && t2 <= 1.0f)
        {
            if(o_v == 0)
                goto __ret_ok;

            o_v[i].x = rc_v2->x;
            o_v[i].y = m;
            i++;

            if(i >= *o_n)
                goto __ret_ok;
        }
__i_4:
        ;
    }

    ret = LW_RET_FAILED;
__ret_ok:
    *o_n = i;
    return ret;

}

int lwGetVec3WeightWithNormal(lwVector3* face,lwVector3* up,const lwVector3* vec,const lwVector3* normal)
{
	int ret = 1;
	float cs = lwVector3Dot(vec,normal);
	if( cs < 0.0f ) {
		// if the angle between vector and normal is exceed 90 degree , we set negative cos value
		//cs = -cs;
		ret = 0;
	}

	*up = *normal;
	lwVector3Scale(up,cs);
	*face = *vec - *up;

	return ret;
}

int lwGetVecNormalAxisMapping(int* i1,int* i2,const lwVector3* vec)
{
	int t1, t2;
	int ret;
    lwVector3 vn( fabsf(vec->x),fabsf(vec->y),fabsf(vec->z) );

	// 
    if( vn.x > vn.y ) {
      if( vn.x > vn.z ) { 
		  t1 = 1; 
		  t2 = 2;
		  ret = 0;
	  }
	  else { 
		  t1 = 0; 
		  t2 = 1;
		  ret = 2;
	  }
    } 
	else {
      if( vn.y > vn.z ) { 
		  t1 = 0; 
		  t2 = 2;
		  ret = 1;
	  }
      else {
		  t1 = 0;
		  t2 = 1;
		  ret = 2;
	  }
    }

	if( i1 && i2 ) {
		*i1 = t1;
		*i2 = t2;
	}

	return ret;
}

int lwCheckVectorInTriangle3D( const lwVector3* v, const lwVector3* v0, const lwVector3* v1, const lwVector3* v2, const lwVector3* n )
{
	int i1, i2;

	lwGetVecNormalAxisMapping( &i1, &i2, n );

	// begin ,a,b	
#if 1
    lwVector3 p( (*v)[i1] - (*v0)[i1],
                (*v1)[i1] - (*v0)[i1],
                (*v2)[i1] - (*v0)[i1] );
    lwVector3 q( (*v)[i2] - (*v0)[i2],
                (*v1)[i2] - (*v0)[i2],
                (*v2)[i2] - (*v0)[i2] );
    float a,b;
    if( p.y == 0.0f ) {
      b=p.x / p.z;
      if( b>=0.0f && b<=1.0f) 
		  a = ( q.x-b * q.z ) / q.y;
      else 
		  return 0;
    }
    else {
      b = ( q.x * p.y - p.x * q.y ) / ( q.z * p.y - p.z * q.y );
      if( b>=0.0f && b<=1.0f ) 
		  a = ( p.x - b * p.z ) / p.y;
      else 
		  return 0;
    }

    return (a>=0 && (a+b)<=1);
	// end
#else
	// begin 
	lwVector2 vh( (*v)[i1], (*v)[i2] );
	lwVector2 v1( (*v0)[i1], (*v0)[i2] );
	lwVector2 v2( (*v1)[i1], (*v1)[i2] );
	lwVector2 v3( (*v2)[i1], (*v2)[i2] );

	return lwCheckVectorInTriangle2D( &vh, &v1, &v2, &v3 );
	// end
#endif

}


///////
int lwCheckVectorInTriangle3D2( int* flag, const lwVector3* v, const lwVector3* v0, const lwVector3* v1, const lwVector3* v2, const lwVector3* n )
{
	// begin
	// first check vector in triangle plane
	lwPlane p;
    lwPlaneFromPointNormal(&p, v0, n);

	if( lwFloatZero( lwPlaneClassify(&p, v), 1e-3f ) == 0 )
		return 0;
	// end

	// 2d mapping
	int i1, i2;

	lwGetVecNormalAxisMapping( &i1, &i2, n );
	
	lwVector2 vh( (*v)[i1], (*v)[i2] );
	lwVector2 va( (*v0)[i1], (*v0)[i2] );
	lwVector2 vb( (*v1)[i1], (*v1)[i2] );
	lwVector2 vc( (*v2)[i1], (*v2)[i2] );
	// ,lwCheckVectorInTriangle2D2
	return lwCheckVectorInTriangle2D2( flag, &vh, &va, &vb, &vc );
}


int lwIntersectRayTriangle( lwVector3* v, float* d, const lwVector3* org,const lwVector3* ray, const lwTriangle* tri )
{
	float dot = lwVector3Dot( &tri->n, ray );

	if( lwFloatZero( dot ) )
		return 0; // parellel

	lwPlane p;
    lwPlaneFromPointNormal(&p, &tri->v0, &tri->n);

	float t = - lwPlaneClassify(&p, org) / dot;

	if( t <= 0 )
		return 0; // ray is neg-direction

	lwVector3 ip_vec;
	lwVector3 rs( *ray );

	// get intersect point
	lwVector3Add( &ip_vec, org, lwVector3Scale( &rs, t ) );

	if( lwCheckVectorInTriangle3D( &ip_vec, tri ) )  
	{
		// get output data
		if( v )
			*v = ip_vec;
		if( d )
			*d = t;

		return 1;
	}

	return 0;

}

// ret[out]: x, y
// e1,e2[in]: coefficient x, y and equation result
void lwGetEquation2( float* ret, const float* e1, const float* e2 )
{
	float delta = 1.0f / ( e1[0] * e2[1] - e1[1] * e2[0] );
	float dx = e1[2] * e2[1] - e2[2] * e1[1];
	float dy = e1[0] * e2[2] - e1[2] * e2[0];
	
	ret[0] = dx * delta;
	ret[1] = dy * delta;
}

// ret[out]: x1, x2
// equation: a1 * x1 + b1 * x2 = c1
// e1: ( a1, b1, c1 ); e2: ( a2, b2, c2 )
void lwGetEquation2f( float* ret, const float* e1, const float* e2 )
{
	if( e1[0] == 0.0f )
	{
		ret[0] = e1[2] / e1[1];
		ret[1] = (e2[2] - e2[1] * ret[0]) / e2[0];
	}
	else
	{
		ret[1] = (e2[0] * e1[2] - e1[0] * e2[2]) / (e2[0] * e1[1] - e1[0] * e2[1] );
		ret[0] = (e1[2] - e1[1] * ret[1]) / e1[0];
	}
}

// ret[out]: x, y, z
// e1,e2,e3[in]: coefficient x, y, z and equation result
void lwGetEquation3( float* ret, const float* e1, const float* e2, const float* e3 )
{
	float d1 = e2[1] * e3[2] - e2[2] * e3[1];
	float d2 = e1[1] * e3[2] - e1[2] * e3[1];
	float d3 = e1[1] * e2[2] - e1[2] * e2[1];

	float delta = e1[0] * d1 - e2[0] * d2 + e3[0] * d3;

	float dx = e1[3] * d1 - e2[3] * d2 + e3[3] * d3;

    assert( delta != 0.0f );

	ret[0] = dx / delta;

	float q1[3] = { e1[1], e1[2], e1[3] - ret[0] * e1[0] };
	float q2[3] = { e2[1], e2[2], e2[3] - ret[0] * e2[0] };

	if( q1[0] * q2[1] == q1[1] * q2[0] ) {

		float q3[3] = { e3[1], e3[2], e3[3] - ret[0] * e3[0] };

		if( q1[0] * q3[1] == q1[1] * q3[0] ) {

			if( q2[0] * q3[1] == q2[1] * q3[0] ) {
				assert( 0 );
			}
			else {
				lwGetEquation2( &ret[1], q2, q3 );
			}
		}
		else {
			lwGetEquation2( &ret[1], q1, q3 );
		}
	}
	else {
		lwGetEquation2( &ret[1], q1, q2 );
	}
		
	
}

// use equation paramters
// if v1 == v2 then the distance is point to point
float lwGetLineOriginDistance2( const lwVector2* v1, const lwVector2* v2 )
{
    lwVector2 k( v2->x - v1->x, v2->y - v1->y );
    float s;

    lwVec2Normalize( &k );

    if( k.y != 0.0f )
    {
        s = (k.x * v1->y - k.y * v1->x) / (k.x * k.x + k.y * k.y);
    }
    else if( k.x != 0.0f )
    {
        s = v1->y / k.x;
    }
    else
    {
        // v1, v2 degenerate one point
        s = sqrtf( v1->x * v1->x + v1->y * v1->y );
    }

    return s;
}

int lwIntersectRaySphere( lwVector3* ret, const lwSphere* sphere, const lwVector3* org, const lwVector3* ray )
{
    lwVector3* cen = (lwVector3*)&sphere->c;
    lwVector3 cen_org( *cen - *org );

    float ray_dis = lwVector3Dot( &cen_org, ray );

    float rd = sphere->r * sphere->r - ( lwVector3Dot( &cen_org, &cen_org ) - ray_dis * ray_dis );

    if( rd < 0.0f )
        return 0;


	if( ret )
	{
        float d = sqrtf( rd );

		*ret = (*org) + ( ray_dis - d ) * (*ray);
	}

    return 1;
}

int lwIntersectSphereSphere( lwVector3* ret, const lwSphere* s1, const lwSphere* s2 )
{
    lwVector3 cc = s1->c - s2->c;

    float ss = lwVector3SquareMagnitude( &cc);

    float rr = s1->r + s2->r; 

    if( ss >= rr * rr )
        return 0;

    if( ret )
    {
        *ret = cc;

        lwVector3Normalize( ret );
        lwVector3Scale( ret, s2->r );
        lwVector3Add( ret, &s2->c );
    }

    return 1;
}

float lwGetDirTurnAngle( const lwVector2* src_dir, const lwVector2* dst_dir )
{   
    // check dir
    float dotv = lwVec2Dot( src_dir, dst_dir );
    // float1.0000113acosf(1.0000113)
    if( dotv > 1.0f )
        dotv = 1.0f;
    float acsv = acosf( dotv );

    if( lwVec2Cross( src_dir, dst_dir ) < 0 )
    {
        acsv = -acsv;
    }

    return acsv;
}

void lwGetLinearTransformParam( float* ret_param, const float* x_var, const float* y_var )
{
	float delta = 1.0f / (x_var[0] - x_var[1]);

	ret_param[0] = (y_var[0] - y_var[1]) * delta;
	ret_param[1] = (x_var[0] * y_var[1] - x_var[1] * y_var[0]) * delta;
}

// end namesapce lsh
LW_END
