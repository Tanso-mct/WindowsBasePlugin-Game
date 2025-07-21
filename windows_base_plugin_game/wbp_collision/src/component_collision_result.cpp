#include "wbp_collision/src/pch.h"
#include "wbp_collision/include/component_collision_result.h"

const WBP_COLLISION_API size_t &wbp_collision::CollisionResultComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

const size_t &wbp_collision::CollisionResultComponent::GetID() const
{
    return CollisionResultComponentID();
}

const wb::IOptionalValue &wbp_collision::CollisionResultComponent::GetCollidedEntityID(size_t index) const
{
    if (index >= collidedEntityIDs_.size())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Index out of range.",
                "Requested index: " + std::to_string(index),
                "Collided count: " + std::to_string(collidedEntityIDs_.size())
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_COLLISION", err);
        wb::ThrowRuntimeError(err);
    }

    return *collidedEntityIDs_[index];
}

const DirectX::XMFLOAT3 &wbp_collision::CollisionResultComponent::GetCollidedFaceNormal(size_t index) const
{
    if (index >= collidedFaceNormals_.size())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Index out of range.",
                "Requested index: " + std::to_string(index),
                "Collided count: " + std::to_string(collidedFaceNormals_.size())
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_COLLISION", err);
        wb::ThrowRuntimeError(err);
    }

    return collidedFaceNormals_[index];
}

bool wbp_collision::CollisionResultComponent::GetCollidedIsTrigger(size_t index) const
{
    if (index >= collidedIsTriggers_.size())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Index out of range.",
                "Requested index: " + std::to_string(index),
                "Collided count: " + std::to_string(collidedIsTriggers_.size())
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_COLLISION", err);
        wb::ThrowRuntimeError(err);
    }

    return collidedIsTriggers_[index];
}

void wbp_collision::CollisionResultComponent::AddCollided
(
    std::unique_ptr<wb::IOptionalValue> entityID, 
    const DirectX::XMFLOAT3 &normal, bool isTrigger
){
    if (entityID == nullptr || !entityID->IsValid())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Invalid entity ID provided.",
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_COLLISION", err);
        wb::ThrowRuntimeError(err);
    }

    collidedEntityIDs_.emplace_back(std::move(entityID));
    collidedFaceNormals_.emplace_back(normal);
    collidedIsTriggers_.emplace_back(isTrigger);
}

void wbp_collision::CollisionResultComponent::ClearCollided()
{
    collidedEntityIDs_.clear();
    collidedFaceNormals_.clear();
    collidedIsTriggers_.clear();
}

namespace wbp_collision
{
    WB_REGISTER_COMPONENT(CollisionResultComponentID(), CollisionResultComponent);
    
} // namespace wbp_collision