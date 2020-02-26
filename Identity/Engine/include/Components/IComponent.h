#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::Objects
{
    class GameObject;
}

namespace Engine::Components
{
    class API_ENGINE IComponent : public Objects::IObject
    {
    public:
        IComponent() = default;

        virtual bool operator==(IComponent* p_other) = 0;
        virtual bool DeleteFromMemory() = 0;
        void SetGameObject(Objects::GameObject* p_gameObject) { m_gameObject = p_gameObject; }
        std::shared_ptr<Objects::GameObject> GetGameObject() const { return std::shared_ptr<Objects::GameObject>(m_gameObject); }

    protected:
        Objects::GameObject* m_gameObject;
    };
}
