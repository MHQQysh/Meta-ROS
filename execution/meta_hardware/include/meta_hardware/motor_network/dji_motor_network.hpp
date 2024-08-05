#ifndef META_HARDWARE__MOTOR_NETWORK__DJI_MOTOR_NETWORK_HPP_
#define META_HARDWARE__MOTOR_NETWORK__DJI_MOTOR_NETWORK_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "meta_hardware/motor_driver/dji_motor_driver.hpp"
#include <CanDriver.hpp>
#include <CanMessage.hpp>

namespace meta_hardware {

class DjiMotorNetwork {
  public:
    explicit DjiMotorNetwork(std::string can_network_name);
    ~DjiMotorNetwork();

    /**
     * @brief Add a DJI motor to the DJI motor network
     * @param motor_model The model name of the motor (GM6020, M3508, M2006)
     * @param dji_motor_id The DJI motor ID (this ID is not unique between
     * different models)
     * @param joint_id This can be basically anything, but it has to be unique.
     * The name comes from ros2_control, where motors are identified by
     * joint_id. But you may use any other unique number.
     */
    void
    add_motor(uint32_t joint_id,
              const std::unordered_map<std::string, std::string> &motor_params);

    /**
     * @brief Read the motor feedback
     * @param joint_id The joint ID of the motor
     * @return A tuple of (position, velocity, effort)
     */
    std::tuple<double, double, double> read(uint32_t joint_id) const;

    /**
     * @brief Write the motor command
     * @param joint_id The joint ID of the motor
     * @param effort The effort to write
     */
    void write(uint32_t joint_id, double effort);

    /**
     * @brief Transmit the motor commands
     */
    void tx();

  private:
    [[noreturn]] void rx_loop();
    std::thread rx_thread_;

    // Five CAN frames for tx
    // 0x1FE: GM6020(current) motor 1-4
    // 0x1FF: M3508/M2006 motor 5-8 / GM6020(voltage) motor 1-4
    // 0x200: M3508/M2006 motor 1-4
    // 0x2FE: GM6020(current) motor  5-8
    // 0x2FF: GM6020(voltage) motor 5-8
    can_frame tx_frame_1fe{
        .can_id = 0x1FE, .can_dlc = 8, .data = {0, 0, 0, 0, 0, 0, 0, 0}};
    can_frame tx_frame_1ff{
        .can_id = 0x1FF, .can_dlc = 8, .data = {0, 0, 0, 0, 0, 0, 0, 0}};
    can_frame tx_frame_200{
        .can_id = 0x200, .can_dlc = 8, .data = {0, 0, 0, 0, 0, 0, 0, 0}};
    can_frame tx_frame_2fe{
        .can_id = 0x2FE, .can_dlc = 8, .data = {0, 0, 0, 0, 0, 0, 0, 0}};
    can_frame tx_frame_2ff{
        .can_id = 0x2FF, .can_dlc = 8, .data = {0, 0, 0, 0, 0, 0, 0, 0}};

    // CAN driver
    std::unique_ptr<sockcanpp::CanDriver> can_driver_;

    // [rx_can_id] -> dji_motor
    // This makes it easy to find the motor object in rx_loop
    std::map<uint32_t, std::shared_ptr<DjiMotor>> rx_id2motor_;

    // [joint_id] -> dji_motor
    // This makes it easy to find the motor object in read() and write()
    std::map<uint32_t, std::shared_ptr<DjiMotor>> joint_id2motor_;
};

} // namespace meta_hardware

#endif // META_HARDWARE__MOTOR_NETWORK__DJI_MOTOR_NETWORK_HPP_
