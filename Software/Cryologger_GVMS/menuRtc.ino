// Real-time clock configuation menu
void menuRtc()
{
  while (1)
  {
    Serial.println();
    Serial.println("Menu: Configure Real-time Clock (RTC)");
    Serial.print("Current datetime: "); printDateTime();
    Serial.print("Current alarm: "); printAlarm();

    Serial.println("1) Set RTC date");
    Serial.println("2) Set RTC time");
    Serial.println("3) Sync RTC to GNSS");
    Serial.println("4) Set alarm");
    Serial.println("5) Set rolling alarm interval");
    Serial.println("6) Set alarm mode");
    Serial.println("x) Exit");

    int incoming = getNumber(menuTimeout); // Timeout after x seconds

    if (incoming == STATUS_PRESSED_X)
    {
      return;
    }
    else if (incoming == STATUS_GETNUMBER_TIMEOUT)
    {
      return;
    }

    if (incoming == 1) // Set RTC date
    {
      rtc.getTime();
      int s = rtc.seconds,
          m = rtc.minute,
          h = rtc.hour,
          dd = rtc.dayOfMonth,
          mm = rtc.month,
          yy = rtc.year;

      Serial.print("Enter current two digit year: ");
      yy = getNumber(menuTimeout);
      if (yy > 2000 && yy < 2100) yy -= 2000;

      Serial.print("Enter current month (1 to 12): ");
      mm = getNumber(menuTimeout);

      Serial.print("Enter current day (1 to 31): ");
      dd = getNumber(menuTimeout);

      rtc.setTime(h, m, s, 0, dd, mm, yy);
    }
    else if (incoming == 2) // Set RTC time
    {
      rtc.getTime();
      int s = rtc.seconds,
          m = rtc.minute,
          h = rtc.hour,
          dd = rtc.dayOfMonth,
          mm = rtc.month,
          yy = rtc.year;

      Serial.print("Enter current hour (0 to 23): ");
      h = getNumber(menuTimeout);

      Serial.print("Enter current minute (0 to 59): ");
      m = getNumber(menuTimeout);

      Serial.print("Enter current second (0 to 59): ");
      s = getNumber(menuTimeout);

      rtc.setTime(h, m, s, 0, dd, mm, yy);
    }
    else if (incoming == 3 && qwiicOnline.uBlox)
    {
      bool dateValid = gnss.getDateValid();
      bool timeValid = gnss.getTimeValid();

      if (!dateValid || !timeValid)
      {
        Serial.println("Warning: GNSS time or date not valid. Please try again.\n");
        rtcSyncFlag = false;
      }
      else
      {
        rtc.setTime(gnss.getHour(),
                    gnss.getMinute(),
                    gnss.getSecond(),
                    gnss.getMillisecond() / 10,
                    gnss.getDay(),
                    gnss.getMonth(),
                    gnss.getYear() - 2000);
        rtcSyncFlag = true;
        Serial.println("RTC synced with GNSS (UTC) time");
      }
    }
    else if (incoming == 4) // Manually set alarm
    {
      Serial.print("Enter alarm hour (0 to 23): ");
      int alarmHour = getNumber(menuTimeout);

      Serial.print("Enter alarm minutes (0 to 59): ");
      int alarmMinute = getNumber(menuTimeout);

      Serial.print("Enter alarm seconds (0 to 59): ");
      int alarmSeconds = getNumber(menuTimeout);

      Serial.print("Enter alarm day (1 to 31): ");
      int alarmDayOfMonth = getNumber(menuTimeout);

      Serial.print("Enter alarm month (1 to 12): ");
      int alarmMonth = getNumber(menuTimeout);

      rtc.setAlarm(alarmHour % 24, alarmMinute % 60, alarmSeconds % 60, 0, alarmDayOfMonth % 32, alarmMonth % 13);
      rtc.setAlarmMode(4); // Set alarm mode
      rtc.attachInterrupt(); // Attach RTC alarm interrupt

      Serial.print("Alarm set: "); printAlarm();
    }
    else if (incoming == 5) // Set rolling alarm interval
    {
      Serial.print("Enter rolling alarm hour interval (0 to 23): ");
      int alarmHours = getNumber(menuTimeout);

      Serial.print("Enter rolling alarm minutes interval (0 to 59): ");
      int alarmMinutes = getNumber(menuTimeout);

      Serial.print("Enter rolling alarm seconds interval (0 to 59): ");
      int alarmSeconds = getNumber(menuTimeout);

      if (alarmHours < 0 || alarmHours > 23)
      {
        Serial.println(F("Error: Alarm interval out of range"));
      }
    }
    else if (incoming == 6) // Set alarm mode
    {
      Serial.println("Alarm modes:");
      Serial.println("0) Alarm interrupt disabled");
      Serial.println("1) Alarm match every year");
      Serial.println("2) Alarm match every month");
      Serial.println("3) Alarm match every week");
      Serial.println("4) Alarm match every day");
      Serial.println("5) Alarm match every hour");
      Serial.println("6) Alarm match every minute");
      Serial.println("7) Alarm match every second");
      Serial.println();
      Serial.println("Select alarm mode (0 to 7): ");

      int alarmMode = getNumber(menuTimeout);
      //alarmMode = constrain(alarmMode, 0, 7);
      if (alarmMode < 0 || alarmMode > 7)
      {
        Serial.println(F("Error: Alarm mode out of range"));
      }
      else
      {
        rtc.setAlarmMode(alarmMode);
        rtc.attachInterrupt(); // Attach RTC alarm interrupt
        Serial.print("Alarm mode set: "); Serial.println(alarmMode);
      }
    }
  }
}

// Print the RTC's current date and time
void printDateTime()
{
  rtc.getTime();
  Serial.printf("20%02d-%02d-%02d %02d:%02d:%02d.%02d\n",
                rtc.year, rtc.month, rtc.dayOfMonth,
                rtc.hour, rtc.minute, rtc.seconds, rtc.hundredths);
}

// Print the RTC's alarm
void printAlarm()
{
  rtc.getAlarm();
  Serial.printf("2020-%02d-%02d %02d:%02d:%02d.%02d\n",
                rtc.alarmMonth, rtc.alarmDayOfMonth,
                rtc.alarmHour, rtc.alarmMinute,
                rtc.alarmSeconds, rtc.alarmHundredths);
}
