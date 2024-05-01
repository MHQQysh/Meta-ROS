#ifndef DBUS_INTERPRETER_H
#define DBUS_INTERPRETER_H

#include "rclcpp/rclcpp.hpp"

#include "operation_interface/msg/dbus_control.hpp"
#include "behavior_interface/msg/move.hpp"

using behavior_interface::msg::Move;
using operation_interface::msg::DbusControl;

class DbusInterpreter
{
public:
    DbusInterpreter(double vel, double aim_sens);

    ~DbusInterpreter();

    void dbus_input(const DbusControl::SharedPtr dbus_msg);

    Move get_move() { return *move_msg_; }

    bool is_active() { return active; }

private:
    bool active;
    Move::SharedPtr move_msg_;

    double max_vel;
    double aim_sens;

    double last_op;
    std::thread timeout_thread;

    void check_timeout();
};

#endif // DBUS_INTERPRETER_H