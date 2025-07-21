#include "wbp_collision/src/pch.h"
#include "wbp_collision/include/component_box_collider.h"

const WBP_COLLISION_API size_t &wbp_collision::BoxColliderComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

const size_t &wbp_collision::BoxColliderComponent::GetID() const
{
    return BoxColliderComponentID();
}

size_t wbp_collision::BoxColliderComponent::GetAABBCount(std::unique_ptr<wb::IAssetContainer> &assetContainer) const
{
    wb::LockedRef<wb::IAsset> asset = assetContainer->ThreadSafeGet(colliderShapeAssetID_);

    // TODO: Implement the logic to retrieve the AABB count from the asset.

    return 0;
}

const std::vector<wbp_collision::PrimitiveAABB> &wbp_collision::BoxColliderComponent::GetAABBs
(
    std::unique_ptr<wb::IAssetContainer> &assetContainer
) const
{
    wb::LockedRef<wb::IAsset> asset = assetContainer->ThreadSafeGet(colliderShapeAssetID_);

    // TODO: Implement the logic to retrieve the AABBs from the asset.

    return std::vector<PrimitiveAABB>();
}

const wbp_collision::PrimitiveAABB &wbp_collision::BoxColliderComponent::GetAABB
(
    size_t index, std::unique_ptr<wb::IAssetContainer> &assetContainer
) const
{
    wb::LockedRef<wb::IAsset> asset = assetContainer->ThreadSafeGet(colliderShapeAssetID_);

    // TODO: Implement the logic to retrieve the specific AABB from the asset.

    static PrimitiveAABB dummyAABB;
    return dummyAABB;
}

namespace wbp_collision
{
    WB_REGISTER_COMPONENT(BoxColliderComponentID(), BoxColliderComponent);

} // namespace wbp_collision