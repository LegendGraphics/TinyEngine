#ifndef RENDERER_RENDERMESHOBJECT_H
#define RENDERER_RENDERMESHOBJECT_H

#include "renderer/resource/RenderObject.h"

namespace te
{
    class RenderContext;
    class RenderDevice;
    class RenderCamera;

    class RenderMeshObject : public RenderObject
    {
    public:
        static RenderObject::Type TYPE;

        RenderMeshObject();
        ~RenderMeshObject();

        void render(RenderContext* context, RenderCamera* camera, RenderDevice* device);

    private:
        uint32  _shader_object_handle;
        uint32  _indexBuf, _posVBuf; // temporary to put it here
        // TODO: consider move this to a GeometryObject? Shader belongs to Material, Material
        // belongs to Mesh (Mesh should be a Node ?), Geometry also belongs to Mesh
    };
}

#endif
