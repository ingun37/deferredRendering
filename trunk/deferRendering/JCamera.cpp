#include "JCamera.h"

#include "JFBOManager.h"

#define PROJ_RATIO (1)
#define PROJ_TOP(N) ((N)*(PROJ_RATIO)/2)
#define PROJ_BOTTOM(N) (-(PROJ_TOP(N)))
#define PROJ_RIGHT(W,H,N) ((PROJ_TOP(N))*(W/H))
#define PROJ_LEFT(W,H,N) (-(PROJ_RIGHT(W,H,N)))

JCamera::JCamera()
{
	targetFBO = NULL;
	tag = 0;
}
int JCamera::setTransform( const JVector3& aPosition, const JVector3& aUp, const JVector3& aLookingAt , float aWidth, float aHeight, float aNear, float aFar)
{
	view = JMatrix44::GetViewMatrix( aPosition, aUp, aLookingAt, &front, &up, &right );
	position = aPosition;
	

	width = aWidth;
	height = aHeight;
	zNear = aNear;
	zFar = aFar;

	return 0;
}

int JCamera::setTargetFBO( JFrameBufferObject* pTarget )
{
	targetFBO = pTarget;

	return 0;
}

JCameraOrtho::JCameraOrtho()
{

}

int JCameraOrtho::setTransform( const JVector3& aPosition, const JVector3& aUp, const JVector3& aLookingAt , float aWidth, float aHeight, float aNear, float aFar)
{
	JCamera::setTransform(aPosition, aUp, aLookingAt, aWidth, aHeight, aNear, aFar);
	//proj = JMatrix44::GetProjectionMatrixOrthogonal(-width/2,width/2, height/2,-height/2, zNear, zFar);
	proj = JMatrix44::GetProjectionMatrixOrthogonal(-width/2,width/2, height/2,-height/2, zNear, zFar);
	projection = JCAMERAPROJECTION_ORTHOGONAL;

	return 0;
}

int JCameraOrtho::setTargetFBO( JFrameBufferObject* pTarget )
{
	return JCamera::setTargetFBO(pTarget);
}

JCameraPerspective::JCameraPerspective()
{

}

int JCameraPerspective::setTransform( const JVector3& aPosition, const JVector3& aUp, const JVector3& aLookingAt , float aWidth, float aHeight, float aNear, float aFar)
{
	JCamera::setTransform(aPosition, aUp, aLookingAt, aWidth, aHeight, aNear, aFar);
	proj = JMatrix44::GetProjectionMatrixPerspective(PROJ_LEFT(width, height, zNear),PROJ_RIGHT(width, height, zNear), PROJ_TOP(zNear), PROJ_BOTTOM(zNear), zNear, zFar);
	projection = JCAMERAPROJECTION_PERSPECTIVE;

	return 0;
}

int JCameraPerspective::setTargetFBO( JFrameBufferObject* pTarget )
{
	return JCamera::setTargetFBO(pTarget);
}
