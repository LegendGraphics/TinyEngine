#ifndef COMMON_COMPONENT_H
#define COMMON_COMPONENT_H

#include <string>
#include <array>
#include <bitset>

#include "base/String.h"
#include "common/Clone.h"


namespace te
{
    class Node;

    enum class ComponentType
    {
        UNDEFINED,
        SPACE_STATE,
        CAMERA_STATE,
        MESH_RENDER,
        LOGIC_BEHAVIOR
    };

    class Component: public Cloneable
    {
    public:
        enum class MetaType
        {
            DATA,
            BEHAVIOR
        };

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

        inline MetaType getMetaType() const { return _meta_type; }

        inline ComponentType getType() const { return _type; }

        inline NodeType getOwnerType() const { return _owner->getNodeType(); }

        template <typename C>
        bool hasComponent();

        template <typename C>
        C* getComponent();

    protected:
        String              _name;
        bool                _enabled;
        Node*               _owner;
        MetaType            _meta_type;
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

    class Behavior : public Component
    {
    public:
        Behavior();
        virtual ~Behavior();
        virtual void init();
        virtual void update();
    };

}


#endif // COMMON_COMPONENT_H