# Phase 1B Part 13

หัวข้อ: Firmware Configurable Motor / Encoder Inversion Foundation

นำ inversion ที่เคย hardcode ใน constructor ออก และเพิ่ม setter สำหรับ motor/encoder พร้อมขยาย `ConfigurePayload` ทั้งฝั่ง ESP32 และ ROS 2 host ให้มี inversion 4 ค่า

Part นี้เตรียม protocol และ firmware ให้พร้อมรับค่า calibration จาก ROS 2 hardware configuration ใน Part ถัดไป
