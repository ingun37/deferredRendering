#include <math.h>
#include <string.h>
#include "JLinearAlgebra.h"



JMatrix44 JMatrix44::GetIdentityMatrix()
{
	JMatrix44 rmat;

	rmat[0][0] = 1;
	rmat[0][1] = 0;
	rmat[0][2] = 0;
	rmat[0][3] = 0;

	rmat[1][0] = 0;
	rmat[1][1] = 1;
	rmat[1][2] = 0;
	rmat[1][3] = 0;

	rmat[2][0] = 0;
	rmat[2][1] = 0;
	rmat[2][2] = 1;
	rmat[2][3] = 0;

	rmat[3][0] = 0;
	rmat[3][1] = 0;
	rmat[3][2] = 0;
	rmat[3][3] = 1;

	return rmat;
}

JMatrix44 JMatrix44::GetRotateMatrix(float x, float y, float z, float radian)
{
	float c = cosf(radian);
	float s = sinf(radian);

	JMatrix44 rmat;

	rmat[0][0] = c+(1-c)*x*x;
	rmat[0][1] = (1-c)*x*y - s*z;
	rmat[0][2] = (1-c)*x*z + s*y;
	rmat[0][3] = 0;

	rmat[1][0] = (1-c)*x*y + s*z;
	rmat[1][1] = c+(1-c)*y*y;
	rmat[1][2] = (1-c)*y*z - s*x;
	rmat[1][3] = 0;

	rmat[2][0] = (1-c)*x*z - s*y;
	rmat[2][1] = (1-c)*y*z + s*x;
	rmat[2][2] = c+(1-c)*z*z;
	rmat[2][3] = 0;

	rmat[3][0] = 0;
	rmat[3][1] = 0;
	rmat[3][2] = 0;
	rmat[3][3] = 1;

	return rmat;
}

JMatrix44 JMatrix44::GetTranslationMatrix( const float dx, const float dy, const float dz )
{
	JMatrix44 rmat = JMatrix44::GetIdentityMatrix();

	rmat[0][3] = dx;
	rmat[1][3] = dy;
	rmat[2][3] = dz;

	return rmat;
}

JMatrix44 JMatrix44::GetViewMatrix( const JVector3& eye, const JVector3& up, const JVector3& at , JVector3* pF, JVector3* pU, JVector3* pR)
{
	JVector3 f, r, u, z;

	f = (at - eye).unitVector();
	r = (f / up).unitVector();
	u = r / f;

	if(pF) *pF = f;
	if(pU) *pU = u;
	if(pR) *pR = r;
	//TODO exception for like crossing same vectors kinda things
	z[0] = 0;
	z[1] = 0;
	z[2] = 0;

	JMatrix44 dotMat, movMat;

	dotMat.r1 = r.getCovariantVector();
	dotMat.r2 = u.getCovariantVector();
	dotMat.r3 = -f.getCovariantVector();
	dotMat.r4 = z.getContravariantVector();

	movMat = JMatrix44::GetTranslationMatrix( -eye[0], -eye[1], -eye[2] );

	return dotMat * movMat;
}

JMatrix44 JMatrix44::GetProjectionMatrixOrthogonal( const float l, const float r, const float t, const float b, const float n, const float f )
{
	JMatrix44 rmat;

	rmat[0][0] = (2)/(r-l);
	rmat[0][1] = 0;
	rmat[0][2] = 0;
	rmat[0][3] = -(r+l)/(r-l);
	
	rmat[1][0] = 0;
	rmat[1][1] = (2)/(t-b);
	rmat[1][2] = 0;
	rmat[1][3] = -(t+b)/(t-b);
	
	rmat[2][0] = 0;
	rmat[2][1] = 0;
	rmat[2][2] = -(2)/(f-n);
	rmat[2][3] = -(f+n)/(f-n);
	
	rmat[3][0] = 0;
	rmat[3][1] = 0;
	rmat[3][2] = 0;
	rmat[3][3] = 1;

	return rmat;
}

JMatrix44 JMatrix44::GetProjectionMatrixPerspective( const float l, const float r, const float t, const float b, const float n, const float f )
{
	JMatrix44 rmat;

	rmat[0][0] = (2*n)/(r-l);
	rmat[0][1] = 0;
	rmat[0][2] = (r+l)/(r-l);
	rmat[0][3] = 0;
	
	rmat[1][0] = 0;
	rmat[1][1] = (2*n)/(t-b);
	rmat[1][2] = (t+b)/(t-b);
	rmat[1][3] = 0;
	
	rmat[2][0] = 0;
	rmat[2][1] = 0;
	rmat[2][2] = -(f+n)/(f-n);
	rmat[2][3] = -(2*n*f)/(f-n);
	
	rmat[3][0] = 0;
	rmat[3][1] = 0;
	rmat[3][2] = -1;
	rmat[3][3] = 0;

	return rmat;
}

