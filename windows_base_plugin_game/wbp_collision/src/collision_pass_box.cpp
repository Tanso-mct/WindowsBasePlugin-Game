#include "wbp_collision/src/pch.h"
#include "wbp_collision/include/collision_pass_box.h"

#include "wbp_collision/include/component_collision_result.h"
#include "wbp_collision/include/component_box_collider.h"

#include "wbp_primitive/include/primitive_helpers.h"
#pragma comment(lib, "wbp_primitive.lib")

#include <DirectXMath.h>
using namespace DirectX;

#include "wbp_transform/plugin.h"
#pragma comment(lib, "wbp_transform.lib")

#include "wbp_identity/plugin.h"
#pragma comment(lib, "wbp_identity.lib")

void wbp_collision::CollisionPassBox::Execute(const wb::SystemArgument &args)
{
    // Get containers to use
    wb::IAssetContainer &assetContainer = args.containerStorage_.GetContainer<wb::IAssetContainer>();

    std::vector<std::unique_ptr<wb::IOptionalValue>> runnerEntityIDs;
    for (const std::unique_ptr<wb::IOptionalValue> &id : (args.entityIDView_)(wbp_collision::BoxColliderComponentID()))
    {
        wb::IEntity *entity = args.entityContainer_.PtrGet(*id);
        if (entity == nullptr) continue; // Skip if entity is null

        wb::IComponent *identityComponent = entity->GetComponent(wbp_identity::IdentityComponentID(), args.componentContainer_);
        wbp_identity::IIdentityComponent *identity = wb::As<wbp_identity::IIdentityComponent>(identityComponent);
        if (identity == nullptr)
        {
            std::string err = wb::CreateErrorMessage
            (
                __FILE__, __LINE__, __FUNCTION__,
                {
                    "Entity does not have IdentityComponent.",
                    "BoxColliderComponent requires IdentityComponent to be set.",
                }
            );
            wb::ConsoleLogErr(err);
            wb::ErrorNotify("WBP_COLLISION", err);
            wb::ThrowRuntimeError(err);
        }

        if (!identity->IsActiveSelf())
        {
            // Skip if the entity is not active
            continue;
        }

        wb::IComponent *boxColliderComponent = entity->GetComponent(wbp_collision::BoxColliderComponentID(), args.componentContainer_);
        wbp_collision::IBoxColliderComponent *boxCollider = wb::As<wbp_collision::IBoxColliderComponent>(boxColliderComponent);
        
        if (!boxCollider->IsCollisionEnabled())
        {
            // Skip if collision is not enabled
            continue;
        }

        wb::IComponent *transformComponent = entity->GetComponent(wbp_transform::TransformComponentID(), args.componentContainer_);
        wbp_transform::ITransformComponent *transform = wb::As<wbp_transform::ITransformComponent>(transformComponent);
        if (transform == nullptr)
        {
            std::string err = wb::CreateErrorMessage
            (
                __FILE__, __LINE__, __FUNCTION__,
                {
                    "Entity does not have TransformComponent.",
                    "BoxColliderComponent requires TransformComponent to be set.",
                }
            );
            wb::ConsoleLogErr(err);
            wb::ErrorNotify("WBP_COLLISION", err);
            wb::ThrowRuntimeError(err);
        }

        if (!transform->WasDirty())
        {
            // Skip if the transform has not changed
            continue;
        }

        // Add the entity to the runner list
        runnerEntityIDs.emplace_back(id->Clone());
    }

    for (const std::unique_ptr<wb::IOptionalValue> &runnerID : runnerEntityIDs)
    {
        wb::IEntity *runnerEntity = args.entityContainer_.PtrGet(*runnerID);

        wb::IComponent *runnerBoxColliderComponent = runnerEntity->GetComponent(wbp_collision::BoxColliderComponentID(), args.componentContainer_);
        wbp_collision::IBoxColliderComponent *runnerBoxCollider = wb::As<wbp_collision::IBoxColliderComponent>(runnerBoxColliderComponent);

        wb::IComponent *runnerCollisionResultComponent = runnerEntity->GetComponent(wbp_collision::CollisionResultComponentID(), args.componentContainer_);
        wbp_collision::ICollisionResultComponent *runnerCollisionResult = wb::As<wbp_collision::ICollisionResultComponent>(runnerCollisionResultComponent);
        
        wb::IComponent *runnerTransformComponent = runnerEntity->GetComponent(wbp_transform::TransformComponentID(), args.componentContainer_);
        wbp_transform::ITransformComponent *runnerTransform = wb::As<wbp_transform::ITransformComponent>(runnerTransformComponent);

        // Create total AABB from the runner's box collider
        wbp_primitive::PrimitiveAABB totalAABB = wbp_primitive::CreateAABBFromAABBs
        (
            runnerBoxCollider->GetAABBs(assetContainer), XMMatrixIdentity()
        );

        // Create movement vector based on runnerTransform
        DirectX::XMFLOAT3 movementVec = 
        {
            runnerTransform->GetPosition().x - runnerTransform->GetPreviousPosition().x,
            runnerTransform->GetPosition().y - runnerTransform->GetPreviousPosition().y,
            runnerTransform->GetPosition().z - runnerTransform->GetPreviousPosition().z
        };

        // Create AABB from movement vector
        wbp_primitive::PrimitiveAABB movementAABB = wbp_primitive::CreateAABBFromAABBMovement
        (
            totalAABB, 
            runnerTransform->GetPreviousWorldMatrixWithoutRot(), 
            runnerTransform->GetWorldMatrixWithoutRot()
        );

        for (const std::unique_ptr<wb::IOptionalValue> &id : (args.entityIDView_)(wbp_collision::BoxColliderComponentID()))
        {
            wb::IEntity *receiverEntity = args.entityContainer_.PtrGet(*id);
            if (receiverEntity == nullptr || receiverEntity->GetID()() == runnerEntity->GetID()())
            {
                // Skip if receiverEntity is null or the same as runner
                continue;
            }

            wb::IComponent *receiverBoxColliderComponent = receiverEntity->GetComponent(wbp_collision::BoxColliderComponentID(), args.componentContainer_);
            wbp_collision::IBoxColliderComponent *receiverBoxCollider = wb::As<wbp_collision::IBoxColliderComponent>(receiverBoxColliderComponent);

            wb::IComponent *receiverTransformComponent = receiverEntity->GetComponent(wbp_transform::TransformComponentID(), args.componentContainer_);
            wbp_transform::ITransformComponent *receiverTransform = wb::As<wbp_transform::ITransformComponent>(receiverTransformComponent);

            for (const wbp_primitive::PrimitiveAABB &receiverAABB : receiverBoxCollider->GetAABBs(assetContainer))
            {
                bool isIntersected = wbp_primitive::IntersectAABBs
                (
                    movementAABB, XMMatrixIdentity(),
                    receiverAABB, receiverTransform->GetWorldMatrixWithoutRot()
                );

                if (!isIntersected)
                {
                    // Skip if AABBs do not intersect
                    continue;
                }

                if (runnerCollisionResult != nullptr)
                {
                    XMFLOAT3 collidedFaceNormal = wbp_primitive::GetCollidedFaceNormal
                    (
                        receiverAABB, receiverTransform->GetWorldMatrixWithoutRot(),
                        movementAABB, XMMatrixIdentity(),
                        movementVec
                    );

                    // Add collision result
                    runnerCollisionResult->AddCollided
                    (
                        receiverEntity->GetID().Clone(), 
                        collidedFaceNormal, receiverBoxCollider->IsTrigger()
                    );
                }
            }
        }

    }
}