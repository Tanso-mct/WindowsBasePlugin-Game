#include "example/src/pch.h"
#include "example/include/feature/component_move_cube.h"

const size_t &example::MoveCubeComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

const size_t &example::MoveCubeComponent::GetID() const
{
    return MoveCubeComponentID();
}

namespace example
{
    WB_REGISTER_COMPONENT(MoveCubeComponentID(), MoveCubeComponent);

} // namespace example