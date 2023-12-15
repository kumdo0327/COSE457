#ifndef __SIMPLE_COMMAND_H__
#define __SIMPLE_COMMAND_H__

#include "frame_view.h"
#include "object_container.h"


class SimpleCmdBase
{
public:
    SimpleCmdBase() : container(ObjectContainer::getInstance()) {}
    virtual ~SimpleCmdBase() = default;
    virtual void execute() = 0;

protected:
    ObjectContainer& container;
};


class EraseCommand : public SimpleCmdBase
{
public:
    EraseCommand(FrameView* frame_view) : SimpleCmdBase(), frame_view(frame_view) {}
    void execute() override
    {
        container.erase(container.getSelection());
        frame_view->setProperty(nullptr);
    }

private:
    FrameView* frame_view;
};


template <int dx, int dy>
class MoveCommand : public SimpleCmdBase
{
public:
    void execute() override
    {
        auto selection = container.getSelection();
        if (selection)
            selection->move(dx, dy);
    }
};


constexpr int BASE_UNIT = 1;
using LeftMoveCommand = MoveCommand<-BASE_UNIT, 0>;
using UpMoveCommand = MoveCommand<0, -BASE_UNIT>;
using RightMoveCommand = MoveCommand<BASE_UNIT, 0>;
using DownMoveCommand = MoveCommand<0, BASE_UNIT>;

constexpr int BIG_UNIT = 8;
using LeftBigMoveCommand = MoveCommand<-BIG_UNIT, 0>;
using UpBigMoveCommand = MoveCommand<0, -BIG_UNIT>;
using RightBigMoveCommand = MoveCommand<BIG_UNIT, 0>;
using DownBigMoveCommand = MoveCommand<0, BIG_UNIT>;

#endif
