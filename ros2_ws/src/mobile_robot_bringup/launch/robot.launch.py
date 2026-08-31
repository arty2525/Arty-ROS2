
#!/usr/bin/env python3

from __future__ import annotations

from pathlib import Path

import yaml
from ament_index_python.packages import get_package_share_directory
from launch import LaunchContext, LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    OpaqueFunction,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration


def _load_configuration(path: str) -> dict:
    with Path(path).open("r", encoding="utf-8") as stream:
        data = yaml.safe_load(stream)

    if not isinstance(data, dict):
        raise RuntimeError(
            "robot configuration must be a YAML mapping"
        )

    return data


def _require(
    mapping: dict,
    path: tuple[str, ...],
):
    value = mapping

    for key in path:
        if not isinstance(value, dict) or key not in value:
            raise RuntimeError(
                "Missing robot configuration key: "
                + ".".join(path)
            )

        value = value[key]

    return value


def _require_bool(
    mapping: dict,
    path: tuple[str, ...],
) -> bool:
    value = _require(mapping, path)

    if not isinstance(value, bool):
        raise RuntimeError(
            "Robot configuration key must be boolean: "
            + ".".join(path)
        )

    return value


def _launch_setup(context: LaunchContext):
    config_path = LaunchConfiguration(
        "config"
    ).perform(context)

    configuration = _load_configuration(
        config_path
    )

    control_launch = (
        Path(
            get_package_share_directory(
                "mobile_robot_control"
            )
        )
        / "launch"
        / "mobile_robot_control.launch.py"
    )

    diagnostics_launch = (
        Path(
            get_package_share_directory(
                "mobile_robot_diagnostics"
            )
        )
        / "launch"
        / "diagnostics.launch.py"
    )

    arguments = {
        "serial_device": str(
            _require(
                configuration,
                ("serial_device",),
            )
        ),
        "baud_rate": str(
            _require(
                configuration,
                ("baud_rate",),
            )
        ),
        "telemetry_timeout_ms": str(
            _require(
                configuration,
                (
                    "communication",
                    "telemetry_timeout_ms",
                ),
            )
        ),
        "wheel_radius": str(
            _require(
                configuration,
                ("geometry", "wheel_radius"),
            )
        ),
        "wheel_separation": str(
            _require(
                configuration,
                (
                    "geometry",
                    "wheel_separation",
                ),
            )
        ),
        "wheel_width": str(
            _require(
                configuration,
                ("geometry", "wheel_width"),
            )
        ),
        "base_length": str(
            _require(
                configuration,
                ("geometry", "base_length"),
            )
        ),
        "base_width": str(
            _require(
                configuration,
                ("geometry", "base_width"),
            )
        ),
        "base_height": str(
            _require(
                configuration,
                ("geometry", "base_height"),
            )
        ),
        "caster_ball_radius": str(
            _require(
                configuration,
                (
                    "geometry",
                    "caster_ball_radius",
                ),
            )
        ),
        "front_caster_offset": str(
            _require(
                configuration,
                (
                    "geometry",
                    "front_caster_offset",
                ),
            )
        ),
        "rear_caster_offset": str(
            _require(
                configuration,
                (
                    "geometry",
                    "rear_caster_offset",
                ),
            )
        ),
        "ticks_per_revolution": str(
            _require(
                configuration,
                (
                    "encoder",
                    "ticks_per_revolution",
                ),
            )
        ),
        "left_motor_inverted": (
            "true"
            if _require_bool(
                configuration,
                ("inversion", "left_motor"),
            )
            else "false"
        ),
        "right_motor_inverted": (
            "true"
            if _require_bool(
                configuration,
                ("inversion", "right_motor"),
            )
            else "false"
        ),
        "left_encoder_inverted": (
            "true"
            if _require_bool(
                configuration,
                ("inversion", "left_encoder"),
            )
            else "false"
        ),
        "right_encoder_inverted": (
            "true"
            if _require_bool(
                configuration,
                ("inversion", "right_encoder"),
            )
            else "false"
        ),
        "max_linear_velocity": str(
            _require(
                configuration,
                (
                    "limits",
                    "max_linear_velocity",
                ),
            )
        ),
        "max_linear_acceleration": str(
            _require(
                configuration,
                (
                    "limits",
                    "max_linear_acceleration",
                ),
            )
        ),
        "max_angular_velocity": str(
            _require(
                configuration,
                (
                    "limits",
                    "max_angular_velocity",
                ),
            )
        ),
        "max_angular_acceleration": str(
            _require(
                configuration,
                (
                    "limits",
                    "max_angular_acceleration",
                ),
            )
        ),
    }

    return [
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                str(control_launch)
            ),
            launch_arguments=arguments.items(),
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                str(diagnostics_launch)
            )
        ),
    ]


def generate_launch_description() -> LaunchDescription:
    default_config = (
        Path(
            get_package_share_directory(
                "mobile_robot_bringup"
            )
        )
        / "config"
        / "robot.yaml"
    )

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "config",
                default_value=str(default_config),
            ),
            OpaqueFunction(
                function=_launch_setup
            ),
        ]
    )
