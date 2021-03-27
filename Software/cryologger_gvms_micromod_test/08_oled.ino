// Configure SSD1306 OLED
void configureOled()
{
#if DEBUG_OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("Warning: SSD1306 allocation failed!");
  }

  // Show initial display buffer contents on the screen
  display.display();
  delay(2000);

  display.clearDisplay(); // Clear the buffer

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("OLED initalized.");
  display.display(); // actually display all of the above
#endif
}
