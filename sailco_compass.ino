/*
   COMPASS STUFF
*/

int16_t comp_readAxis(uint8_t axis) {
  // X 0, Z 1, Y 2
  uint8_t reg;
  uint8_t val1, val2;
  int16_t retval;

  reg = 3 + axis * 2;
  val1 = (int16_t)_comp_read(reg);
  val2 = (int16_t)_comp_read(reg + 1);

  retval = (int16_t)((uint16_t)(val1 << 8) | (uint16_t)val2);

  return retval;
}

uint8_t _comp_read(uint8_t reg) {
  C_WTB(ADDR_COMP); Wire.write(reg); C_WTE();
  Wire.requestFrom(ADDR_COMP, 1, true);
  while (!Wire.available()) { };
  return Wire.read();
}

void _comp_waitReady() {
  bool compassReady = false;
  uint8_t val;

  while (!compassReady) {
    val = _comp_read(9);
    if (val && (uint8_t)1) compassReady = true;
  }
}

void comp_init() {
  // 00 cfg reg A: 01110000b = 16d + 32d + 64d = 70h
  // 01 cfg reg B: 00100000b = 32d = 20h
  // 02 mode reg:  00000001b = 1d          // these are: single meas mode, default params (see specs)

  // 03, 04 data X, A&B (A MSB, B LSB) // two's complement form
  // 05, 06 data Z, A&B (A MSB, B LSB) // two's complement form
  // 07, 08 data Y, A&B (A MSB, B LSB) // two's complement form
  // 09 status reg: 000000LRb, L = LOCK, R = READY
  // 10 id reg A: 01001000b
  // 11 id reg B: 00110100b
  // 12 id reg C: 00110011b

  // data addr: 0x3C (writing)
  //            0x3D (reading)
  // I2C format: send data address

  C_WTB(ADDR_COMP); Wire.write(0x00); Wire.write(0x70); C_WTE();
  C_WTB(ADDR_COMP); Wire.write(0x01); Wire.write(0x20); C_WTE();
  C_WTB(ADDR_COMP); Wire.write(0x02); Wire.write(0x00); C_WTE();

  _comp_waitReady();
}
