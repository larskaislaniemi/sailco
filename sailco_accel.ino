/*
 * stream mode: latest 32 samples in the FIFO
 * watermark interrupt once fifo has more samples than defined by "samples bits" of FIFO_CTL register
 * 
 * read from fifo:
 * x, y, z 0x32..0x37 (DATAX, DATAY, DATAZ)
 * these hold _oldest_ values
 * 
 * 0x1E..0x20 offsets x, y, z
 *   . 15.6 mg/LSB
 * 0x38 FIFO_CTL
 *   . D7-D6: fifo mode: 00b = no fifo; 01b = FIFO; 10b = stream; 11b = Trigger
 *   . D5: trigger = 0b
 *   . D4-D0: samples: in stream mode: how many fifo entries are needed to trigger watermark interrupt
 * 0x39 FIFO_STATUS
 *   . D7: fifo trig: 1b = trigger event has occurred
 *   . D6: 0
 *   . D5-D0: entries: how many data values stored in fifo
 * 0x2C BW_RATE (data rate, power mode)
 *   . D7-D5: 0
 *   . D4: LOW_POWER (0b = normal, 1b = low power)
 *   . D3-D0: Rate
 *   . Rate codes (docs Table 7): bits 1010b = 100 Hz
 *   . default 0x0A (100 Hz)
 * 0x2D POWER_CTL
 *   . D7-D6: 0
 *   . D5: Link bit = 0b
 *   . D4: AUTO_SLEEP = 0b
 *   . D3: Measure = 1b (INIT!)
 *   . D2: Sleep = 0b
 *   . D1-D0: Wakeup 00b
 * 0x31 DATA_FORMAT
 *   . D7: Self test: 1b==self test force, 0b==no self test force applied
 *   . D6: SPI: = 0 (spi mode)
 *   . D5: int invert = = 0b (active high)
 *   . D4: 0 
 *   . D3: full res: 1b == output res <-> g range set by range bits --> scale factor = 4mg/LSB
 *                   0b == 10bit mode, range bits determine max g range and scale factor
 *   . D2: justify: 1b == MSB mode
 *                  0b == right justified mode, sign extension
 *   . D1-D0: range: 00b +- 2g
 *                   01b +- 4g
 *                   10b +- 8g
 *                   11b +- 16g
 *                   
 * Notes:
 *   Access to collect the data from FIFO is provided through the
 *   DATAX, DATAY, and DATAZ registers. FIFO reads must be
 *   done in burst or multiple-byte mode because each FIFO level is
 *   cleared after any read (single- or multiple-byte) of FIFO.
 *   
 *   single-byte write: reg addr | data
 *   multi-byte write: reg addr | data | data | ...
 *   single-byte read: reg addr | data
 *   multi-byte read: reg addr | data | data | ...
 *   
 */

void accel_init() {
  /* switch off measurements */
  C_WTB(ADDR_ACCE); Wire.write(0x2D); Wire.write(0); C_WTE();
  
  /* offsets */
  C_WTB(ADDR_ACCE); Wire.write(0x1E); Wire.write(0x00); C_WTE();
  C_WTB(ADDR_ACCE); Wire.write(0x1F); Wire.write(0x00); C_WTE();
  C_WTB(ADDR_ACCE); Wire.write(0x20); Wire.write(0x00); C_WTE();  // val = 64 => 15.6mg*64 = 1g -> adjust z axis to neutral

  /* fifo mode: stream; 16 fifo entries cause interrupt */
  C_WTB(ADDR_ACCE); Wire.write(0x38); Wire.write(128+16); C_WTE();

  /* normal power mode, ACCEL_RATE = 100 Hz */
  C_WTB(ADDR_ACCE); Wire.write(0x2C); Wire.write(10); C_WTE();

  /* full res, right justified, +- 16g */ 
  C_WTB(ADDR_ACCE); Wire.write(0x31); Wire.write(8+3); C_WTE();
  
  /* set to measure mode */
  C_WTB(ADDR_ACCE); Wire.write(0x2D); Wire.write(8); C_WTE();
}

void accel_getData(int16_t *x, int16_t *y, int16_t *z) {
  /* wait until accelerometer has gathered ACCEL_NSAMPLES samples
   *  with the current measurement rate */
  while (millis() < accel_lastRead + ACCEL_NSAMPLES*1000/ACCEL_RATE) {};
  accel_lastRead = millis();
  
  uint8_t acceldata[6], bytecount;

  *x = *y = *z = 0;
  
  for (uint8_t ns = 0; ns < ACCEL_NSAMPLES; ns++) {
    C_WTB(ADDR_ACCE);
    Wire.write(0x32);
    C_WTE();
    
    bytecount = Wire.requestFrom(ADDR_ACCE, 6);
    
    if (bytecount != 6) {
      Serial.println("Accelerometer does not give data!");
      return;
    }
    
    for (uint8_t i = 0; i < 6; i++) {
      acceldata[i] = Wire.read();
    }
    
    *x += (int16_t)((uint16_t)acceldata[0] | ((uint16_t)acceldata[1] << 8));
    *y += (int16_t)((uint16_t)acceldata[2] | ((uint16_t)acceldata[3] << 8));
    *z += (int16_t)((uint16_t)acceldata[4] | ((uint16_t)acceldata[5] << 8));
    
    if (ns < ACCEL_NSAMPLES-1) delayMicroseconds(5); // accelerometer wants 5 µs pause between reads
  }  
  
  *x /= ACCEL_NSAMPLES;
  *y /= ACCEL_NSAMPLES;
  *z /= ACCEL_NSAMPLES;
  
  return;
}

