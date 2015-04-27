#include "JLevel.h"
#include "JProgramManager.h"
#include "JFBOManager.h"
#include "JGlobalVariables.h"
int JLevel::pushMesh( JMesh* pMesh )
{
	if(pMesh)
	{
		meshes.push_back(pMesh);
		return 0;
	}
	return -1;
}

int JLevel::pushCamera( JCamera* pCamera )
{
	if(pCamera)
	{
		cameras.push_back(pCamera);
		return 0;
	}
	return -1;
}

JLevel::JLevel():shadowCamera(0),shadowShader(0),mngProg(0)
{
}

int JLevel::initJLevel( JProgramManager& argMngProg )
{
	mngProg = &argMngProg;
	return 0;
}

//after this, textures attached to fbo would be filled.
int JLevel::draw()
{
	//-------------shadow depth rendering
	JFrameBufferObject *shadowFBO = NULL;
	JMatrix44 shadowPV;
	if( shadowCamera && shadowShader)
	{
		shadowFBO = shadowCamera->targetFBO;
		shadowPV = shadowCamera->proj * shadowCamera->view;
		if(shadowFBO)
		{
			shadowFBO->bind();
			shadowFBO->setOutputDrawBuffer();

			mngProg->useProgram( shadowShader );

			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			for( unsigned int mi = 0;mi<meshes.size();mi++)
			{
				JMesh* mesh = meshes[mi];
				if(mesh->material && mesh->material->shaderinfo)
				{
					JMatrix44 mM = JMatrix44::GetTranslationMatrix( mesh->position[0], mesh->position[1], mesh->position[2] );
					mngProg->setUniformVariables_DirShadow(shadowPV* mM);
					mesh->draw();
				}
			}
			shadowFBO->unbind();
		}
	}

	//-------------------actual rendering
	for( unsigned int ci=0 ; ci<cameras.size() ; ci++ )
	{
		JCamera* camera = cameras[ci];
		JFrameBufferObject* fbo = camera->targetFBO;

		JMatrix44 mV = camera->view;
		JMatrix44 mP = camera->proj;

		shaderInfo* currentShader=NULL;
		if(fbo)
		{
			fbo->bind();
			fbo->setOutputDrawBuffer();
			
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
			for( unsigned int mi = 0;mi<meshes.size();mi++)
			{
				JMesh* mesh = meshes[mi];
				if(mesh->material && mesh->material->shaderinfo)
				{
					JMaterial* material = mesh->material;
					shaderInfo* sInfo = mesh->material->shaderinfo;
					mngProg->useProgram( sInfo );
					
					JMatrix44 mM = JMatrix44::GetTranslationMatrix( mesh->position[0], mesh->position[1], mesh->position[2] );

					bool uniformVariableSetsuccess = true;
						
					//setting uniform variable and output drawbuffer
					switch(sInfo->shaderKind)
					{
					case JSHADERKIND_DIFFUSE:
						if( mngProg->setUniformVariables_Diffuse(mP*mV*mM) != 0 )
						{
							uniformVariableSetsuccess = false;
						}
						break;
					case JSHADERKIND_TEXUNLIT:
						if(material->texObj && material->texObj->bufID > 0)
						{
							mngProg->setUniformVariables_TexUnlit(mP * mV * mM, material->texObj);
						}
						else
							uniformVariableSetsuccess = false;

						break;
					case JSHADERKIND_DEFERRED:
						mngProg->setUniformVariables_Deferred( mP * mV * mM, material->texObj, shadowPV, shadowFBO? shadowFBO->getTextureObjectOfCanvas(BRUSH_DEPTH) : NULL, mM );
						break;
					default:
						uniformVariableSetsuccess = false;
						break;
					}
					////////////////////////////////////////////////


					if(uniformVariableSetsuccess == false) break;
					
					mesh->draw();
				}
			}
			fbo->unbind();
		}
		else
		{
			//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			for( unsigned int mi = 0;mi<meshes.size();mi++)
			{
				JMesh* mesh = meshes[mi];
				if(mesh->material && mesh->material->shaderinfo)
				{
					JMaterial* material = mesh->material;
					shaderInfo* sInfo = mesh->material->shaderinfo;
					if(currentShader!=sInfo)
					{
						currentShader = sInfo;
						if( mngProg->useProgram( currentShader ) != 0 )
							break;
					}

					//TODO don't calculate transform matrix per every mesh
					JMatrix44 mM = JMatrix44::GetTranslationMatrix( mesh->position[0], mesh->position[1], mesh->position[2] );

					bool uniformVariableSetsuccess = true;
					switch(currentShader->shaderKind)
					{
					case JSHADERKIND_DIFFUSE:
						if( mngProg->setUniformVariables_Diffuse(mP*mV*mM) != 0 )
						{
							uniformVariableSetsuccess = false;
						}
						break;
					case JSHADERKIND_TEXUNLIT:
						if(material->texObj && material->texObj->bufID > 0)
						{
							mngProg->setUniformVariables_TexUnlit(mP * mV * mM, material->texObj);
						}
						else
							uniformVariableSetsuccess = false;

						break;
					case JSHADERKIND_FINALDEFERRED:
						if(material->extexObj1&&material->extexObj2&&material->extexObj3&&material->extexObj4&&material->extexObj5)
						{
							mngProg->setUniformVariables_FinalDeferred(mP * mV * mM, JGlobalVariables::gWorldCameraEyePos, JGlobalVariables::gSunlightDir, material->extexObj1, material->extexObj2, material->extexObj3, material->extexObj4, material->extexObj5 );
						}
						else
							uniformVariableSetsuccess = false;
						break;
					default:
						uniformVariableSetsuccess = false;
						break;
					}

					if(uniformVariableSetsuccess == false)
						break;

					mesh->draw();
				}
			}
		}
	}

	//-------------debug rendering

	return 0;
}

int JLevel::removeMesh(JMesh* pMesh)
{
	if(pMesh)
	{
		for(int i=0;i<meshes.size();i++)
		{
			if(meshes[i]==pMesh)
			{
				meshes.erase( meshes.begin() + i);
				break;
			}
		}
		return 0;
	}
	return -1;
}

int JLevel::removeCamera(JCamera* pCamera)
{
	if(pCamera)
	{
		for(int i=0;i<cameras.size();i++)
		{
			if(cameras[i]==pCamera)
			{
				cameras.erase( cameras.begin() + i);
				break;
			}
		}
		return 0;
	}
	return -1;
}
