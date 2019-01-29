#define TZ_INFO "MST7MDT6,M3.2.0/02:00:00,M11.1.0/02:00:00" //"America/Denver"
#define PRINT_DELAY 10

#include <sys/time.h>
#include <Ticker.h>
Ticker time_print;

int setUnixtime(int32_t unixtime) {
  timeval epoch = {unixtime, 0};
  return settimeofday((const timeval*)&epoch, 0);
}

void timePrint() {
  struct tm now;
  getLocalTime(&now,0);
  if (now.tm_year >= 117) Serial.println(&now," %B %d %Y %H:%M:%S (%A)");
}

void setup() {
  Serial.begin(115200);
  setenv("TZ", TZ_INFO, 1);
  tzset();
  time_print.attach(PRINT_DELAY, timePrint);
  Serial.setTimeout(LONG_MAX); //don't timeout
}

void loop() {
  String timeStr = Serial.readStringUntil('\n');
  uint32_t timeInt32 = atoi(timeStr.c_str());
  if (timeInt32 == 0) return;
  if (timeInt32 < 1480000000) {
    Serial.println("Please enter a Unix epoch time after 01/01/2017");
  } else {
    setUnixtime(timeInt32);
  }
}
