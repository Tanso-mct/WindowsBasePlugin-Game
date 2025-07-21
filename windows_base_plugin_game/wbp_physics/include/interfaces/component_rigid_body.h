#pragma once
#include "windows_base/windows_base.h"

#include <DirectXMath.h>

namespace wbp_physics
{
    class IRigidBodyComponent : public wb::IComponent
    {
    public:
        virtual ~IRigidBodyComponent() = default;

        virtual const bool &IsKinematic() const = 0;
        virtual void SetKinematic(const bool &kinematic) = 0;

        virtual const DirectX::XMFLOAT3 &GetInitialPos() = 0;
        virtual void SetInitialPos(const DirectX::XMFLOAT3 &pos) = 0;

        virtual const DirectX::XMFLOAT3 &GetTerminalPos() = 0;
        virtual void SetTerminalPos(const DirectX::XMFLOAT3 &pos) = 0;

        virtual const DirectX::XMFLOAT3 &GetVelocity() = 0;
        virtual void SetVelocity(const DirectX::XMFLOAT3 &velocity) = 0;
    };

} // namespace wbp_physics