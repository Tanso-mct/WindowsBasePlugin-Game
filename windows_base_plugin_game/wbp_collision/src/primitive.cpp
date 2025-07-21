#include "wbp_collision/src/pch.h"
#include "wbp_collision/include/primitive.h"

using namespace DirectX;

namespace
{
    void UpdateCenter
    (
        XMFLOAT3 &center,
        const XMFLOAT3 &min, const XMFLOAT3 &max
    ){
        center.x = min.x + (max.x - min.x) * 0.5f;
        center.y = min.y + (max.y - min.y) * 0.5f;
        center.z = min.z + (max.z - min.z) * 0.5f;
    }

    void UpdateExtents
    (
        XMFLOAT3 &extents,
        const XMFLOAT3 &min, const XMFLOAT3 &max
    ){
        extents.x = (max.x - min.x) * 0.5f;
        extents.y = (max.y - min.y) * 0.5f;
        extents.z = (max.z - min.z) * 0.5f;
    }

    void UpdateMin
    (
        XMFLOAT3 &min,
        const XMFLOAT3 &center, const XMFLOAT3 &extents
    ){
        min.x = center.x - extents.x;
        min.y = center.y - extents.y;
        min.z = center.z - extents.z;
    }

    void UpdateMax
    (
        XMFLOAT3 &max,
        const XMFLOAT3 &center, const XMFLOAT3 &extents
    ){
        max.x = center.x + extents.x;
        max.y = center.y + extents.y;
        max.z = center.z + extents.z;
    }

} // namespace

wbp_collision::PrimitivePoint::PrimitivePoint()
{
    position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

wbp_collision::PrimitivePoint::PrimitivePoint(const XMFLOAT3 &position)
{
    position_ = position;
}

wbp_collision::PrimitiveAABB::PrimitiveAABB()
{
    min_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    max_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    center_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    extents_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

wbp_collision::PrimitiveAABB::PrimitiveAABB(const XMFLOAT3 &min, const XMFLOAT3 &max)
{
    min_ = min;
    max_ = max;
    UpdateCenter(center_, min_, max_);
    UpdateExtents(extents_, min_, max_);
}

void wbp_collision::PrimitiveAABB::SetMin(const XMFLOAT3 &min)
{
    min_ = min;
    UpdateCenter(center_, min_, max_);
    UpdateExtents(extents_, min_, max_);
}

void wbp_collision::PrimitiveAABB::SetMax(const XMFLOAT3 &max)
{
    max_ = max;
    UpdateCenter(center_, min_, max_);
    UpdateExtents(extents_, min_, max_);
}

void wbp_collision::PrimitiveAABB::SetCenter(const XMFLOAT3 &center)
{
    center_ = center;
    UpdateMin(min_, center_, extents_);
    UpdateMax(max_, center_, extents_);
}

void wbp_collision::PrimitiveAABB::SetExtents(const XMFLOAT3 &extents)
{
    extents_ = extents;
    UpdateMin(min_, center_, extents_);
    UpdateMax(max_, center_, extents_);
}

wbp_collision::PrimitiveRay::PrimitiveRay()
{
    origin_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    direction_ = INITIAL_DIR;
}

wbp_collision::PrimitiveRay::PrimitiveRay(const XMFLOAT3 &origin, const XMFLOAT3 &direction)
{
    origin_ = origin;

    XMVECTOR dirVec = XMVector3Normalize(XMLoadFloat3(&direction));
    XMStoreFloat3(&direction_, dirVec);
}

void wbp_collision::PrimitiveRay::SetDirection(const DirectX::XMFLOAT3 &direction)
{
    XMVECTOR dirVec = XMVector3Normalize(XMLoadFloat3(&direction));
    XMStoreFloat3(&direction_, dirVec);
}