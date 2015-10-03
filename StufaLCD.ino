// Micronova Pellet Stove Controller Panel Test
// It is a test for 4-line segment LCD
// For more details see http://k3a.me/ir-controller-for-pellet-stove-with-micronova-controller-stufe-e-pellet-aria-ir-telecomando/
// Licence: GNU GPL v3

const int SCK_pin = 13; // D2 (out SPI clock)
const int MOSI_pin = 11; // D1 (out SPI data)
const int SS_pin = 2; // A, B, C together (chip select) (high to enable as A=1 B=1 C=1 will cause LCD chip select go LOW)
int frame = 0;
byte charAnim = ' ';

byte lcdData[45];
typedef int lcdAlnumSegments[16];
typedef int lcdNumSegments[9];
typedef byte lcdAlnumBits[16];
typedef byte lcdNumBits[9];

// status lines (left, top first row, top second row) 
int lcdLines[15] = {155, 153, 156, 150, 116, 117, 30, 37, 190, 197, 276, 313, 316, 317, 310};
// signal symbol: 277

// -------- NUMERIC PART ------------------

// mapping to byte index and bit index
// 12 numeric characters (starting first row from left, second row follows)
lcdNumSegments lcdNum[12] = {  {401, 403, 406, 407, 405, 402, 404, -1, 400},
                               {411, 413, 416, 417, 415, 412, 414, -1, 410},
                               {421, 423, 426, 427, 425, 422, 424, 420, -1},
                               {431, 433, 436, 437, 435, 432, 434, 430, -1},
                               
                               {327, 326, 323, 321, 322, 325, 324, -1, 320},
                               {337, 336, 333, 331, 332, 335, 334, -1, 330},
                               {347, 346, 343, 341, 342, 345, 344, 340, -1},
                               {357, 356, 353, 351, 352, 355, 354, 350, -1},
                               {367, 366, 363, 361, 362, 365, 364, -1, 360},
                               {377, 376, 373, 371, 372, 375, 374, -1, 370},
                               {387, 386, 383, 381, 382, 385, 384, 380, -1},
                               {397, 396, 393, 391, 392, 395, 394, 390, -1}};

// charmap
lcdNumBits bitsNum[] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0}, // 
  {1, 1, 1, 1, 1, 1, 0, 0, 0}, //0
  {0, 1, 1, 0, 0, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1, 0, 0}, //2
  {1, 1, 1, 1, 0, 0, 1, 0, 0}, //3
  {0, 1, 1, 0, 0, 1, 1, 0, 0}, //4
  {1, 0, 1, 1, 0, 1, 1, 0, 0}, //5
  {1, 0, 1, 1, 1, 1, 1, 0, 0}, //6
  {1, 1, 1, 0, 0, 0, 0, 0, 0}, //7
  {1, 1, 1, 1, 1, 1, 1, 0, 0}, //8
  {1, 1, 1, 1, 0, 1, 1, 0, 0}, //9
  {1, 1, 1, 0, 1, 1, 1, 0, 0}, //A
  {0, 0, 1, 1, 1, 1, 1, 0, 0}, //B
  {1, 0, 0, 1, 1, 1, 0, 0, 0}, //C
  {0, 1, 1, 1, 1, 0, 1, 0, 0}, //D
  {1, 0, 0, 1, 1, 1, 1, 0, 0}, //E
  {1, 0, 0, 0, 1, 1, 1, 0, 0}, //F
  {1, 1, 1, 1, 0, 1, 1, 0, 0}, //G
  {0, 1, 1, 0, 1, 1, 1, 0, 0}, //H
  {0, 0, 0, 0, 1, 1, 0, 0, 0}, //I
  {1, 1, 1, 1, 0, 0, 0, 0, 0}, //J
  {0, 0, 0, 1, 1, 1, 0, 0, 0}, //K
  {0, 0, 0, 1, 1, 1, 0, 0, 0}, //L
  {1, 0, 1, 0, 1, 0, 0, 0, 0}, //M
  {0, 0, 1, 0, 1, 0, 1, 0, 0}, //N
  {1, 1, 1, 1, 1, 1, 0, 0, 0}, //O
  {1, 1, 0, 0, 1, 1, 1, 0, 0}, //P
  {1, 1, 1, 0, 0, 1, 1, 0, 0}, //Q
  {0, 0, 0, 0, 1, 0, 1, 0, 0}, //R
  {1, 0, 1, 1, 0, 1, 1, 0, 0}, //S
  {0, 0, 0, 1, 1, 1, 1, 0, 0}, //T
  {0, 1, 1, 1, 1, 1, 0, 0, 0}, //U
  {0, 0, 1, 1, 1, 0, 0, 0, 0}, //V
  {0, 1, 0, 1, 0, 1, 0, 0, 0}, //W
  {0, 1, 1, 0, 0, 0, 1, 0, 0}, //X
  {0, 1, 1, 1, 0, 1, 1, 0, 0}, //Y
  {0, 1, 0, 0, 1, 0, 1, 0, 0}, //Z
  {1, 1, 0, 0, 0, 0, 1, 0, 0}, //c
};

