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

JVertexAttributeInfo* JVertex::getFixedVertexAttributeInfoArray(int i)
{
	static JVertexAttributeInfo fixedparameters[JVERTEXATTNUM];
	static int inited = 0;
	if(inited == 0)
	{
		memset(fixedparameters[0].varname, 0, sizeof(fixedparameters[0].varname));
		strncpy(fixedparameters[0].varname, "position", 8);
		fixedparameters[0].location = 0;
		fixedparameters[0].elementnum = 3;
		fixedparameters[0].type = GL_FLOAT;
		fixedparameters[0].willNormalize = 0;
		fixedparameters[0].offset = (unsigned char*)NULL + 0;
		fixedparameters[0].stride = sizeof(JVertex);

		memset(fixedparameters[1].varname, 0, sizeof(fixedparameters[1].varname));
		strncpy(fixedparameters[1].varname, "normal", 6);
		fixedparameters[1].location = 1;
		fixedparameters[1].elementnum = 3;
		fixedparameters[1].type = GL_FLOAT;
		fixedparameters[1].willNormalize = 1;
		fixedparameters[1].offset = (unsigned char*)NULL + (sizeof(float) * 3);
		fixedparameters[1].stride = sizeof(JVertex);

		memset(fixedparameters[2].varname, 0, sizeof(fixedparameters[2].varname));
		strncpy(fixedparameters[2].varname, "uv", 2);
		fixedparameters[2].location = 2;
		fixedparameters[2].elementnum = 2;
		fixedparameters[2].type = GL_FLOAT;
		fixedparameters[2].willNormalize = 0;
		fixedparameters[2].offset = (unsigned char*)NULL + (sizeof(float) * 6);
		fixedparameters[2].stride = sizeof(JVertex);

		memset(fixedparameters[3].varname, 0, sizeof(fixedparameters[3].varname));
		strncpy(fixedparameters[3].varname, "diffuse", 2);
		fixedparameters[3].location = 3;
		fixedparameters[3].elementnum = 4;
		fixedparameters[3].type = GL_FLOAT;
		fixedparameters[3].willNormalize = 0;
		fixedparameters[3].offset = (unsigned char*)NULL + (sizeof(float) * 8);
		fixedparameters[3].stride = sizeof(JVertex);

		inited = 1;
	}
	if(i >= 0 && i<JVERTEXATTNUM)
		return &(fixedparameters[i]);
	return NULL;
}

JVertex::JVertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a, float u, float v)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;

	normal[0] = nx;
	normal[1] = ny;
	normal[2] = nz;

	uv[0] = u;
	uv[1] = v;

	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
}

JVertex::JVertex()
{

}