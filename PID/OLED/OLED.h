#ifndef _OLED_H_
#define _OLED_H_


typedef enum {
  OLED_COLOR_NORMAL = 0, // 正常模式 黑底白字
  OLED_COLOR_REVERSED    // 反色模式 白底黑字
} OLED_ColorMode;



void OLED_ShowCHINESE(uint8_t Line, uint8_t Column, uint8_t Num);
void OLED_SetPixel(uint8_t x, uint8_t y, OLED_ColorMode color);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowFloatNum(uint8_t Line, uint8_t Column, float Number, uint8_t intLength, uint8_t radLenth);
void OLED_ShowSignedFloatNum(uint8_t Line, uint8_t Column, float Number, uint8_t intLength, uint8_t radLenth);
#endif