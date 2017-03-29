#ifndef COMMON_RESOURCE_H
#define COMMON_RESOURCE_H

#include <unordered_map>
#include <bitset>
#include <array>
#include <set>

#include "common/Clone.h"
#include "base/RefPtr.hpp"
#include "base/String.h"
#include "common/ClassType.hpp"
#include "base/Singleton.hpp"

namespace te
{
    const int MAX_AMOUNT_OF_RES_MANANGER = 12;
}

namespace te
{
    enum class ResourceType : unsigned long
    {
        Undefined =         (0 << 8),
        SceneGraph =        (1 << 8),
        Mesh =              (2 << 8),
        Skeleton =          (3 << 8),
        Animation =         (4 << 8),
        Material =          (5 << 8),
        Code =              (6 << 8),
        Shader =            (7 << 8),
        Texture =           (8 << 8),
        ParticleEffect =    (9 << 8),
        Pipeline =         (10 << 8)
    };

    using ResourceHandle = unsigned long;

    struct ResourceDescriptor
    {
        ResourceDescriptor() {}
        ResourceDescriptor(const ResourceDescriptor& des) 
        {
            *this = des;
        }

        ResourceDescriptor(ResourceHandle handle, ResourceType type, const String& id)
        {
            this->handle = handle;
            this->type = type;
            this->id = id;
        }

        ResourceHandle  handle;
        ResourceType    type;
        String     id;
    };

    class Resource /*: public Object*/
    {
    public:
        Resource();
        //Resource(const Resource& resource, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);
        virtual ~Resource();

        //OBJECT_META_FUNCTION(Resource);

        virtual bool load(const String& res) = 0;
        virtual void unload() = 0;

        ResourceHandle  getResourceHandle() const { return _descriptor.handle; }
        ResourceType    getResourceType() const { return _descriptor.type; }
        String     getResourceId() const { return _descriptor.id; }
        void descriptor(const ResourceDescriptor& des);

    protected:
        ResourceDescriptor      _descriptor;
    };

    using ResourcePtr = RefPtr<Resource>;

    class ResourceManager /*: public Object*/
    {
    public:
        ResourceManager(ResourceType type);
//        ResourceManager(const ResourceManager& res_mgr, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);
        virtual ~ResourceManager();

//        OBJECT_META_FUNCTION(ResourceManager);
        virtual ResourceHandle create(const String& res) = 0; // using file path as unified id

        //ResourceType getType() const { return _type; }

//        virtual bool create(const String& res) = 0;
        
        void add(Resource* resource);
        void remove(ResourceHandle handle);
        bool has(Resource* resource);
        bool has(ResourceHandle handle);
        bool exist(const String& id);

    protected:
        ResourceHandle getNextLocalResHandle();
        ResourceHandle generateGlobalResHandle();
        ResourceDescriptor buildDescriptor(const String& id);

        using ResourceMap = std::unordered_map<ResourceHandle, Resource*>;
        using ExistingMap = std::unordered_map<String, ResourceHandle>;
        using FreeList = std::vector<ResourceHandle>;
    protected:
        ResourceMap     _resources;
        ExistingMap     _id_maps;
        ResourceType    _type;
        ResourceHandle  _next_handle;
        FreeList        _free_list;
    };

    class ResourceMapper : public Singleton<ResourceMapper> /*: public Object*/
    {
    public:
        ResourceMapper();
        ////ResourceMap(const ResourceMap& res_map, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);
        //virtual ~ResourceMapper();

        //OBJECT_META_FUNCTION(ResourceMap);

        void initialize();

        template <typename T, typename ... Args>
        T* add(Args&& ... args);

        template <typename T>
        void remove();

        template <typename T>
        bool has();

        template <typename T>
        T* get();

    protected:
        ResourceManager*    getResManager(int manager_id);
        ResourceManager*    addResManager(ResourceManager* mgr, int manager_id);
        void                removeResManager(int manager_id);
        bool                hasResManager(int manager_id);

    protected:
        using ResourceManagerMap = std::array<ResourceManager*, MAX_AMOUNT_OF_RES_MANANGER>;
        using ManagerTypeList = std::bitset<MAX_AMOUNT_OF_RES_MANANGER>;

        ResourceManagerMap      _mgr_map;
        ManagerTypeList         _mgr_types;
    };

    template <typename T>
    T* ResourceMapper::get()
    {
        static_assert(std::is_base_of<ResourceManager, T>(), "T is not a resource manager, cannot retrieve T");
        return static_cast<T*>(getResManager(getResManagerTypeId<T>()));
    }

    template <typename T, typename ... Args>
    T* ResourceMapper::add(Args&& ... args)
    {
        static_assert(std::is_base_of<ResourceManager, T>(), "T is not a resource manager, cannot add T to resource mapper");
        auto manager = new T{ std::forward<Args>(args)... };
        addResManager(manager, getResManagerTypeId<T>());
        return manager;
    }

    template <typename T>
    void ResourceMapper::remove()
    {
        static_assert(std::is_base_of<ResourceManager, T>(), "T is not a resource manager, cannot remove T from resource mapper");
        removeResManager(getResManagerTypeId<T>());
    }

    template <typename T>
    bool ResourceMapper::has()
    {
        static_assert(std::is_base_of<ResourceManager, T>(), "T is not a resource manager, cannot determine if resource mapper has T");
        return hasResManager(getResManagerTypeId<T>());
    }
}

#endif // COMMON_RESOURCE_H