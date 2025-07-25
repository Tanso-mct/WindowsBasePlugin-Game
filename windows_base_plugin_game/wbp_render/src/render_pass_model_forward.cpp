﻿#include "wbp_render/src/pch.h"
#include "wbp_render/include/render_pass_model_forward.h"

#include "wbp_render/include/system_render.h"
#include "wbp_render/include/component_mesh_renderer.h"

using Microsoft::WRL::ComPtr;

#include <DirectXMath.h>
using namespace DirectX;

#include "directx12_helper/include/d3dx12.h"
#pragma comment(lib, "d3d12.lib")

#include "wbp_d3d12/plugin.h"
#pragma comment(lib, "wbp_d3d12.lib")

#include "wbp_identity/plugin.h"
#pragma comment(lib, "wbp_identity.lib")

#include "wbp_transform/plugin.h"
#pragma comment(lib, "wbp_transform.lib")

#include "wbp_model/plugin.h"
#pragma comment(lib, "wbp_model.lib")

void wbp_render::ModelForwardRenderPass::Initialize(ID3D12CommandAllocator *commandAllocator)
{
    HRESULT hr = E_FAIL;

    // Get gpu context
    wbp_d3d12::GPUContext &gpuContext = wbp_d3d12::GPUContext::GetInstance();
    if (!gpuContext.IsCreated())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "GPU context is not created."
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_RENDER", err);
        wb::ThrowRuntimeError(err);
    }

    /*******************************************************************************************************************
     * Creation of RootSignature
    /******************************************************************************************************************/

    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

    hr = gpuContext.GetDevice()->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
    }

    // テクスチャ用RANGEの作成
    // CD3DX12_DESCRIPTOR_RANGE1 ranges[1];

    // テクスチャのSRVを設定
    // ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

    // SamplerStateの設定
    // D3D12_STATIC_SAMPLER_DESC sampler = {};
    // sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    // sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    // sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    // sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    // sampler.MipLODBias = 0;
    // sampler.MaxAnisotropy = 0;
    // sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    // sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    // sampler.MinLOD = 0.0f;
    // sampler.MaxLOD = D3D12_FLOAT32_MAX;
    // sampler.ShaderRegister = 0;
    // sampler.RegisterSpace = 0;
    // sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;

    const UINT PARAMETER_COUNT = 4;
    CD3DX12_ROOT_PARAMETER1 rootParameters[PARAMETER_COUNT];

    // Set constant buffers
    rootParameters[0].InitAsConstantBufferView
    (
        ROOT_SIGNATURE_VIEW_MATRIX, 0, 
        D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_VERTEX
    );
    rootParameters[1].InitAsConstantBufferView
    (
        ROOT_SIGNATURE_PROJECTION_MATRIX, 0, 
        D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_VERTEX
    );
    rootParameters[2].InitAsConstantBufferView
    (
        ROOT_SIGNATURE_WORLD_MATRIX, 0, 
        D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_VERTEX
    );
    rootParameters[3].InitAsConstantBufferView
    (
        ROOT_SIGNATURE_BONE_MATRIX, 0, 
        D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_VERTEX
    );

    // Set texture descriptor table
    // rootParameters[4].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

    rootSignatureDesc.Init_1_1
    (
        PARAMETER_COUNT, 
        rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    // Integrate parameters to create configuration binaries
    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    hr = D3DX12SerializeVersionedRootSignature
    (
        &rootSignatureDesc, featureData.HighestVersion, signature.GetAddressOf(), error.GetAddressOf()
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to serialize root signature.",
                hrStr.c_str(),
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_RENDER", err);
        wb::ThrowRuntimeError(err);
    }

    // Create ID3D12RootSignature with the binary created by merging.
    hr = gpuContext.GetDevice()->CreateRootSignature
    (
        0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(rootSignature_.GetAddressOf())
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to create root signature.",
                hrStr.c_str(),
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_RENDER", err);
        wb::ThrowRuntimeError(err);
    }

    /*******************************************************************************************************************
     * Creation of PipelineStateObject
    /******************************************************************************************************************/

    struct
    {
        std::unique_ptr<unsigned char[]> data = nullptr;
        fpos_t size = 0;
    } vs, ps;
    vs.data = wb::LoadFileData("../resources/wbp_render/shaders/datas/model_forward_vs.cso", vs.size);
    ps.data = wb::LoadFileData("../resources/wbp_render/shaders/datas/model_forward_ps.cso", ps.size);

    D3D12_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        
        { "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES", 1, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

    psoDesc.InputLayout = { layout, _countof(layout) };
    psoDesc.pRootSignature = rootSignature_.Get();

    psoDesc.VS.BytecodeLength = vs.size;
    psoDesc.PS.BytecodeLength = ps.size;

    psoDesc.VS.pShaderBytecode = vs.data.get();
    psoDesc.PS.pShaderBytecode = ps.data.get();

    // Rasterizer Settings
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    psoDesc.SampleDesc.Count = 1;

    // Blend State Setting
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

    // Manual settings for alpha blending of textures to work
    D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = psoDesc.BlendState.RenderTarget[0];
    defaultRenderTargetBlendDesc.BlendEnable = FALSE;

    // AlphaBlend Settings
    defaultRenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    defaultRenderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    defaultRenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
    defaultRenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    defaultRenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    defaultRenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    defaultRenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // AlphaBlend settings are reflected in all RenderTargets
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    {
        psoDesc.BlendState.RenderTarget[i] = defaultRenderTargetBlendDesc;
    }

    // Enable only the first target
    psoDesc.BlendState.RenderTarget[0].BlendEnable = true;

    // Depth Stencil Settings
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    psoDesc.DepthStencilState.DepthEnable = TRUE;
    psoDesc.DepthStencilState.StencilEnable = TRUE;

    psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

    const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp = // Stencil comparison operation initialization
    { 
        D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP,
        D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS 
    };
    psoDesc.DepthStencilState.FrontFace = defaultStencilOp;
    psoDesc.DepthStencilState.BackFace = defaultStencilOp;

    // Primitive Topology Type
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // Render Target Format
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Create PipelineStateObject
    hr = gpuContext.GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(pipelineState_.GetAddressOf()));
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to create pipeline state object.",
                hrStr.c_str(),
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_RENDER", err);
        wb::ThrowRuntimeError(err);
    }

    /*******************************************************************************************************************
     * Creation of DescriptorHeap
    /******************************************************************************************************************/

    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};

    cbvHeapDesc.NumDescriptors = 1024;

    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    hr = gpuContext.GetDevice()->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(srvHeap_.GetAddressOf()));
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Failed to create descriptor heap.",
                hrStr.c_str(),
            }
        );
        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WBP_RENDER", err);
        wb::ThrowRuntimeError(err);
    }

    /*******************************************************************************************************************
     * Creation of CommandLists
    /******************************************************************************************************************/

    commandLists_.clear();
    commandLists_.resize(wbp_render::RENDER_TARGET_COUNT);
    for (UINT i = 0; i < wbp_render::RENDER_TARGET_COUNT; ++i)
    {
        wbp_d3d12::CreateCommandList
        (
            gpuContext.GetDevice(), commandAllocator,
            commandLists_[i]
        );
    }
}

