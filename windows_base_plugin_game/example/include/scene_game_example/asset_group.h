#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class GameExampleAssetGroup : public wb::AssetGroup
    {
    public:
        GameExampleAssetGroup();
        ~GameExampleAssetGroup() override = default;
    };

} // namespace example