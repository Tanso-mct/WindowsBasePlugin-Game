#include "example/src/pch.h"
#include "example/include/scene_game_example/asset_group.h"

#include "wbp_fbx_loader/plugin.h"
#pragma comment(lib, "wbp_fbx_loader.lib")

#include "wbp_model/plugin.h"
#pragma comment(lib, "wbp_model.lib")

#if defined(EXAMPLE_MODE_FBX_LOADER)

const size_t &example::MockAssetFactoryID()
{
    static size_t id = wb::IDFactory::CreateAssetFactoryID();
    return id;
}

std::unique_ptr<wb::IAsset> example::MockAssetFactory::Create(wb::IFileData &fileData) const
{
    return nullptr;
}

const size_t &example::MockAssetID()
{
    static size_t id = wb::IDFactory::CreateAssetID();
    return id;
}

namespace example
{
    WB_REGISTER_ASSET(MockAssetID, MockAssetFactoryID(), wbp_fbx_loader::FBXFileLoaderID(), "../resources/example/basic_humanoid.fbx");

} // namespace example

#elif defined(EXAMPLE_MODE_MODEL)

const size_t &example::BasicHumanoidModelAssetID()
{
    static size_t id = wb::IDFactory::CreateAssetID();
    return id;
}

namespace example
{
    WB_REGISTER_ASSET
    (
        BasicHumanoidModelAssetID, wbp_model::ModelAssetFactoryID(), wbp_fbx_loader::FBXFileLoaderID(), 
        "../resources/example/basic_humanoid.fbx"
    );

} // namespace example

#endif



example::GameExampleAssetGroup::GameExampleAssetGroup()
{
#if defined(EXAMPLE_MODE_FBX_LOADER)

    AddAssetID(example::MockAssetID());

#elif defined(EXAMPLE_MODE_MODEL)

    AddAssetID(example::BasicHumanoidModelAssetID());

#endif
}