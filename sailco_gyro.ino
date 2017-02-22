/*
   GYRO SUBROUTINES
*/

//initializes the gyroscope
void gyro_init() {
  /*****************************************
    ITG 3200
    power management set to:
    clock select = internal oscillator
    no reset, no sleep mode
    no standby mode
    sample rate to = 125Hz
    parameter to +/- 2000 degrees/sec
    low pass filter = 5Hz
    no interrupt
  ******************************************/
  C_WTB(ADDR_GYRO);
  Wire.write(GYRO_PWR_MGM); Wire.write(0x00);
  C_WTE();
  C_WTB(ADDR_GYRO);
  Wire.write(GYRO_SMPLRT_DIV); Wire.write(0x07); // EB, 50, 80, 7F, DE, 23, 20, FF
  C_WTE();
  C_WTB(ADDR_GYRO);
  Wire.write(GYRO_DLPF_FS); Wire.write(0x1E); // +/- 2000 dgrs/sec, 1KHz, 1E, 19
  C_WTE();
  C_WTB(ADDR_GYRO);
  Wire.write(GYRO_INT_CFG); Wire.write(0x00);
  C_WTE();
}

void gyro_getData(int16_t* result) {
  /**************************************
    Gyro ITG-3200 I2C
    registers:
    temp MSB = 1B, temp LSB = 1C
    x axis MSB = 1D, x axis LSB = 1E
    y axis MSB = 1F, y axis LSB = 20
    z axis MSB = 21, z axis LSB = 22
  *************************************/
  uint8_t regAddress = 0x1B;
  int16_t temp, x, y, z;
  int16_t buff[GYRO_TO_READ];
  // offsets are chip specific.
  const int16_t gyro_offx = 120;
  const int16_t gyro_offy = 20;
  const int16_t gyro_offz = 93;
  C_WTB(ADDR_GYRO);
  Wire.write(regAddress);
  C_WTE();

  C_WTB(ADDR_GYRO);
  Wire.requestFrom(ADDR_GYRO, GYRO_TO_READ);
  uint8_t i = 0;
  while (Wire.available()) {
    buff[i] = (int16_t)(Wire.read());
    i++;
  }
  C_WTE();
  result[0] = ((buff[2] << 8) | buff[3]) + gyro_offx;
  result[1] = ((buff[4] << 8) | buff[5]) + gyro_offy;
  result[2] = ((buff[6] << 8) | buff[7]) + gyro_offz;
  result[3] = (buff[0] << 8) | buff[1]; // temperature
}
