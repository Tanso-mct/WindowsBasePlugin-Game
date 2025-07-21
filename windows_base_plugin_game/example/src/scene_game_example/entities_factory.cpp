#include "example/src/pch.h"
#include "example/include/scene_game_example/entities_factory.h"

#include "example/include/mode.h"

#include "wbp_transform/plugin.h"
#pragma comment(lib, "wbp_transform.lib")

#include "wbp_identity/plugin.h"
#pragma comment(lib, "wbp_identity.lib")

#include "wbp_render/plugin.h"
#pragma comment(lib, "wbp_render.lib")

void example::GameExampleEntitiesFactory::Create
(
    wb::IAssetContainer &assetCont, 
    wb::IEntityContainer &entityCont, 
    wb::IComponentContainer &componentCont, 
    wb::IEntityIDView &entityIDView
) const
{
#if defined(EXAMPLE_MODE_TRANSFORM)

    // Create a root entity
    std::unique_ptr<wb::IOptionalValue> rootEntityID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(entityCont, entityIDView);
        rootEntityID = entity().GetID().Clone();

        entity().AddComponent(wbp_transform::TransformComponentID(), componentCont);
    }

    // Initialize the root entity
    {
        wb::IEntity *entity = entityCont.PtrGet(*rootEntityID);

        wb::IComponent *transformComponent = entity->GetComponent(wbp_transform::TransformComponentID(), componentCont);
        wbp_transform::ITransformComponent *transform = wb::As<wbp_transform::ITransformComponent>(transformComponent);

        transform->SetLocalPosition(DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f));
        transform->SetLocalRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        transform->SetLocalScale(DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f));
    }

    // Create a child A entity
    std::unique_ptr<wb::IOptionalValue> childAEntityID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(entityCont, entityIDView);
        childAEntityID = entity().GetID().Clone();

        entity().AddComponent(wbp_transform::TransformComponentID(), componentCont);
    }

    // Initialize the child A entity
    {
        wb::IEntity *entity = entityCont.PtrGet(*childAEntityID);

        wb::IComponent *transformComponent = entity->GetComponent(wbp_transform::TransformComponentID(), componentCont);
        wbp_transform::ITransformComponent *transform = wb::As<wbp_transform::ITransformComponent>(transformComponent);

        transform->SetLocalPosition(DirectX::XMFLOAT3(5.0f, 0.0f, 0.0f));
        transform->SetLocalRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        transform->SetLocalScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

        // Get the root entity
        wb::IEntity *rootEntity = entityCont.PtrGet(*rootEntityID);

        // Set the parent of the child A entity to the root entity
        transform->SetParent(entity, rootEntity, entityCont, componentCont);
    }

    // Create a child B entity
    std::unique_ptr<wb::IOptionalValue> childBEntityID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(entityCont, entityIDView);
        childBEntityID = entity().GetID().Clone();

        entity().AddComponent(wbp_transform::TransformComponentID(), componentCont);
    }

    // Initialize the child B entity
    {
        wb::IEntity *entity = entityCont.PtrGet(*childBEntityID);

        wb::IComponent *transformComponent = entity->GetComponent(wbp_transform::TransformComponentID(), componentCont);
        wbp_transform::ITransformComponent *transform = wb::As<wbp_transform::ITransformComponent>(transformComponent);

        transform->SetLocalPosition(DirectX::XMFLOAT3(-5.0f, 0.0f, 0.0f));
        transform->SetLocalRotation(DirectX::XMFLOAT3(0.0f, 90.0f, 0.0f));
        transform->SetLocalScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

        // Get the root entity
        wb::IEntity *rootEntity = entityCont.PtrGet(*rootEntityID);

        // Set the parent of the child B entity to the root entity
        transform->SetParent(entity, rootEntity, entityCont, componentCont);
    }

    // Create a grandchild entity
    std::unique_ptr<wb::IOptionalValue> grandChildEntityID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(entityCont, entityIDView);
        grandChildEntityID = entity().GetID().Clone();

        entity().AddComponent(wbp_transform::TransformComponentID(), componentCont);
    }

    // Initialize the grandchild entity
    {
        wb::IEntity *entity = entityCont.PtrGet(*grandChildEntityID);

        wb::IComponent *transformComponent = entity->GetComponent(wbp_transform::TransformComponentID(), componentCont);
        wbp_transform::ITransformComponent *transform = wb::As<wbp_transform::ITransformComponent>(transformComponent);

        transform->SetLocalPosition(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f));
        transform->SetLocalRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        transform->SetLocalScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

        // Get the child A entity
        wb::IEntity *childAEntity = entityCont.PtrGet(*childAEntityID);

        // Set the parent of the grandchild entity to the child A entity
        transform->SetParent(entity, childAEntity, entityCont, componentCont);
    }

    // Output the explanation
    {
        std::string msg = wb::CreateMessage
        ({
            "[WindowsBasePlugin-Game : wbp_transform]",
            "This example demonstrates the TransformComponent's parent-child relationships.",
            "The root entity has two children: A and B.",
            "Child A has a grandchild entity.",
            "The TransformSystem will update the positions, rotations, and scales based on these relationships."
            "",
            "Root Entity ID: " + std::to_string((*rootEntityID)()),
            "Child A Entity ID: " + std::to_string((*childAEntityID)()),
            "Child B Entity ID: " + std::to_string((*childBEntityID)()),
            "Grandchild Entity ID: " + std::to_string((*grandChildEntityID)()),
            "",
            "Press Space to output the current transform of the entities."
            "Press W, A, S, D to move the root entity.",
        });
        wb::ConsoleLog(msg);
    }

