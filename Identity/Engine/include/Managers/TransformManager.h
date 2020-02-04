#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Transform.h>

namespace Engine::Managers
{
    class API_ENGINE TransformManager
    {
    public:
        ~TransformManager();
        TransformManager(const TransformManager&) = delete;
        TransformManager(const TransformManager&&) = delete;

        static TransformManager* GetInstance();

        static int32_t AddTransform(ObjectElements::Transform& p_transform);

        static std::shared_ptr<ObjectElements::Transform> FindTransform(uint32_t p_id);
        static std::map<int, std::shared_ptr<ObjectElements::Transform>>& GetAllTransforms() { return GetInstance()->m_transforms; }
        static std::shared_ptr<ObjectElements::Transform> GetTransform(uint32_t p_id);

    private:
        TransformManager() = default;
        inline static TransformManager* m_instance = nullptr;
        std::map<int, std::shared_ptr<ObjectElements::Transform>> m_transforms;
    };
}
