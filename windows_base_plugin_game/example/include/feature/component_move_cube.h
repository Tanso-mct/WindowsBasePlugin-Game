#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    const size_t &MoveCubeComponentID();

    class IMoveCubeComponent : public wb::IComponent
    {
    public:
        virtual ~IMoveCubeComponent() = default;

        virtual float &GetSpeed() = 0;
    };


    class MoveCubeComponent : public IMoveCubeComponent
    {
    private:
        float speed_ = 1.0f;

    public:
        MoveCubeComponent() = default;
        virtual ~MoveCubeComponent() = default;

        MoveCubeComponent(const MoveCubeComponent &) = delete;
        MoveCubeComponent &operator=(const MoveCubeComponent &) = delete;

        /***************************************************************************************************************
         * IComponent implementation
        /**************************************************************************************************************/

        virtual const size_t &GetID() const override;

        /***************************************************************************************************************
         * IMoveCubeComponent implementation
        /**************************************************************************************************************/

        virtual float &GetSpeed() override { return speed_; }
    };
}