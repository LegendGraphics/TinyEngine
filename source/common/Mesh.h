#ifndef COMMON_MESH_H
#define COMMON_MESH_H

#include "common/Object.h"
#include "math/Vector4.h"
#include "math/Vector3.h"
#include "math/Vector2.h"
#include "shape/BoundingBox.h"
#include "base/RefPtr.hpp"
#include "common/Resource.h"

#include "renderer/Resource/RenderResource.h"

namespace te
{

// I use a top-down method of using polymorphism + virtual function tricks to code the run-time status of the different vertex type.
// Another way to do so is a bottom-up method by encapsulating VertexBuffer from memory structure to upper layer structure.
    enum class VertexAttribute
    {
        VERTEX_POSITION,
        VERTEX_NORMAL,
        VERTEX_TANGENT,
        VERTEX_BITANGENT,
        VERTEX_SKINNED,
        VERTEX_TEXCOORD0,
        VERTEX_TEXCOORD1
    };

    enum class VertexType
    {
        VERTEX_P,
        VERTEX_PN,
        VERTEX_PNTB,
        VERTEX_PNTB_SKINNED
    };

    struct Vertex_P
    {
        Vector3 position;
    };

    struct Vertex_PN
    {
        Vector3 position;
        Vector3 normal;
    };

    struct Vertex_PNTB
    {
        Vector3 position;
        Vector3 normal;
        Vector3 tangent;
        Vector3 bitangent;
    };

    struct Vertex_PNTB_Skinned
    {
        using JointIndices = int[4];
        using JointWeights = float[4];

        Vector3 position;
        Vector3 normal;
        Vector3 tangent;
        Vector3 bitangent;
        JointIndices joint_indices;
        JointWeights joint_weights;
    };

    using Vertex_P_Array = std::vector<Vertex_P>;
    using Vertex_PN_Array = std::vector<Vertex_PN>;
    using Vertex_PNTB_Array = std::vector<Vertex_PNTB>;
    using Vertex_PNTB_Skinned_Array = std::vector<Vertex_PNTB_Skinned>;

    struct VertexArray
    {
    private:
        class ArrayType;
        class P_Array;
        class PN_Array;
        class PNTB_Array;
        class PNTB_Skinned_Array;


        void*           _vertex_buffer;
        ArrayType*      _type;
        size_t          _size;

    private:
        class ArrayType
        {
        public:
            ArrayType() {}
            virtual ~ArrayType() {}

            virtual void initialize(size_t size, void*& array) {}
            virtual Vector3& position(size_t index, void*& array) { assert("No Position Attribute!" && false); return Vector3(); }
            virtual Vector3& normal(size_t index, void*& array) { assert("No Normal Attribute!" && false); return Vector3(); }
            virtual Vector3& tangent(size_t index, void*& array) { assert("No tangent Attribute!" && false); return Vector3(); }
            virtual Vector3& bitangent(size_t index, void*& array) { assert("No bitangent Attribute!" && false); return Vector3(); }
        };

        class P_Array : public ArrayType
        {
        public:
            P_Array() {}
            virtual ~P_Array() {}

            virtual void initialize(size_t size, void*& array) { array = new Vertex_P_Array(size); }
            virtual Vector3& position(size_t index, void*& array) { return (*((Vertex_P_Array*)(array)))[index].position; }
        };

        class PN_Array : public ArrayType
        {
        public:
            PN_Array() {}
            virtual ~PN_Array() {}

            virtual void initialize(size_t size, void*& array) { array = new Vertex_PN_Array(size); }
            virtual Vector3& position(size_t index, void*& array) { return (*((Vertex_PN_Array*)(array)))[index].position; }
            virtual Vector3& normal(size_t index, void*& array) { return (*((Vertex_PN_Array*)(array)))[index].normal; }
        };

        class PNTB_Array : public ArrayType
        {
        public:
            PNTB_Array() {}
            virtual ~PNTB_Array() {}

            virtual void initialize(size_t size, void*& array) { array = new Vertex_PNTB_Array(size); }
            virtual Vector3& position(size_t index, void*& array) { return (*((Vertex_PNTB_Array*)(array)))[index].position; }
            virtual Vector3& normal(size_t index, void*& array) { return (*((Vertex_PNTB_Array*)(array)))[index].normal; }
            virtual Vector3& tangent(size_t index, void*& array) { return (*((Vertex_PNTB_Array*)(array)))[index].tangent; }
            virtual Vector3& bitangent(size_t index, void*& array) { return (*((Vertex_PNTB_Array*)(array)))[index].bitangent; }
        };

