#ifndef JCamera_h
#define JCamera_h

#include "JLinearAlgebra.h"
#include <string>


using namespace std;

enum JCamera_PROJECTION { JCAMERAPROJECTION_PERSPECTIVE, JCAMERAPROJECTION_ORTHOGONAL };
enum JCamera_DEFERLEVEL { J_DEPTH };

class JFrameBufferObject;

class JCamera
{
public:
	int tag;

	JVector3 position;
	JVector3 up;
	JVector3 front;
	JVector3 right;
	
	float width;
	float height;
	float zNear;
	float zFar;
	JMatrix44 view;
	JMatrix44 proj;

	JFrameBufferObject* targetFBO;

	JCamera_PROJECTION projection;

	virtual int setTransform( const JVector3& aPosition, const JVector3& aUp, const JVector3& aLookingAt , float aWidth, float aHeight, float aNear, float aFar);
	virtual int setTargetFBO( JFrameBufferObject* pTarget );
	JCamera();
};

class JCameraOrtho : public JCamera
{
public:
	virtual int setTransform( const JVector3& aPosition, const JVector3& aUp, const JVector3& aLookingAt , float aWidth, float aHeight, float aNear, float aFar);
	virtual int setTargetFBO( JFrameBufferObject* pTarget );
	JCameraOrtho();
};

class JCameraPerspective : public JCamera
{
public:
	virtual int setTransform( const JVector3& aPosition, const JVector3& aUp, const JVector3& aLookingAt , float aWidth, float aHeight, float aNear, float aFar);
	virtual int setTargetFBO( JFrameBufferObject* pTarget );
	JCameraPerspective();
};

#endif
