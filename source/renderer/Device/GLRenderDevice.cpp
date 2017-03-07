#include "../3rdparty/glew/include/GL/glew.h"

#include "GLRenderDevice.h"
#include "RenderContext.h"
#include "renderer/Resource/RenderResourceContext.h"
#include "renderer/runtime/RenderCamera.h"

#include "math/Vector4.h"

namespace te
{
    static const char *defaultShaderVS =
        "uniform mat4 viewProjMat;\n"
        "uniform mat4 worldMat;\n"
        "attribute vec3 vertPos;\n"
        "void main() {\n"
        "	gl_Position = viewProjMat * worldMat * vec4( vertPos, 1.0 );\n"
        "}\n";

    static const char *defaultShaderFS =
        "uniform vec4 color;\n"
        "void main() {\n"
        "	gl_FragColor = color;\n"
        "}\n";

    GLRenderDevice::GLRenderDevice()
        : RenderDevice()
    {

    }

    GLRenderDevice::~GLRenderDevice()
    {

    }

    bool GLRenderDevice::open()
    {
        char* vendor = (char*)glGetString(GL_VENDOR);
        char* renderer = (char*)glGetString(GL_RENDERER);
        char* version = (char*)glGetString(GL_VERSION);

        bool bOK = true;
        bOK = bOK && createDefaultShader(getDefaultVSCode(), getDefaultFSCode(), _defaultShader);

        return bOK;
    }

    void GLRenderDevice::close()
    {

    }

    void GLRenderDevice::dispatch(RenderContext * context_)
    {
        // do actual command
        // in stingray, this is done by XXXRenderContext, which is an internal
        // class within XXXRenderDevice and has nothing to do with RenderContext
        // XXXRenderDevice may hold an immediate_context. For deferred rendering
        // there might be other context, need investigation

        // For current dev stage, we simply let the device do the actual command
        for (RenderContext::Command& command : context_->commands())
        {
            if (RenderContext::CommandType::UPDATE_INDEX_BUFFER == command.command_type)
            {
                RenderContext::IndexCmdStream* c_stream = static_cast<RenderContext::IndexCmdStream*>(command.head);
                //_newIndexBuf = c_stream->bufHandle;
                //_indexFormat = c_stream->idxFormat;
                delete c_stream;
            }
            else if (RenderContext::CommandType::UPDATE_VERTEX_BUFFER == command.command_type)
            {
                RenderContext::VertexCmdStream* c_stream = static_cast<RenderContext::VertexCmdStream*>(command.head);
                // each shader has an array of a structure InputLayouts { bool valid; int8 attribIndices[16]; }
                // it's related to how the vertex buffer is organized in OpenGL
                _newVAO = c_stream->vaoHandle;
                _curBaseIndex = c_stream->baseIndex;
                _curBaseVertex = c_stream->baseVertex;
                _curNumIndices = c_stream->numIndices;
                delete c_stream;
            }
            else if (RenderContext::CommandType::BIND_SHADER_OBJECT == command.command_type)
            {
                RenderCamera* curCamera = context_->_camera;
                RenderContext::ShaderCmdStream* c_stream = static_cast<RenderContext::ShaderCmdStream*>(command.head);
                if (c_stream)
                {
                    // TODO
                    // use setShaderConst()
                    // header of all uniform data is in ShaderCmdStream::data
                    // ShaderCmdStream::variables give information for how to read
                }
                else
                {
                    // debug mode, use default shader
                    if (_curShaderHandle != _defaultShader.shader_handle)
                        bindShader(_defaultShader.shader_handle);

                    // set view params
                    if (_defaultShader.uni_view_mat >= 0)
                        setShaderConst(_defaultShader.uni_view_mat, shader_data::MATRIX4X4, &curCamera->getViewMat());
                    if (_defaultShader.uni_proj_mat >= 0)
                        setShaderConst(_defaultShader.uni_proj_mat, shader_data::MATRIX4X4, &curCamera->getProjectionMat());
                    if (_defaultShader.uni_view_proj_mat >= 0)
                        setShaderConst(_defaultShader.uni_view_proj_mat, shader_data::MATRIX4X4, &curCamera->getViewProjctionMat());

                    // set default color
                    float color[4] = { 0.75f, 0.5, 0.25f, 1 };
                    setShaderConst(_defaultShader.custom_uniform_handles["color"], shader_data::VECTOR4, &color);
                }
            }
            else if (RenderContext::CommandType::RENDER == command.command_type)
            {
                draw();
            }
        }

        context_->commands().clear();
    }

