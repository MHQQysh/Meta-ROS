// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from can_interfaces:msg/PosVelData.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "can_interfaces/msg/detail/pos_vel_data__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace can_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void PosVelData_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) can_interfaces::msg::PosVelData(_init);
}

void PosVelData_fini_function(void * message_memory)
{
  auto typed_message = static_cast<can_interfaces::msg::PosVelData *>(message_memory);
  typed_message->~PosVelData();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember PosVelData_message_member_array[3] = {
  {
    "id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(can_interfaces::msg::PosVelData, id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "p_des",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(can_interfaces::msg::PosVelData, p_des),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "v_des",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(can_interfaces::msg::PosVelData, v_des),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers PosVelData_message_members = {
  "can_interfaces::msg",  // message namespace
  "PosVelData",  // message name
  3,  // number of fields
  sizeof(can_interfaces::msg::PosVelData),
  PosVelData_message_member_array,  // message members
  PosVelData_init_function,  // function to initialize message memory (memory has to be allocated)
  PosVelData_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t PosVelData_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &PosVelData_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace can_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<can_interfaces::msg::PosVelData>()
{
  return &::can_interfaces::msg::rosidl_typesupport_introspection_cpp::PosVelData_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, can_interfaces, msg, PosVelData)() {
  return &::can_interfaces::msg::rosidl_typesupport_introspection_cpp::PosVelData_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
