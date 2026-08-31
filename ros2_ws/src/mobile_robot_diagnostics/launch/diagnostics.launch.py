#!/usr/bin/env python3
from pathlib import Path
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description() -> LaunchDescription:
    config_file = Path(get_package_share_directory("mobile_robot_diagnostics")) / "config" / "diagnostics.yaml"
    return LaunchDescription([
        Node(
            package="mobile_robot_diagnostics",
            executable="runtime_monitor",
            name="mobile_robot_runtime_monitor",
            output="screen",
            parameters=[str(config_file)],
        )
    ])
