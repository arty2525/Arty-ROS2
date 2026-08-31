# Phase 1B Part 18

หัวข้อ: Host UART Resynchronization + Transactional Fault Recovery

เพิ่ม:

- `FrameStreamParser` ฝั่ง ROS 2
- bytewise magic resynchronization
- no RX flush on malformed frame
- fragmented frame support
- multiple frames per serial read
- pending frame queue
- CRC recovery tests
- garbage-prefix recovery tests
- Configure → ClearFaults → Activate lifecycle
