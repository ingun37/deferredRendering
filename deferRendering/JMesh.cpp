#include "JMesh.h"
#include <iostream>
int JVBO::allocVBO( int aVertexNum, int aStructSize )
{
	structSize = aStructSize;
	vertexNum = aVertexNum;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertexNum * structSize, NULL, GL_STATIC_DRAW);


	/*
GL_STREAM_DRAW Buffer contents will be set once by
	the application and used infrequently for drawing.
GL_STREAM_READ Buffer contents will be set once as
	output from an OpenGL command
	and used infrequently for drawing.
GL_STREAM_COPY Buffer contents will be set once as
	output from an OpenGL command
	and used infrequently for drawing or
	copying to other images.
GL_STATIC_DRAW Buffer contents will be set once by
	the application and used frequently
	for drawing or copying to other
	images.
GL_STATIC_READ Buffer contents will be set once as
	output from an OpenGL command
	and queried many times by the
	application.
GL_STATIC_COPY Buffer contents will be set once as
	output from an OpenGL command
	and used frequently for drawing or
	copying to other images.
GL_DYNAMIC_DRAW Buffer contents will be updated
	frequently by the application and
	used frequently for drawing or
	copying to other images.
GL_DYNAMIC_READ Buffer contents will be updated
	frequently as output from OpenGL
	commands and queried many times
	by the application.
GL_DYNAMIC_COPY Buffer contents will be updated
	frequently as output from OpenGL
	commands and used frequently for
	drawing or copying to other images.

*/
	glBindBuffer(GL_ARRAY_BUFFER,0);
	//glBindBuffer(0);//asdf
	glBindVertexArray(0);

	return 0;//TODO error check
}

int JVBO::setVBO( void* data )
{
	glBindVertexArray(vao);
	//TODO update subdata.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * structSize, data, GL_STATIC_DRAW);
	for(int i=0;i<JVERTEXATTNUM;i++)
	{
		JVertexAttributeInfo* attinfo = JVertex::getFixedVertexAttributeInfoArray(i,vertexNum);
		if(attinfo)
		{
			glVertexAttribPointer(
				i,
				attinfo->elementnum,
				GL_FLOAT,//todo : initial value is float. make it flexible.
				attinfo->willNormalize,
				attinfo->stride,
				(const void*)attinfo->offsetFirst
				);
			glEnableVertexAttribArray(i);
		}
		else
		{
			return -1;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 0;
}

int JVBO::clear()
{
	glDeleteVertexArrays(1,&vao);
	glDeleteBuffers(1,&vbo);
	vao = vbo = structSize = vertexNum = 0;
	return 0;
}

int JIBO::allocIBO(int aIndexNum)
{
	indexNum = aIndexNum;

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(unsigned int), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	return 0;
}

int JIBO::setIBO(unsigned int* data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(unsigned int), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	return 0;
}

int JIBO::clear()
{
	glDeleteBuffers(1,&ibo);
	return 0;
}


JMesh::JMesh()
{
	material = NULL;
	tag = 0;
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
	position[3] = 1;
	vbData = NULL;
}

int JMesh::pushVertex(const JVertex& vertex)
{
	vertices.push_back(vertex);
	return 0;
}

int JMesh::setMaterial( JMaterial* mat )
{
	material = mat;
	return 0;
}

JMaterial* JMesh::getMaterial()
{
	return material;
}

int JMesh::refreshVertexIndexBuffer()
{
	int result;
	if( jvbo.vbo > 0 )
		jvbo.clear();

	vbData = new char[ sizeof(JVertex) * vertices.size() ];
	memset(vbData,0,sizeof(JVertex)*vertices.size());
	rearrangeData();

	result = jvbo.allocVBO( vertices.size(), sizeof(JVertex) );
	if(result != 0)
		return -1;

	result = jvbo.setVBO( vbData );

	if(result != 0)
		return -1;

	if( jibo.ibo > 0 )
		jibo.clear();

	result = jibo.allocIBO( indices.size() );
	if(result != 0)
		return -1;

	result = jibo.setIBO( &(indices[0]) );

	if(result != 0)
		return -1;
	
	return 0;
}

int JMesh::draw()
{
	if( jvbo.vao > 0 && jibo.ibo > 0 )
	{
		glBindVertexArray(jvbo.vao);

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, jibo.ibo );
		glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		//glDrawArrays(GL_TRIANGLES,0,jvbo->vertexNum);
		glBindVertexArray(0);
	}

	return 0;
}

int JMesh::pushIndex(const unsigned int index)
{
	indices.push_back(index);
	return 0;
}

int JMesh::clearGLBuffers()
{
	jibo.clear();
	jvbo.clear();
	return 0;
}

int JMesh::clearData()
{
	vertices.clear();
	indices.clear();
	return 0;
}

int JMesh::clearAll()
{
	clearData();
	clearGLBuffers();
	return 0;
}

int JMesh::reserveVertexLen(const unsigned int len)
{
	vertices.reserve( len );
	return 0;
}

int JMesh::reserveIndexLen(const unsigned int len)
{
	indices.reserve( len );
	return 0;
}

int JMesh::resetVBData()
{
	int result;
	if( jvbo.vbo > 0 )
		result = jvbo.setVBO( vbData );//TODO : subdata

	if(result != 0)
		return -1;

	return 0;
}

int JMesh::rearrangeData()
{
	if(vertices.size()==0 || vbData == NULL)
		return -1;

	//fill position
	
	for(int atti=0;atti<JVERTEXATTNUM;atti++)
	{
		JVertexAttributeInfo& infoarr = *JVertex::getFixedVertexAttributeInfoArray(atti,vertices.size());
		/*std::cout<<"copying "<<atti<<"'s att" <<endl;
		std::cout<<"offsetBuffer "<< infoarr.offsetBuffer <<endl;
		std::cout<<"chunksize "<< infoarr.chunkSize <<endl;
		std::cout<<" "<<atti<<"'s att" <<endl;*/
		for(unsigned int i=0;i<vertices.size();i++)
		{
			memcpy( vbData + infoarr.offsetBuffer + i*infoarr.chunkSize, ((char*)&vertices[i]) + infoarr.offsetObject, infoarr.nameitSize );
		}
	}
	return 0;
}

int JMesh::resetSubData(JVERTEXATTRIBUTE att, unsigned int idx, void* data)
{
	if(vbData == NULL )
		return -1;
	JVertexAttributeInfo& infoarr = *JVertex::getFixedVertexAttributeInfoArray(att,vertices.size());

	memcpy(vbData+infoarr.offsetBuffer + idx*infoarr.chunkSize,data,infoarr.chunkSize);

	return 0;
}

int JMesh::zeroMemorySubDataAttribute(JVERTEXATTRIBUTE att)
{
	JVertexAttributeInfo& infoarr = *JVertex::getFixedVertexAttributeInfoArray(att,vertices.size());
	memset(vbData+infoarr.offsetBuffer,0,infoarr.chunkSize*vertices.size());
	return 0;
}