// --------- ALPHANUMERIC PART --------------

// mapping to byte index and bit index
// 16 alphanumeric LCD characters (starting first row from left, second row follows)
lcdAlnumSegments lcdAlnum[16] = {{10, 31, 32, 23, 3, 0, 11, 20, 21, 22, 12, 13, 2, 1, -1, 33},                    //0
                                 {50, 71, 72, 63, 43, 40, 51, 60, 61, 62, 52, 53, 42, 41, 70, 73},                //1
                                 {90, 111, 112, 103, 83, 80, 91, 100, 101, 102, 92, 93, 82, 81, 110, -1},         //2
                                 {130, 151, 152, 143, 123, 120, 131, 140, 141, 142, 132, 133, 122, 121, -1, -1},  //3
                                 {170, 191, 192, 183, 163, 160, 171, 180, 181, 182, 172, 173, 162, 161, -1, 193}, //4
                                 {210, 231, 232, 223, 203, 200, 211, 220, 221, 222, 212, 213, 202, 201, 230, 233},//5
                                 {250, 271, 272, 263, 243, 240, 251, 260, 261, 262, 252, 253, 242, 241, 270, -1}, //6
                                 {290, 311, 312, 303, 283, 280, 291, 300, 301, 302, 292, 293, 282, 281, -1, -1},  //7
                                 
                                 {14, 34, 35, 27, 7, 4, 15, 24, 25, 26, 16, 17, 6, 5, -1, 36},                    //8
                                 {54, 74, 75, 67, 47, 44, 55, 64, 65, 66, 56, 57, 46, 45, 76, 77},                //9
                                 {94, 114, 115, 107, 87, 84, 95, 104, 105, 106, 96, 97, 86, 85, 113, -1},         //10
                                 {134, 154, 155, 147, 127, 124, 135, 144, 145, 146, 136, 137, 126, 125, -1, -1},  //11
                                 {174, 194, 195, 187, 167, 164, 175, 184, 185, 186, 176, 177, 166, 165, -1, 196}, //12
                                 {214, 234, 235, 227, 207, 204, 215, 224, 225, 226, 216, 217, 206, 205, 236, 237},//13
                                 {254, 274, 275, 267, 247, 244, 255, 264, 265, 266, 256, 257, 246, 245, 273, -1}, //14
                                 {294, 314, 315, 307, 287, 284, 295, 304, 305, 306, 296, 297, 286, 285, -1, -1}}; //15

