#include "referee_serial/robot_state.hpp"
#include <bitset>
#include <cstdint>
#include <vector>

bool RobotState::is_wanted_pre(const std::vector<uint8_t> &prefix)
{
    if (prefix[0] != 0xA5) return false;

    uint16_t length = static_cast<uint16_t>(prefix[1]) | (static_cast<uint16_t>(prefix[2]) << 8);
    uint16_t cmd_id = static_cast<uint16_t>(prefix[5]) | (static_cast<uint16_t>(prefix[6]) << 8);

    if (cmd_id != 0x0201 || length != 27) return false;
    return true;
}

RobotState::RobotState(const std::vector<uint8_t> &frame)
{
    // copy the uint8_t vector to the struct
    std::copy(frame.begin(), frame.end(), reinterpret_cast<uint8_t*>(&interpreted));
}

operation_interface::msg::RobotState RobotState::msg()
{
    operation_interface::msg::RobotState msg;
    msg.robot_id = interpreted.data.robot_id;
    msg.robot_level = interpreted.data.robot_level;
    msg.remain_hp = interpreted.data.remain_HP;
    msg.max_hp = interpreted.data.max_HP;
    msg.shooter_id1_17mm_cooling_rate = interpreted.data.shooter_id1_17mm_cooling_rate;
    msg.shooter_id1_17mm_cooling_limit = interpreted.data.shooter_id1_17mm_cooling_limit;
    msg.shooter_id1_17mm_speed_limit = interpreted.data.shooter_id1_17mm_speed_limit;
  
    msg.shooter_id2_17mm_cooling_rate = interpreted.data.shooter_id2_17mm_cooling_rate;
    msg.shooter_id2_17mm_cooling_limit = interpreted.data.shooter_id2_17mm_cooling_limit;
    msg.shooter_id2_17mm_speed_limit = interpreted.data.shooter_id2_17mm_speed_limit;
  
    msg.shooter_id1_42mm_cooling_rate = interpreted.data.shooter_id1_42mm_cooling_rate;
    msg.shooter_id1_42mm_cooling_limit = interpreted.data.shooter_id1_42mm_cooling_limit;
    msg.shooter_id1_42mm_speed_limit = interpreted.data.shooter_id1_42mm_speed_limit;

    msg.chassis_power_limit = interpreted.data.chassis_power_limit;
    msg.mains_power_gimbal_output = interpreted.data.mains_power_gimbal_output;
    msg.mains_power_chassis_output = interpreted.data.mains_power_chassis_output;
    msg.mains_power_shooter_output = interpreted.data.mains_power_shooter_output;
    return msg;
}
