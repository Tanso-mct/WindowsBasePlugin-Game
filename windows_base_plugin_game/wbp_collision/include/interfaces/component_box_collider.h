#pragma once
#include "windows_base/windows_base.h"

#include "wbp_collision/include/primitive.h"

#include <DirectXMath.h>

namespace wbp_collision
{
    class IBoxColliderComponent : public wb::IComponent
    {
    public:
        virtual ~IBoxColliderComponent() = default;

        // Sets whether or not collision judgments are made.
        virtual void SetCollisionEnabled(bool enabled) = 0;
        virtual bool IsCollisionEnabled() const = 0;

        // Whether the movement vector changes when hit
        virtual void SetTrigger(bool isTrigger) = 0;
        virtual bool IsTrigger() const = 0;

        // Set the ID of the Asset, which has the shape of a collider
        virtual void SetColliderShapeAssetID(size_t colliderShapeAssetID) = 0;

        virtual size_t GetAABBCount(std::unique_ptr<wb::IAssetContainer> &assetContainer) const = 0;
        virtual const std::vector<PrimitiveAABB> &GetAABBs(std::unique_ptr<wb::IAssetContainer> &assetContainer) const = 0;
        virtual const PrimitiveAABB &GetAABB(size_t index, std::unique_ptr<wb::IAssetContainer> &assetContainer) const = 0;
    };

} // namespace wbp_collision