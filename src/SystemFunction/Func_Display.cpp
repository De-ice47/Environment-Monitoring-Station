#include <Func_Display.hpp>

U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display = &SSD1309::display;

void SetupDisplay(){
    SSD1309::Begin();
  SSD1309::display.setContrast(64);
  LL::Println("[SSD1309] Display Active");
}
void DrawLogs()
{
  Clear();
  int logsToShow = min((uint8_t)9, LL::GetLineCount());
  int startIndex = LL::GetLineCount() - logsToShow;
  display->setFont(u8g2_font_tiny5_t_all);
  for (uint8_t i = startIndex, yPos = 0; i < logsToShow; i++, yPos++)
  {
    display->drawStr(2, 6 + yPos * 6, LL::GetLine(i));
  }
  Draw();
}