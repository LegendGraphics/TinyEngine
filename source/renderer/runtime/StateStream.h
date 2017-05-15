#ifndef RENDERER_STATE_STREAM_H
#define RENDERER_STATE_STREAM_H

#include "renderer/resource/RenderObject.h"
#include "renderer/resource/RenderResource.h"
#include "renderer/resource/RenderMeshObject.h"
#include "math/Matrix.h"

namespace te
{
    class RenderContext;
    class RenderResourceContext;

    // intermediate stream data between engine and renderer
    class StreamMsg
    {
    public:
        enum MsgType
        {
            CREATE,
            UPDATE,
            RENDER,
            NOT_INITIALIZED
        };

    public:
        StreamMsg() : _data(nullptr), _type(NOT_INITIALIZED), _handle(nullptr) {};
        virtual ~StreamMsg() {};

        void setMsgType(MsgType type) { _type = type; }
        MsgType getMsgType() { return _type; }
        void setHandle(Handle* handle) { _handle = handle; }
        Handle* getHandle() { return _handle; }
        virtual void process(RenderObject*& render_object, RenderContext* rc, RenderResourceContext* rrc);
        virtual void feedData(void* data) { _data = data; }

    protected:
        virtual void create(RenderObject*& render_object, RenderResourceContext* rrc) = 0;
        virtual void update(RenderObject*& render_object, RenderResourceContext* rrc) = 0;
        virtual void render(RenderObject*& render_object, RenderContext* rc) = 0;

    protected:
        void*         _data;
        MsgType       _type;
        Handle*       _handle;
    };

    using StateStream = std::vector<StreamMsg*>;

    // camera data stream
    class CameraStreamMsg : public StreamMsg
    {
    public:
        struct Data
        {
            RenderCamera*   _camera;

        };
    public:
        virtual ~CameraStreamMsg();

    protected:
        virtual void create(RenderObject*& render_object, RenderResourceContext* rrc);
        virtual void update(RenderObject*& render_object, RenderResourceContext* rrc);
        virtual void render(RenderObject*& render_object, RenderContext* rc);

    };


    // test a mesh stream msg
    class MeshStreamMsg : public StreamMsg
    {
    public:
        struct Data
        {
            RenderMeshObject*   rmo;
            Matrix              model_mat;
        };

    public:
        virtual ~MeshStreamMsg();

    protected:
        virtual void create(RenderObject*& render_object, RenderResourceContext* rrc);
        virtual void update(RenderObject*& render_object, RenderResourceContext* rrc);
        virtual void render(RenderObject*& render_object, RenderContext* rc);
    };
}

#endif