ID3D12GraphicsCommandList *wbp_render::ModelForwardRenderPass::Execute
(
    const size_t &currentFrameIndex,
    ID3D12Resource* cameraViewMatBuff, ID3D12Resource* cameraProjectionMatBuff,
    const wb::SystemArgument &args
){
    // Get containers to use
    wb::IWindowContainer &windowContainer = args.containerStorage_.GetContainer<wb::IWindowContainer>();
    wb::IAssetContainer &assetContainer = args.containerStorage_.GetContainer<wb::IAssetContainer>();

    // Get the window facade for the current window
    wb::IWindowFacade &window = windowContainer.Get(args.belongWindowID_);
    wbp_d3d12::IWindowD3D12Facade *d3d12Window = wb::As<wbp_d3d12::IWindowD3D12Facade>(&window);

    // Get this frame's command list
    ID3D12GraphicsCommandList *commandList = commandLists_[currentFrameIndex].Get();

    commandList->Reset(d3d12Window->GetCommandAllocator(), pipelineState_.Get());
    d3d12Window->SetBarrierToRenderTarget(commandList);
    d3d12Window->SetRenderTarget(wbp_render::DEPTH_STENCIL_FOR_DRAW, commandList);

    commandList->SetGraphicsRootSignature(rootSignature_.Get());
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ID3D12DescriptorHeap *ppHeaps[] = { srvHeap_.Get() };
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    // Set view and projection matrices by camera's matrices
    commandList->SetGraphicsRootConstantBufferView
    (
        ROOT_SIGNATURE_VIEW_MATRIX,
        cameraViewMatBuff->GetGPUVirtualAddress()
    );
    commandList->SetGraphicsRootConstantBufferView
    (
        ROOT_SIGNATURE_PROJECTION_MATRIX,
        cameraProjectionMatBuff->GetGPUVirtualAddress()
    );

    for (const std::unique_ptr<wb::IOptionalValue> &id : (args.entityIDView_)(wbp_render::MeshRendererComponentID()))
    {
        wb::IEntity *entity = args.entityContainer_.PtrGet(*id);
        if (entity == nullptr) continue; // Skip if entity is null

        wb::IComponent *identityComponent = entity->GetComponent(wbp_identity::IdentityComponentID(), args.componentContainer_);
        wbp_identity::IIdentityComponent *identity = wb::As<wbp_identity::IIdentityComponent>(identityComponent);
        if (identity == nullptr)
        {
            std::string err = wb::CreateErrorMessage
            (
                __FILE__, __LINE__, __FUNCTION__,
                {
                    "Entity does not have IdentityComponent.",
                    "MeshRendererComponent requires IdentityComponent to be set.",
                }
            );
            wb::ConsoleLogErr(err);
            wb::ErrorNotify("WBP_RENDER", err);
            wb::ThrowRuntimeError(err);
        }

        if (!identity->IsActiveSelf())
        {
            // Skip if the entity is not active
            continue;
        }

        wb::IComponent *transformComponent = entity->GetComponent(wbp_transform::TransformComponentID(), args.componentContainer_);
        wbp_transform::ITransformComponent *transform = wb::As<wbp_transform::ITransformComponent>(transformComponent);
        if (transform == nullptr)
        {
            std::string err = wb::CreateErrorMessage
            (
                __FILE__, __LINE__, __FUNCTION__,
                {
                    "Entity does not have TransformComponent.",
                    "MeshRendererComponent requires TransformComponent to be set.",
                }
            );
            wb::ConsoleLogErr(err);
            wb::ErrorNotify("WBP_RENDER", err);
            wb::ThrowRuntimeError(err);
        }

        wb::IComponent *meshRendererComponent = entity->GetComponent(wbp_render::MeshRendererComponentID(), args.componentContainer_);
        wbp_render::IMeshRendererComponent *meshRenderer = wb::As<wbp_render::IMeshRendererComponent>(meshRendererComponent);

        // Update world matrix buffer
        {
            XMMATRIX transposedMat = XMMatrixTranspose(transform->GetWorldMatrix());
            wbp_d3d12::UpdateBuffer
            (
                meshRenderer->GetWorldMatBuffer(),
                &transposedMat, sizeof(XMMATRIX)
            );
        }
        
        commandList->SetGraphicsRootConstantBufferView
        (
            ROOT_SIGNATURE_WORLD_MATRIX,
            meshRenderer->GetWorldMatBuffer()->GetGPUVirtualAddress()
        );

        wb::LockedRef<wb::IAsset> modelAsset = assetContainer.ThreadSafeGet(meshRenderer->GetModelAssetID());
        wbp_model::IModelAsset *model = wb::As<wbp_model::IModelAsset>(&modelAsset());
        if (model == nullptr)
        {
            std::string err = wb::CreateErrorMessage
            (
                __FILE__, __LINE__, __FUNCTION__,
                {
                    "The model which MeshRendererComponent refers to is not a ModelAsset.",
                    "MeshRendererComponent requires ModelAsset to be set.",
                }
            );
            wb::ConsoleLogErr(err);
            wb::ErrorNotify("WBP_RENDER", err);
            wb::ThrowRuntimeError(err);
        }

        for (const wbp_model::MeshGPUData &meshData : model->GetMeshDatas())
        {
            commandList->IASetVertexBuffers(0, 1, &meshData.vertexBufferView);
            commandList->IASetIndexBuffer(&meshData.indexBufferView);

            commandList->DrawIndexedInstanced
            (
                meshData.indexCount, 
                1, // Instance count
                0, // Start index location
                0, // Base vertex location
                0 // Start instance location
            );
        }
    }

    d3d12Window->SetBarrierToPresent(commandList);
    wbp_d3d12::CloseCommand(commandList);

    return commandList;
}