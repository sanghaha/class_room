#include "pch.h"
#include "State.h"

bool StateMachine::OnLeftClickEvent(int32 x, int32 y)
{
    if (_currentState)
    {
        _currentState->OnLeftClickEvent(x, y);
    }
    return false;
}
