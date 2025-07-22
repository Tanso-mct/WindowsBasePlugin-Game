#include "wbp_collision/src/pch.h"
#include "wbp_collision/include/component_ray_collider.h"

const WBP_COLLISION_API size_t &wbp_collision::RayColliderComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

const size_t &wbp_collision::RayColliderComponent::GetID() const
{
    return RayColliderComponentID();
}

namespace wbp_collision
{
    WB_REGISTER_COMPONENT(RayColliderComponentID(), RayColliderComponent);

} // namespace wbp_collision