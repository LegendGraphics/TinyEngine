#include "vivid/common/Material.h"
#include "vivid/common/Shader.h"
#include "vivid/io/ResourceLoader.h"
#include "vivid/io/Logger.h"

namespace vivid
{
    bool Material::load(const String & res)
    {
        return ResourceLoader::load(this, res);
    }

    void Material::unload()
    {

    }

    ShaderPtr  Material::getShader()
    {
        return ResourceMapper::getInstance()->get<ShaderManager>()->getShader(_shader);
    }

    std::vector<TexturePtr> Material::getTextures()
    {
        std::vector<TexturePtr> textures;
        for (auto& sampler : _samplers)
        {
            textures.push_back(
                ResourceMapper::getInstance()->get<TextureManager>()->getTexture(sampler.second));
        }
        return textures;
    }

    void Material::setShaderUniforms()
    {
        ShaderUniforms* shader_uniforms = &getShader()->uniforms;
        for (auto& uniform : _uniform_map)
        {
            shader_uniforms->setUniform(uniform.first, &uniform.second.data[0], uniform.second.size, uniform.second.type);
        }
    }

    float Material::getFloat(const String& name)
    {
        if (getUniformType(name) == shader_data::SCALAR)
            return *getUniformValue(name);
        else
        {
            cLog << StringUtils::format("Uniform %s is not a float", name);
            return 0;
        }
    }

    Vector3 Material::getVector3(const String& name)
    {
        if (getUniformType(name) == shader_data::VECTOR3)
        {
            float* value = getUniformValue(name);
            return Vector3(value[0], value[1], value[2]);
        }
        else
        {
            cLog << StringUtils::format("Uniform %s is not a vector3", name);
            return Vector3();
        }
    }

    Matrix Material::getMatrix(const String& name)
    {
        if (getUniformType(name) == shader_data::MATRIX4X4)
        {
            float* value = getUniformValue(name);
            return Matrix(value[0], value[1], value[2], value[3],
                value[4], value[5], value[6], value[7],
                value[8], value[9], value[10], value[11],
                value[12], value[13], value[14], value[15]);
        }
        else
        {
            cLog << StringUtils::format("Uniform %s is not a Matrix", name);
            return Matrix();
        }
    }

    // get texture unit from shader
    void Material::setTexture(const String& name, ResourceHandle texture)
    {
        ShaderPtr shader = ResourceMapper::getInstance()->get<ShaderManager>()->getShader(_shader);
        if (shader->samplers.hasSampler(name))
        {
            auto& samplers = shader->samplers.getSamplers();
            TexturePtr tex_ptr = ResourceMapper::getInstance()->get<TextureManager>()->getTexture(texture);
            tex_ptr->setTexUnit(samplers[name].tex_unit);
            _samplers.insert({ name, texture });
        }
    }


    // get uniform type from shader
    void Material::setUniform(const String& name, float a, float b, float c, float d)
    {
        ShaderPtr shader = ResourceMapper::getInstance()->get<ShaderManager>()->getShader(_shader);
        if (shader->uniforms.hasUniform(name))
        {
            auto& uniforms = shader->uniforms.getUniforms();
            if (uniforms[name].value.type == shader_data::SCALAR)
                setFloat(name, a);
            else if (uniforms[name].value.type == shader_data::VECTOR2)
                setVector2(name, a, b);
            else if (uniforms[name].value.type == shader_data::VECTOR3)
                setVector3(name, a, b, c);
            else if (uniforms[name].value.type == shader_data::VECTOR4)
                setVector4(name, a, b, c, d);
        }
    }


    void Material::setFloat(const String& name, float value)
    {
        setUniform(name, &value, 1, shader_data::SCALAR);
    }

    void Material::setVector2(const String& name, float a, float b)
    {
        setVector2(name, Vector2(a, b));
    }

    void Material::setVector2(const String& name, const Vector2& value)
    {
        setUniform(name, &value.x, 2, shader_data::VECTOR2);
    }

    void Material::setVector3(const String& name, float a, float b, float c)
    {
        setVector3(name, Vector3(a, b, c));
    }

    void Material::setVector3(const String& name, const Vector3& value)
    {
        setUniform(name, &value.x, 3, shader_data::VECTOR3);
    }

    void Material::setVector4(const String& name, float a, float b, float c, float d)
    {
        setVector4(name, Vector4(a, b, c, d));
    }

    void Material::setVector4(const String& name, const Vector4& value)
    {
        setUniform(name, &value.x, 4, shader_data::VECTOR4);
    }

    void Material::setMatrix(const String& name, const Matrix& value)
    {
        setUniform(name, value.ptr(), 4 * 4, shader_data::MATRIX4X4);
    }

    bool Material::hasUniform(const String& name)
    {
        if (_uniform_map.find(name) != _uniform_map.end()) return true;
        else return false;
    }

    void Material::setUniform(const String& name, const float* value, int size, shader_data::UniformType type)
    {
        if (hasUniform(name))
        {
            ShaderUniformValue& uniform = _uniform_map[name];
            uniform.type = type;
            uniform.size = size;
            uniform.data.assign(value, value + size);
        }
        else
        {
            ShaderUniformValue uniform;
            uniform.type = type;
            uniform.size = size;
            uniform.data.assign(value, value + size);
            _uniform_map.insert({ name, uniform });
        }
    }

    

    float* Material::getUniformValue(const String& name)
    {
        if (hasUniform(name) && _uniform_map[name].size) return &_uniform_map[name].data[0];
        else return nullptr;
    }

    int Material::getUniformSize(const String& name)
    {
        if (hasUniform(name)) return _uniform_map[name].size;
        else return 0;
    }

    shader_data::UniformType Material::getUniformType(const String& name)
    {
        if (hasUniform(name)) return _uniform_map[name].type;
        else return shader_data::UNKNOWN_CLASS;
    }


    MaterialManager::MaterialManager()
        :ResourceManager(ResourceType::Material)
    {}

    MaterialManager::~MaterialManager() {}

    ResourceHandle MaterialManager::create(const String& res)
    {
        if (ResourceHandle handle = getResourceHandle(res)) return handle;
        else
        {
            MaterialPtr mat = new Material;
            if (mat->load(res))
            {
                mat->descriptor(buildDescriptor(res));
                add(mat.get());
                return getResourceHandle(res);
            }
            else return 0;
        }
    }

    MaterialPtr MaterialManager::getMaterial(ResourceHandle handle)
    {
        if (has(handle)) return dynamic_cast_ptr<Resource, Material>(getResourcePtr(handle));
        else return nullptr;
    }

    MaterialPtr MaterialManager::getMaterial(const String& res)
    {
        return getMaterial(getResourceHandle(res));
    }
}