        class PNTB_Skinned_Array : public ArrayType
        {
        public:
            PNTB_Skinned_Array() {}
            virtual ~PNTB_Skinned_Array() {}

            virtual void initialize(size_t size, void*& array) { array = new Vertex_PNTB_Skinned_Array(size); }
            virtual Vector3& position(size_t index, void*& array) { return (*((Vertex_PNTB_Skinned_Array*)(array)))[index].position; }
            virtual Vector3& normal(size_t index, void*& array) { return (*((Vertex_PNTB_Skinned_Array*)(array)))[index].normal; }
            virtual Vector3& tangent(size_t index, void*& array) { return (*((Vertex_PNTB_Skinned_Array*)(array)))[index].tangent; }
            virtual Vector3& bitangent(size_t index, void*& array) { return (*((Vertex_PNTB_Skinned_Array*)(array)))[index].bitangent; }
        };

    public:
        VertexArray()
            :_type(new ArrayType)
        {
        }
        ~VertexArray()
        {
            delete _type;
        }

        void convert(VertexType type)
        {
            if (_type) delete _type;
            switch (type)
            {
            case VertexType::VERTEX_P:
                _type = new P_Array;
                break;
            case VertexType::VERTEX_PN:
                _type = new PN_Array;
                break;
            case VertexType::VERTEX_PNTB:
                _type = new PNTB_Array;
                break;
            case VertexType::VERTEX_PNTB_SKINNED:
                _type = new PNTB_Skinned_Array;
                break;
            default:
                _type = new ArrayType;
                break;
            }
        }

        void* buffer() { 
            Vertex_PNTB_Array* a = (Vertex_PNTB_Array*)_vertex_buffer;
            return &(*a)[0].position.x;
           // return aa;
            /*float* aaa = (float*)aa;
            return aaa;
            for (int i = 0; i < 100; ++i) std::cout << aaa[i] << " ";
            return _vertex_buffer; */
        }

        size_t size() { return _size; }

        void initialize(size_t size)
        {
            _type->initialize(size, _vertex_buffer);
            _size = size;
        }

        Vector3& position(size_t index)
        {
            return _type->position(index, _vertex_buffer);
        }

        Vector3& normal(size_t index)
        {
            return _type->normal(index, _vertex_buffer);
        }

        Vector3& tangent(size_t index)
        {
            return _type->tangent(index, _vertex_buffer);
        }

        Vector3& bitangent(size_t index)
        {
            return _type->bitangent(index, _vertex_buffer);
        }
    };

    using IndexArray = std::vector<uint32>;

    /*class MeshRes : public Resource
    {
    public:
        MeshRes();
        bool load(const std::string & res);
        void release();

        friend class Mesh;

    protected:
        bool _skinned;
        std::vector<Vertex_PNTB> _vertices;
        std::vector<int>    _triangles;
    };*/

    //template <typename T>
    class Mesh: public Resource
    {
    /*public:
        using VertexArray = std::vector<T>;
        using IndexArray = std::vector<uint32>;
        using RawArray = std::vector<float>;*/

    public:
        Mesh();
   //     Mesh(const Mesh& mesh, const CopyOperator& copypo = CopyOperator::SHALLOW_COPY);
        virtual ~Mesh() = default;

   //     OBJECT_META_FUNCTION(Mesh);

        friend class ResourceLoader;

        bool load(const std::string & res);
        void unload();

        bool isSkinned() const { return _skinned; }
        //VertexArray& getVertices() { return _vertices; }
        IndexArray& getTriangles() { return _triangles; }
        std::vector<float>& getVertices() { return _testVertices; }
        RenderResource& getVertexDeclaration() { return _vertex_declaration; }
        RenderResource& getIndexBuffer() { return _index_buffer; }
        std::vector<RenderResource>& getVertexBuffers() { return _vertex_buffers; }

    protected:
        VertexArray             _vertices;
        IndexArray              _triangles;
        std::vector<float>     _testVertices;

        RefPtr<BoundingBox> _bounding;

        bool _skinned;

        std::vector<RenderResource> _vertex_buffers;
        RenderResource              _index_buffer;
        RenderResource              _vertex_declaration;
    };

    class MeshManager : public ResourceManager
    {
    public:
        MeshManager();
        ~MeshManager();

        bool create(const std::string& res);
    };
}



#endif // COMMON_MESH_H