#include <Arduino.h>
#include <Wire.h>
#include "sailco.h"
#include <math.h>

// 1 = draw all measurements as bars
// 2 = show zyx values for compass/accelerometer
// 3 = draw compass line
// 4 = angle of acc/mag.f. vector vs x/y/z directions
#define DISPLAYMODE 4


// gyro related variables saved from loop to loop
int16_t hx, hy, hz;
int16_t gyro_max_temp, gyro_max_h;

// compass related
int16_t comp_max_c;
int16_t cx, cz, cy;
int16_t cres;         // LSB/Gauss

// accelerometer related
int16_t ax, ay, az;
int16_t accel_max;
int16_t ares;         // LSB/g

unsigned long accel_lastRead;

// screen
Screen* screen0;


void int2str(int16_t num, char *str, uint8_t len) {
  uint8_t rem, i;
  int8_t sign;

  if (num < 0) {
    num = -num;
    sign = -1;
  }
  else sign = 1;

  for (i = len - 1; i > 0; i--) {
    if (num == 0 && i != len - 1) {
      str[i] = (char)32;
    } else {
      rem = num % 10;
      str[i] = (char)(48 + rem);
      num = num / 10;
    }
  }

  if (sign < 0) {
    str[0] = (char)45;
  } else {
    str[0] = (char)43;
  }
}


/*
   SETUP
*/
void setup(void) {
  //Serial.begin(115200);
  Wire.begin();

  accel_lastRead = millis();

  scr_init();
  gyro_init();
  comp_init();
  accel_init();

  screen0 = (Screen*)malloc(sizeof(Screen));

  //scr_clear(screen0);
  //char title[] = "SAILCO";
  //scr_print(screen0, (SCR_X-1)-6*(4+1)-1, 1, SCR_C_WHITE, title, 8);
  //char title1[] = "gyro";
  //scr_print(screen0, 5, 1, SCR_C_WHITE, title1, 4);
  //char title2[] = "comp";
  //scr_print(screen0, 35, 1, SCR_C_WHITE, title, 4);

  gyro_max_temp = 0;
  gyro_max_h = 0;

  comp_max_c = 0;

  accel_max = 0;

  scr_clear(screen0);
  //char axes[] = "Z X Y Z X Y";
  //scr_print(screen0, 2, 0, SCR_C_WHITE, axes, 12);
  //scr_drawLine(screen0, 0, 0, 218, 215, SCR_C_WHITE);
  //scr_write(screen0);
}


/*
   MAIN LOOP
*/
void loop(void) {
  /*
     0x1E GY     +  + HMC5883L
     0x3C OLED  SSD1306
     0x53 GY     + ADXL345 +
     0x68 GY  ITG3205 gyro
  */

  int16_t gyro_res[3];
  uint8_t barx, bary, barh, barsep;

  barsep = 8;
  barh = 27;
  bary = 36;

#if DISPLAYMODE == 1
  scr_drawLine(screen0, 0, bary, 127, bary, SCR_C_WHITE);

  /*
      read and plot gyro data
  */
  gyro_getData(gyro_res);
  hx = gyro_res[0] / 14.375;
  hy = gyro_res[1] / 14.375;
  hz = gyro_res[2] / 14.375;

  if (abs(hx) > gyro_max_h) gyro_max_h = abs(hx);
  if (abs(hy) > gyro_max_h) gyro_max_h = abs(hy);
  if (abs(hz) > gyro_max_h) gyro_max_h = abs(hz);

  barx = barsep / 2;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;

  barx = barsep / 2;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * hx) / gyro_max_h), SCR_C_WHITE); barx += barsep;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * hz) / gyro_max_h), SCR_C_WHITE); barx += barsep;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * hy) / gyro_max_h), SCR_C_WHITE); barx += barsep;

  /*
     read and plot compass data
  */
  comp_readAxes(&cx, &cy, &cz);

  if (abs(cx) > comp_max_c) comp_max_c = abs(cx);
  if (abs(cy) > comp_max_c) comp_max_c = abs(cy);
  if (abs(cz) > comp_max_c) comp_max_c = abs(cz);

  barx = 10 + barsep / 2 + 3 * barsep;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;

  barx = 10 + barsep / 2 + 3 * barsep;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * cx) / comp_max_c), SCR_C_WHITE); barx += barsep;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * cz) / comp_max_c), SCR_C_WHITE); barx += barsep;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * cy) / comp_max_c), SCR_C_WHITE); barx += barsep;

  /*
      read and plot accelerometer data
  */
  accel_getData(&ax, &ay, &az);
  //Serial.print(ax/64); Serial.print("\t");  // 256 LSB/g at full res
  //Serial.print(ay/64); Serial.print("\t");
  //Serial.print(az/64); Serial.print("\t"); Serial.println("");

  if (abs(ax) > accel_max) accel_max = abs(ax);
  if (abs(ay) > accel_max) accel_max = abs(ay);
  if (abs(az) > accel_max) accel_max = abs(az);

  barx = 20 + barsep / 2 + 6 * barsep;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;
  scr_drawLine(screen0, barx, bary + barh, barx, bary - barh, SCR_C_BLACK); barx += barsep;

  barx = 20 + barsep / 2 + 6 * barsep;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * ax) / accel_max), SCR_C_WHITE); barx += barsep;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * az) / accel_max), SCR_C_WHITE); barx += barsep;
  scr_drawLine(screen0, barx, bary, barx, (uint8_t)(bary + (barh * ay) / accel_max), SCR_C_WHITE); barx += barsep;

