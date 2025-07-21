#include "wbp_fbx_loader/src/pch.h"
#include "wbp_fbx_loader/include/file_loader_fbx.h"

#include "wbp_fbx_loader/include/file_data_fbx.h"

#include <DirectXMath.h>
using namespace DirectX;

#include <array>

#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")
using namespace fbxsdk;

const WBP_FBX_LOADER_API size_t &wbp_fbx_loader::FBXFileLoaderID()
{
    static size_t id = wb::IDFactory::CreateFileLoaderID();
    return id;
}

const size_t &wbp_fbx_loader::FBXFileLoader::GetID() const
{
    return wbp_fbx_loader::FBXFileLoaderID();
}

namespace
{

    void ProcessNode(FbxNode* node, wbp_fbx_loader::IFBXFileData& fbxData)
    {
        if (!node) return;

        FbxMesh* mesh = node->GetMesh();
        if (mesh)
        {
            FbxLayerElementMaterial* materialElement = mesh->GetElementMaterial();

            fbxData.GetMeshes().emplace_back();

            // Set mesh name
            fbxData.GetMeshes().back().name_ = node->GetName();

            // Get the global transform of the node
            FbxAMatrix transform = node->EvaluateGlobalTransform();

            FbxVector4* controlPoints = mesh->GetControlPoints();
            int controlPointCount = mesh->GetControlPointsCount();
            for (int idx = 0; idx < controlPointCount; ++idx)
            {
                FbxVector4 pos = transform.MultT(controlPoints[idx]); // Convert
                fbxData.GetMeshes().back().vertices_.emplace_back();
                fbxData.GetMeshes().back().vertices_.back().position_ = XMFLOAT3
                (
                    (float)pos[0],
                    (float)pos[1],
                    (float)pos[2]
                );
            }

            // Get indices
            int* indices = mesh->GetPolygonVertices();
            int polygonCount = mesh->GetPolygonCount();
            int offset = 0;
            for (int pi = 0; pi < polygonCount; ++pi)
            {
                int polySize = mesh->GetPolygonSize(pi);
                if (polySize == 3)
                {
                    fbxData.GetMeshes().back().indices_.emplace_back(static_cast<ULONG>(indices[offset + 0]));
                    fbxData.GetMeshes().back().indices_.emplace_back(static_cast<ULONG>(indices[offset + 1]));
                    fbxData.GetMeshes().back().indices_.emplace_back(static_cast<ULONG>(indices[offset + 2]));
                }

                offset += polySize;

                for (int vi = 0; vi < polySize; ++vi)
                {
                    int ctrlPointIndex = mesh->GetPolygonVertex(pi, vi);

                    // Get the material
                    int materialIndex = materialElement->GetIndexArray().GetAt(pi);
                    FbxSurfaceMaterial* material = node->GetMaterial(materialIndex);

                    // Get material name
                    fbxData.GetMeshes().back().materialName_ = material->GetName();

                    // Get diffuse color
                    FbxProperty diffuseProp = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
                    if (diffuseProp.IsValid())
                    {
                        FbxDouble3 diffuse = diffuseProp.Get<FbxDouble3>();
                        fbxData.GetMeshes().back().vertices_[ctrlPointIndex].color_ = XMFLOAT4
                        (
                            (float)diffuse[0],
                            (float)diffuse[1],
                            (float)diffuse[2],
                            1.0f
                        );
                    }

                    // Get normals
                    FbxVector4 normal;
                    if (mesh->GetPolygonVertexNormal(pi, vi, normal))
                    {
                        // Transform normals with node rotation information (no need to scale or translate)
                        FbxAMatrix transform = node->EvaluateGlobalTransform();
                        FbxAMatrix normalTransform = transform;
                        normalTransform.SetT(FbxVector4(0,0,0,0)); // Parallel shift invalid
                        normal = normalTransform.MultT(normal);
                        normal.Normalize();

                        fbxData.GetMeshes().back().vertices_[ctrlPointIndex].normal_ = XMFLOAT3
                        (
                            (float)normal[0],
                            (float)normal[1],
                            (float)normal[2]
                        );
                    }
                }
            }
        }

        // Child nodes are also processed recursively
        int childCount = node->GetChildCount();
        for (int i = 0; i < childCount; ++i)
        {
            ProcessNode(node->GetChild(i), fbxData);
        }
    }

} // namespace

