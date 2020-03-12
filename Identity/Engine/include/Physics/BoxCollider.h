#pragma once
#include <Export.h>
#include <Components/IComponent.h>

namespace Engine::Components
{
    class API_ENGINE BoxCollider: public IComponent
    {
    public:
        BoxCollider(Objects::GameObject* p_gameObject);
        ~BoxCollider();

    private:

    };
}
