#!/usr/bin/env python3

from __future__ import annotations

import math
import os
import tempfile
from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchContext, LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction, RegisterEventHandler
from launch.event_handlers import OnShutdown
from launch.substitutions import Command, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

_REQUIRED_POSITIVE = (
    "wheel_radius", "wheel_separation", "wheel_width", "base_length",
    "base_width", "base_height", "ticks_per_revolution",
    "max_linear_velocity", "max_linear_acceleration",
    "max_angular_velocity", "max_angular_acceleration",
)
_REQUIRED_BOOLEAN = (
    "left_motor_inverted", "right_motor_inverted",
    "left_encoder_inverted", "right_encoder_inverted",
)


def _float_argument(context: LaunchContext, name: str) -> float:
    try:
        value = float(LaunchConfiguration(name).perform(context))
    except ValueError as error:
        raise RuntimeError(f"Launch argument '{name}' must be numeric") from error
    if not math.isfinite(value) or value <= 0.0:
        raise RuntimeError(f"Launch argument '{name}' must be finite and greater than zero")
    return value


def _boolean_argument(context: LaunchContext, name: str) -> str:
    raw = LaunchConfiguration(name).perform(context).strip().lower()
    if raw not in {"true", "false"}:
        raise RuntimeError(f"Launch argument '{name}' must be true or false")
    return raw


def _positive_integer_argument(context: LaunchContext, name: str) -> int:
    try:
        value = int(LaunchConfiguration(name).perform(context))
    except ValueError as error:
        raise RuntimeError(f"Launch argument '{name}' must be an integer") from error
    if value <= 0:
        raise RuntimeError(f"Launch argument '{name}' must be greater than zero")
    return value


def _create_controller_file(context: LaunchContext, values: dict[str, float]) -> str:
    template = (Path(get_package_share_directory("mobile_robot_control")) / "config" / "controllers.template.yaml").read_text(encoding="utf-8")
    replacements = {
        "__WHEEL_SEPARATION__": values["wheel_separation"],
        "__WHEEL_RADIUS__": values["wheel_radius"],
        "__MAX_LINEAR_VELOCITY__": values["max_linear_velocity"],
        "__MIN_LINEAR_VELOCITY__": -values["max_linear_velocity"],
        "__MAX_LINEAR_ACCELERATION__": values["max_linear_acceleration"],
        "__MAX_LINEAR_DECELERATION__": -values["max_linear_acceleration"],
        "__MAX_LINEAR_ACCELERATION_REVERSE__": -values["max_linear_acceleration"],
        "__MAX_LINEAR_DECELERATION_REVERSE__": values["max_linear_acceleration"],
        "__MAX_ANGULAR_VELOCITY__": values["max_angular_velocity"],
        "__MIN_ANGULAR_VELOCITY__": -values["max_angular_velocity"],
        "__MAX_ANGULAR_ACCELERATION__": values["max_angular_acceleration"],
        "__MAX_ANGULAR_DECELERATION__": -values["max_angular_acceleration"],
        "__MAX_ANGULAR_ACCELERATION_REVERSE__": -values["max_angular_acceleration"],
        "__MAX_ANGULAR_DECELERATION_REVERSE__": values["max_angular_acceleration"],
    }
    for token, replacement in replacements.items():
        template = template.replace(token, f"{replacement:.12g}")
    if "__" in template:
        raise RuntimeError("Unresolved controller configuration token")
    descriptor, path = tempfile.mkstemp(prefix="arty_ros2_controllers_", suffix=".yaml")
    with os.fdopen(descriptor, "w", encoding="utf-8") as stream:
        stream.write(template)
    context.locals["controller_parameter_file"] = path
    return path


def _cleanup(context: LaunchContext, *_) -> None:
    path = context.locals.get("controller_parameter_file")
    if path:
        try:
            os.unlink(path)
        except FileNotFoundError:
            pass


def _launch_setup(context: LaunchContext):
    values = {name: _float_argument(context, name) for name in _REQUIRED_POSITIVE}
    booleans = {name: _boolean_argument(context, name) for name in _REQUIRED_BOOLEAN}
    serial_device = LaunchConfiguration("serial_device").perform(context)
    baud_rate = _positive_integer_argument(context, "baud_rate")
    telemetry_timeout_ms = _positive_integer_argument(context, "telemetry_timeout_ms")
    if not serial_device:
        raise RuntimeError("serial_device must not be empty")

    controller_file = _create_controller_file(context, values)
    description_file = Path(get_package_share_directory("mobile_robot_description")) / "urdf" / "mobile_robot.urdf.xacro"
    robot_description = ParameterValue(
        Command([
            "xacro ", str(description_file),
            " serial_device:=", serial_device,
            " baud_rate:=", str(baud_rate),
            " telemetry_timeout_ms:=", str(telemetry_timeout_ms),
            " ticks_per_revolution:=", str(values["ticks_per_revolution"]),
            " left_motor_inverted:=", booleans["left_motor_inverted"],
            " right_motor_inverted:=", booleans["right_motor_inverted"],
            " left_encoder_inverted:=", booleans["left_encoder_inverted"],
            " right_encoder_inverted:=", booleans["right_encoder_inverted"],
            " wheel_radius:=", str(values["wheel_radius"]),
            " wheel_separation:=", str(values["wheel_separation"]),
            " wheel_width:=", str(values["wheel_width"]),
            " base_length:=", str(values["base_length"]),
            " base_width:=", str(values["base_width"]),
            " base_height:=", str(values["base_height"]),
        ]),
        value_type=str,
    )

    return [
        Node(package="robot_state_publisher", executable="robot_state_publisher", output="screen", parameters=[{"robot_description": robot_description}]),
        Node(package="controller_manager", executable="ros2_control_node", output="screen", parameters=[controller_file], remappings=[("~/robot_description", "/robot_description")]),
        Node(package="controller_manager", executable="spawner", arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager", "--param-file", controller_file, "--unload-on-kill"], output="screen"),
        Node(package="controller_manager", executable="spawner", arguments=["diff_drive_controller", "--controller-manager", "/controller_manager", "--param-file", controller_file, "--unload-on-kill"], output="screen"),
        RegisterEventHandler(OnShutdown(on_shutdown=[OpaqueFunction(function=_cleanup)])),
    ]


def generate_launch_description() -> LaunchDescription:
    arguments = [
        ("serial_device", "/dev/serial0"), ("baud_rate", "115200"),
        ("telemetry_timeout_ms", "500"), ("wheel_radius", "0.0"),
        ("wheel_separation", "0.0"), ("wheel_width", "0.0"),
        ("base_length", "0.0"), ("base_width", "0.0"),
        ("base_height", "0.0"), ("ticks_per_revolution", "0.0"),
        ("left_motor_inverted", "false"), ("right_motor_inverted", "false"),
        ("left_encoder_inverted", "false"), ("right_encoder_inverted", "false"),
        ("max_linear_velocity", "0.0"), ("max_linear_acceleration", "0.0"),
        ("max_angular_velocity", "0.0"), ("max_angular_acceleration", "0.0"),
    ]
    return LaunchDescription([*(DeclareLaunchArgument(name, default_value=value) for name, value in arguments), OpaqueFunction(function=_launch_setup)])
