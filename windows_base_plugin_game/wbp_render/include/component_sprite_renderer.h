#pragma once
#include "wbp_render/include/dll_config.h"
#include "windows_base/windows_base.h"

#include "wbp_render/include/interfaces/component_sprite_renderer.h"

#include <wrl/client.h>

namespace wbp_render
{
    const WBP_RENDER_API size_t &SpriteRendererComponentID();

    class WBP_RENDER_API SpriteRendererComponent : public ISpriteRendererComponent
    {
    private:
        size_t texture2DAssetID_;
        Microsoft::WRL::ComPtr<ID3D12Resource> worldMatBuffer_;

    public:
        SpriteRendererComponent();
        ~SpriteRendererComponent() override = default;

        /***************************************************************************************************************
         * IComponent implementation
        /**************************************************************************************************************/

        const size_t &GetID() const override;

        /***************************************************************************************************************
         * ISpriteRendererComponent implementation
        /**************************************************************************************************************/

        const size_t &GetTexture2DAssetID() const override { return texture2DAssetID_; }
        void SetTexture2DAssetID(const size_t &texture2DAssetID) override { texture2DAssetID_ = texture2DAssetID; }

        ID3D12Resource *GetWorldMatBuffer() override { return worldMatBuffer_.Get(); }
    };

} // namespace wbp_render