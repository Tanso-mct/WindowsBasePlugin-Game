#pragma once
#include "windows_base/windows_base.h"

#include "example/include/mode.h"

namespace example
{

#if defined(EXAMPLE_MODE_FBX_LOADER)

    const size_t &MockAssetFactoryID();

    class MockAssetFactory : public wb::IAssetFactory
    {
    public:
        MockAssetFactory() = default;
        ~MockAssetFactory() override = default;

        std::unique_ptr<wb::IAsset> Create(wb::IFileData &fileData) const override;
    };

    const size_t &MockAssetID();

#elif defined(EXAMPLE_MODE_MODEL) || defined(EXAMPLE_MODE_RENDER)

    const size_t &CharacterModelAssetID();

#endif

    class GameExampleAssetGroup : public wb::AssetGroup
    {
    public:
        GameExampleAssetGroup();
        ~GameExampleAssetGroup() override = default;
    };

} // namespace example