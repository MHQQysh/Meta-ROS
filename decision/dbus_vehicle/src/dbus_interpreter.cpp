#include "dbus_vehicle/dbus_interpreter.h"
#include <cmath>

DbusInterpreter::DbusInterpreter(double max_vel, double max_omega, double aim_sens, double deadzone)
    : max_vel(max_vel), max_omega(max_omega), aim_sens(aim_sens), deadzone(deadzone)
{
    // initialize buttons and axes
    active = false;
    ls_x = ls_y = rs_x = rs_y = wheel = 0;
    lsw = rsw = "";

    // initialize move, shoot, and aim
    move_ = std::make_shared<Move>();
    shoot_ = std::make_shared<Shoot>();
    aim_ = std::make_shared<Aim>();

    // initialize update thread
    update_thread = std::thread([this](){
        while (rclcpp::ok())
        {
            update();
            std::this_thread::sleep_for(std::chrono::milliseconds(PERIOD));
        }
    });
}

DbusInterpreter::~DbusInterpreter()
{
    if (update_thread.joinable()) update_thread.join();
}

void DbusInterpreter::input(const operation_interface::msg::DbusControl::SharedPtr msg)
{
    ls_x = msg->ls_x; apply_deadzone(ls_x); // forward is positive
    ls_y = msg->ls_y; apply_deadzone(ls_y); // left is positive
    rs_x = msg->rs_x; apply_deadzone(rs_x); // up is positive
    rs_y = msg->rs_y; apply_deadzone(rs_y); // left is positive
    wheel = msg->wheel; apply_deadzone(wheel);
    lsw = msg->lsw;
    rsw = msg->rsw;
}

void DbusInterpreter::update()
{
    active = (lsw == "MID") ? true : false;
    move_->vel_x = max_vel * ls_x;
    move_->vel_y = max_vel * ls_y;
    move_->omega = max_omega * wheel;
    aim_->pitch += aim_sens * rs_x * PERIOD / 1000; curb(aim_->pitch, M_PI_4);
    aim_->yaw += aim_sens * rs_y * PERIOD / 1000;
    
    if (rsw == "UP")
    {
        shoot_->fric_state = false;
        shoot_->feed_state = false;
    }
    else if (rsw == "MID")
    {
        shoot_->fric_state = true;
        shoot_->feed_state = false;
    }
    else if (rsw == "DOWN")
    {
        shoot_->fric_state = true;
        shoot_->feed_state = true;
    }
}

void DbusInterpreter::apply_deadzone(double &val)
{
    if (val < deadzone && val > -deadzone)
    {
        val = 0;
    }
}

Move::SharedPtr DbusInterpreter::get_move() const
{
    return move_;
}

Shoot::SharedPtr DbusInterpreter::get_shoot() const
{
    return shoot_;
}

Aim::SharedPtr DbusInterpreter::get_aim() const
{
    return aim_;
}

void DbusInterpreter::curb(double &val, double max_val)
{
    if (val > max_val)
    {
        val = max_val;
    }
    else if (val < -max_val)
    {
        val = -max_val;
    }
}