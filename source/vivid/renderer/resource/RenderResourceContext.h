#ifndef VIVID_RENDERER_RENDER_RESOURCE_CONTEXT_H
#define VIVID_RENDERER_RENDER_RESOURCE_CONTEXT_H

#include <vector>

#include "vivid/renderer/resource/ResourceStream.h"

namespace vivid
{
    class RenderResourceContext
    {
    public:
        RenderResourceContext();
        virtual ~RenderResourceContext();

        struct Message
        {
            resource_stream::MessageType type;
            void* head;
        };
        typedef std::vector<Message> Messages;

        Messages& messages() { return _messages; };
        const Messages& messages() const { return _messages; };

    private:
        Messages _messages;
    };
}

#endif