std::unique_ptr<wb::IFileData> wbp_fbx_loader::FBXFileLoader::Load(std::string_view path)
{
    std::unique_ptr<wbp_fbx_loader::IFBXFileData> fbxFileData = std::make_unique<wbp_fbx_loader::FileDataFBX>();
    HRESULT hr = E_FAIL;

    fbxsdk::FbxManager* fbxManager = nullptr;
    FbxScene* fbxScene = nullptr;
    FbxImporter* fbxImporter = nullptr;
    FbxNode* fbxNode = nullptr;

    fbxManager = fbxsdk::FbxManager::Create();

    fbxImporter = FbxImporter::Create(fbxManager, path.data());
    if (fbxImporter == nullptr)
    {
        fbxImporter->Destroy();
        fbxScene->Destroy();
        fbxManager->Destroy();

        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to create FbxImporter.",
                "File path: " + std::string(path)
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_FBX_LOADER", err);
        wb::ThrowRuntimeError(err);
    }

    fbxScene = FbxScene::Create(fbxManager, path.data());
    if (fbxScene == nullptr)
    {
        fbxImporter->Destroy();
        fbxScene->Destroy();
        fbxManager->Destroy();

        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to create FbxScene.",
                "File path: " + std::string(path)
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_FBX_LOADER", err);
        wb::ThrowRuntimeError(err);
    }

    bool res = fbxImporter->Initialize(path.data());
    if (res == false)
    {
        fbxImporter->Destroy();
        fbxScene->Destroy();
        fbxManager->Destroy();

        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to initialize FbxImporter.",
                "File path: " + std::string(path),
                fbxImporter->GetStatus().GetErrorString()
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_FBX_LOADER", err);
        wb::ThrowRuntimeError(err);
    }

    // sceneにインポート
    if (fbxImporter->Import(fbxScene) == false)
    {
        fbxImporter->Destroy();
        fbxScene->Destroy();
        fbxManager->Destroy();

        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to import FbxScene.",
                "File path: " + std::string(path),
                fbxImporter->GetStatus().GetErrorString()
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_FBX_LOADER", err);
        wb::ThrowRuntimeError(err);
    }

    FbxGeometryConverter converter(fbxManager);
    converter.Triangulate(fbxScene, true);

    int animCount = fbxImporter->GetAnimStackCount();

    FbxAxisSystem dx = FbxAxisSystem::DirectX;
    if (fbxScene->GetGlobalSettings().GetAxisSystem() != dx)
    {
        dx.DeepConvertScene(fbxScene);
    }

    fbxNode = fbxScene->GetRootNode();
    FbxNode* rootNode = fbxScene->GetRootNode();
    if (rootNode)
    {
        ProcessNode(rootNode, *fbxFileData);
    }
    else
    {
        fbxImporter->Destroy();
        fbxScene->Destroy();
        fbxManager->Destroy();

        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to get root node from FbxScene.",
                "File path: " + std::string(path)
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_FBX_LOADER", err);
        wb::ThrowRuntimeError(err);
    }

    fbxImporter->Destroy();
    fbxScene->Destroy();
    fbxManager->Destroy();

    // Cast to IFileData
    std::unique_ptr<wb::IFileData> fileData = wb::UniqueAs<wb::IFileData>(fbxFileData);
    return fileData;
}

namespace wbp_fbx_loader
{
    WB_REGISTER_FILE_LOADER(FBXFileLoaderID(), FBXFileLoader);

} // namespace wbp_fbx_loader