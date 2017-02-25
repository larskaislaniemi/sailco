/*
 * COMMON STUFF
 */
#define C_WTB(ADDR) Wire.beginTransmission(ADDR)
#define C_WTE() Wire.endTransmission()

#define ADDR_SCR  0x3C
#define ADDR_GYRO 0x68
#define ADDR_COMP 0x1E
#define ADDR_ACCE 0x53

/*
 * SCREEN RELATED
 */
//#define SCR_CRD(x,y) ((uint8_t)(x & y << 4))
#define SCR_CTL_CMD (byte)(0x00)
#define SCR_CTL_DAT (byte)(0x40)

#define SCR_C_WHITE 1
#define SCR_C_BLACK 0
#define SCR_C_INVERT 2

#define SCR_X 128
#define SCR_Y 64
#define SCR_BIT 1
#define SCR_MEMSIZE 64*128/8 // bytes
#define SCR_PAGESIZE 128 // bytes
#define SCR_NPAGES 8
#define SCR_DATABLK 16 // SCR_PAGESIZE must be multiple of this; 4 <= SCR_DATABLK < 32

struct Screen {
  uint8_t mem[SCR_MEMSIZE]; // screen memory
  uint8_t status;           // bits: pages that have unwritten changes (0~7)
  uint8_t cur_x, cur_y;
};

/*
 * GYRO RELATED
 */
#define GYRO_SMPLRT_DIV 0x15
#define GYRO_DLPF_FS 0x16
#define GYRO_INT_CFG 0x17
#define GYRO_PWR_MGM 0x3E
#define GYRO_TO_READ 8 // 2 bytes for each axis x, y, z


/*
 * ACCELEROMETER RELATED
 */
#define ACCEL_NSAMPLES 16  //
#define ACCEL_RATE 100    // Hz
