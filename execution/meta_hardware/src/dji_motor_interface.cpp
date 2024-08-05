#include <iostream>
#include <limits>
#include <memory>
#include <vector>

#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "meta_hardware/dji_motor_interface.hpp"
#include "meta_hardware/motor_network/dji_motor_network.hpp"
#include "rclcpp/rclcpp.hpp"

namespace meta_hardware {
using hardware_interface::HW_IF_EFFORT;
using hardware_interface::HW_IF_POSITION;
using hardware_interface::HW_IF_VELOCITY;

MetaRobotDjiMotorNetwork::~MetaRobotDjiMotorNetwork() {
    on_deactivate(rclcpp_lifecycle::State());
}

hardware_interface::CallbackReturn MetaRobotDjiMotorNetwork::on_init(
    const hardware_interface::HardwareInfo &info) {
    if (hardware_interface::SystemInterface::on_init(info) !=
        CallbackReturn::SUCCESS) {
        return CallbackReturn::ERROR;
    }

    joint_interface_data_.resize(info_.joints.size());
    joint_motors_info_.resize(info_.joints.size());

    return CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MetaRobotDjiMotorNetwork::on_configure(
    const rclcpp_lifecycle::State & /*previous_state*/) {
    std::string can_network_name =
        info_.hardware_parameters.at("can_network_name");

    dji_motor_network_ = std::make_unique<DjiMotorNetwork>(can_network_name);

    // Add the motors to the motor networks
    for (size_t i = 0; i < info_.joints.size(); ++i) {
        const auto &joint = info_.joints[i];
        const auto &joint_params = joint.parameters;

        dji_motor_network_->add_motor(i, joint_params);
        joint_motors_info_[i].name = info_.joints[i].name;
        joint_motors_info_[i].mechanical_reduction =
            std::stod(info_.joints[i].parameters.at("mechanical_reduction"));
    }

    return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface>
MetaRobotDjiMotorNetwork::export_state_interfaces() {
    std::vector<hardware_interface::StateInterface> state_interfaces;

    // Helper function to check if the interface exists
    auto contains_interface =
        [](const std::vector<hardware_interface::InterfaceInfo> &interfaces,
           const std::string &interface_name) {
            return std::find_if(
                       interfaces.begin(), interfaces.end(),
                       [&interface_name](
                           const hardware_interface::InterfaceInfo &interface) {
                           return interface.name == interface_name;
                       }) != interfaces.end();
        };

    for (size_t i = 0; i < info_.joints.size(); ++i) {
        const auto &joint_state_interfaces = info_.joints[i].state_interfaces;
        if (contains_interface(joint_state_interfaces, "position")) {
            state_interfaces.emplace_back(
                info_.joints[i].name, HW_IF_POSITION,
                &joint_interface_data_[i].state_position);
        }
        if (contains_interface(joint_state_interfaces, "velocity")) {
            state_interfaces.emplace_back(
                info_.joints[i].name, HW_IF_VELOCITY,
                &joint_interface_data_[i].state_velocity);
        }
        if (contains_interface(joint_state_interfaces, "effort")) {
            state_interfaces.emplace_back(
                info_.joints[i].name, HW_IF_EFFORT,
                &joint_interface_data_[i].state_effort);
        }
    }

    return state_interfaces;
}

std::vector<hardware_interface::CommandInterface>
MetaRobotDjiMotorNetwork::export_command_interfaces() {
    std::vector<hardware_interface::CommandInterface> command_interfaces;

    // Helper function to check if the interface exists
    auto contains_interface =
        [](const std::vector<hardware_interface::InterfaceInfo> &interfaces,
           const std::string &interface_name) {
            return std::find_if(
                       interfaces.begin(), interfaces.end(),
                       [&interface_name](
                           const hardware_interface::InterfaceInfo &interface) {
                           return interface.name == interface_name;
                       }) != interfaces.end();
        };

    for (size_t i = 0; i < info_.joints.size(); ++i) {
        const auto &joint_command_interfaces =
            info_.joints[i].command_interfaces;
        if (contains_interface(joint_command_interfaces, "position")) {
            command_interfaces.emplace_back(
                info_.joints[i].name, HW_IF_POSITION,
                &joint_interface_data_[i].command_position);
            joint_motors_info_[i].command_pos = true;
        }
        if (contains_interface(joint_command_interfaces, "velocity")) {
            command_interfaces.emplace_back(
                info_.joints[i].name, HW_IF_VELOCITY,
                &joint_interface_data_[i].command_velocity);
            joint_motors_info_[i].command_vel = true;
        }
        if (contains_interface(joint_command_interfaces, "effort")) {
            command_interfaces.emplace_back(
                info_.joints[i].name, HW_IF_EFFORT,
                &joint_interface_data_[i].command_effort);
            joint_motors_info_[i].command_eff = true;
        }
    }

    return command_interfaces;
}

hardware_interface::CallbackReturn MetaRobotDjiMotorNetwork::on_activate(
    const rclcpp_lifecycle::State & /*previous_state*/) {

    return CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MetaRobotDjiMotorNetwork::on_deactivate(
    const rclcpp_lifecycle::State & /*previous_state*/) {

    return CallbackReturn::SUCCESS;
}

hardware_interface::return_type
MetaRobotDjiMotorNetwork::read(const rclcpp::Time & /*time*/,
                               const rclcpp::Duration & /*period*/) {

    for (size_t i = 0; i < joint_motors_info_.size(); ++i) {
        auto [position, velocity, effort] = dji_motor_network_->read(i);

        position /= joint_motors_info_[i].mechanical_reduction;
        velocity /= joint_motors_info_[i].mechanical_reduction;
        effort *= joint_motors_info_[i].mechanical_reduction;

        joint_interface_data_[i].state_position = position;
        joint_interface_data_[i].state_velocity = velocity;
        joint_interface_data_[i].state_effort = effort;
    }

    return hardware_interface::return_type::OK;
}

hardware_interface::return_type
MetaRobotDjiMotorNetwork::write(const rclcpp::Time & /*time*/,
                                const rclcpp::Duration & /*period*/) {

    for (size_t i = 0; i < joint_motors_info_.size(); ++i) {
        double effort = joint_interface_data_[i].command_effort;

        effort /= joint_motors_info_[i].mechanical_reduction;

        // Check if the command is valid
        // If a command interface exists, the command must not be NaN
        if (joint_motors_info_[i].command_eff && std::isnan(effort)) {
            continue;
        }

        // Write the command to the motor network
        dji_motor_network_->write(i, effort);
    }

    // Some motor network implementations require a separate tx() call
    dji_motor_network_->tx();

    return hardware_interface::return_type::OK;
}

} // namespace meta_hardware

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(meta_hardware::MetaRobotDjiMotorNetwork,
                       hardware_interface::SystemInterface)
