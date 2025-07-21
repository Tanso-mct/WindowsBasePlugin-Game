#pragma once
#include "windows_base/windows_base.h"

#include <d3d12.h>

namespace wbp_render
{
    class IRenderPass
    {
    public:
        IRenderPass() = default;
        virtual ~IRenderPass() = default;

        virtual ID3D12GraphicsCommandList* Execute(const wb::SystemArgument &args) = 0;
    };

} // namespace wbp_render