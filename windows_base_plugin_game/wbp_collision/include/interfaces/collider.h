﻿#pragma once

namespace wbp_collision
{
    constexpr bool DEFAULT_COLLISION_ENABLED = true;
    constexpr bool DEFAULT_IS_TRIGGER = false;

    class ICollider
    {
    public:
        virtual ~ICollider() = default;

        // Sets whether or not collision judgments are made.
        virtual void SetCollisionEnabled(bool enabled) = 0;
        virtual bool IsCollisionEnabled() const = 0;

        // Whether the movement vector changes when hit
        virtual void SetTrigger(bool isTrigger) = 0;
        virtual bool IsTrigger() const = 0;
    };

} // namespace wbp_collision