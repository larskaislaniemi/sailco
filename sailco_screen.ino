char _scr_arr_letters[] = { ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~' };
uint8_t _scr_arr_bitmaps[] = { 
0, 0, 0,  // ' '
128, 136, 128,  // '!'
160, 10, 0, // '"'
160, 174, 174,  // '#'
96, 108, 76,  // '$'
128, 66, 40,  // '%'
192, 236, 106,  // '&'
128, 8, 0,  // '''
64, 136, 72,  // '('
128, 68, 132, // ')'
160, 164, 0,  // '*'
0, 228, 4,  // '+'
0, 0, 132,  // ','
0, 224, 0,  // '-'
0, 0, 128,  // '.'
32, 66, 136,  // '/'
96, 170, 202, // '0'
64, 76, 68, // '1'
192, 66, 232, // '2'
192, 66, 194, // '3'
160, 234, 34, // '4'
224, 200, 194,  // '5'
96, 232, 234, // '6'
224, 66, 136, // '7'
224, 234, 234,  // '8'
224, 234, 194,  // '9'
0, 8, 8,  // ':'
0, 4, 132,  // ';'
32, 132, 36,  // '<'
0, 14, 14,  // '='
128, 36, 132, // '>'
224, 66, 64,  // '?'
64, 234, 104, // '@'
64, 234, 170, // 'A'
192, 202, 202,  // 'B'
96, 136, 104, // 'C'
192, 170, 202,  // 'D'
224, 232, 232,  // 'E'
224, 232, 136,  // 'F'
96, 232, 106, // 'G'
160, 234, 170,  // 'H'
224, 68, 228, // 'I'
32, 34, 74, // 'J'
160, 202, 170,  // 'K'
128, 136, 232,  // 'L'
160, 238, 170,  // 'M'
160, 238, 174,  // 'N'
64, 170, 74,  // 'O'
192, 202, 136,  // 'P'
64, 170, 110, // 'Q'
192, 234, 172,  // 'R'
96, 72, 194,  // 'S'
224, 68, 68,  // 'T'
160, 170, 106,  // 'U'
160, 170, 68, // 'V'
160, 234, 174,  // 'W'
160, 74, 170, // 'X'
160, 74, 68,  // 'Y'
224, 66, 232, // 'Z'
224, 136, 232,  // '['
0, 72, 2, // '\'
224, 34, 226, // ']'
64, 10, 0,  // '^'
0, 0, 224,  // '_'
128, 4, 0,  // '`'
0, 108, 234,  // 'a'
128, 172, 202,  // 'b'
0, 134, 104,  // 'c'
32, 166, 106, // 'd'
0, 166, 108,  // 'e'
32, 228, 68,  // 'f'
166, 46, 4, // 'g'
128, 172, 170,  // 'h'
128, 128, 136,  // 'i'
2, 34, 74,  // 'j'
128, 202, 172,  // 'k'
192, 68, 228, // 'l'
0, 238, 174,  // 'm'
0, 172, 170,  // 'n'
0, 164, 74, // 'o'
172, 202, 8,  // 'p'
166, 106, 2,  // 'q'
0, 134, 136,  // 'r'
0, 198, 198,  // 's'
64, 78, 100,  // 't'
0, 170, 106,  // 'u'
0, 170, 78, // 'v'
0, 234, 238,  // 'w'
0, 74, 164, // 'x'
170, 38, 4, // 'y'
0, 110, 236,  // 'z'
96, 132, 100, // '{'
128, 8, 136,  // '|'
192, 36, 196, // '}'
96, 12, 0,  // '~'
};

uint8_t _scr_getBitmapVal(char letter, byte index) {
  uint16_t nletters;
  uint16_t nbitmaps;

  if (index > 2) { Serial.println("index > 2"); return 0; }
  
  nletters = sizeof(_scr_arr_letters) / sizeof(char);
  nbitmaps = sizeof(_scr_arr_bitmaps) / sizeof(uint8_t);
  if (nletters != nbitmaps/3) { Serial.println("nletters != nbitmaps/3"); return 0; }
  
  for (uint16_t i = 0; i < nletters; i++) {
    if (_scr_arr_letters[i] == letter) {
      if (i*3 + index >= nbitmaps) { Serial.println("i*3 + index >= nbitmaps"); return 0; }
      return _scr_arr_bitmaps[i * 3 + index];
    }
  }
  return 0;
}

void scr_init() {
  C_WTB(ADDR_SCR);
  _scr_cmd(0xAE);                             // display off
  _scr_cmd(0xD5); _scr_arg(0x80);             // divide ratio, clock
  _scr_cmd(0xA8); _scr_arg(0x3f);             // multiplex ratio
  _scr_cmd(0x0D); _scr_arg(0x00);             // display offset
  _scr_cmd(0x04);                             // display start line to zero
  _scr_cmd(0x8d); _scr_arg(0x14);             // charge pumup enabled
  _scr_cmd(0x20); _scr_arg(0x02);             // page addressing mode
  _scr_cmd(0xA1);                             // segment remap
  _scr_cmd(0xC8);                             // scan dir reverse
  _scr_cmd(0xDA); _scr_arg(0xDA);             // COM pin HW config
  _scr_cmd(0x81); _scr_arg(0x80);             // contrast
  _scr_cmd(0xD9); _scr_arg(0xF1);             // pre-charge period
  _scr_cmd(0xDB); _scr_arg(0x40);             // VCOMH deselect level
  _scr_cmd(0x2E);                             // deactivate scroll
  _scr_cmd(0xA3); _scr_arg(0); _scr_arg(64);  // loc of fixed/scroll rows
  _scr_cmd(0xA6);                             // no color inversion
  _scr_cmd(0xA4);                             // ram mode
  _scr_cmd(0xAF);                             // display on, normal mode
  C_WTE();

  C_WTB(ADDR_SCR);
  _scr_cmd(0xAE);                             // display off
  _scr_cmd(0x20); _scr_arg(0x00);             // horizontal addressing mode
  _scr_cmd(0x21); _scr_arg(0); _scr_arg(127); // col start/end
  _scr_cmd(0x22); _scr_arg(0); _scr_arg(7);   // page start/end
  _scr_cmd(0xAF);                             // display on, normal mode
  C_WTE();

  // switch on the whole disaply for 0.2 s
  C_WTB(ADDR_SCR); _scr_cmd(0xA5); C_WTE();
  delay(200);
  C_WTB(ADDR_SCR); _scr_cmd(0xA4); C_WTE();
}

void _scr_cmd(uint8_t cmd) {
  _scr_cmd(cmd, false);
}

void _scr_arg(uint8_t cmd) {
  _scr_cmd(cmd, true);
}

void _scr_cmd(uint8_t cmd, bool arg) {
  if (!arg) Wire.write(SCR_CTL_CMD);
  Wire.write(cmd);
}

void _scr_send(uint8_t addr, uint8_t cmd) {
  C_WTB(addr);
  _scr_cmd(cmd);
  C_WTE();
}

void _scr_setMem(Screen* scr, uint16_t i, uint8_t val) {
  if (i >= SCR_MEMSIZE) return;
  scr->mem[i] = val;
  scr->status = scr->status | (1 << (i / SCR_PAGESIZE)); // mark which page was changed
}

uint8_t _scr_getMem(Screen* scr, uint16_t i) {
  if (i >= SCR_MEMSIZE) return 0;
  return scr->mem[i];
}

void scr_drawPixel(Screen* scr, uint8_t x1, uint8_t y1, uint8_t c) {
  // assumes horizontal paging!
  uint8_t ipage, icol, irow, mask;
  uint16_t ibyte;

  if (x1 >= SCR_X || y1 >= SCR_Y) return; 
  ipage = y1 / (SCR_Y / SCR_NPAGES);
  icol = x1;
  irow = y1 - ipage * 8;
  ibyte = ipage * SCR_PAGESIZE + icol;
  mask = 1 << irow;
  if (ibyte > SCR_MEMSIZE || ipage >= SCR_NPAGES) return;
  if (c == SCR_C_WHITE) _scr_setMem(scr, ibyte, _scr_getMem(scr, ibyte) | mask);
  else if (c == SCR_C_BLACK) _scr_setMem(scr, ibyte, _scr_getMem(scr, ibyte) & (mask ^ 0xFF));
  else if (c == SCR_C_INVERT) _scr_setMem(scr, ibyte, _scr_getMem(scr, ibyte) ^ mask);

}

void scr_drawArcCCW(Screen* scr, uint8_t x1, uint8_t y1, uint8_t r, uint8_t a1, uint8_t a2, uint8_t c) {
  scr_drawArc(scr, x1, y1, r, a1, a2, c, 0);
}

void scr_drawArcCW(Screen* scr, uint8_t x1, uint8_t y1, uint8_t r, uint8_t a1, uint8_t a2, uint8_t c) {
  scr_drawArc(scr, x1, y1, r, a1, a2, c, 1);
}

void scr_drawArc(Screen* scr, uint8_t x1, uint8_t y1, uint8_t r, uint8_t a1, uint8_t a2, uint8_t c, uint8_t dir) {
  float dircoef;
  uint8_t x, y, a, prevx, prevy;
  bool first = true;
  if (dir == 0) dircoef = 1.0;
  else dircoef = -1.0;

  for (a = a1; a < a2; a++) {
    y = y1 - (int)(r * sin(dircoef * PI * (float)a / 180.0));
    x = x1 + (int)(r * cos(dircoef * PI * (float)a / 180.0));
    if (x != prevx || y != prevy) {
      scr_drawPixel(scr, x, y, c);
    }
    first = false;
    prevx = x;
    prevy = y;
  }
}

void scr_drawLine(Screen* scr, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t c) {
  scr_drawLine(scr, x1, y1, x2, y2, c, false);
}

void scr_drawLine(Screen* scr, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t c, bool skipFirst) {
  float dist, fract;
  uint16_t steps, x, y, prevx, prevy;
  bool first = true;

  dist = sqrt( sq((float)(x2 - x1)) + sq((float)(y2 - y1)) );
  steps = (int)dist + 1;
  for (uint16_t i = 0; i < steps; i++) {
    fract = ((float)i / (float)steps);
    x = x1 + (int)(fract * (float)(x2 - x1));
    y = y1 + (int)(fract * (float)(y2 - y1));
    if (first || x != prevx || y != prevy) {
      scr_drawPixel(scr, x, y, c);
    }
    first = false;
  }
}

void scr_drawChar(Screen* scr, uint8_t c, char letter) {
  scr_drawChar(scr, c, letter, NULL, NULL);
}

void scr_drawChar(Screen* scr, uint8_t c, char letter, uint8_t* dx, uint8_t* dy) {

  for (byte rowCouple = 0; rowCouple < 3; rowCouple++) {
    uint8_t bits;
    bits = _scr_getBitmapVal(letter, rowCouple);
    
    for (uint8_t ic = 0; ic < 4; ic++) {
      if (bits & (8 >> ic)) {
        scr_drawPixel(scr, scr->cur_x + ic, scr->cur_y + 2*rowCouple, c);
      }
      if (bits & (128 >> ic)) {
        scr_drawPixel(scr, scr->cur_x + ic, scr->cur_y + 2*rowCouple + 1, c);
      }
    }
  }

  if (dx != NULL) *dx = 4;
  if (dy != NULL) *dy = 6;
}

/*
void scr_drawLetter(Screen* scr, uint8_t x1, uint8_t y1, uint8_t c, char letter) {
  uint8_t dx, dy;
  scr_drawLetter(scr, x1, y1, c, letter, &dx, &dy);
}

void findLetterStart(uint8_t* arr, byte loc, uint16_t arrsize) {
  byte counter = 0;
  for (uint16_t i = 0; i < arrsize; i++) {
    if (counter == loc) return i;
    if (arr[i] == 0xFF) counter++;
  }
  return arrsize-1;
}

void scr_drawLetter(Screen* scr, uint8_t x1, uint8_t y1, uint8_t c, char letter, uint8_t* dx, uint8_t* dy) {
  uint8_t nValidLetters = 2;
  char validLetters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };

  // lines
  uint8_t xys[] = {
    SCR_CRD(0,4), SCR_CRD(0,2),   SCR_CRD(1,0), SCR_CRD(1,1),   SCR_CRD(3,1), SCR_CRD(3,4),   SCR_CRD(1,2), SCR_CRD(2,2),   255,  // a
    
    
    
  }; 
  uint16_t xyssize = sizeof(xys)/ sizeof(uint8_t);

  *dx = 0;
  *dy = 0;

  uint16_t pc = 0;
  
  for (uint8_t i = 0; i < nValidLetters; i++) {
    if (validLetters[i] == letter) {
      pc = findLetterStart(xys, i, xyssize);
      if (pc >= xyssize-1) continue;
      for (uint16_t k = pc; k < xyssize-1; k = k + 2) {
        if (xys[k] == 0xFF) break;
        if (xys[k]   > *dx) *dx = xys[k];
        if (xys[k+1] > *dy) *dy = xys[k+1];
        scr_drawLine(scr, x1 + xys[k], y1 + xys[k+1], x1 + xys[k + 2], y1 + xys[k + 3], c);
      }
      break;
    }
  }
}
*/

void scr_print(Screen* scr, uint8_t x1, uint8_t y1, uint8_t c, char* str, uint8_t len) {
  uint8_t dx, dy;

  scr->cur_x = x1;
  scr->cur_y = y1;
  
  for (uint8_t i = 0; i < len; i++) {
    scr_drawChar(scr, c, str[i], &dx, &dy);
    scr->cur_x = scr->cur_x + dx + 1;
  }
  return;
}

void scr_clear(Screen* scr) {
  for (uint16_t i = 0; i < SCR_MEMSIZE; i++) {
    scr->mem[i] = 0;
  }
  scr->status = 0xFF;
  scr->cur_x = 0;
  scr->cur_y = 0;
}

void scr_write(Screen* scr) {
  /*
     write the display ram to display
  */
  byte page;

  uint16_t i, j, ib;
  uint8_t ip, icol;

  for (ib = 0; ib < (SCR_MEMSIZE / SCR_DATABLK); ib++) {
    i = SCR_DATABLK * ib;
    C_WTB(ADDR_SCR);
    Wire.write(SCR_CTL_DAT);
    for (j = 0; j < SCR_DATABLK; j++) {
      Wire.write(_scr_getMem(scr, i + j));
    }
    C_WTE();
  }
  
  // clear status 'unwritten changes'
  scr->status = 0x00;
}

