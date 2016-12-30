#ifndef RENDERER_RENDERRESOURCE_H
#define RENDERER_RENDERRESOURCE_H

#include "base/Globals.h"

namespace te
{
    struct RenderResource
    {
        enum 
        {
            PIPELINE,
            NOT_INITIALIZED = 0xFFFFFFFF
        };
        typedef uint32 Type;
        typedef uint32 Handle;

        RenderResource() : type(NOT_INITIALIZED), render_resource_handle(NOT_INITIALIZED) {};
        RenderResource(Type t) : type(t), render_resource_handle(NOT_INITIALIZED) {};
        RenderResource(Type t, Handle h) : type(t), render_resource_handle(h) {};
        virtual ~RenderResource() {};

        Type type;
        Handle render_resource_handle;
    };
}

#endif