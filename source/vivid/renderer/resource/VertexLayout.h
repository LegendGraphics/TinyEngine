#ifndef VIVID_RENDERER_VERTEX_LAYOUT_H
#define VIVID_RENDERER_VERTEX_LAYOUT_H
#include <unordered_map>
#include "vivid/base/Types.h"
#include "vivid/base/String.h"
#include "vivid/renderer/resource/VertexLayoutType.h"
#include "vivid/renderer/resource/RenderResource.h"

namespace vivid
{
    // Vertex Layout should be loaded from file like pipeline resource

    // this seems to be a common concept
    // in all platforms, put it here for now
    // Vertex Layout is to tell the GPU
    // how attributes of a vertex is
    // stored in a chunk of memory
    struct VertexLayoutAttrib
    {
        String       semantic_name;
        uint32       vb_slot;
        uint32       size; // number of component
        uint32       offset; // offset in bytes, for example, after a size 2 of float, offset will be 2 * (32 / 8) = 8
        //uint32       stride; // stride in bytes, for example, if two attributes XYZ and RGBA are stored together,
                             // the stride will be 7 * (32 / 8) = 28
                             // I decide to compute the stride when creating VAO
    };
    typedef std::vector<VertexLayoutAttrib> VertexLayout;

    struct VertexLayoutPredefinition : public RenderResource
    {
        std::unordered_map<vertex_layout::Type, VertexLayout> vertex_layout_map;

        void init();
        const VertexLayout& getLayout(vertex_layout::Type vl_type);

        VertexLayoutPredefinition() : RenderResource(OTHER) { init(); }
        VertexLayoutPredefinition(RenderResource::Type t) : RenderResource(t) { init(); }
    };
}

#endif
