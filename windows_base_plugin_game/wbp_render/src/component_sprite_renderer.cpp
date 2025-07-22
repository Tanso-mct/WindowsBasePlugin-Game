#include "wbp_render/src/pch.h"
#include "wbp_render/include/component_sprite_renderer.h"

#include <DirectXMath.h>
using namespace DirectX;

#include "wbp_d3d12/plugin.h"
#pragma comment(lib, "wbp_d3d12.lib")

const WBP_RENDER_API size_t &wbp_render::SpriteRendererComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

const size_t &wbp_render::SpriteRendererComponent::GetID() const
{
    return SpriteRendererComponentID();
}

wbp_render::SpriteRendererComponent::SpriteRendererComponent()
{
    // Get gpu context
    wbp_d3d12::GPUContext &gpuContext = wbp_d3d12::GPUContext::GetInstance();
    if (!gpuContext.IsCreated())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "GPU context is not created."
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_RENDER", err);
        wb::ThrowRuntimeError(err);
    }

    wbp_d3d12::CreateBuffer(gpuContext.GetDevice(), worldMatBuffer_, sizeof(XMMATRIX));
}

namespace wbp_render
{
    WB_REGISTER_COMPONENT(SpriteRendererComponentID(), SpriteRendererComponent);

} // namespace wbp_render