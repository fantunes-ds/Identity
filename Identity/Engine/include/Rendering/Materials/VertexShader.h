#pragma once
#include <memory>
#include <string>

#include <WinSetup.h>
#include <wrl/client.h>
#include <Objects/IObject.h>
#include <Rendering/Buffers/VertexConstantBuffer.h>

namespace Engine::Rendering::Materials
{
    class VertexShader : public Objects::IObject
    {
    public:
        VertexShader();
        ~VertexShader();

        static std::shared_ptr<VertexShader> LoadShader(const std::string& p_path, const std::string& p_name);
        void LoadShader(const std::string& p_path);

        void BindShader();
        void GenerateConstantBuffer();

        inline void SetPath(const std::string& p_path) { m_path = p_path; }
        [[nodiscard]] inline const std::string& GetPath() const { return m_path; }

    private:
        std::string m_path{};

        Microsoft::WRL::ComPtr<ID3D11VertexShader> pixelShader;
        Buffers::VertexConstantBuffer m_pcb;
        Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
    };
}
