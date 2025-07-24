#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    const size_t &MoveCubeSystemID();

    class MoveCubeSystem : public wb::ISystem
    {
    public:
        MoveCubeSystem() = default;
        virtual ~MoveCubeSystem() = default;

        MoveCubeSystem(const MoveCubeSystem &) = delete;
        MoveCubeSystem &operator=(const MoveCubeSystem &) = delete;

        /***************************************************************************************************************
         * ISystem implementation
        /**************************************************************************************************************/

        virtual const size_t &GetID() const override;
        virtual void Initialize(wb::IAssetContainer &assetContainer) override;
        virtual void Update(const wb::SystemArgument &args) override;
    };

} // namespace example