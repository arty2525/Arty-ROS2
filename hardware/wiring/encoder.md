# การต่อ Quadrature Encoder

| Encoder | ESP32 |
|---|---:|
| ซ้าย A | GPIO32 |
| ซ้าย B | GPIO33 |
| ขวา A | GPIO34 |
| ขวา B | GPIO35 |
| GND | GND |
| VCC | ตาม datasheet ของ Encoder |

ก่อนต่อ ต้องยืนยันระดับแรงดันเอาต์พุต Encoder ด้วย datasheet หรือเครื่องมือวัด

- 3.3V push-pull สามารถต่อโดยตรง
- open-collector ใช้ pull-up ภายนอกไปยัง 3.3V
- 5V ต้องใช้ level shifter หรือตัวแบ่งแรงดันที่เหมาะสม
- GPIO34 และ GPIO35 ไม่มี internal pull-up
