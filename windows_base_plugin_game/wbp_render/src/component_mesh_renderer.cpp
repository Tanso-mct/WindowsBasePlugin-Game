#include "wbp_render/src/pch.h"
#include "wbp_render/include/component_mesh_renderer.h"

const size_t &wbp_render::MeshRendererComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

namespace wbp_render
{
    WB_REGISTER_COMPONENT(MeshRendererComponentID(), MeshRenderComponent);

} // namespace wbp_render