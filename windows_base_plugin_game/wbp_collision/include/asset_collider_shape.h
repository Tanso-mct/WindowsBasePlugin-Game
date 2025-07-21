#pragma once
#include "wbp_collision/include/dll_config.h"
#include "windows_base/windows_base.h"

#include "wbp_collision/include/interfaces/asset_collider_shape.h"

namespace wbp_collision
{
    class ColliderShapeAsset : public IColliderShapeAsset
    {
    private:
        std::vector<DirectX::XMFLOAT3> vertices_;
        PrimitiveAABB aabb_;

    public:
        ColliderShapeAsset() = default;
        ~ColliderShapeAsset() override = default;

        std::vector<DirectX::XMFLOAT3> GetVertices() const override { return vertices_; }
        PrimitiveAABB GetAABB() const override { return aabb_; }
    };

} // namespace wbp_collision