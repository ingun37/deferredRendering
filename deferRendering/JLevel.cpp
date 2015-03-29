#include "JLevel.h"
#include "JProgramManager.h"
#include "JFBOManager.h"

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

JLevel::JLevel()
{
	
}

//after this, textures attached to fbo would be filled.
int JLevel::draw()
{
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
					JProgramManager::Inst()->useProgram( sInfo );
					

					JMatrix44 mM = JMatrix44::GetTranslationMatrix( mesh->position[0], mesh->position[1], mesh->position[2] );

					bool uniformVariableSetsuccess = true;
						
					//setting uniform variable and output drawbuffer
					switch(sInfo->shaderKind)
					{
					case JSHADERKIND_DIFFUSE:
						if( JProgramManager::Inst()->setUniformVariables_Diffuse(mP*mV*mM) != 0 )
						{
							uniformVariableSetsuccess = false;
						}
						break;
					case JSHADERKIND_TEXUNLIT:
						if(material->texObj && material->texObj->bufID > 0)
						{
							JProgramManager::Inst()->setUniformVariables_TexUnlit(mP * mV * mM, material->texObj);
						}
						else
							uniformVariableSetsuccess = false;

						break;
					case JSHADERKIND_DEFERRED:
						JProgramManager::Inst()->setUniformVariables_Deferred( mP * mV * mM );
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
						if( JProgramManager::Inst()->useProgram( currentShader ) != 0 )
							break;
					}

					//TODO don't calculate transform matrix per every mesh
					JMatrix44 mM = JMatrix44::GetTranslationMatrix( mesh->position[0], mesh->position[1], mesh->position[2] );

					bool uniformVariableSetsuccess = true;
					switch(currentShader->shaderKind)
					{
					case JSHADERKIND_DIFFUSE:
						if( JProgramManager::Inst()->setUniformVariables_Diffuse(mP*mV*mM) != 0 )
						{
							uniformVariableSetsuccess = false;
						}
						break;
					case JSHADERKIND_TEXUNLIT:
						if(material->texObj && material->texObj->bufID > 0)
						{
							JProgramManager::Inst()->setUniformVariables_TexUnlit(mP * mV * mM, material->texObj);
						}
						else
							uniformVariableSetsuccess = false;

						break;
					case JSHADERKIND_DEFERRED: //in this case, it is meant to not to be here.
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
	return 0;
}
