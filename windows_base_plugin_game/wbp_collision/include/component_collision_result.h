#pragma once
#include "wbp_collision/include/dll_config.h"
#include "windows_base/windows_base.h"

#include "wbp_collision/include/interfaces/component_collision_result.h"

namespace wbp_collision
{
    const WBP_COLLISION_API size_t &CollisionResultComponentID();

    class WBP_COLLISION_API CollisionResultComponent : public ICollisionResultComponent
    {
    private:
        std::vector<std::unique_ptr<wb::IOptionalValue>> collidedEntityIDs_;
        std::vector<DirectX::XMFLOAT3> collidedFaceNormals_;

    public:
        CollisionResultComponent() = default;
        ~CollisionResultComponent() override = default;

        CollisionResultComponent(const CollisionResultComponent &) = delete;
        CollisionResultComponent &operator=(const CollisionResultComponent &) = delete;

        /***************************************************************************************************************
         * IComponent implementation
        /**************************************************************************************************************/

        virtual const size_t &GetID() const override;

        /***************************************************************************************************************
         * ICollisionResultComponent implementation
        /**************************************************************************************************************/

        bool IsCollided() const override { return GetCollidedCount() > 0; }
        size_t GetCollidedCount() const override { return collidedEntityIDs_.size(); }

        const wb::IOptionalValue &GetCollidedEntityID(size_t index) const override;
        const DirectX::XMFLOAT3 &GetCollidedFaceNormal(size_t index) const override;

        void AddCollided(std::unique_ptr<wb::OptionalValue> entityID, const DirectX::XMFLOAT3 &normal) override;
        void ClearCollided() override;
    };

} // namespace wbp_collision