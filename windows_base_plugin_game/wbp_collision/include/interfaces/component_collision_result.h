#pragma once
#include "windows_base/windows_base.h"

#include <DirectXMath.h>

namespace wbp_collision
{
    class ICollisionResultComponent : public wb::IComponent
    {
    public:
        virtual ~ICollisionResultComponent() = default;

        virtual bool IsCollided() const = 0;
        virtual size_t GetCollidedCount() const = 0;

        virtual const wb::IOptionalValue &GetCollidedEntityID(size_t index) const = 0;
        virtual const DirectX::XMFLOAT3 &GetCollidedFaceNormal(size_t index) const = 0;
        virtual bool GetCollidedIsTrigger(size_t index) const = 0;

        virtual void AddCollided
        (
            std::unique_ptr<wb::IOptionalValue> entityID, 
            const DirectX::XMFLOAT3 &normal, bool isTrigger
        ) = 0;
        virtual void ClearCollided() = 0;
    };
}
