#pragma once
#include "windows_base/windows_base.h"

#include "wbp_collision/include/interfaces/collider.h"

#include "wbp_primitive/include/primitive.h"

namespace wbp_collision
{
    class IRayColliderComponent : public wb::IComponent, public ICollider
    {
    public:
        virtual ~IRayColliderComponent() = default;

        virtual const wbp_primitive::PrimitiveRay &GetRay() const = 0;

        virtual void CastRay() = 0;
        virtual void EnableContinuousRayCasting(bool enable) = 0;
    }; 

} // namespace wbp_collision