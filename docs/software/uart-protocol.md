# UART Binary Protocol

UART ระหว่าง Raspberry Pi 4 และ ESP32 ใช้ 115200 baud, 8N1

Frame ประกอบด้วย Magic `0xA55A`, Version `1`, Message Type, Sequence, Payload Size, Payload และ CRC-16/CCITT-FALSE (polynomial `0x1021`, initial `0xFFFF`).

Message types: `0x01` Velocity, `0x02` Enable, `0x03` ClearFaults, `0x04` SetPid, `0x05` Heartbeat, `0x06` Configure, `0x81` Telemetry, `0x82` Ack.

`CommandConfigure` ส่ง `float32 ticks_per_revolution` ซึ่งต้อง finite และมากกว่า 0. ESP32 จะหยุดมอเตอร์ ปิด enable reset encoder baseline และตอบ ACK ด้วย sequence เดียวกันก่อนที่ host จะอนุญาตให้ activate hardware.

ACK payload มี `command_type` และ `status`: 0 Accepted, 1 Invalid payload, 2 Invalid configuration, 3 Rejected by state, 4 Unsupported.
