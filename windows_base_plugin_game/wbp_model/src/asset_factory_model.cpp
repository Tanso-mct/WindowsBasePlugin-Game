#include "wbp_model/src/pch.h"
#include "wbp_model/include/asset_factory_model.h"

const WBP_MODEL_API size_t &wbp_model::ModelAssetFactoryID()
{
    static size_t id = wb::IDFactory::CreateAssetFactoryID();
    return id;
}

std::unique_ptr<wb::IAsset> wbp_model::ModelAssetFactory::Create(wb::IFileData &fileData) const
{
    return nullptr;
}

namespace wbp_model
{
    WB_REGISTER_ASSET_FACTORY(ModelAssetFactoryID(), ModelAssetFactory)

} // namespace wbp_model