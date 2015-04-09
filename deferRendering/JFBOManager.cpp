#include "JFBOManager.h"
#include "JTextureManager.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
// convert OpenGL internal format enum to string
///////////////////////////////////////////////////////////////////////////////
std::string convertInternalFormatToString(GLenum format)
{
	std::string formatName;

	switch(format)
	{
	case GL_STENCIL_INDEX:      // 0x1901
		formatName = "GL_STENCIL_INDEX";
		break;
	case GL_DEPTH_COMPONENT:    // 0x1902
		formatName = "GL_DEPTH_COMPONENT";
		break;
	case GL_ALPHA:              // 0x1906
		formatName = "GL_ALPHA";
		break;
	case GL_RGB:                // 0x1907
		formatName = "GL_RGB";
		break;
	case GL_RGBA:               // 0x1908
		formatName = "GL_RGBA";
		break;
	case GL_LUMINANCE:          // 0x1909
		formatName = "GL_LUMINANCE";
		break;
	case GL_LUMINANCE_ALPHA:    // 0x190A
		formatName = "GL_LUMINANCE_ALPHA";
		break;
	case GL_R3_G3_B2:           // 0x2A10
		formatName = "GL_R3_G3_B2";
		break;
	case GL_ALPHA4:             // 0x803B
		formatName = "GL_ALPHA4";
		break;
	case GL_ALPHA8:             // 0x803C
		formatName = "GL_ALPHA8";
		break;
	case GL_ALPHA12:            // 0x803D
		formatName = "GL_ALPHA12";
		break;
	case GL_ALPHA16:            // 0x803E
		formatName = "GL_ALPHA16";
		break;
	case GL_LUMINANCE4:         // 0x803F
		formatName = "GL_LUMINANCE4";
		break;
	case GL_LUMINANCE8:         // 0x8040
		formatName = "GL_LUMINANCE8";
		break;
	case GL_LUMINANCE12:        // 0x8041
		formatName = "GL_LUMINANCE12";
		break;
	case GL_LUMINANCE16:        // 0x8042
		formatName = "GL_LUMINANCE16";
		break;
	case GL_LUMINANCE4_ALPHA4:  // 0x8043
		formatName = "GL_LUMINANCE4_ALPHA4";
		break;
	case GL_LUMINANCE6_ALPHA2:  // 0x8044
		formatName = "GL_LUMINANCE6_ALPHA2";
		break;
	case GL_LUMINANCE8_ALPHA8:  // 0x8045
		formatName = "GL_LUMINANCE8_ALPHA8";
		break;
	case GL_LUMINANCE12_ALPHA4: // 0x8046
		formatName = "GL_LUMINANCE12_ALPHA4";
		break;
	case GL_LUMINANCE12_ALPHA12:// 0x8047
		formatName = "GL_LUMINANCE12_ALPHA12";
		break;
	case GL_LUMINANCE16_ALPHA16:// 0x8048
		formatName = "GL_LUMINANCE16_ALPHA16";
		break;
	case GL_INTENSITY:          // 0x8049
		formatName = "GL_INTENSITY";
		break;
	case GL_INTENSITY4:         // 0x804A
		formatName = "GL_INTENSITY4";
		break;
	case GL_INTENSITY8:         // 0x804B
		formatName = "GL_INTENSITY8";
		break;
	case GL_INTENSITY12:        // 0x804C
		formatName = "GL_INTENSITY12";
		break;
	case GL_INTENSITY16:        // 0x804D
		formatName = "GL_INTENSITY16";
		break;
	case GL_RGB4:               // 0x804F
		formatName = "GL_RGB4";
		break;
	case GL_RGB5:               // 0x8050
		formatName = "GL_RGB5";
		break;
	case GL_RGB8:               // 0x8051
		formatName = "GL_RGB8";
		break;
	case GL_RGB10:              // 0x8052
		formatName = "GL_RGB10";
		break;
	case GL_RGB12:              // 0x8053
		formatName = "GL_RGB12";
		break;
	case GL_RGB16:              // 0x8054
		formatName = "GL_RGB16";
		break;
	case GL_RGBA2:              // 0x8055
		formatName = "GL_RGBA2";
		break;
	case GL_RGBA4:              // 0x8056
		formatName = "GL_RGBA4";
		break;
	case GL_RGB5_A1:            // 0x8057
		formatName = "GL_RGB5_A1";
		break;
	case GL_RGBA8:              // 0x8058
		formatName = "GL_RGBA8";
		break;
	case GL_RGB10_A2:           // 0x8059
		formatName = "GL_RGB10_A2";
		break;
	case GL_RGBA12:             // 0x805A
		formatName = "GL_RGBA12";
		break;
	case GL_RGBA16:             // 0x805B
		formatName = "GL_RGBA16";
		break;
	case GL_DEPTH_COMPONENT16:  // 0x81A5
		formatName = "GL_DEPTH_COMPONENT16";
		break;
	case GL_DEPTH_COMPONENT24:  // 0x81A6
		formatName = "GL_DEPTH_COMPONENT24";
		break;
	case GL_DEPTH_COMPONENT32:  // 0x81A7
		formatName = "GL_DEPTH_COMPONENT32";
		break;
	case GL_DEPTH_STENCIL:      // 0x84F9
		formatName = "GL_DEPTH_STENCIL";
		break;
	case GL_RGBA32F:            // 0x8814
		formatName = "GL_RGBA32F";
		break;
	case GL_RGB32F:             // 0x8815
		formatName = "GL_RGB32F";
		break;
	case GL_RGBA16F:            // 0x881A
		formatName = "GL_RGBA16F";
		break;
	case GL_RGB16F:             // 0x881B
		formatName = "GL_RGB16F";
		break;
	case GL_DEPTH24_STENCIL8:   // 0x88F0
		formatName = "GL_DEPTH24_STENCIL8";
		break;
	default:
		stringstream ss;
		ss << "Unknown Format(0x" << std::hex << format << ")" << std::ends;
		formatName = ss.str();
	}

	return formatName;
}

