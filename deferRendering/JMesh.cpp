#include "JMesh.h"

JVBO::JVBO()
{
	vbo = vao = structSize = vertexNum = 0;
}

int JVBO::allocVBO( int aVertexNum, int aStructSize )
{
	structSize = aStructSize;
	vertexNum = aVertexNum;

	void* dump;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	dump = (void*)malloc(structSize * vertexNum);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * structSize, dump, GL_STATIC_DRAW);//TODO : check if dump i necessary


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
	free(dump);

	//glBindBuffer(0);//asdf
	glBindVertexArray(0);

	return 0;//TODO error check
}

int JVBO::setVBO( void* data )
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * structSize, data, GL_STATIC_DRAW);
	for(int i=0;i<JVERTEXATTNUM;i++)
	{
		JVertexAttributeInfo* attinfo = JVertex::getFixedVertexAttributeInfoArray(i);
		if(attinfo)
		{
			//position
			glVertexAttribPointer(
				i,
				attinfo->elementnum,
				attinfo->type,
				attinfo->willNormalize,
				attinfo->stride,
				attinfo->offset
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

JIBO::JIBO()
{
	ibo = indexNum = 0;
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


JMesh::JMesh()
{
	material = NULL;
	jvbo = NULL;
	jibo = NULL;
	tag = 0;
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
	position[3] = 1;
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
	if(jvbo == NULL)
	{
		jvbo = new JVBO();
		result = jvbo->allocVBO( vertices.size(), sizeof(JVertex) );
		if(result != 0)
			return -1;
	}

	result = jvbo->setVBO( &(vertices[0]) );

	if(result != 0)
		return -1;

	if(jibo == NULL)
	{
		jibo = new JIBO();
		result = jibo->allocIBO( indices.size() );
		if(result != 0)
			return -1;
	}

	result = jibo->setIBO( &(indices[0]) );

	if(result != 0)
		return -1;
	
	return 0;
	
	//TODO if number of vertex is different then realloc jvbo
}

int JMesh::draw()
{
	if( jvbo && jvbo->vao > 0 && jibo && jibo->ibo > 0 )
	{
		glBindVertexArray(jvbo->vao);

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, jibo->ibo );
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

