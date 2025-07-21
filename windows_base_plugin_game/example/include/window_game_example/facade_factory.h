#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    constexpr UINT RENDER_TARGET_COUNT = 2;
    constexpr UINT DEPTH_STENCIL_COUNT = 1;

    class GameExampleWindowFacadeFactory : public wb::IWindowFacadeFactory
    {
    public:
        GameExampleWindowFacadeFactory() = default;
        ~GameExampleWindowFacadeFactory() override = default;

        std::unique_ptr<wb::IWindowFacade> Create() const override;
    };

} // namespace example