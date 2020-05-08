#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include <Managers/ResourceManager.h>

namespace Engine::Components
{
    class API_ENGINE ModelComponent: public IComponent
    {
    public:
        ModelComponent(Objects::GameObject* p_gameObject);
        ModelComponent(Objects::GameObject* p_gameObject, std::shared_ptr<ModelComponent> p_other);
        ModelComponent(Objects::GameObject* p_gameObject, const int32_t p_id);
        ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_name);
        ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_file, const std::string& p_name);
        virtual ~ModelComponent() = default;

        void Serialize(std::ostream& p_stream) override;
        void Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block) override;

        bool operator==(IComponent* p_other) override;

        inline void SetModel(int32_t p_id) { m_model = p_id; }
        inline int32_t GetModel() const { return m_model; }

        inline void SetMaterial(const std::string& p_name) { m_material = Managers::ResourceManager::GetMaterial(p_name); }
        inline std::shared_ptr<Rendering::Materials::Material> GetMaterial() const { return m_material; }

        bool DeleteFromMemory() override;
        void SetActive(bool p_active) override { m_isActive = p_active; }

    private:
        int32_t m_model;
        std::shared_ptr<Rendering::Materials::Material> m_material;
    };
}