std::string getRenderbufferParameters(GLuint id)
{
	if(glIsRenderbuffer(id) == GL_FALSE)
		return "Not Renderbuffer object";

	int width, height, format;
	std::string formatName;
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);    // get renderbuffer width
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);  // get renderbuffer height
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format); // get renderbuffer internal format
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	formatName = convertInternalFormatToString(format);

	std::stringstream ss;
	ss << width << "x" << height << ", " << formatName;
	return ss.str();
}

std::string getTextureParameters(GLuint id)
{
	if(glIsTexture(id) == GL_FALSE)
		return "Not texture object";

	int width, height, format;
	std::string formatName;
	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
	glBindTexture(GL_TEXTURE_2D, 0);

	formatName = convertInternalFormatToString(format);

	std::stringstream ss;
	ss << width << "x" << height << ", " << formatName;
	return ss.str();
}

void printFramebufferInfo()
{
	std::cout << "\n===== FBO STATUS =====\n";

	// print max # of colorbuffers supported by FBO
	int colorBufferCount = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
	std::cout << "Max Number of Color Buffer Attachment Points: " << colorBufferCount << std::endl;

	int objectType;
	int objectId;

	// print info of the colorbuffer attachable image
	for(int i = 0; i < colorBufferCount; ++i)
	{
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0+i,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
			&objectType);
		if(objectType != GL_NONE)
		{
			glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0+i,
				GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
				&objectId);

			std::string formatName;

			std::cout << "Color Attachment " << i << ": ";
			if(objectType == GL_TEXTURE)
			{
				std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
			}
			else if(objectType == GL_RENDERBUFFER)
			{
				std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
			}
		}
	}

	// print info of the depthbuffer attachable image
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
		&objectType);
	if(objectType != GL_NONE)
	{
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
			&objectId);

		std::cout << "Depth Attachment: ";
		switch(objectType)
		{
		case GL_TEXTURE:
			std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
			break;
		case GL_RENDERBUFFER:
			std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
			break;
		}
	}

	// print info of the stencilbuffer attachable image
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
		GL_STENCIL_ATTACHMENT,
		GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
		&objectType);
	if(objectType != GL_NONE)
	{
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			GL_STENCIL_ATTACHMENT,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
			&objectId);

		std::cout << "Stencil Attachment: ";
		switch(objectType)
		{
		case GL_TEXTURE:
			std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
			break;
		case GL_RENDERBUFFER:
			std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
			break;
		}
	}

	std::cout << std::endl;
}

