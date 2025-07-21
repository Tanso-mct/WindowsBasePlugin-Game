#pragma once
#include "wbp_collision/include/dll_config.h"

#include "wbp_collision/include/primitive.h"

#include <vector>

namespace wbp_collision
{
    WBP_COLLISION_API wbp_collision::PrimitiveAABB CreateAABBFromAABBs
    (
        const std::vector<wbp_collision::PrimitiveAABB> &aabbs, 
        const DirectX::XMMATRIX &convertMat = DirectX::XMMatrixIdentity()
    );

    WBP_COLLISION_API wbp_collision::PrimitiveAABB CreateAABBFromVec
    (
        const DirectX::XMFLOAT3 &vec, const DirectX::XMFLOAT3 &origin
    );

    WBP_COLLISION_API wbp_collision::PrimitiveAABB CreateAABBFromAABBMovement
    (
        const wbp_collision::PrimitiveAABB &aabb,
        const DirectX::XMMATRIX &beforeMat, const DirectX::XMMATRIX &afterMat
    );

    WBP_COLLISION_API bool IntersectAABBs
    (
        const wbp_collision::PrimitiveAABB &aabb1, const DirectX::XMMATRIX &aabb1ConvertMat,
        const wbp_collision::PrimitiveAABB &aabb2, const DirectX::XMMATRIX &aabb2ConvertMat
    );

    WBP_COLLISION_API DirectX::XMFLOAT3 GetCollidedFaceNormal
    (
        const wbp_collision::PrimitiveAABB &aabb1, const DirectX::XMMATRIX &aabb1ConvertMat,
        const wbp_collision::PrimitiveAABB &aabb2, const DirectX::XMMATRIX &aabb2ConvertMat,
        const DirectX::XMFLOAT3 &movement
    );

} // namespace wbp_collision