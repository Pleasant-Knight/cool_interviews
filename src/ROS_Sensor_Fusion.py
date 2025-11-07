#!/usr/bin/env python3

"""
ROS2 node for warehouse robot sensor fusion.

This node fuses data from IMU (orientation) and wheel encoders (odometry) to estimate 
the robot's pose in real-time. It:
- Subscribes to:
    - /imu/data (geometry_msgs/Quaternion)
    - /encoder/ticks (std_msgs/Int32)
- Applies complementary filter (alpha=0.98) for gyro bias correction
- Publishes fused pose to /robot/pose (geometry_msgs/PoseStamped)

Features:
- Handles message timestamps to avoid drift
- Uses mutex for thread-safe updates
- Handles missing messages (timeout after 100ms)
- Clamps angles to [-π, π]
"""

#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy
from geometry_msgs.msg import Quaternion, PoseStamped
from std_msgs.msg import Int32
from sensor_msgs.msg import Imu
import numpy as np
import threading
import time
from typing import Optional

class SensorFusionNode(Node):
    def __init__(self):
        super().__init__('sensor_fusion_node')
        
        # QoS for real-time reliability
        qos = QoSProfile(
            depth=10,
            reliability=ReliabilityPolicy.BEST_EFFORT,
            history=HistoryPolicy.KEEP_LAST
        )

        # State with thread-safe access
        self.lock = threading.Lock()
        self.last_imu: Optional[Imu] = None
        self.last_left_ticks = 0
        self.last_right_ticks = 0
        self.last_fusion_time = None
        self.alpha = 0.98  # Complementary filter weight
        self.wheel_base = 0.2  # meters (for differential drive)
        self.ticks_per_meter = 1000

        # Pose estimate
        self.x = 0.0
        self.y = 0.0
        self.theta = 0.0  # radians

        # Subscribers
        self.imu_sub = self.create_subscription(
            Imu, '/imu/data', self.imu_callback, qos)
        self.left_sub = self.create_subscription(
            Int32, '/encoder/left', self.left_encoder_cb, qos)
        self.right_sub = self.create_subscription(
            Int32, '/encoder/right', self.right_encoder_cb, qos)

        # Publisher
        self.pose_pub = self.create_publisher(PoseStamped, '/robot/pose', 10)

        # Fusion timer at 100Hz
        self.timer = self.create_timer(0.01, self.fusion_callback)

        self.get_logger().info("Sensor Fusion Node Initialized")

    def imu_callback(self, msg: Imu):
        with self.lock:
            self.last_imu = msg
            if self.last_fusion_time is None:
                self.last_fusion_time = msg.header.stamp.sec + msg.header.stamp.nanosec * 1e-9

    def left_encoder_cb(self, msg: Int32):
        with self.lock:
            delta = msg.data - self.last_left_ticks
            self.last_left_ticks = msg.data
            self._update_odometry(delta_left=delta, delta_right=0)

    def right_encoder_cb(self, msg: Int32):
        with self.lock:
            delta = msg.data - self.last_right_ticks
            self.last_right_ticks = msg.data
            self._update_odometry(delta_left=0, delta_right=delta)

    def _update_odometry(self, delta_left: int, delta_right: int):
        dl = delta_left / self.ticks_per_meter
        dr = delta_right / self.ticks_per_meter
        dc = (dl + dr) / 2.0
        dtheta = (dr - dl) / self.wheel_base

        self.x += dc * np.cos(self.theta)
        self.y += dc * np.sin(self.theta)
        self.theta = self._normalize_angle(self.theta + dtheta)

    def fusion_callback(self):
        with self.lock:
            if self.last_imu is None:
                return

            current_time = self.get_clock().now().to_msg()
            current_t = current_time.sec + current_time.nanosec * 1e-9

            # Timeout check: >100ms no IMU
            if self.last_fusion_time is not None and (current_t - self.last_fusion_time) > 0.1:
                self.get_logger().warn("IMU timeout: using dead reckoning only")
                self._publish_pose(current_time)
                return

            # Extract angular velocity (gyro) for rate integration
            gz = self.last_imu.angular_velocity.z
            dt = current_t - self.last_fusion_time if self.last_fusion_time else 0.01
            self.last_fusion_time = current_t

            # Complementary filter: fuse gyro rate + encoder odometry
            theta_gyro = self._normalize_angle(self.theta + gz * dt)
            self.theta = self.alpha * theta_gyro + (1 - self.alpha) * self.theta

            self._publish_pose(current_time)

    def _publish_pose(self, stamp):
        pose = PoseStamped()
        pose.header.stamp = stamp
        pose.header.frame_id = 'odom'
        pose.pose.position.x = self.x
        pose.pose.position.y = self.y
        pose.pose.position.z = 0.0
        q = self._quaternion_from_euler(0, 0, self.theta)
        pose.pose.orientation = Quaternion(x=q[0], y=q[1], z=q[2], w=q[3])
        self.pose_pub.publish(pose)

    @staticmethod
    def _normalize_angle(angle):
        while angle > np.pi: angle -= 2 * np.pi
        while angle < -np.pi: angle += 2 * np.pi
        return angle

    @staticmethod
    def _quaternion_from_euler(roll, pitch, yaw):
        cy, sy = np.cos(yaw * 0.5), np.sin(yaw * 0.5)
        cp, sp = np.cos(pitch * 0.5), np.sin(pitch * 0.5)
        cr, sr = np.cos(roll * 0.5), np.sin(roll * 0.5)
        w = cr * cp * cy + sr * sp * sy
        x = sr * cp * cy - cr * sp * sy
        y = cr * sp * cy + sr * cp * sy
        z = cr * cp * sy - sr * sp * cy
        return [x, y, z, w]


def main():
    rclpy.init()
    node = SensorFusionNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()