#!/usr/bin/env python3
"""ตรวจสอบค่าคอนฟิกฮาร์ดแวร์ Arty-ROS2 โดยไม่สร้างค่ากายภาพสมมติ."""

from __future__ import annotations

import argparse
import math
from pathlib import Path
import sys
from typing import Any

try:
    import yaml
except ImportError as exc:
    raise SystemExit(
        "ไม่พบ PyYAML: ติดตั้ง dependency ของ ROS 2 workspace ด้วย rosdep ก่อนรันสคริปต์นี้"
    ) from exc


KNOWN_WHEEL_RADIUS_M = 0.0325

POSITIVE_OR_UNRESOLVED = (
    ("geometry", "wheel_separation"),
    ("geometry", "wheel_width"),
    ("geometry", "base_length"),
    ("geometry", "base_width"),
    ("geometry", "base_height"),
    ("geometry", "caster_ball_radius"),
    ("geometry", "front_caster_offset"),
    ("geometry", "rear_caster_offset"),
    ("encoder", "ticks_per_revolution"),
    ("limits", "max_linear_velocity"),
    ("limits", "max_linear_acceleration"),
    ("limits", "max_angular_velocity"),
    ("limits", "max_angular_acceleration"),
)

BOOLEAN_PARAMETERS = (
    ("inversion", "left_motor"),
    ("inversion", "right_motor"),
    ("inversion", "left_encoder"),
    ("inversion", "right_encoder"),
)


def nested_value(config: dict[str, Any], section: str, key: str) -> Any:
    section_value = config.get(section)
    if not isinstance(section_value, dict):
        raise ValueError(f"ไม่พบ section '{section}' หรือรูปแบบไม่ถูกต้อง")
    if key not in section_value:
        raise ValueError(f"ไม่พบพารามิเตอร์ '{section}.{key}'")
    return section_value[key]


def finite_number(value: Any, name: str) -> float:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError(f"'{name}' ต้องเป็นตัวเลข")
    number = float(value)
    if not math.isfinite(number):
        raise ValueError(f"'{name}' ต้องเป็นค่าจำนวนจริงที่ finite")
    return number


def validate(config: dict[str, Any], require_complete: bool) -> list[str]:
    errors: list[str] = []

    try:
        serial_device = config["serial_device"]
        if not isinstance(serial_device, str) or not serial_device.strip():
            errors.append("'serial_device' ต้องเป็นข้อความที่ไม่ว่าง")
    except KeyError:
        errors.append("ไม่พบพารามิเตอร์ 'serial_device'")

    try:
        baud_rate = config["baud_rate"]
        if isinstance(baud_rate, bool) or not isinstance(baud_rate, int) or baud_rate <= 0:
            errors.append("'baud_rate' ต้องเป็นจำนวนเต็มมากกว่า 0")
    except KeyError:
        errors.append("ไม่พบพารามิเตอร์ 'baud_rate'")

    try:
        telemetry_timeout = finite_number(
            nested_value(config, "communication", "telemetry_timeout_ms"),
            "communication.telemetry_timeout_ms",
        )
        if telemetry_timeout <= 0:
            errors.append("'communication.telemetry_timeout_ms' ต้องมากกว่า 0")
    except ValueError as exc:
        errors.append(str(exc))

    try:
        wheel_radius = finite_number(
            nested_value(config, "geometry", "wheel_radius"),
            "geometry.wheel_radius",
        )
        if not math.isclose(
            wheel_radius,
            KNOWN_WHEEL_RADIUS_M,
            rel_tol=0.0,
            abs_tol=1e-12,
        ):
            errors.append(
                "'geometry.wheel_radius' ต้องเท่ากับ 0.0325 m "
                "ตามล้อขับเส้นผ่านศูนย์กลาง 65 mm ที่ยืนยันแล้ว"
            )
    except ValueError as exc:
        errors.append(str(exc))

    unresolved: list[str] = []
    for section, key in POSITIVE_OR_UNRESOLVED:
        name = f"{section}.{key}"
        try:
            number = finite_number(nested_value(config, section, key), name)
            if number < 0:
                errors.append(f"'{name}' ห้ามติดลบ")
            elif number == 0:
                unresolved.append(name)
        except ValueError as exc:
            errors.append(str(exc))

    for section, key in BOOLEAN_PARAMETERS:
        name = f"{section}.{key}"
        try:
            value = nested_value(config, section, key)
            if not isinstance(value, bool):
                errors.append(f"'{name}' ต้องเป็น boolean true/false")
        except ValueError as exc:
            errors.append(str(exc))

    if require_complete and unresolved:
        errors.append(
            "ยังมีค่ากายภาพ/ขีดจำกัดที่ยังไม่ยืนยัน: " + ", ".join(unresolved)
        )

    return errors


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "ตรวจสอบ robot.yaml โดยยอมให้ค่า 0.0 หมายถึงยังไม่ได้วัด "
            "หรือบังคับให้ทุกค่ากายภาพถูกยืนยันแล้ว"
        )
    )
    parser.add_argument("config", type=Path)
    parser.add_argument(
        "--require-complete",
        action="store_true",
        help="ให้ค่า 0.0 ในพารามิเตอร์กายภาพ/limits เป็นข้อผิดพลาด",
    )
    args = parser.parse_args()

    if not args.config.is_file():
        print(f"ERROR: ไม่พบไฟล์ {args.config}", file=sys.stderr)
        return 2

    try:
        with args.config.open("r", encoding="utf-8") as stream:
            loaded = yaml.safe_load(stream)
    except yaml.YAMLError as exc:
        print(f"ERROR: YAML ไม่ถูกต้อง: {exc}", file=sys.stderr)
        return 2

    if not isinstance(loaded, dict):
        print("ERROR: root ของ robot.yaml ต้องเป็น mapping", file=sys.stderr)
        return 2

    errors = validate(loaded, args.require_complete)
    if errors:
        for error in errors:
            print(f"ERROR: {error}", file=sys.stderr)
        return 1

    print("robot.yaml validation: PASS")
    if not args.require_complete:
        print(
            "หมายเหตุ: ค่า 0.0 ยังอนุญาตให้ใช้แทนค่าที่ยังไม่ได้วัด "
            "แต่ production launch ต้องปฏิเสธการเริ่มระบบจนกว่าจะยืนยันครบ"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
