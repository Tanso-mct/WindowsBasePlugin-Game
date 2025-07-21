#include "wbp_render/src/pch.h"
#include "wbp_render/include/component_mesh_renderer.h"

const WBP_RENDER_API size_t &wbp_render::MeshRendererComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

const size_t &wbp_render::MeshRenderComponent::GetID() const
{
    return MeshRendererComponentID();
}

namespace wbp_render
{
    WB_REGISTER_COMPONENT(MeshRendererComponentID(), MeshRenderComponent);

} // namespace wbp_render