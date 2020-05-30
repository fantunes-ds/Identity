#pragma once
#include <memory>
#include <string>

#include <WinSetup.h>
#include <wrl/client.h>
#include <Objects/IObject.h>
#include <Rendering/Buffers/PixelConstantBuffer.h>

namespace Engine::Rendering::Materials
{
    class PixelShader : public Objects::IObject, public Serialize::ISerializeable
    {
    public:
        PixelShader() = default;
        ~PixelShader() = default;

        /**
         * @brief Create a new PixelShader from the path and the name given
         * @param p_path The path of the PixelShader to load
         * @param p_name The name you want to give to the PixelShader
         * @return Return a shared_ptr of the created PixelShader
         */
        static std::shared_ptr<PixelShader> LoadShader(const std::string& p_path, const std::string& p_name);

        /**
         * @brief Used to bind the PixelShader to the rendering context
         */
        void BindShader() const;
        /**
         * @brief Used to unbind the PixelShader from the rendering context
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
        [[nodiscard]] inline const Buffers::PixelConstantBuffer GetPCB() const { return m_pcb; }
        [[nodiscard]] inline const Microsoft::WRL::ComPtr<ID3DBlob> GetBlob() const { return m_blob; }


        void Serialize(std::ostream& p_stream) override;
        void Unserialize(std::istream& p_stream) override {}
    private:
        std::string m_path{};

        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
        Buffers::PixelConstantBuffer m_pcb;
        Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
    };
}
