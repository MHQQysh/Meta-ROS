waiting for me to add more




# Meta-ROS

![humble](https://github.com/Meta-Team/Meta-ROS/actions/workflows/ros-humble.yml/badge.svg)

## Basic Info

Created by Yao Xinchen, 2023/7/13. \
Based on ROS2 Humble. \
Tested on Ubuntu 22.04. \
Used for RoboMaster Team Meta to control robots.

This project is hoped to be modular, readable and easy to maintain.

## Doc


[Meta-ROS-Doc Web](https://meta-team.github.io/Meta-ROS-Wiki)







## Structure

4 main layers: Perception -> Decision -> Decomposition -> Execution

## Quickstart

### Clone the Project
First, create a workspace and clone the project.

```Bash
mkdir -p meta_ws/src && cd meta_ws/src
git clone --recurse-submodules https://github.com/Meta-Team/Meta-ROS
```

### Install dependencies
This project is fully compatible with `rosdep`, you can easily install the dependencies with this command

```Bash
rosdep install -y --rosdistro humble --from-paths . --ignore-src
```

> **Note:** You may need to manually install Eigen3 because it cannot be automatically resolved by `rosdep`.

### Compile the Project
Simply run

```Bash
colcon build --symlink-install --cmake-args '-DCMAKE_EXPORT_COMPILE_COMMANDS=On'
```