JFrameBufferObject::JFrameBufferObject()
{
	bufferID = -1;//Any glGen* function would return ID of 0. 0 is for only special use.

	mngTex = NULL;
	stencilID = 0;
}

int JFrameBufferObject::reset( int jfbo_brushes, GLsizei aWidth, GLsizei aHeight )
{
	if( bufferID == -1 )
	{
		glGenFramebuffers( 1, &bufferID );
	}

	if( bufferID == -1 )
		return -1;

	width = aWidth;
	height = aHeight;

	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);

	/*glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, aWidth);
	glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, aHeight);
	glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);*/

	int result;
	try
	{
		brushes = jfbo_brushes;
		if( (jfbo_brushes & BRUSH_DIFFUSE) )
		{
			if( colorTex.bufID == -1 || colorTex.width != aWidth || colorTex.height != aHeight )
			{
				mngTex->deleteTexture( colorTex );
				if( mngTex->makeTexture( colorTex, aWidth, aHeight, JTEXTUREKIND_COLOR ) != 0 )
					throw;
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex.bufID, 0);
		}
		else if( !(jfbo_brushes & BRUSH_DIFFUSE) )
		{
			mngTex->deleteTexture( colorTex );
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		}

		if( (jfbo_brushes & BRUSH_POSITION) )
		{
			if( positionTex.bufID == -1 || positionTex.width != aWidth || positionTex.height != aHeight )
			{
				mngTex->deleteTexture( positionTex );
				if(mngTex->makeTexture( positionTex, aWidth, aHeight, JTEXTUREKIND_VECTOR ) != 0)
					throw;
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, positionTex.bufID, 0);
		}
		else if( !(jfbo_brushes & BRUSH_POSITION) )
		{
			mngTex->deleteTexture( positionTex );
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
		}

		if( (jfbo_brushes & BRUSH_NORMAL) )
		{
			if( normalTex.bufID == -1 || normalTex.width != aWidth || normalTex.height != aHeight )
			{
				mngTex->deleteTexture( normalTex );
				if(mngTex->makeTexture( normalTex, aWidth, aHeight, JTEXTUREKIND_VECTOR ) != 0)
					throw;
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normalTex.bufID, 0);
		}
		else if( !(jfbo_brushes & BRUSH_NORMAL) )
		{
			mngTex->deleteTexture( normalTex );
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
		}
		//----------------------------------------------------
		if( (jfbo_brushes & BRUSH_TEX) )
		{
			if( texTex.bufID == -1 || texTex.width != aWidth || texTex.height != aHeight )
			{
				mngTex->deleteTexture( texTex );
				if( mngTex->makeTexture( texTex, aWidth, aHeight, JTEXTUREKIND_COLOR ) != 0)
					throw;
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, texTex.bufID, 0);
		}
		else if( !(jfbo_brushes & BRUSH_TEX) )
		{
			mngTex->deleteTexture( texTex );
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, 0, 0);
		}
		//----------------------------------------------------
		if( (jfbo_brushes & BRUSH_SHADOW) )
		{
			if( shadowTex.bufID == -1 || shadowTex.width != aWidth || shadowTex.height != aHeight )
			{
				mngTex->deleteTexture( shadowTex );
				if(mngTex->makeTexture( shadowTex, aWidth, aHeight, JTEXTUREKIND_COLOR ) != 0)
					throw;
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, shadowTex.bufID, 0);
		}
		else if( !(jfbo_brushes & BRUSH_SHADOW) )
		{
			mngTex->deleteTexture( shadowTex );
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, 0, 0);
		}
		if( (jfbo_brushes & BRUSH_DEPTH) )
		{
			if( depthTex.bufID == -1 || depthTex.width != aWidth || depthTex.height != aHeight )
			{
				mngTex->deleteTexture( depthTex );
				if(mngTex->makeTexture( depthTex, aWidth, aHeight, JTEXTUREKIND_DEPTH )!=0)
					throw;
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex.bufID, 0);
			/*glGenRenderbuffers(1, &depthRBO);
			glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);*/
		}
		else if( !(jfbo_brushes & BRUSH_DEPTH) )
		{
			mngTex->deleteTexture( depthTex );
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
			/*glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
			depthRBO = 0;*/
		}
	/*
		The width and height of framebuffer-attachable image must be not zero.
		If an image is attached to a color attachment point, then the image must have a color-renderable internal format. (GL_RGBA, GL_DEPTH_COMPONENT, GL_LUMINANCE, etc)
		If an image is attached to GL_DEPTH_ATTACHMENT, then the image must have a depth-renderable internal format. (GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, etc)
		If an image is attached to GL_STENCIL_ATTACHMENT, then the image must have a stencil-renderable internal format. (GL_STENCIL_INDEX, GL_STENCIL_INDEX8, etc)
		FBO must have at least one image attached.
		All images attached a FBO must have the same width and height.
		All images attached the color attachment points must have the same internal format.
	*/

		printFramebufferInfo();

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
			throw 0;
		
	}
	catch( ... )
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return -1;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 0;
}

