#pragma once
#include <Export.h>

namespace Engine::Tools::ASSIMP
{
    /***
     * @brief static class used to convert ASSIMP values to Identity Engine's types.
     */
    class API_ENGINE ASSIMPConversion
    {
    public:
        ASSIMPConversion() = delete;
        ~ASSIMPConversion() = delete;

        static GPM::Matrix4F Mat4x4ToGPM(const aiMatrix4x4& p_matrix);
    };
}
