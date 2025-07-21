#include "wbp_collision/src/pch.h"
#include "wbp_collision/include/component_box_collider.h"

#include "wbp_collision/include/interfaces/asset_collider_shape.h"

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

    wbp_collision::IColliderShapeAsset *colliderShapeAsset = wb::As<wbp_collision::IColliderShapeAsset>(&asset());
    if (colliderShapeAsset == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "The asset with the ID does not implement or not collect IColliderShapeAsset.",
                "BoxColliderComponent requires IColliderShapeAsset to be set.",
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_COLLISION", err);
        wb::ThrowRuntimeError(err);
    }

    return colliderShapeAsset->GetAABBs().size();
}

const std::vector<wbp_collision::PrimitiveAABB> &wbp_collision::BoxColliderComponent::GetAABBs
(
    std::unique_ptr<wb::IAssetContainer> &assetContainer
) const
{
    wb::LockedRef<wb::IAsset> asset = assetContainer->ThreadSafeGet(colliderShapeAssetID_);

    wbp_collision::IColliderShapeAsset *colliderShapeAsset = wb::As<wbp_collision::IColliderShapeAsset>(&asset());
    if (colliderShapeAsset == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "The asset with the ID does not implement or not collect IColliderShapeAsset.",
                "BoxColliderComponent requires IColliderShapeAsset to be set.",
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_COLLISION", err);
        wb::ThrowRuntimeError(err);
    }

    return colliderShapeAsset->GetAABBs();
}

const wbp_collision::PrimitiveAABB &wbp_collision::BoxColliderComponent::GetAABB
(
    size_t index, std::unique_ptr<wb::IAssetContainer> &assetContainer
) const
{
    wb::LockedRef<wb::IAsset> asset = assetContainer->ThreadSafeGet(colliderShapeAssetID_);

    wbp_collision::IColliderShapeAsset *colliderShapeAsset = wb::As<wbp_collision::IColliderShapeAsset>(&asset());
    if (colliderShapeAsset == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "The asset with the ID does not implement or not collect IColliderShapeAsset.",
                "BoxColliderComponent requires IColliderShapeAsset to be set.",
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_COLLISION", err);
        wb::ThrowRuntimeError(err);
    }

    return colliderShapeAsset->GetAABBs().at(index);
}

namespace wbp_collision
{
    WB_REGISTER_COMPONENT(BoxColliderComponentID(), BoxColliderComponent);

} // namespace wbp_collision