#elif DISPLAYMODE == 2

  scr_clear(screen0);

  char compNum[6];

  // READ DATA
  comp_readAxes(&cx, &cy, &cz);
  accel_getData(&ax, &ay, &az);


  // PRINT COMPASS DATA AND TITLES
  int2str(cx, compNum, 6);
  scr_print(screen0, 10, 10, SCR_C_WHITE, compNum, 6);
  compNum[0] = 'X';
  scr_print(screen0,  0, 10, SCR_C_WHITE, compNum, 1);

  int2str(cy, compNum, 6);
  scr_print(screen0, 10, 20, SCR_C_WHITE, compNum, 6);
  compNum[0] = 'Y';
  scr_print(screen0,  0, 20, SCR_C_WHITE, compNum, 1);

  int2str(cz, compNum, 6);
  scr_print(screen0, 10, 30, SCR_C_WHITE, compNum, 6);
  compNum[0] = 'Z';
  scr_print(screen0,  0, 30, SCR_C_WHITE, compNum, 1);


  // PRINT ACCEL DATA
  ax = (100 * ax) / ares;
  ay = (100 * ay) / ares;
  az = (100 * az) / ares;

  int2str(ax, compNum, 6);
  scr_print(screen0, 50, 10, SCR_C_WHITE, compNum, 6);

  int2str(ay, compNum, 6);
  scr_print(screen0, 50, 20, SCR_C_WHITE, compNum, 6);

  int2str(az, compNum, 6);
  scr_print(screen0, 50, 30, SCR_C_WHITE, compNum, 6);

#elif DISPLAYMODE == 3
  scr_clear(screen0);

  comp_readAxes(&cx, &cy, &cz);

  cx = (cx * 30) / cres;
  cy = (cy * 30) / cres;

  scr_drawLine(screen0, 63, 31, 63 + cx, 31 + cy, SCR_C_WHITE);

#elif DISPLAYMODE == 4
  double dax, day, daz, dcx, dcy, dcz, xangle, yangle, zangle;
  char compNum[6];
  
  scr_clear(screen0);
  
  comp_readAxes(&cx, &cy, &cz);
  accel_getData(&ax, &ay, &az);

  dax = (double)ax;
  day = (double)ay;
  daz = (double)az;

  dcx = (double)cx;
  dcy = (double)cy;
  dcz = (double)cz;
  
  xangle = 180.0 * acos(dax / sqrt(dax*dax + day*day + daz*daz)) / M_PI;
  yangle = 180.0 * acos(day / sqrt(dax*dax + day*day + daz*daz)) / M_PI;
  zangle = 180.0 * acos(daz / sqrt(dax*dax + day*day + daz*daz)) / M_PI;

  int2str((int16_t)(xangle), compNum, 6);
  scr_print(screen0, 10, 20, SCR_C_WHITE, compNum, 6); 
  int2str((int16_t)(yangle), compNum, 6);
  scr_print(screen0, 10, 30, SCR_C_WHITE, compNum, 6);  
  int2str((int16_t)(zangle), compNum, 6);
  scr_print(screen0, 10, 40, SCR_C_WHITE, compNum, 6);  
  
  xangle = 180.0 * acos(dcx / sqrt(dcx*dcx + dcy*dcy + dcz*dcz)) / M_PI;
  yangle = 180.0 * acos(dcy / sqrt(dcx*dcx + dcy*dcy + dcz*dcz)) / M_PI;
  zangle = 180.0 * acos(dcz / sqrt(dcx*dcx + dcy*dcy + dcz*dcz)) / M_PI;

  int2str((int16_t)(xangle), compNum, 6);
  scr_print(screen0, 50, 20, SCR_C_WHITE, compNum, 6); 
  int2str((int16_t)(yangle), compNum, 6);
  scr_print(screen0, 50, 30, SCR_C_WHITE, compNum, 6);  
  int2str((int16_t)(zangle), compNum, 6);
  scr_print(screen0, 50, 40, SCR_C_WHITE, compNum, 6);
  
#endif


  scr_write(screen0);


  /*
     display print examples
  */
  //scr_drawLetter(screen0, 45, 35, SCR_C_WHITE, 'L');
  //scr_drawArcCW(screen0, 60, 25, 20, 90, 180, SCR_C_WHITE);
  //scr_drawArcCCW(screen0, 60, 25, 20, 90, 180, SCR_C_WHITE);
  //char koe[] = { 'I', 'L', 'S' };
  //scr_print(screen0, 10, 20, SCR_C_WHITE, koe, 3);
  //scr_drawLine(screen0, 63, 63, 20, 0, SCR_C_WHITE);
  //scr_drawLine(screen0, 1, 1, 2, 2, SCR_C_WHITE);
  //scr_write(screen0);

  //scr_clear(screen0);
  //scr_drawLine(screen0, 0, 0, 218, 215, SCR_C_WHITE);

}



