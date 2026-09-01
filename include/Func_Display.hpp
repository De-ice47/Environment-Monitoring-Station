#include <SSD1309.hpp>
#include <LoggerLite.hpp>

extern U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display;

void SetupDisplay();
inline void Clear() 
{
  SSD1309::display.clearBuffer();
}
inline void Draw()
{
  SSD1309::display.sendBuffer();
}
void DrawLogs();