#ifndef __KEY_STATE_H__
#define __KEY_STATE_H__

#include "simple_command.h"


class KeyContext;
class KeyStateBase
{
public:
    virtual ~KeyStateBase() = default;
    virtual void left() const = 0;
    virtual void up() const = 0;
    virtual void right() const = 0;
    virtual void down() const = 0;
    virtual void back(KeyContext* context) const;
    virtual void shift(KeyContext* context) const {}
};


class NonKeyState : public KeyStateBase
{
public:
    void left() const override  { LeftMoveCommand().execute(); }
    void up() const override    { UpMoveCommand().execute(); }
    void right() const override { RightMoveCommand().execute(); }
    void down() const override  { DownMoveCommand().execute(); }
    void shift(KeyContext* context) const override;
};


class ShiftState : public KeyStateBase
{
public:
    void left() const override  { LeftBigMoveCommand().execute(); }
    void up() const override    { UpBigMoveCommand().execute(); }
    void right() const override { RightBigMoveCommand().execute(); }
    void down() const override  { DownBigMoveCommand().execute(); }
};




class KeyContext
{
public:
    KeyContext(FrameView* frame_view) : _state(std::make_unique<NonKeyState>()), _frame_view(frame_view) {}
    void setState(std::unique_ptr<KeyStateBase> state)  { _state = std::move(state); }
    FrameView* getFrameView() const                     { return _frame_view; }

public:
    void left()     { _state->left(); }
    void up()       { _state->up(); }
    void right()    { _state->right(); }
    void down()     { _state->down(); }
    void back()     { _state->back(this); }
    void shift()    { _state->shift(this); }

private:
    std::unique_ptr<KeyStateBase> _state;
    FrameView* _frame_view;
};
#endif
