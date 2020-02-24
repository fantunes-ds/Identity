#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Transform.h>

namespace Engine::Containers
{
    class API_ENGINE TransformContainer
    {
    public:
        ~TransformContainer();
        TransformContainer(const TransformContainer&) = delete;
        TransformContainer(const TransformContainer&&) = delete;

        static TransformContainer* GetInstance();

        static int32_t AddTransform();
        static int32_t AddTransform(ObjectElements::Transform& p_transform);

        static std::shared_ptr<ObjectElements::Transform> FindTransform(uint32_t p_id);
        static std::map<int32_t, std::shared_ptr<ObjectElements::Transform>>& GetAllTransforms() { return GetInstance()->m_transforms; }
        static std::shared_ptr<ObjectElements::Transform> GetTransform(uint32_t p_id);

    private:
        TransformContainer() = default;
        inline static TransformContainer* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<ObjectElements::Transform>> m_transforms;
    };
}