#elif defined(EXAMPLE_MODE_IDENTITY)

    // Create a entity which has IdentityComponent
    std::unique_ptr<wb::IOptionalValue> entityID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(entityCont, entityIDView);
        entityID = entity().GetID().Clone();

        entity().AddComponent(wbp_identity::IdentityComponentID(), componentCont);
    }

    // Initialize the entity with IdentityComponent
    {
        wb::IEntity *entity = entityCont.PtrGet(*entityIDView(0).front());

        wb::IComponent *identityComponent = entity->GetComponent(wbp_identity::IdentityComponentID(), componentCont);
        wbp_identity::IIdentityComponent *identity = wb::As<wbp_identity::IIdentityComponent>(identityComponent);

        identity->SetName("Example Entity");
        identity->SetTag(1);
        identity->SetLayer(2);
        identity->SetActiveSelf(true);
    }

    // Output the explanation
    {
        std::string msg = wb::CreateMessage
        ({
            "[WindowsBasePlugin-Game : wbp_identity]",
            "This example demonstrates the IdentityComponent.",
            "An entity with IdentityComponent has been created.",
            "It has a name, tag, layer, and active state.",
            "",
            "Entity ID: " + std::to_string((*entityID)()),
            "",
            "Press Space to output the identity information of the entity."
        });
        wb::ConsoleLog(msg);
    }

#elif defined(EXAMPLE_MODE_FBX_LOADER)

    // Output the explanation
    {
        std::string msg = wb::CreateMessage
        ({
            "[WindowsBasePlugin-Game : wbp_fbx_loader]",
            "This example demonstrates the FBX file loading.",
            "You can see the loaded data to set break points in the debugger.",
            "The file you need to set break points is 'wbp_fbx_loader/src/file_loader_fbx.cpp'.",
        });
        wb::ConsoleLog(msg);
    }

#elif defined(EXAMPLE_MODE_MODEL)
    // Output the explanation
    {
        std::string msg = wb::CreateMessage
        ({
            "[WindowsBasePlugin-Game : wbp_model]",
            "This example demonstrates the ModelAsset loading.",
            "You can see the loaded data to set break points in the debugger.",
            "The file you need to set break points is 'wbp_fbx_loader/src/asset_factory_model.cpp'.",
        });
        wb::ConsoleLog(msg);
    }

#elif defined(EXAMPLE_MODE_RENDER)
    
    // Create a camera entity
    std::unique_ptr<wb::IOptionalValue> cameraEntityID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(entityCont, entityIDView);
        cameraEntityID = entity().GetID().Clone();

        entity().AddComponent(wbp_identity::IdentityComponentID(), componentCont);
        entity().AddComponent(wbp_transform::TransformComponentID(), componentCont);
        entity().AddComponent(wbp_render::CameraComponentID(), componentCont);
    }

    // Initialize the root entity
    {
        wb::IEntity *entity = entityCont.PtrGet(*cameraEntityID);

        wb::IComponent *identityComponent = entity->GetComponent(wbp_identity::IdentityComponentID(), componentCont);
        wbp_identity::IIdentityComponent *identity = wb::As<wbp_identity::IIdentityComponent>(identityComponent);
        identity->SetName("Main Camera");

        wb::IComponent *transformComponent = entity->GetComponent(wbp_transform::TransformComponentID(), componentCont);
        wbp_transform::ITransformComponent *transform = wb::As<wbp_transform::ITransformComponent>(transformComponent);
        transform->SetLocalPosition(DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f));
        transform->SetLocalRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        transform->SetLocalScale(DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f));
    }


    // Output the explanation
    {
        std::string msg = wb::CreateMessage
        ({
            "[WindowsBasePlugin-Game : wbp_render]",
            "This example demonstrates the Render.",
        });
        wb::ConsoleLog(msg);
    }

#else
    std::string msg = wb::CreateMessage
    ({
        "[WindowsBasePlugin-Game]",
        "This is the example application for the WindowsBasePlugin-Game.",
        "There are modes available to demonstrate various features.",
        "Please define one of the modes in 'example/include/mode.h'."
    });
    wb::ConsoleLog(msg);

#endif
}