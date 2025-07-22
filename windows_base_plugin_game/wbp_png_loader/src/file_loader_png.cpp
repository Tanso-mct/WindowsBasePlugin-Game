#include "wbp_png_loader/src/pch.h"
#include "wbp_png_loader/include/file_loader_png.h"

#include "wbp_png_loader/include/file_data_png.h"

#pragma comment(lib, "DirectXTex.lib")

const WBP_PNG_LOADER_API size_t &wbp_png_loader::PNGFileLoaderID()
{
    static size_t id = wb::IDFactory::CreateFileLoaderID();
    return id;
}

const size_t &wbp_png_loader::PNGFileLoader::GetID() const
{
    return wbp_png_loader::PNGFileLoaderID();
}

std::unique_ptr<wb::IFileData> wbp_png_loader::PNGFileLoader::Load(std::string_view path)
{
    std::unique_ptr<wbp_png_loader::IPNGFileData> pngFileData = std::make_unique<wbp_png_loader::FileDataPNG>();

    HRESULT hr = DirectX::LoadFromWICFile
    (
        std::wstring(path.begin(), path.end()).c_str(),
        DirectX::WIC_FLAGS_NONE, &pngFileData->GetMetadata(), pngFileData->GetImage()
    );

    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to load PNG file.",
                "File path: " + std::string(path),
                "HRESULT: " + hrStr
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_PNG_LOADER", err);
        wb::ThrowRuntimeError(err);
    }

    // Cast to IFileData
    std::unique_ptr<wb::IFileData> fileData = wb::UniqueAs<wb::IFileData>(pngFileData);
    return fileData;
}

namespace wbp_png_loader
{
    WB_REGISTER_FILE_LOADER(PNGFileLoaderID(), PNGFileLoader);

} // namespace wbp_png_loader