int JFrameBufferObject::bind()
{
	if(bufferID > 0)
	{
		glBindFramebuffer( GL_FRAMEBUFFER, bufferID );
	}
	else
		return -1;
	return 0;
}

int JFrameBufferObject::unbind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	return 0;
}

int JFrameBufferObject::setOutputDrawBuffer()
{
	//according to https://www.opengl.org/wiki/Framebuffer_Object ...
	//The minimum value for this is 8, so you are guaranteed to be able to have at least color attachments 0-7

	/*
	least number of drawBuffers is 8 (according to above.. )
	*/
	GLenum drawBuffers[16];

	int attachmentCnt = 0;


	
	if( brushes & JFBO_BRUSHES::BRUSH_DIFFUSE)
	{
		drawBuffers[attachmentCnt] = GL_COLOR_ATTACHMENT0;
		attachmentCnt++;
	}
	if( brushes & JFBO_BRUSHES::BRUSH_POSITION)
	{
		drawBuffers[attachmentCnt] = GL_COLOR_ATTACHMENT1;
		attachmentCnt++;
	}
	if( brushes & JFBO_BRUSHES::BRUSH_NORMAL)
	{
		drawBuffers[attachmentCnt] = GL_COLOR_ATTACHMENT2;
		attachmentCnt++;
	}
	if( brushes & JFBO_BRUSHES::BRUSH_TEX)
	{
		drawBuffers[attachmentCnt] = GL_COLOR_ATTACHMENT3;
		attachmentCnt++;
	}
	if( brushes & JFBO_BRUSHES::BRUSH_SHADOW)
	{
		drawBuffers[attachmentCnt] = GL_COLOR_ATTACHMENT4;
		attachmentCnt++;
	}
	/*NO!!! dont use depth componenent as output drawbuffer
	if( brushes & JFBO_BRUSHES::BRUSH_DEPTH)
	{
		drawBuffers[attachmentCnt] = GL_DEPTH_ATTACHMENT;
		attachmentCnt++;
	}*/

	if( attachmentCnt==0 )
		return -1;

	glDrawBuffers( attachmentCnt, drawBuffers );

	return 0;
}

JTextureObject* JFrameBufferObject::getTextureObjectOfCanvas( JFBO_BRUSHES whichTex )
{
	switch(whichTex)
	{
	case BRUSH_DIFFUSE:
		return colorTex.bufID==-1 ? NULL : &colorTex;
		break;
	case BRUSH_DEPTH:
		return depthTex.bufID==-1 ? NULL : &depthTex;
		break;
	case BRUSH_POSITION:
		return positionTex.bufID==-1 ? NULL : &positionTex;
		break;
	case BRUSH_NORMAL:
		return normalTex.bufID==-1 ? NULL : &normalTex;
		break;
	case BRUSH_TEX:
		return texTex.bufID==-1 ? NULL : &texTex;
		break;
	case BRUSH_SHADOW:
		return shadowTex.bufID==-1 ? NULL : &shadowTex;
		break;
	}
	return NULL;
}

int JFrameBufferObject::initFBO(JTextureManager& argMngTex)
{
	mngTex = &argMngTex;
	return 0;
}

int JFBOManager::makeCanvasWithAttribute( JFrameBufferObject& fbo, int jfbo_brushes, GLsizei width, GLsizei height )
{
	fbo.initFBO( *mngTex );
	if(fbo.reset( jfbo_brushes, width, height ) == -1)
		return -1;
	return 0;
}

int JFBOManager::initFBOManager(JTextureManager& argMngTex)
{
	mngTex = &argMngTex;
	return 0;
}


