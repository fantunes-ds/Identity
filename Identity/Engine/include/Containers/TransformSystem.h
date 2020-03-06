#pragma once
#include <Export.h>
#include <Components/Transform.h>
#include <Systems/ISystem.h>

namespace Engine::Containers
{
    class API_ENGINE TransformSystem : public Systems::ISystem
    {
    public:
        ~TransformSystem();
        TransformSystem(const TransformSystem&) = delete;
        TransformSystem(const TransformSystem&&) = delete;

        static TransformSystem* GetInstance();


        void Update(float p_deltaTime) override;
        // static void Update(float p_deltaTime);

        static int32_t AddTransform();
        static int32_t AddTransform(std::shared_ptr<Components::Transform> p_transform);

        static std::shared_ptr<Components::Transform> FindTransform(uint32_t p_id);
        static std::map<int32_t, std::shared_ptr<Components::Transform>>& GetAllTransforms() { return GetInstance()->m_transforms; }
        static std::vector<std::shared_ptr<Components::Transform>> GetTransforms();
        static std::shared_ptr<Components::Transform> GetTransform(uint32_t p_id);

    private:
        TransformSystem() = default;
        inline static TransformSystem* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<Components::Transform>> m_transforms;
    };
}
