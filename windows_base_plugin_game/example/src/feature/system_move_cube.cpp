#include "example/src/pch.h"
#include "example/include/feature/system_move_cube.h"

#include "example/include/mode.h"

#include "example/include/feature/component_move_cube.h"

#include <DirectXMath.h>
using namespace DirectX;

#include "wbp_transform/plugin.h"
#pragma comment(lib, "wbp_transform.lib")

#include "wbp_identity/plugin.h"
#pragma comment(lib, "wbp_identity.lib")

#include "wbp_physics/plugin.h"
#pragma comment(lib, "wbp_physics.lib")

const size_t &example::MoveCubeSystemID()
{
    static size_t id = wb::IDFactory::CreateSystemID();
    return id;
}

const size_t &example::MoveCubeSystem::GetID() const
{
    return MoveCubeSystemID();
}

void example::MoveCubeSystem::Initialize(wb::IAssetContainer &assetContainer)
{
    
}

void example::MoveCubeSystem::Update(const wb::SystemArgument &args)
{
    // Get containers to use
    wb::IWindowContainer &windowContainer = args.containerStorage_.GetContainer<wb::IWindowContainer>();
    wb::IMonitorContainer &monitorContainer = args.containerStorage_.GetContainer<wb::IMonitorContainer>();
    wb::IAssetContainer &assetContainer = args.containerStorage_.GetContainer<wb::IAssetContainer>();

    // Get the window facade for the current window
    wb::IWindowFacade &window = windowContainer.Get(args.belongWindowID_);

    // Get the keyboard and mouse monitors
    wb::IKeyboardMonitor *keyboardMonitor = nullptr;
    wb::IMouseMonitor *mouseMonitor = nullptr;
    for (const size_t &monitorID : window.GetMonitorIDs())
    {
        wb::IMonitor &monitor = monitorContainer.Get(monitorID);
        if (keyboardMonitor == nullptr) keyboardMonitor = wb::As<wb::IKeyboardMonitor>(&monitor);
        if (mouseMonitor == nullptr) mouseMonitor = wb::As<wb::IMouseMonitor>(&monitor);
    }

    for (const std::unique_ptr<wb::IOptionalValue> &id : (args.entityIDView_)(example::MoveCubeComponentID()))
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
                    "MoveCubeComponent requires IdentityComponent to be set.",
                }
            );
            wb::ConsoleLogErr(err);
            wb::ErrorNotify("WBP_CONTROLLER", err);
            wb::ThrowRuntimeError(err);
        }

        if (!identity->IsActiveSelf())
        {
            // Skip if the entity is not active
            continue;
        }

        wb::IComponent *moveCubeComponent = entity->GetComponent(example::MoveCubeComponentID(), args.componentContainer_);
        example::IMoveCubeComponent *moveCube = wb::As<example::IMoveCubeComponent>(moveCubeComponent);

        wb::IComponent *rigidBodyComponent = entity->GetComponent(wbp_physics::RigidBodyComponentID(), args.componentContainer_);
        wbp_physics::IRigidBodyComponent *rigidBody = wb::As<wbp_physics::IRigidBodyComponent>(rigidBodyComponent);
        if (rigidBody == nullptr)
        {
            std::string err = wb::CreateErrorMessage
            (
                __FILE__, __LINE__, __FUNCTION__,
                {
                    "Entity does not have RigidBodyComponent.",
                    "MoveCubeComponent requires RigidBodyComponent to be set.",
                }
            );
            wb::ConsoleLogErr(err);
            wb::ErrorNotify("WBP_CONTROLLER", err);
            wb::ThrowRuntimeError(err);
        }

        XMFLOAT3 axis(0.0f, 0.0f, 0.0f);
        float moveValue = moveCube->GetSpeed() * args.deltaTime_;
        if (keyboardMonitor->GetKey(wb::KeyCode::I)) axis.z += 1;
        if (keyboardMonitor->GetKey(wb::KeyCode::K)) axis.z -= 1;
        if (keyboardMonitor->GetKey(wb::KeyCode::L)) axis.x += 1;
        if (keyboardMonitor->GetKey(wb::KeyCode::J)) axis.x -= 1;
        if (keyboardMonitor->GetKey(wb::KeyCode::H)) axis.y += 1;
        if (keyboardMonitor->GetKey(wb::KeyCode::N)) axis.y -= 1;

        if (axis.x != 0.0f || axis.y != 0.0f || axis.z != 0.0f)
        {
            // Normalize the axis
            float length = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
            axis.x /= length;
            axis.y /= length;
            axis.z /= length;

            // Scale the axis by moveValue
            axis.x *= moveValue;
            axis.y *= moveValue;
            axis.z *= moveValue;
        }

        rigidBody->SetVelocity(axis);
    }
}

namespace example
{
    WB_REGISTER_SYSTEM(MoveCubeSystem, MoveCubeSystemID())

} // namespace example