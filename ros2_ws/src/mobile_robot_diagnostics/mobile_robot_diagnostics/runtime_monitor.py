#!/usr/bin/env python3
from __future__ import annotations
from dataclasses import dataclass
from typing import Optional
import rclpy
from diagnostic_msgs.msg import DiagnosticArray, DiagnosticStatus, KeyValue
from nav_msgs.msg import Odometry
from rclpy.node import Node
from sensor_msgs.msg import JointState


@dataclass
class TopicWatch:
    name: str
    timeout_seconds: float
    last_seen_nanoseconds: Optional[int] = None

    def mark(self, now_nanoseconds: int) -> None:
        self.last_seen_nanoseconds = now_nanoseconds

    def age_seconds(self, now_nanoseconds: int) -> Optional[float]:
        if self.last_seen_nanoseconds is None:
            return None
        return (now_nanoseconds - self.last_seen_nanoseconds) / 1_000_000_000.0

    def is_fresh(self, now_nanoseconds: int) -> bool:
        age = self.age_seconds(now_nanoseconds)
        return age is not None and age <= self.timeout_seconds


class RuntimeMonitor(Node):
    def __init__(self) -> None:
        super().__init__("mobile_robot_runtime_monitor")
        self.declare_parameter("publish_rate_hz", 2.0)
        self.declare_parameter("joint_states_timeout_sec", 1.0)
        self.declare_parameter("odom_timeout_sec", 1.0)
        self.declare_parameter("joint_states_topic", "/joint_states")
        self.declare_parameter("odom_topic", "/diff_drive_controller/odom")
        publish_rate_hz = float(self.get_parameter("publish_rate_hz").value)
        joint_timeout = float(self.get_parameter("joint_states_timeout_sec").value)
        odom_timeout = float(self.get_parameter("odom_timeout_sec").value)
        joint_topic = str(self.get_parameter("joint_states_topic").value)
        odom_topic = str(self.get_parameter("odom_topic").value)
        if publish_rate_hz <= 0.0 or joint_timeout <= 0.0 or odom_timeout <= 0.0:
            raise ValueError("diagnostic rates and timeouts must be greater than zero")
        self._joint_watch = TopicWatch(joint_topic, joint_timeout)
        self._odom_watch = TopicWatch(odom_topic, odom_timeout)
        self._publisher = self.create_publisher(DiagnosticArray, "/diagnostics", 10)
        self.create_subscription(JointState, joint_topic, self._on_joint_states, 20)
        self.create_subscription(Odometry, odom_topic, self._on_odometry, 20)
        self.create_timer(1.0 / publish_rate_hz, self._publish)

    def _now(self) -> int:
        return self.get_clock().now().nanoseconds

    def _on_joint_states(self, _: JointState) -> None:
        self._joint_watch.mark(self._now())

    def _on_odometry(self, _: Odometry) -> None:
        self._odom_watch.mark(self._now())

    @staticmethod
    def _status(watch: TopicWatch, now_ns: int) -> DiagnosticStatus:
        age = watch.age_seconds(now_ns)
        status = DiagnosticStatus()
        status.name = f"Arty-ROS2 topic: {watch.name}"
        status.hardware_id = "arty_ros2"
        if age is None:
            status.level = DiagnosticStatus.WARN
            status.message = "ยังไม่เคยได้รับข้อมูล"
            age_text = "never"
        elif watch.is_fresh(now_ns):
            status.level = DiagnosticStatus.OK
            status.message = "ข้อมูลปกติ"
            age_text = f"{age:.3f}"
        else:
            status.level = DiagnosticStatus.ERROR
            status.message = "ข้อมูลขาดหายเกินเวลาที่กำหนด"
            age_text = f"{age:.3f}"
        status.values = [
            KeyValue(key="topic", value=watch.name),
            KeyValue(key="timeout_seconds", value=f"{watch.timeout_seconds:.3f}"),
            KeyValue(key="age_seconds", value=age_text),
        ]
        return status

    def _publish(self) -> None:
        now = self.get_clock().now()
        msg = DiagnosticArray()
        msg.header.stamp = now.to_msg()
        msg.status = [self._status(self._joint_watch, now.nanoseconds), self._status(self._odom_watch, now.nanoseconds)]
        self._publisher.publish(msg)


def main(args=None) -> None:
    rclpy.init(args=args)
    node = RuntimeMonitor()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
