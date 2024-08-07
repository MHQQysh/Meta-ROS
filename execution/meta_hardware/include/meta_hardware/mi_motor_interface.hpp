#ifndef META_HARDWARE__MI_MOTOR_INTERFACE_HPP_
#define META_HARDWARE__MI_MOTOR_INTERFACE_HPP_

#include <map>
#include <string>
#include <vector>

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "meta_hardware/motor_network/mi_motor_network.hpp"
#include "meta_hardware/visibility_control.h"
#include "rclcpp/macros.hpp"
#include "rclcpp_lifecycle/state.hpp"

namespace meta_hardware {
class MetaRobotMiMotorNetwork : public hardware_interface::SystemInterface {
  public:
    ~MetaRobotMiMotorNetwork() override;
    TEMPLATES__ROS2_CONTROL__VISIBILITY_PUBLIC
    hardware_interface::CallbackReturn
    on_init(const hardware_interface::HardwareInfo &info) override;

    TEMPLATES__ROS2_CONTROL__VISIBILITY_PUBLIC
    hardware_interface::CallbackReturn
    on_configure(const rclcpp_lifecycle::State &previous_state) override;

    TEMPLATES__ROS2_CONTROL__VISIBILITY_PUBLIC
    std::vector<hardware_interface::StateInterface>
    export_state_interfaces() override;

    TEMPLATES__ROS2_CONTROL__VISIBILITY_PUBLIC
    std::vector<hardware_interface::CommandInterface>
    export_command_interfaces() override;

    TEMPLATES__ROS2_CONTROL__VISIBILITY_PUBLIC
    hardware_interface::CallbackReturn
    on_activate(const rclcpp_lifecycle::State &previous_state) override;

    TEMPLATES__ROS2_CONTROL__VISIBILITY_PUBLIC
    hardware_interface::CallbackReturn
    on_deactivate(const rclcpp_lifecycle::State &previous_state) override;

    TEMPLATES__ROS2_CONTROL__VISIBILITY_PUBLIC
    hardware_interface::return_type
    read(const rclcpp::Time &time, const rclcpp::Duration &period) override;

    TEMPLATES__ROS2_CONTROL__VISIBILITY_PUBLIC
    hardware_interface::return_type
    write(const rclcpp::Time &time, const rclcpp::Duration &period) override;

  private:
    class JointInterfaceData {
      public:
        double command_position;
        double command_velocity;
        double command_effort;
        double state_position;
        double state_velocity;
        double state_effort;
    };
    std::vector<JointInterfaceData> joint_interface_data_;

    class JointMotorInfo {
      public:
        std::string name;
        double mechanical_reduction;
        double offset;
        bool command_pos;
        bool command_vel;
        bool command_eff;
    };
    std::vector<JointMotorInfo>
        joint_motor_info_; // local cache of joint motor info

    std::unique_ptr<MiMotorNetwork> mi_motor_network_;
};

} // namespace meta_hardware

#endif // META_HARDWARE__MI_MOTOR_INTERFACE_HPP_
