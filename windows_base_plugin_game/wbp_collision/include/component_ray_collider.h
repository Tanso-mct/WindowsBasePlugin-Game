#pragma once
#include "wbp_collision/include/dll_config.h"
#include "windows_base/windows_base.h"

#include "wbp_collision/include/interfaces/component_ray_collider.h"

namespace wbp_collision
{
    const WBP_COLLISION_API size_t &RayColliderComponentID();

    class WBP_COLLISION_API RayColliderComponent : public IRayColliderComponent
    {
    private:
        bool collisionEnabled_ = DEFAULT_COLLISION_ENABLED;
        bool isTrigger_ = DEFAULT_IS_TRIGGER;

        wbp_primitive::PrimitiveRay ray_;
        bool casted_ = false;
        bool casting_ = false;

    public:
        RayColliderComponent() = default;
        ~RayColliderComponent() override = default;

        /***************************************************************************************************************
         * IComponent implementation
        /**************************************************************************************************************/

        const size_t &GetID() const override;

        /***************************************************************************************************************
         * ICollider implementation
        /**************************************************************************************************************/

        void SetCollisionEnabled(bool enabled) override { collisionEnabled_ = enabled; }
        bool IsCollisionEnabled() const override { return collisionEnabled_; }

        void SetTrigger(bool isTrigger) override { isTrigger_ = isTrigger; }
        bool IsTrigger() const override { return isTrigger_; }

        /***************************************************************************************************************
         * IRayColliderComponent implementation
        /**************************************************************************************************************/

        wbp_primitive::PrimitiveRay &GetRay() override { return ray_; }

        bool IsCast() const override { return casted_; }
        void SetCast(bool casted) override { casted_ = casted; }

        bool IsCasting() const override { return casting_; }
        void SetCasting(bool casting) override { casting_ = casting; }

    };


} // namespace wbp_collision