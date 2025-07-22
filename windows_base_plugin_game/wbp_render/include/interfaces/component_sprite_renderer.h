#pragma once
#include "windows_base/windows_base.h"

#include <d3d12.h>

namespace wbp_render
{
    class ISpriteRendererComponent : public wb::IComponent
    {
    public:
        virtual ~ISpriteRendererComponent() override = default;

        virtual const size_t &GetTexture2DAssetID() const = 0;
        virtual void SetTexture2DAssetID(const size_t &texture2DAssetID) = 0;

        virtual ID3D12Resource *GetWorldMatBuffer() = 0;
    };

} // namespace wbp_render