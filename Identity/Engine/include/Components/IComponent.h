#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::Objects
{
    class GameObject;
}

namespace Engine::Components
{
    enum Type
    {
        UNSET = -1,
        TRANSFORM = 0,
        MODEL = 1,
        BOX_COLLIDER = 2,
        CAMERA = 3,
        LIGHT = 4,
        SOUND = 5,
        SPHERE_COLLIDER = 6
    };
    class API_ENGINE IComponent : public Objects::IObject
    {
    public:
        IComponent(Objects::GameObject* p_gameObject, const Type& p_type);

        virtual bool operator==(IComponent* p_other) = 0;
        virtual bool DeleteFromMemory() = 0;
        virtual void SetActive(bool p_active) = 0;
        virtual void Serialize(std::ostream& p_stream) = 0;
        virtual void Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block) = 0;

        virtual bool RemoveComponent() = 0;

        const Type GetType() const { return m_type; }
        void SetGameObject(Objects::GameObject* p_gameObject);
        [[nodiscard]] Objects::GameObject* GetGameObject() const;

        virtual bool IsActive() { return m_isActive; }

    protected:
        Objects::GameObject* m_gameObject;
        bool m_isActive = true;
        Type m_type = UNSET;
    };
}
