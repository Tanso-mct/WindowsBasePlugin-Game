#pragma once
#include "wbp_collision/include/dll_config.h"
#include "windows_base/windows_base.h"

#include "wbp_collision/include/interfaces/component_box_collider.h"

namespace wbp_collision
{
    const WBP_COLLISION_API size_t &BoxColliderComponentID();

    constexpr bool DEFAULT_COLLISION_ENABLED = true;
    constexpr bool DEFAULT_IS_TRIGGER = false;
    constexpr size_t DEFAULT_BOX_SHAPE_ASSET_ID = 0;

    class WBP_COLLISION_API BoxColliderComponent : public IBoxColliderComponent
    {
    private:
        bool collisionEnabled_ = DEFAULT_COLLISION_ENABLED;
        bool isTrigger_ = DEFAULT_IS_TRIGGER;
        size_t colliderShapeAssetID_ = DEFAULT_BOX_SHAPE_ASSET_ID;

    public:
        BoxColliderComponent() = default;
        ~BoxColliderComponent() override = default;

        /***************************************************************************************************************
         * IComponent implementation
        /**************************************************************************************************************/

        virtual const size_t &GetID() const override;

        /***************************************************************************************************************
         * IBoxColliderComponent implementation
        /**************************************************************************************************************/

        void SetCollisionEnabled(bool enabled) override { collisionEnabled_ = enabled; }
        bool IsCollisionEnabled() const override { return collisionEnabled_; }

        void SetTrigger(bool isTrigger) override { isTrigger_ = isTrigger; }
        bool IsTrigger() const override { return isTrigger_; }

        void SetColliderShapeAssetID(size_t colliderShapeAssetID) override { colliderShapeAssetID_ = colliderShapeAssetID; }

        size_t GetAABBCount(std::unique_ptr<wb::IAssetContainer> &assetContainer) const override;
        const std::vector<PrimitiveAABB> &GetAABBs(std::unique_ptr<wb::IAssetContainer> &assetContainer) const override;
        const PrimitiveAABB &GetAABB(size_t index, std::unique_ptr<wb::IAssetContainer> &assetContainer) const override;
    };

} // namespace wbp_collision