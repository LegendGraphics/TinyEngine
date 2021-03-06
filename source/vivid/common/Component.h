#ifndef VIVID_COMMON_COMPONENT_H
#define VIVID_COMMON_COMPONENT_H

#include <string>
#include <array>
#include <bitset>
#include <unordered_map>
#include <functional>

#include "vivid/base/String.h"
#include "vivid/base/Singleton.hpp"
#include "vivid/common/Clone.h"


namespace vivid
{
    class Node;

    // In the future, we need to build a dynamic component system, which could 
    // register custom component type
    enum class ComponentType
    {
        UNDEFINED,
        SPACE_STATE,
        CAMERA_STATE,
        MESH_RENDER,
        LOGIC_BEHAVIOR,
        UPLOAD_TO_RENDER
    };


    class ComponentRegister: public Singleton<ComponentRegister>
    {
    private:
        using ComponentCreator = std::function<Component*(const String& res)>;
        using ComponentMap = std::unordered_map<String, std::tuple<ComponentCreator, int>>;

    public:
        bool registerCom(const String& name, ComponentCreator creator, int id);
        void unregisterCom(const String& name);

        bool hasRegistered(const String& name);
        Component* createComponent(const String& name, const String& res, int id);
    private:
        ComponentMap    _cmap;
    };

    class Component: public Cloneable
    {
    public:
        Component();
        Component(ComponentType type);
        Component(const Component& component, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);
        virtual ~Component();

        ENABLE_CLONE(Component);

        inline bool isEnabled() const { return _enabled; }
        void setEnabled(bool enabled);

        inline const String& getName() const { return _name; }
        void setName(const String& name);

        inline Node* getOwner() const { return _owner; }
        void setOwner(Node* owner);

        inline ComponentType getType() const { return _type; }

        template <typename C>
        bool hasComponent();

        template <typename C>
        C* getComponent();

        virtual int getTypeId() { return -1; };


    protected:
        String              _name;
        bool                _enabled;
        Node*               _owner;
        ComponentType       _type;
    };


    template <typename C>
    bool Component::hasComponent()
    {
        if (_owner) return _owner->hasComponent<C>();
        else return false;
    }

    template <typename C>
    C* Component::getComponent()
    {
        if (_owner) return _owner->getComponent<C>();
        else return nullptr;
    }

    class ComData : public Component
    {
    public:
        ComData() = default;
        ComData(ComponentType type);
        ComData(const ComData& data, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);
        virtual ~ComData() = default;

        ENABLE_CLONE(ComData);

        virtual int getTypeId() { return -1; };


    };

    class Behavior : public Component
    {
    public:
        Behavior() = default;
        Behavior(ComponentType type);
        Behavior(const Behavior& behavior, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);
        virtual ~Behavior() = default;

        ENABLE_CLONE(Behavior);

        virtual int getTypeId() { return -1; };


        virtual void init();
        virtual void update();
    };

    class Render : public Component
    {
    public:
        Render() = default;
        Render(ComponentType type);
        Render(const Render& render, const CopyOperator& copyop = CopyOperator::SHALLOW_COPY);
        virtual ~Render() = default;

        ENABLE_CLONE(Render);

        virtual int getTypeId() { return -1; };

        virtual void init();
        virtual void update();
    };

}


#endif // VIVID_COMMON_COMPONENT_H
