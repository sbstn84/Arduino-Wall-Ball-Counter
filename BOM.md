# Bill of materials – one target

Prices as of 21 Nov 2026, incl. VAT, without shipping. Other shops work too; the order numbers are from reichelt.de.

| Qty | Part | Order no. | Shop | Price |
|---|---|---|---|---|
| 1 | Arduino UNO R4 WiFi (ABX00087) | ARD UNO R4 WIFI | [reichelt](https://www.reichelt.de/de/de/shop/produkt/arduino_uno_rev4_wifi_ra4m1_esp32-s3-353108) | 25.00 € |
| 1 | Seeed Grove Base Shield V2.0 (103030000) | ARD SHD BASE V2 | [reichelt](https://www.reichelt.de/de/de/shop/produkt/arduino_shield_-_base_shield_v2-191204) | 5.30 € |
| 2 | Grove 3-axis accelerometer ADXL345, ±16 g | GRV 3AXIS ACC16 | [reichelt](https://www.reichelt.de/de/de/arduino-grove-3-achen-beschleunigungsmesser-16g-digital-a-grv-3axis-acc16-p191206.html) | 2 × 9.45 € |
| 1 | Grove cable 4-pin, 50 cm, buckled (5-pack, you need 2) | GRV CABLE4PIN50F | [reichelt](https://www.reichelt.de/de/de/shop/produkt/arduino_-_grove_universal-kabel_4-pin_50cm_fixiert_5er-pack_-191171) | 3.75 € |
| 1 | Grove Wrapper 1×1, yellow (4-pack, you need 2) | – | [BerryBase](https://www.berrybase.de/seeed-grove-wrapper-1x1-gelb-4er-pack) | 1.90 € |
| 1 | KKSB enclosure for Arduino UNO R4, aluminium/polycarbonate, incl. screws | KKSB CASE UNO R4 | [reichelt](https://www.reichelt.de/de/de/shop/produkt/gehaeuse_fuer_arduino_uno_r4_alu_poly_schwarz-386756) | 13.20 € |
| | **Total (parts above)** | | | **≈ 68 €** |

## Also needed (not priced)
- **Power supply** for the Arduino: a plain DC adapter (7–12 V, 5.5 × 2.1 mm barrel plug, centre positive) into the DC socket of the case, **or** any USB-C charger/cable into the USB-C port. Please check the Arduino documentation for the exact limits.
- **Strong double-sided tape** to mount the wrappers on the back of the target.
- A fine file or sandpaper (to flatten one side of each wrapper, see the README).
- Cable ties or tape to guide the sensor cables (optional).

## Wiring
- Base Shield switch on **5V**
- Sensor O (top) → any **I2C** socket
- Sensor U (bottom) → socket **D6**

See [docs/wiring.png](docs/wiring.png).
