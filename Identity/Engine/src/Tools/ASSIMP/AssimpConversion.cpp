#include <stdafx.h>
#include <Tools/ASSIMP/ASSIMPConversion.h>

using namespace Engine::Tools::ASSIMP;

GPM::Matrix4F ASSIMPConversion::Mat4x4ToGPM(const aiMatrix4x4& p_matrix)
{
    return Matrix4F
    {
        p_matrix.a1, p_matrix.a2, p_matrix.a3, p_matrix.a4,
        p_matrix.b1, p_matrix.b2, p_matrix.b3, p_matrix.b4,
        p_matrix.c1, p_matrix.c2, p_matrix.c3, p_matrix.c4,
        p_matrix.d1, p_matrix.d2, p_matrix.d3, p_matrix.d4
    };
}
