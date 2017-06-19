#include "vivid/common/Component.h"

namespace vivid
{
    Component::Component()
        :_enabled(true),
        _owner(nullptr),
        _type(ComponentType::UNDEFINED)
    {

    }

    Component::Component(ComponentType type)
        :_enabled(true),
        _owner(nullptr),
        _type(type)
    {}

    Component::Component(const Component& component, const CopyOperator& copyop)
    {
        copyop(&component);
    }

    Component::~Component() {}


    void Component::setEnabled(bool enabled)
    {
        _enabled = enabled;
    }

    void Component::setName(const String& name)
    {
        _name = name;
    }

    void Component::setOwner(Node* owner)
    {
        _owner = owner;
    }

    ComData::ComData(ComponentType type)
        :Component(type)
    {}

    ComData::ComData(const ComData& Data, const CopyOperator& copyop)
    {
        copyop(&Data);
    }

    Behavior::Behavior(ComponentType type)
        :Component(type)
    {}

    Behavior::Behavior(const Behavior& behavior, const CopyOperator& copyop)
    {
        copyop(&behavior);
    }


    void Behavior::init()
    {}

    void Behavior::update()
    {}

    Render::Render(ComponentType type)
        :Component(type)
    {}

    Render::Render(const Render& render, const CopyOperator& copyop)
    {
        copyop(&render);
    }

    void Render::init()
    {}

    void Render::update()
    {}
}







