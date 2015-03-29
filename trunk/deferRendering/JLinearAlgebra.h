#ifndef JLinearAlgebra_h
#define JLinearAlgebra_h

#define fitN(IDX,N) ((unsigned int)(IDX -(floorf(((float)IDX)/N) * N)))
//#define GLEW_STATIC
#include <GL/glew.h>
#include <math.h>
template <unsigned int N>
class JVector
{
public:
	float val[N];

	float length() const
	{
		float accu = 0;
		for(int i=0;i<N;i++)
			accu += val[i] * val[i];
		return sqrtf( accu );
	}

	JVector<N> unitVector() const
	{
		float len = length();

		return (*this)/len;
	}
	float angleBetween( const JVector<N>& v ) const
	{
		JVector<N> unit1 = unitVector();
		JVector<N> unit2 = v.unitVector();

		float cosVal = unit1 * unit2;

		float angle = acosf( cosVal );

		return angle;
	}

	float& operator []( int i) const
	{
		return ((float*)this)[ fitN(i,N) ];
	}
	JVector<N> operator /(const JVector<N>& v) const //cross product
	{
		JVector<N> ret;

		for(int i=0;i<N;i++)
		{
			ret[i] = ( val[ fitN( i+1,N ) ] * v[ fitN(i+2,N) ] ) - ( val[ fitN(i-1,N) ] * v[ fitN(i-2,N) ] );
		}
		return ret;
	}

	JVector<N> operator /(const float f) const
	{
		JVector<N> ret;

		for( int i=0;i<N;i++)
		{
			ret[i] = val[i]/f;
		}

		return ret;
	}

	float operator *(const JVector<N>& v) const //dot product
	{
		float accu = 0;
		for(int i=0;i<N;i++)
			accu += val[i] * v[i];
		return accu;
	}

	JVector<N> operator *(const float f) const
	{
		JVector<N> v;
		for( int i=0 ; i < N ; i++ )
			v[i] = val[i] * f;
		return v;
	}

	JVector<N> operator -() const
	{
		return (*this) * -1;
	}
	JVector<N> operator -( const JVector<N>& v) const
	{
		JVector<N> r;
		for(int i=0;i<N;i++)
		{
			r[i] = val[i] - v[i];
		}
		return r;
	}

	JVector<N> operator +( const JVector<N>& v) const
	{
		JVector<N> r;
		for(int i=0;i<N;i++)
		{
			r[i] = val[i] + v[i];
		}
		return r;
	}

	JVector<N+1> getCovariantVector( ) const//normal
	{
		JVector<N+1> ret;
		for(int i=0;i<N;i++)
			ret[i] = val[i];
		ret[N] = 0;

		return ret;
	}
	JVector<N+1> getContravariantVector ( ) const//other than nomal
	{
		JVector<N+1> ret;

		for(int i=0;i<N;i++)
			ret[i] = val[i];
		ret[N] = 1;

		return ret;
	}

	JVector()
	{
		for(unsigned int i=0;i<N;i++)
			val[i]=0;
	}
};

typedef JVector<2> JVector2;
typedef JVector<3> JVector3;
typedef JVector<4> JVector4;

class JMatrix44
{
public:
	JVector4 r1;
	JVector4 r2;
	JVector4 r3;
	JVector4 r4;
	
	static JMatrix44 GetIdentityMatrix();
	static JMatrix44 GetRotateMatrix( float x, float y, float z, float angle );
	static JMatrix44 GetTranslationMatrix( const float dx, const float dy, const float dz );
	static JMatrix44 GetViewMatrix( const JVector3& eye, const JVector3& up, const JVector3& at , JVector3* pF = 0, JVector3* pU = 0, JVector3* pR = 0);
	static JMatrix44 GetProjectionMatrixOrthogonal( const float left, const float right, const float top, const float bottom, const float zNear, const float zFar );
	static JMatrix44 GetProjectionMatrixPerspective( const float left, const float right, const float top, const float bottom, const float zNear, const float zFar );

	JVector4& operator[]( int i ) const;
	JMatrix44 operator*( const JMatrix44& m ) const;
	JVector4 operator*( const JVector4& v ) const;
};



typedef struct JVertexAttributeInfoRaw
{
	char varname[128];
	int location;
	int elementnum;
	GLenum type;
	int willNormalize;
	GLvoid* offset;
	int stride;
}JVertexAttributeInfo;

enum JVERTEXATTRIBUTE
{
	JVERTEXATTPOSITION,
	JVERTEXATTNORMAL,
	JVERTEXATTUV,
	JVERTEXATTDIFFUSE,
	JVERTEXATTNUM
};

class JVertex
{
public:
	JVector3 position;
	JVector3 normal;
	JVector2 uv;
	JVector4 diffuse;
	

	static JVertexAttributeInfo* getFixedVertexAttributeInfoArray(int i);

	JVertex( float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a, float u, float v);
	JVertex();
};
#endif
