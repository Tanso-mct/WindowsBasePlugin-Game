#pragma once
#include "windows_base/windows_base.h"

#include "wbp_collision/include/primitive.h"

#include <DirectXMath.h>

namespace wbp_collision
{
    class IColliderShapeAsset : public wb::IAsset
    {
    public:
        virtual ~IColliderShapeAsset() = default;

        virtual std::vector<DirectX::XMFLOAT3> &GetVertices() = 0;

        virtual std::vector<PrimitiveAABB> &GetAABBs() = 0;
        virtual PrimitiveAABB &GetTotalAABB() = 0;
    };

} // namespace wbp_collision