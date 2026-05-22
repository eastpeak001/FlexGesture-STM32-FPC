# FlexGesture V2 Key Parts

This is not a complete BOM. It lists the key components that should be checked carefully before ordering and assembly.

| Part | Use | Package / Notes | Critical |
| --- | --- | --- | --- |
| ESP32-WROOM-32E-N8 | Main controller, BLE, firmware execution | ESP32 module; antenna keep-out required | Yes |
| LSM6DSOTR | 6-axis IMU | LGA sensor; I2C address expected `0x6A` when SDO/SA0 = GND | Yes |
| CH340C | USB-UART bridge | Used for serial flashing/debug | Yes |
| TP4055, LCSC C28441 | Single-cell Li-ion charger | PROG resistor set to 10kΩ for small pouch cell | Yes |
| TPS63031DSKR | 3.3V buck-boost regulator | Keep inductor and capacitors close | Yes |
| B5819W | USB power-path diode | VIN_5V to SYS_IN path | Yes |
| AO3401A | P-MOS battery power path | Manually verify G/S/D footprint mapping | Yes |
| MSK12C02 | Slide/power switch | Manually verify common terminal | Yes |
| Type-C, C5187472 | USB power and serial connection | Board-edge connector | Yes |
| 4.7kΩ resistors | I2C SDA/SCL pull-ups | Pull up to 3V3 | Yes |
| 10kΩ resistors | BOOT / EN / TP4055 PROG related resistors | Check each function against schematic | Yes |
| 100nF capacitors | Local decoupling | Place close to IC power pins | Yes |
| 10uF capacitors | Bulk/local filtering | Power rail stability | Yes |
| 22uF capacitors | TPS63031 input/output bulk capacitance | Follow regulator layout guidance | Yes |
| 1.5uH inductor | TPS63031 power inductor | Saturation current and DCR matter | Yes |

Assembly notes:

- LSM6DSOTR is a small package and should be assembled with suitable SMT process or controlled hot-plate/hot-air reflow.
- ESP32 antenna area must not be covered by copper, routing, battery or metal.
- Battery polarity and connector/pad labels must be checked before connecting the cell.
