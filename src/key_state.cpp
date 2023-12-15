#include "key_state.h"


void KeyStateBase::back(KeyContext* context) const
{
    EraseCommand(context->getFrameView()).execute();
}


void NonKeyState::shift(KeyContext* context) const
{
    context->setState(std::make_unique<ShiftState>());
}