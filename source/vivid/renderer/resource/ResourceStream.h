#ifndef VIVID_RENDERER_RESOURCE_STREAM_H
#define VIVID_RENDERER_RESOURCE_STREAM_H

#include "vivid/base/Types.h"

#include "vivid/renderer/resource/ShaderUniform.h"
#include "vivid/renderer/resource/VertexLayoutType.h"
#include "vivid/renderer/resource/Image.h"

namespace vivid
{
    namespace resource_stream
    {
        enum class MessageType : uint8
        {
            ALLOC_VERTEX_BUFFER, DEALLOC_VERTEX_BUFFER,
            ALLOC_VERTEX_DECLARATVIVID_ION, DEALLOC_VERTEX_DECLARATVIVID_ION,
            ALLOC_INDEX_BUFFER, DEALLOC_INDEX_BUFFER,
            ALLOC_SHADER, DEALLOC_SHADER,
            ALLOC_RENDER_TARGET, DEALLOC_RENDER_TARGET,
            ALLOC_TEXTURE, DEALLOC_TEXTURE, REALLOC_TEXTURE,
        };

        struct IndexStream
        {
            uint32 size; // size in bytes
            void* raw_data;
            GPUHandle* res;
        };

        struct VertexStream
        {
            uint32 stride; // number of component for each element in a vertex array, in bytes
                           // for example an array for position has element {XYZ} and its stride is 3
            uint32 size; // size in bytes
            void* raw_data;
            GPUHandle* res;
        };

        struct VertexDeclarationStream
        {
            vertex_layout::Type layout_type;
            std::vector<GPUHandle*> vertex_buffers; // notice: vertex_buffers might be duplicated here
                                                    // because multiple attributes may share one vertex buffer
            GPUHandle*              index_buffer;
            GPUHandle*              res;
        };

        struct ShaderStream
        {
            GPUHandle*          res;
            String              vs;
            String              fs;
            ShaderUniforms*     uniforms;
            ShaderSamplers*     samplers;
            vertex_layout::Type layout_type;
        };

        struct TextureStream
        {
            GPUHandle*          res;
            void*               raw_data;
            uint32              width;
            uint32              height;
            uint32              depth;
            image_data::Type    type;
            image_data::Format  format;
            bool                has_mips;
        };
    }
}

#endif
