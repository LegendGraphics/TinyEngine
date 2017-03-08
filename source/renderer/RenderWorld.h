#ifndef RENDERER_RENDERWORLD_H
#define RENDERER_RENDERWORLD_H

#include "base/Globals.h"
#include "renderer/resource/RenderObject.h"

namespace te
{
    class RenderCamera;
    class PipelineResource;

    struct RenderQueueItem
    {
        RenderObject  *node;
        float      sortKey;
        uint32  type;

        RenderQueueItem() {}
        RenderQueueItem(float sortKey, RenderObject *node)
            : node(node), sortKey(sortKey) {}
    };

    typedef std::vector< RenderQueueItem > RenderQueue;

    // RenderWorld is the entrance of all "render" aspect things of the game world
    class RenderWorld : public RenderObject
    {
    public:
        static RenderObject::Type TYPE;

        RenderWorld();
        ~RenderWorld();

        struct RenderParams
        {
            class RenderDevice* _device;
            PipelineResource* _pipelineRes;
            RenderCamera* _camera;
            RenderQueue _renderQueue;
        };
        void render(RenderParams& params);

    protected:
        void createPrimitives(RenderDevice* device);

    private:
//        ShaderObject _defaultColorShader;

        //uint32  _vbCube, _ibCube, _vbSphere, _ibSphere;
        //uint32  _vbCone, _ibCone, _vbFSPoly;
    };
}

#endif