JVector4& JMatrix44::operator[] ( int i ) const
{

	return ((JVector4*)this)[ fitN(i,4) ];
}

JVector4 JMatrix44::operator* (const JVector4& v) const
{
	JVector4 ret;
	ret[0] = r1 * v;
	ret[1] = r2 * v;
	ret[2] = r3 * v;
	ret[3] = r4 * v;
	return ret;
}

JMatrix44 JMatrix44::operator* ( const JMatrix44& m ) const
{
	float accu;
	JMatrix44 ret;

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			accu = 0;

			for(int k=0;k<4;k++)
			{
				accu += (*this)[i][k] * m[k][j];
			}

			ret[i][j] = accu;
		}
	}

	return ret;
}
//todo make them inline
void JMatrix44::operator*=(const float f)
{
	r1 *= f;
	r2 *= f;
	r3 *= f;
	r4 *= f;
}

bool JMatrix44::InverseMatrix(const float m[16], float invOut[16])
{
	float inv[16], det;
	int i;

	inv[0] = m[5]  * m[10] * m[15] - 
		m[5]  * m[11] * m[14] - 
		m[9]  * m[6]  * m[15] + 
		m[9]  * m[7]  * m[14] +
		m[13] * m[6]  * m[11] - 
		m[13] * m[7]  * m[10];

	inv[4] = -m[4]  * m[10] * m[15] + 
		m[4]  * m[11] * m[14] + 
		m[8]  * m[6]  * m[15] - 
		m[8]  * m[7]  * m[14] - 
		m[12] * m[6]  * m[11] + 
		m[12] * m[7]  * m[10];

	inv[8] = m[4]  * m[9] * m[15] - 
		m[4]  * m[11] * m[13] - 
		m[8]  * m[5] * m[15] + 
		m[8]  * m[7] * m[13] + 
		m[12] * m[5] * m[11] - 
		m[12] * m[7] * m[9];

	inv[12] = -m[4]  * m[9] * m[14] + 
		m[4]  * m[10] * m[13] +
		m[8]  * m[5] * m[14] - 
		m[8]  * m[6] * m[13] - 
		m[12] * m[5] * m[10] + 
		m[12] * m[6] * m[9];

	inv[1] = -m[1]  * m[10] * m[15] + 
		m[1]  * m[11] * m[14] + 
		m[9]  * m[2] * m[15] - 
		m[9]  * m[3] * m[14] - 
		m[13] * m[2] * m[11] + 
		m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] - 
		m[0]  * m[11] * m[14] - 
		m[8]  * m[2] * m[15] + 
		m[8]  * m[3] * m[14] + 
		m[12] * m[2] * m[11] - 
		m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] + 
		m[0]  * m[11] * m[13] + 
		m[8]  * m[1] * m[15] - 
		m[8]  * m[3] * m[13] - 
		m[12] * m[1] * m[11] + 
		m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] - 
		m[0]  * m[10] * m[13] - 
		m[8]  * m[1] * m[14] + 
		m[8]  * m[2] * m[13] + 
		m[12] * m[1] * m[10] - 
		m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] - 
		m[1]  * m[7] * m[14] - 
		m[5]  * m[2] * m[15] + 
		m[5]  * m[3] * m[14] + 
		m[13] * m[2] * m[7] - 
		m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] + 
		m[0]  * m[7] * m[14] + 
		m[4]  * m[2] * m[15] - 
		m[4]  * m[3] * m[14] - 
		m[12] * m[2] * m[7] + 
		m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] - 
		m[0]  * m[7] * m[13] - 
		m[4]  * m[1] * m[15] + 
		m[4]  * m[3] * m[13] + 
		m[12] * m[1] * m[7] - 
		m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] + 
		m[0]  * m[6] * m[13] + 
		m[4]  * m[1] * m[14] - 
		m[4]  * m[2] * m[13] - 
		m[12] * m[1] * m[6] + 
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] + 
		m[1] * m[7] * m[10] + 
		m[5] * m[2] * m[11] - 
		m[5] * m[3] * m[10] - 
		m[9] * m[2] * m[7] + 
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] - 
		m[0] * m[7] * m[10] - 
		m[4] * m[2] * m[11] + 
		m[4] * m[3] * m[10] + 
		m[8] * m[2] * m[7] - 
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] + 
		m[0] * m[7] * m[9] + 
		m[4] * m[1] * m[11] - 
		m[4] * m[3] * m[9] - 
		m[8] * m[1] * m[7] + 
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] - 
		m[0] * m[6] * m[9] - 
		m[4] * m[1] * m[10] + 
		m[4] * m[2] * m[9] + 
		m[8] * m[1] * m[6] - 
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}