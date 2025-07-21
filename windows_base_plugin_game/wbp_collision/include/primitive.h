﻿#pragma once
#include "wbp_collision/include/dll_config.h"

#include <DirectXMath.h>

namespace wbp_collision
{
    enum class PrimitiveType
    {
        Point,
        Box,
        Ray,
        Size
    };

    class WBP_COLLISION_API IPrimitive
    {
    public:
        virtual ~IPrimitive() = default;
        virtual PrimitiveType GetType() const = 0;
    };

    class WBP_COLLISION_API PrimitivePoint : public IPrimitive
    {
    private:
        DirectX::XMFLOAT3 position_;

    public:
        PrimitivePoint();
        PrimitivePoint(const DirectX::XMFLOAT3 &position);
        virtual ~PrimitivePoint() = default;

        /***************************************************************************************************************
         * IPrimitive implementation
        /**************************************************************************************************************/

        virtual PrimitiveType GetType() const override { return PrimitiveType::Point; }

        /***************************************************************************************************************
         * PrimitivePoint specific methods
        /**************************************************************************************************************/

        const DirectX::XMFLOAT3 &GetPosition() const { return position_; }
        void SetPosition(const DirectX::XMFLOAT3 &position) { position_ = position; }
    };


    class WBP_COLLISION_API PrimitiveAABB : public IPrimitive
    {
    private:
        DirectX::XMFLOAT3 min_;
        DirectX::XMFLOAT3 max_;
        DirectX::XMFLOAT3 center_;
        DirectX::XMFLOAT3 extents_;
        DirectX::XMFLOAT3 size_;

    public:
        PrimitiveAABB();
        PrimitiveAABB(const DirectX::XMFLOAT3 &min, const DirectX::XMFLOAT3 &max);
        PrimitiveAABB(DirectX::XMVECTOR minVec, DirectX::XMVECTOR maxVec);
        virtual ~PrimitiveAABB() = default;

        /***************************************************************************************************************
         * IPrimitive implementation
        /**************************************************************************************************************/

        virtual PrimitiveType GetType() const override { return PrimitiveType::Box; }

        /***************************************************************************************************************
         * PrimitiveAABB specific methods
        /**************************************************************************************************************/

        const DirectX::XMFLOAT3 &GetMin() const { return min_; }
        DirectX::XMVECTOR GetMinVec() const { return XMLoadFloat3(&min_); }
        void SetMin(const DirectX::XMFLOAT3 &min);

        const DirectX::XMFLOAT3 &GetMax() const { return max_; }
        DirectX::XMVECTOR GetMaxVec() const { return XMLoadFloat3(&max_); }
        void SetMax(const DirectX::XMFLOAT3 &max);

        const DirectX::XMFLOAT3 &GetCenter() const { return center_; }
        DirectX::XMVECTOR GetCenterVec() const { return XMLoadFloat3(&center_); }
        void SetCenter(const DirectX::XMFLOAT3 &center);

        const DirectX::XMFLOAT3 &GetExtents() const { return extents_; }
        DirectX::XMVECTOR GetExtentsVec() const { return XMLoadFloat3(&extents_); }
        void SetExtents(const DirectX::XMFLOAT3 &extents);

        const DirectX::XMFLOAT3 &GetSize() const { return size_; }
        DirectX::XMVECTOR GetSizeVec() const { return XMLoadFloat3(&size_); }
        void SetSize(const DirectX::XMFLOAT3 &size);
    };

    constexpr DirectX::XMFLOAT3 INITIAL_DIR = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

    class WBP_COLLISION_API PrimitiveRay : public IPrimitive
    {
    private:
        DirectX::XMFLOAT3 origin_;
        DirectX::XMFLOAT3 direction_;

    public:
        PrimitiveRay();
        PrimitiveRay(const DirectX::XMFLOAT3 &origin, const DirectX::XMFLOAT3 &direction);
        virtual ~PrimitiveRay() = default;

        /***************************************************************************************************************
         * IPrimitive implementation
        /**************************************************************************************************************/

        virtual PrimitiveType GetType() const override { return PrimitiveType::Ray; }

        /***************************************************************************************************************
         * PrimitiveRay specific methods
        /**************************************************************************************************************/

        const DirectX::XMFLOAT3 &GetOrigin() const { return origin_; }
        DirectX::XMVECTOR GetOriginVec() const { return XMLoadFloat3(&origin_); }
        void SetOrigin(const DirectX::XMFLOAT3 &origin) { origin_ = origin; }

        const DirectX::XMFLOAT3 &GetDirection() const { return direction_; }
        DirectX::XMVECTOR GetDirectionVec() const { return XMLoadFloat3(&direction_); }
        void SetDirection(const DirectX::XMFLOAT3 &direction);
    };



} // namespace wbp_collision