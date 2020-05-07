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
        IComponent(Objects::GameObject* p_gameObject);

        virtual bool operator==(IComponent* p_other) = 0;
        virtual bool DeleteFromMemory() = 0;
        virtual void SetActive(bool p_active) = 0;
        virtual void Serialize(std::ostream& p_stream) = 0;
        virtual void Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block) = 0;

        void SetGameObject(Objects::GameObject* p_gameObject);
        [[nodiscard]] Objects::GameObject* GetGameObject() const;

        virtual bool IsActive() { return m_isActive; }

    protected:
        Objects::GameObject* m_gameObject;
        bool m_isActive = true;
    };
}