    void GLRenderDevice::dispatch(RenderResourceContext * context_)
    {
        for (RenderResourceContext::Message& msg : context_->messages())
        {
            if (RenderResourceContext::MessageType::ALLOC_INDEX_BUFFER == msg.type)
            {
                // default type unsigned int
                RenderResourceContext::IndexStream* i_stream
                    = static_cast<RenderResourceContext::IndexStream*>(msg.head);
                RenderResource* res = i_stream->res;
                TE_ASSERT(RenderResource::INDEX_STREAM == res->type, "Render Resource Type doesn't match!");
                res->render_resource_handle = createIndexBuffer(i_stream->size, i_stream->raw_data);
            }
            else if (RenderResourceContext::MessageType::ALLOC_VERTEX_BUFFER == msg.type)
            {
                // default type float
                RenderResourceContext::VertexStream* v_stream
                    = static_cast<RenderResourceContext::VertexStream*>(msg.head);
                RenderResource* res = v_stream->res;
                TE_ASSERT(RenderResource::VERTEX_STREAM == res->type, "Render Resource Type doesn't match!");
                res->render_resource_handle = createVertexBuffer(v_stream->size, v_stream->stride, v_stream->raw_data);
            }
            else if (RenderResourceContext::MessageType::ALLOC_VERTEX_DECLARATION == msg.type)
            {
                RenderResourceContext::VertexDeclarationStream* vd_stream
                    = static_cast<RenderResourceContext::VertexDeclarationStream*>(msg.head);
                RenderResource* res = vd_stream->res;
                TE_ASSERT(RenderResource::VERTEX_DECLARATION == res->type, "Render Resource Type doesn't match!");

                const VertexLayout& vl = _vertexDeclaration.getLayout(vd_stream->layout_type);
                TE_ASSERT(vl.size() <= vd_stream->vertex_buffers.size(),
                    "Num of Vertex Layout Attributes should no larger than Num of Vertex Buffers!");

                std::vector<uint32> locations;
                for (auto& attri : vl)
                {
                    locations.push_back(attri.vbSlot);
                }
                std::vector<uint32> vertexHandles;
                for (auto buffer : vd_stream->vertex_buffers)
                {
                    vertexHandles.push_back(buffer->render_resource_handle);
                }
                uint32 indexHandle = vd_stream->index_buffer->render_resource_handle;
                createVertexArray(locations.size(), &locations[0], &vertexHandles[0], indexHandle);
            }
        }
    }

    uint32 GLRenderDevice::createVertexBuffer(uint32 size, uint32 stride, const void * data)
    {
        GLBuffer buf;
        buf.type = GL_ARRAY_BUFFER;
        buf.size = size;
        buf.stride = stride;
        glGenBuffers(1, &buf.glObj);
        glBindBuffer(buf.type, buf.glObj);
        glBufferData(buf.type, size, data, GL_DYNAMIC_DRAW);
        glBindBuffer(buf.type, 0);

        return _buffers.add(buf);
    }

    uint32 GLRenderDevice::createIndexBuffer(uint32 size, const void * data)
    {
        GLBuffer buf;

        buf.type = GL_ELEMENT_ARRAY_BUFFER;
        buf.size = size;
        buf.stride = 1; // if we use VAO, index buffer seems not important
        glGenBuffers(1, &buf.glObj);
        glBindBuffer(buf.type, buf.glObj);
        glBufferData(buf.type, size, data, GL_DYNAMIC_DRAW);
        glBindBuffer(buf.type, 0);

        return _buffers.add(buf);
    }

    te::uint32 GLRenderDevice::createVertexArray(uint32 nLoc, const uint32* locations, const uint32* vertexHandles, uint32 indexHandle)
    {
        uint32 vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        for (uint32 i = 0; i < nLoc; ++i)
        {
            GLBuffer& vBuf = _buffers.getRef(vertexHandles[i]);
            glBindBuffer(vBuf.type, vBuf.glObj);
            glVertexAttribPointer(locations[i], vBuf.stride, GL_FLOAT, GL_FALSE, 0, NULL);
        }

        GLBuffer& iBuf = _buffers.getRef(indexHandle);
        glBindBuffer(iBuf.type, iBuf.glObj); // bind index buffer to VAO

        glBindVertexArray(0);
        return _vaos.add(vao);
    }

    void GLRenderDevice::destroyBuffer(uint32 bufObj)
    {
        if (0 == bufObj) return;

        GLBuffer& buf = _buffers.getRef(bufObj);
        glDeleteBuffers(1, &buf.glObj);

        _buffers.remove(bufObj);
    }

    void GLRenderDevice::updateBufferData(uint32 bufObj, uint32 offset, uint32 size, void * data)
    {
        const GLBuffer& buf = _buffers.getRef(bufObj);
        TE_ASSERT(offset + size <= buf.size, "offset + size should be no larger than buf.size!");

        glBindBuffer(buf.type, buf.glObj);
        
        if (0 == offset && size == buf.size)
        {
            glBufferData(buf.type, size, data, GL_DYNAMIC_DRAW);
            return;
        }

        glBufferSubData(buf.type, offset, size, data);
    }

