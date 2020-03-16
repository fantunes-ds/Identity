#pragma once

#include <Export.h>
#include <map>
#include <memory>
#include <Rendering/Materials/Material.h>

namespace Engine::Containers
{
    class API_ENGINE MaterialContainer
    {
    public:
        ~MaterialContainer();

        MaterialContainer(const MaterialContainer&) = delete;
        MaterialContainer(const MaterialContainer&&) = delete;

        static MaterialContainer* GetInstance();

        static int32_t AddMaterial(Rendering::Materials::Material* p_material);
        static int32_t AddMaterial(const std::string& p_name);
        static bool RemoveMaterial(int32_t p_id);

        static int32_t FindMaterial(const std::string& p_name);

        static std::shared_ptr<Rendering::Materials::Material> GetMaterial(int32_t p_id);
        static std::shared_ptr<Rendering::Materials::Material> GetMaterial(const std::string& p_name);
        inline static std::map<int32_t, std::shared_ptr<Rendering::Materials::Material>>& GetAllMaterials() { return GetInstance()->m_material; }

    private:
        MaterialContainer() = default;

        inline static MaterialContainer* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<Rendering::Materials::Material>> m_material;
    };
}
