// OpenGL phong shader abstraction
// -------------------------------------------------------------------
// Copyright (C) 2010 OpenEngine.dk (See AUTHORS) 
// Modified by Anders Bach Nielsen <abachn@daimi.au.dk> - 21. Nov 2007
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OPENGL_PHONG_SHADER_RESOURCE_H_
#define _OPENGL_PHONG_SHADER_RESOURCE_H_

// #include <Resources/IShaderResource.h>
#include <Resources/OpenGLShader.h>
#include <Resources/ITexture2D.h>
#include <Geometry/Mesh.h>
#include <Core/IListener.h>
#include <Renderers/OpenGL/LightRenderer.h>

namespace OpenEngine {
namespace Resources {
        
using Geometry::MeshPtr;
using Core::IListener;
using Renderers::OpenGL::LightCountChangedEventArg;
using Renderers::OpenGL::LightRenderer;

class PhongShader: public OpenGLShader, public IListener<LightCountChangedEventArg> {
private:
    // IShaderResourcePtr shader;
    MeshPtr mesh;
    LightRenderer& lr;
    ITexture2DPtr ambient, diffuse, specular, bump, opacity;
    IDataBlockPtr tans, bitans;
    GLint tanLoc, bitanLoc;
    unsigned int lights;

    inline void Update();
public:
    PhongShader(MeshPtr mesh, LightRenderer& lr);
    virtual ~PhongShader();
    void ApplyShader();
    void Handle(LightCountChangedEventArg arg);
};

}
}

#endif //_OPENGL_PHONG_SHADER_RESOURCE_H_