    const char* GLRenderDevice::getDefaultVSCode()
    {
        return defaultShaderVS;
    }

    const char* GLRenderDevice::getDefaultFSCode()
    {
        return defaultShaderFS;
    }

    te::uint32 GLRenderDevice::createShader(const char* vertexShaderSrc, const char* fragmentShaderSrc, vertex_layout::Type vlType)
    {
        uint32 programObj = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
        if (0 == programObj) return 0;
        if (!configShaderVertexLayout(programObj, vlType)) return 0;
        if (!linkShaderProgram(programObj)) return 0;

        uint32 shaderHandle = _shaders.add(GLShader());
        GLShader& shader = _shaders.getRef(shaderHandle);
        shader.oglProgramObj = programObj;

        return shaderHandle;
    }

    uint32 GLRenderDevice::createShaderProgram(const char * vertexShaderSrc, const char * fragmentShaderSrc)
    {
        int infologLength = 0;
        int charsWritten = 0;
        char *infoLog = 0x0;
        int status;

        // vertex shader
        uint32 vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShaderSrc, 0x0);
        glCompileShader(vs);
        glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            // Get info
            glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infologLength);
            if (infologLength > 1)
            {
                infoLog = new char[infologLength];
                glGetShaderInfoLog(vs, infologLength, &charsWritten, infoLog);
                _shaderLog = _shaderLog + "[Vertex Shader]\n" + infoLog;
                delete[] infoLog; infoLog = 0x0;
            }

            glDeleteShader(vs);
            return 0;
        }

        // Fragment shader
        uint32 fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShaderSrc, 0x0);
        glCompileShader(fs);
        glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infologLength);
            if (infologLength > 1)
            {
                infoLog = new char[infologLength];
                glGetShaderInfoLog(fs, infologLength, &charsWritten, infoLog);
                _shaderLog = _shaderLog + "[Fragment Shader]\n" + infoLog;
                delete[] infoLog; infoLog = 0x0;
            }

            glDeleteShader(vs);
            glDeleteShader(fs);
            return 0;
        }

        // Shader program
        uint32 program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    bool GLRenderDevice::configShaderVertexLayout(uint32 programObj, vertex_layout::Type vlType)
    {
        const VertexLayout& vl = _vertexDeclaration.getLayout(vlType);
        for (const VertexLayoutAttrib& attr : vl)
        {
            glBindAttribLocation(programObj, attr.vbSlot, attr.semanticName.c_str());
        }
        return true;
    }

    bool GLRenderDevice::linkShaderProgram(uint32 programObj)
    {
        int infologLength = 0;
        int charsWritten = 0;
        char *infoLog = 0x0;
        int status;

        _shaderLog = "";

        glLinkProgram(programObj);
        glGetProgramiv(programObj, GL_INFO_LOG_LENGTH, &infologLength);
        if (infologLength > 1)
        {
            infoLog = new char[infologLength];
            glGetProgramInfoLog(programObj, infologLength, &charsWritten, infoLog);
            _shaderLog = _shaderLog + "[Linking]\n" + infoLog;
            delete[] infoLog; infoLog = 0x0;
        }

        glGetProgramiv(programObj, GL_LINK_STATUS, &status);
        if (!status) return false;

        return true;
    }

    int GLRenderDevice::getShaderConstLoc(uint32 shaderHandle, const char* name)
    {
        GLShader& shader = _shaders.getRef(shaderHandle);
        return glGetUniformLocation(shader.oglProgramObj, name);
    }

    void GLRenderDevice::setShaderConst(int loc, shader_data::Class type, void * values, uint32 count)
    {
        switch (type)
        {
        case shader_data::SCALAR:
            glUniform1fv(loc, count, (float*)values);
            break;
        case shader_data::VECTOR2:
            glUniform2fv(loc, count, (float*)values);
            break;
        case shader_data::VECTOR3:
            glUniform3fv(loc, count, (float*)values);
            break;
        case shader_data::VECTOR4:
            glUniform4fv(loc, count, (float*)values);
            break;
        case shader_data::MATRIX4X4:
            glUniformMatrix4fv(loc, count, false, (float*)values);
            break;
        }
    }

    void GLRenderDevice::bindShader(uint32 shaderHandle)
    {
        if (0 != shaderHandle)
        {
            GLShader& shader = _shaders.getRef(shaderHandle);
            glUseProgram(shader.oglProgramObj);
        }
        else
        {
            glUseProgram(0);
        }

        _curShaderHandle = shaderHandle;
    }

    uint32 GLRenderDevice::createRenderBuffer(uint32 width, uint32 height, image_data::Format format, bool depth, uint32 numColBufs)
    {
        GLRenderTarget renderTarget;
        glGenFramebuffers(1, &renderTarget.glFbo);

        if (numColBufs > 0)
        {
            for (uint32 i = 0; i < numColBufs; ++i)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.glFbo);
                // create color texture
                uint32 texObj = createTexture(width, height, 1, image_data::IMAGE2D, image_data::RGBA32F, false);
                TE_ASSERT(texObj != 0, "Not a Valid Texture Object!");
                updateTextureData(texObj, 0, nullptr);
                // attach the texture to fbo
                GLTexture& tex = _textures.getRef(texObj);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex.glObj, 0);
            }
        }

        uint32 buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, renderTarget.glFbo);
        glDrawBuffers(numColBufs, buffers);

        if (depth)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.glFbo);
            // create a depth texture
            uint32 texObj = createTexture(width, height, 1, image_data::IMAGE2D, image_data::DEPTH, false);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
            updateTextureData(texObj, 0, nullptr);
            GLTexture& tex = _textures.getRef(texObj);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.glObj, 0);
        }

        // check if everything is OK
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderTarget.glFbo);
        GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        TE_ASSERT(e != GL_FRAMEBUFFER_COMPLETE, "There is a problem with the FBO");
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        return _renderTargets.add(renderTarget);
    }

    uint32 GLRenderDevice::createTexture(int width, int height, int depth, image_data::Type type, image_data::Format format, bool hasMips)
    {
        GLTexture tex;
        tex.format = format;
        tex.width = width;
        tex.height = height;
        tex.depth = depth;
        tex.hasMips = hasMips;

        switch (type)
        {
        case image_data::IMAGE2D:
            tex.glType = GL_TEXTURE_2D;
            break;
        case image_data::IMAGE3D:
            tex.glType = GL_TEXTURE_3D;
            break;
        case image_data::IMAGECUBE:
            tex.glType = GL_TEXTURE_CUBE_MAP;
            break;
        }

        glGenTextures(1, &tex.glObj);
        glBindTexture(tex.glType, tex.glObj);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        return _textures.add(tex);
    }

    void GLRenderDevice::updateTextureData(uint32 texObj, int mipLevel, const void* pixels)
    {
        const GLTexture tex = _textures.getRef(texObj);

        glBindTexture(tex.glType, tex.glObj);

        int inputFormat = GL_BGRA, inputType = GL_UNSIGNED_BYTE;
        switch (tex.format)
        {
        case image_data::RGBA16F:
            inputFormat = GL_RGBA;
            inputType = GL_FLOAT;
            break;
        case image_data::RGBA32F:
            inputFormat = GL_RGBA;
            inputType = GL_FLOAT;
            break;
        case image_data::DEPTH:
            inputFormat = GL_DEPTH_COMPONENT;
            inputType = GL_FLOAT;
        };
        glTexImage2D(tex.glType, mipLevel, inputFormat, tex.width, tex.height, 0, inputFormat, inputType, pixels);
        glBindTexture(tex.glType, 0);
    }

    void GLRenderDevice::commitGeneralUniforms()
    {

    }

    void GLRenderDevice::draw()
    {
        if (commitStates())
        {
            // base index is the start index of sub-component (3*numFace)
            // base vertex is the start vertex of sub-component (3*numVert)
            glDrawElementsBaseVertex(GL_TRIANGLES,
                _curNumIndices,
                GL_UNSIGNED_INT,
                (void*)(sizeof(unsigned int)*_curBaseIndex),
                _curBaseVertex);
        }
    }

    bool GLRenderDevice::commitStates()
    {
        _curVAO = _newVAO;
        _prevShaderHandle = _curShaderHandle;

        glBindVertexArray(_vaos.getRef(_curVAO));

        return false;
    }

    bool GLRenderDevice::createDefaultShader(const char* vertexShader, const char* fragmentShader, ShaderObject& so)
    {
        uint32 shaderHandle = createShader(vertexShader, fragmentShader, vertex_layout::Position);
        if (0 == shaderHandle) return false;

        so.shader_handle = shaderHandle;
        bindShader(shaderHandle);

        so.custom_uniform_handles["color"] = getShaderConstLoc(shaderHandle, "color");

        so.uni_view_mat = getShaderConstLoc(shaderHandle, "viewMat");
        so.uni_proj_mat = getShaderConstLoc(shaderHandle, "projMat");
        so.uni_view_proj_mat = getShaderConstLoc(shaderHandle, "viewProjMat");
        so.uni_view_proj_mat_inv = getShaderConstLoc(shaderHandle, "viewProjMatInv");

        return true;
    }
}