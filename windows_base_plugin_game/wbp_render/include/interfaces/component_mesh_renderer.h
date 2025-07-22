#pragma once
#include "windows_base/windows_base.h"

#include <d3d12.h>

namespace wbp_render
{
    class IMeshRendererComponent : public wb::IComponent
    {
    public:
        virtual ~IMeshRendererComponent() override = default;

        virtual const size_t &GetModelAssetID() const = 0;
        virtual void SetModelAssetID(const size_t &modelAssetID) = 0;

        virtual ID3D12Resource *GetWorldMatBuffer() = 0;
    };

} // namespace wbp_render