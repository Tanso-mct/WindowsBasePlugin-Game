#include "wbp_collision/src/pch.h"
#include "wbp_collision/include/system_collision.h"

#include "wbp_collision/include/collision_pass_box.h"

const WBP_COLLISION_API size_t &wbp_collision::CollisionSystemID()
{
    static size_t id = wb::IDFactory::CreateSystemID();
    return id;
}

wbp_collision::CollisionSystem::CollisionSystem()
{
    // Initialize collision passes. The order of addition to vector is the order of execution.
    passes_.clear();
    passes_.emplace_back(std::make_unique<wbp_collision::CollisionPassBox>());
}

const size_t &wbp_collision::CollisionSystem::GetID() const
{
    return CollisionSystemID();
}

void wbp_collision::CollisionSystem::Initialize(wb::IAssetContainer &assetContainer)
{

}

void wbp_collision::CollisionSystem::Update(const wb::SystemArgument &args)
{
    for (const std::unique_ptr<wbp_collision::ICollisionPass> &pass : passes_)
    {
        pass->Execute(args);
    }
}

namespace wbp_collision
{
    WB_REGISTER_SYSTEM(CollisionSystem, CollisionSystemID());

} // namespace wbp_collision