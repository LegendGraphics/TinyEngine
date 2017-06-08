#ifndef COMMON_UPLOADER_H
#define COMMON_UPLOADER_H

#include "common/Component.h"
#include "common/Material.h"
#include "common/Camera.h"
#include "common/SpaceState.h"
#include "renderer/runtime/StateStream.h"

namespace te
{
    class UploadToRender : public Render
    {
    public:
        UploadToRender();

        void setActionType(stream_message::ActionType type);

        void update();

    protected:
        virtual void assembleUpdateMsg();
        virtual void assembleRenderMsg();

        // build state stream msg
        void updateMesh();
        void updateTexture();
        void updateShader();

        void renderMesh();
        void renderTexture();
        void renderShader();

    protected:
        void fillShaderUniforms();
        void setCameraState(MaterialPtr mtl);
        void setSpaceState(MaterialPtr mtl);


        /* void uploadMaterial();
         void uploadCameraState();

     private:
         void setWorldPosition();*/

    private:
        class RenderInterface*              _renderer;
        stream_message::ActionType          _action_type;
    };
}

#endif // COMMON_UPLOADER_H


