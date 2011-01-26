// OpenGL phong shader abstraction
// -------------------------------------------------------------------
// Copyright (C) 2010 OpenEngine.dk (See AUTHORS) 
// Modified by Anders Bach Nielsen <abachn@daimi.au.dk> - 21. Nov 2007
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Resources/PhongShader.h>

#include <Resources/DirectoryManager.h>
#include <Logging/Logger.h>
#include <Resources/Texture2D.h>
#include <Geometry/GeometrySet.h>

namespace OpenEngine {
namespace Resources {
        
using namespace Geometry;
    PhongShader::PhongShader(MeshPtr mesh, LightRenderer& lr)
    : OpenGLShader(DirectoryManager::FindFileInPath("extensions/OpenGLRenderer/shaders/PhongShader.glsl"))
    , mesh(mesh)
    , lr(lr)
    , lights(1) // hack ... cannot compile shader with zero lights.
{

    lr.LightCountChangedEvent().Attach(*this);
    MaterialPtr mat = mesh->GetMaterial();
    tans = mesh->GetGeometrySet()->GetAttributeList("tangent");
    bitans = mesh->GetGeometrySet()->GetAttributeList("bitangent");
    ambient = mat->Get2DTextures()["ambient"];
    diffuse = mat->Get2DTextures()["diffuse"];
    specular = mat->Get2DTextures()["specular"];
    bump = mat->Get2DTextures()["normal"];
    if (!bump)
        bump = mat->Get2DTextures()["height"];
    if (bump && bump->GetChannels() < 3) bump.reset();

    logger.info << "mesh texs: " << mesh->GetGeometrySet()->GetTexCoords().size() << logger.end;

    Update();

}

PhongShader::~PhongShader() {

}

void PhongShader::Update() {
    MaterialPtr mat = mesh->GetMaterial();
    if (ambient) {
        AddDefine("AMBIENT_MAP");
        SetTexture("ambientMap", ambient);
    }
    
    if (diffuse) {
        AddDefine("DIFFUSE_MAP");
        AddDefine("DIFFUSE_INDEX", mat->GetUVIndex(diffuse));
        SetTexture("diffuseMap", diffuse);

        logger.info << "diffuse index: " << mat->GetUVIndex(diffuse) << logger.end;
    }

    if (specular) {
        AddDefine("SPECULAR_MAP");
        SetTexture("specularMap", specular);
    }

    if (bump && tans && bitans) {
        logger.info << "bump channels: " << (unsigned int)bump->GetChannels() << logger.end;
        AddDefine("BUMP_MAP");
        AddDefine("BUMP_INDEX", mat->GetUVIndex(bump));
        SetTexture("bumpMap", bump);

        logger.info << "bump index: " << mat->GetUVIndex(bump) << logger.end;
    }    

    AddDefine("NUM_LIGHTS", lights); 
}

void PhongShader::Handle(LightCountChangedEventArg arg) {
    if (arg.count == lights) return;
    lights = arg.count;
    if (lights == 0) return;

    // logger.info << "# of lights changed to " << lights << ". Recompiling phong shader..." << logger.end;

    ClearDefines();

    Unload();
    
    Update();

    Load();
}

void PhongShader::ApplyShader() {
    if (lights == 0) return;
    OpenGLShader::ApplyShader();
    if (bump && tans && bitans) {
        SetAttribute("tangent", tans);
        SetAttribute("bitangent", bitans);
    }


    // if (tans && bitans) {
    //     tanLoc = glGetAttribLocation(shaderProgram,"tangent");
    //     // logger.info << "shaderProg: " << shaderProgram << logger.end;
    //     // logger.info << "tanloc: " << tanLoc << logger.end;
    //     CHECK_FOR_GL_ERROR();
    //     glEnableClientState(GL_VERTEX_ARRAY);
    //     CHECK_FOR_GL_ERROR();
    //     glEnableVertexAttribArray(tanLoc);
    //     CHECK_FOR_GL_ERROR();
    //     glVertexAttribPointer(tanLoc, tans->GetDimension(), GL_FLOAT, 0, 0, tans->GetVoidDataPtr());
    //     CHECK_FOR_GL_ERROR();

    //     bitanLoc = glGetAttribLocation(shaderProgram,"bitangent");
    //     // logger.info << "bitanloc: " << bitanLoc << logger.end;
    //     CHECK_FOR_GL_ERROR();
    //     glEnableVertexAttribArray(bitanLoc);
    //     CHECK_FOR_GL_ERROR();
    //     glVertexAttribPointer(bitanLoc, bitans->GetDimension(), bitans->GetType(), 0, 0, bitans->GetVoidDataPtr());
    //     CHECK_FOR_GL_ERROR();
    // }
}


}
}

