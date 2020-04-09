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
        VertexShader() = default;
        ~VertexShader() = default;

        /**
         * @brief Create a new VertexShader from the path and the name given
         * @param p_path The path of the VertexShader to load
         * @param p_name The name you want to give to the VertexShader
         * @return Return a shared_ptr of the created VertexShader
         */
        static std::shared_ptr<VertexShader> LoadShader(const std::string& p_path, const std::string& p_name);

        /**
         * @brief Used to bind the VertexShader to the rendering context
         */
        void BindShader() const;
        /**
         * @brief Used to unbind the VertexShader from the rendering context
         */
        void UnBindShader();
        /**
         * @brief Used to generate the constant buffer of the shader
         */
        void GenerateConstantBuffer();
        /**
         * @brief Used to bind the constant buffer to the rendering context
         */
        void BindConstantBuffer() const;
        /**
         * @brief Used to unbind the constant buffer from the rendering context
         */
        void UnBindConstantBuffer() const;

        inline void SetPath(const std::string& p_path) { m_path = p_path; }
        [[nodiscard]] inline const std::string& GetPath() const { return m_path; }

    private:
        std::string m_path{};

        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
        Buffers::VertexConstantBuffer m_vcb;
        Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
    };
}
