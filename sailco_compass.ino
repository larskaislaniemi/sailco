/*
   COMPASS STUFF

   00 cfg reg a
   01 cfg reg b
   02 mode reg
   03..08 xzy data reg
   09 status reg
   10..12 id reg a..c

   
   cfg reg a: 7    unused
              65   num of samples averaged (00 1, 01 2, 10 4, 11 8)
              432  data output rate (Hz 000 0.75, 001 1.5, 010 3, 011 7.5, 100 [15], 101 30, 110 75, 111 xx)
              10   meas cfg bit (normal, pos bias, neg bias, xx)

              use: 01010000b = 16+64 (4 samples, 15 Hz)

   cfg reg b: 765  gain cfg bits. output range is always -2048..2047; -4096 == overflow

                   000 +- 0.88 Ga, 0.73 mG/LSB
                   001 +- 1.3 Ga,  0.92 mG/LSB (default)
                   ...
                   111 +- 8.1 Ga,  4.35 mg/LSB
                   
              43210 all zeros

              use: 00100000b = 32 = 0x20, 0.92 mg/LSB = 1090 LSB/Ga

   mode reg: default val 0x01
             765432 set pin = high speed I2C
             10     operating mode: 00 continuous measurement
                                    01 single meas
                                    10 idle mode
                                    11 idle mode
                                    
   status reg: 765432 all zeros
               1      lock
               0      rdy (data available)
   
*/

void comp_readAxes(int16_t *x, int16_t *y, int16_t *z) {
  uint8_t inData[6], n;

  if (!_comp_waitReady(false)) {
    //*x = *y = *z = 0;
    return;
  }
  
  C_WTB(ADDR_COMP); Wire.write(0x03); C_WTE(); 

  n = Wire.requestFrom(ADDR_COMP, 6, true);
  if (n != 6) {
    //*x = *y = *z = 0;
    return;
  }

  for (uint8_t i = 0; i < 6; i++) {
    inData[i] = Wire.read();
  }

  *x = (int16_t)((uint16_t)(inData[0] << 8) | (uint16_t)inData[1]);
  *z = (int16_t)((uint16_t)(inData[2] << 8) | (uint16_t)inData[3]);
  *y = (int16_t)((uint16_t)(inData[4] << 8) | (uint16_t)inData[5]);
}

bool _comp_waitReady(bool block) {
  bool compassReady = false;
  uint8_t val;

  while (!compassReady) {
    C_WTB(ADDR_COMP); Wire.write(9); C_WTE();
    Wire.requestFrom(ADDR_COMP, 1, true);
    while (!Wire.available()) { };
    val = Wire.read();
    if (val && (uint8_t)1) compassReady = true;
  }
}

void comp_init() {
  // 00 cfg reg A: 01010000b = 16d + 64d = 0x50 // 4 samples, 15 Hz
  // 01 cfg reg B: 00100000b = 32d = 20h        // 0.92 mg/LSB
  // 02 mode reg:  00000000b = 0d               // continuous meas mode

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

  cres = 1090;   // LSB/Ga
  
  C_WTB(ADDR_COMP); Wire.write(0x00); Wire.write(0x50); C_WTE();
  C_WTB(ADDR_COMP); Wire.write(0x01); Wire.write(0x20); C_WTE();
  C_WTB(ADDR_COMP); Wire.write(0x02); Wire.write(0x00); C_WTE();

  _comp_waitReady(true);
}