// ascii char to charMap
byte char2alnum[] {
  // from decimal 32 (space) incl.
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0, 0, 0, 0, 0, 0, 11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36
};
// charMap to ascii
byte alnum2char[] {
  ' ', '0', '1', '2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','c'
};
// charmap
lcdAlnumBits bitsAlnum[] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //
  {1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, //0
  {0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //2
  {1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, //3
  {0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //4
  {1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0}, //5 may be different
  {1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //6
  {1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //7
  {1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //8
  {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //9
  {1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //A
  {1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0}, //B
  {1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //C
  {1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, //D
  {1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, //E
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, //F
  {1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, //G
  {0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //H
  {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, //I
  {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //J
  {0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0}, //K
  {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //L
  {0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0}, //M
  {0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, //N
  {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //O
  {1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //P
  {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, //Q
  {1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0}, //R
  {1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, //S
  {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, //T
  {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //U
  {0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, //V
  {0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0}, //W
  {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0}, //X
  {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0}, //Y
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0} //Z
};

void setup()
{
  pinMode(SS_pin, OUTPUT);
  pinMode(SCK_pin, OUTPUT);
  pinMode(MOSI_pin, OUTPUT);
  
  digitalWrite(SS_pin, LOW);
  digitalWrite(SCK_pin, LOW);
}

// send 1 to 8 bits
// when numbits < 8, LSB bits are sent
void bitBang(byte _send, int numbits=8)
{
  byte _receive = 0;
  
  for(int i=numbits-1; i>=0; i--)
  {
    digitalWrite(MOSI_pin, bitRead(_send, i));
    digitalWrite(SCK_pin, HIGH);
    digitalWrite(SCK_pin, LOW);
  }
}

void WriteChar(unsigned pos, char chr)
{
  if (pos > 15)
    pos = 15;
  
  unsigned char uchr = *(unsigned char*)&chr;
  lcdAlnumBits& b = bitsAlnum[ char2alnum[uchr-32]];
  lcdAlnumSegments& s = lcdAlnum[pos];
  
  for (int i=0; i<16; ++i)
    if (b[i])
    {
      int bpos = s[i];
      if (bpos >=0)
      {
        int byteid = bpos/10;
        int bitpos = bpos%10;
        lcdData[byteid] |= 1 << bitpos;
      }
    }
}

void WriteCharStr(int line, const char* str)
{
  int len = strlen(str);
  if (len>8)
    len = 8;
    
  for (int i=0; i<len; ++i)
    WriteChar(8*line+i, str[i]);
}

void WriteNum(unsigned pos, char chr)
{
  if (pos > 11)
    pos = 11;
  
  unsigned char uchr = *(unsigned char*)&chr;
  lcdNumBits& b = bitsNum[ char2alnum[uchr-32]];
  lcdNumSegments& s = lcdNum[pos];
  
  for (int i=0; i<9; ++i)
    if (b[i])
    {
      int bpos = s[i];
      if (bpos >=0)
      {
        int byteid = bpos/10;
        int bitpos = bpos%10;
        lcdData[byteid] |= 1 << bitpos;
      }
    }
}

void WriteNumStr(int line, const char* str)
{
  int len = strlen(str);
  if (line == 0 && len>4)
    len = 4;
  else if (line == 1 && len>8)
    len = 8;
    
  for (int i=0; i<len; ++i)
    WriteNum(4*line+i, str[i]);
}

#define TEST_WRITING_LINES 1
#define TEST_SEGMENTS 2
#define TEST_CHARACTER_ANIM 3

#define WHAT_TO_TEST TEST_WRITING_LINES

void loop()
{
  memset(lcdData, 0, 45);
  
#if WHAT_TO_TEST == TEST_CHARACTER_ANIM
  // character animation
  for (int i=0; i<16; ++i)
    WriteChar(i, charAnim);
  
  if (++charAnim > 'Z')
    charAnim = ' ';
#endif
    
#if WHAT_TO_TEST == TEST_WRITING_LINES
  WriteNumStr(0, "AZ10");
  WriteNumStr(1, "01234567");
  WriteCharStr(0, "ABCDEFGH");
  WriteCharStr(1, "1234590Z");
#endif
  
  digitalWrite(SCK_pin, HIGH);
  digitalWrite(SS_pin, LOW);
  digitalWrite(SS_pin, HIGH);
  
  bitBang(0b11000, 5); 
  bitBang(0b00000110);
  
  digitalWrite(SCK_pin, HIGH);
  digitalWrite(SS_pin, LOW);
  digitalWrite(SS_pin, HIGH);
  
  bitBang(0b11000, 5);
  bitBang(0b00000010);
  
  digitalWrite(SCK_pin, HIGH);
  digitalWrite(SS_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(SS_pin, HIGH);
  
  bitBang(0b110, 3);
  bitBang(0b10000000); //--------------------------
  
  // 45 bytes of segment bits follow
  
  // animating segments
#if WHAT_TO_TEST == TEST_SEGMENTS
 /* const int row = 19; // specify row (byte) index: min 0, max 44 incl
  for(int i=0; i<row; i++)
    bitBang(0b00000000);
  bitBang(1<<frame);
  for(int i=0; i<44-row; i++)
    bitBang(0b00000000);*/
#else
  // dynamic lcd data
  for(int i=0; i<45; i++)
    bitBang(lcdData[i]);
#endif
    
  /* original boot up data
  bitBang(0b00001001);
  bitBang(0b01111000);
  bitBang(0b10000001);
  bitBang(0b00000000);
  bitBang(0b00001101);
  bitBang(0b01110001);
  bitBang(0b10001000);
  bitBang(0b00110000);
  bitBang(0b11011101);
  bitBang(0b00010001);
  bitBang(0b01100110);
  bitBang(0b00010010);
  bitBang(0b10010100);
  bitBang(0b00010001);
  bitBang(0b10001010);
  bitBang(0b00110110);
  bitBang(0b00000100);
  bitBang(0b00000000);
  bitBang(0b00000010);
  bitBang(0b00000001);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000001);
  bitBang(0b00000110);
  bitBang(0b00000100);
  bitBang(0b00000000);
  bitBang(0b00000010);
  bitBang(0b00000000);
  bitBang(0b00001001);
  bitBang(0b00000001);
  bitBang(0b00001000);
  bitBang(0b00000111);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);
  bitBang(0b00000000);*/
  
  digitalWrite(SS_pin, LOW);
  
  delay(1000);
  if (++frame == 8)
    frame=0;
}












