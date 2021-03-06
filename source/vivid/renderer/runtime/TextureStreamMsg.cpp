#include "vivid/renderer/runtime/StateStream.h"
#include "vivid/common/Texture.h"

namespace vivid
{
    TextureStreamMsg::TextureStreamMsg(stream_message::ActionType type, Handle handle, void* data)
        :ResourceStreamMsg(type, handle, data)
    {
    }

    TextureStreamMsg::~TextureStreamMsg()
    {
    }

    void TextureStreamMsg::setHandle(Handle handle)
    {
        _handle = handle;
        static_cast<Texture*>(_data)->setROHandle(handle);
    }

    void TextureStreamMsg::update(RenderObject* render_object, RenderResourceContext* rrc)
    {
        RenderTextureObject* rto = static_cast<RenderTextureObject*>(render_object);
        if (!rto) return;

        // do something if we want to update the status of rmo
        // usually get the data in the message, parse it and set the rmo accordingly
        rto->parseStreamMsg(this);

        rto->update(rrc);
    }

    void TextureStreamMsg::render(RenderObject* render_object, RenderContext* rc)
    {
        RenderTextureObject* rto = static_cast<RenderTextureObject*>(render_object);
        if (!rto) return;

        // do something if we want to update the status of rmo
        // usually get the data in the message, parse it and set the rmo accordingly

        //// get model matrix
        //Data* data = static_cast<Data*>(_data);
        //rmo->setModelMat(data->model_mat);

        rto->render(rc);
    }
}