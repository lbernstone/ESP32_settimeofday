/* settimeofday example
  Copyright 2019 Larry Bernstone (lbernstone@gmail.com)
  All rights reserved

  This example is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

#define TZ_INFO "MST7MDT6,M3.2.0/02:00:00,M11.1.0/02:00:00" //"America/Denver" 
// see http://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
#define PRINT_DELAY 10 //seconds

#include <sys/time.h>
#include <Ticker.h>
Ticker time_print;

/* This function sets the internal time. The timeval
   struct is seconds, milliseconds. settimeofday parameters are
   epoch_time, timezone. 
*/
int setUnixtime(int32_t unixtime) {
  timeval epoch = {unixtime, 0};
  return settimeofday((const timeval*)&epoch, 0);
}

void timePrint() {
  struct tm now;
  getLocalTime(&now,0);
  if (now.tm_year >= 117) Serial.println(&now, "%B %d %Y %H:%M:%S (%A)");
}

void setup() {
  Serial.begin(115200);
  setenv("TZ", TZ_INFO, 1);
  tzset(); // Assign the local timezone from setenv
  time_print.attach(PRINT_DELAY, timePrint); // Schedule the timeprint function
  disableCore0WDT(); //turn off WDT since we will have a blocking task
  Serial.setTimeout(LONG_MAX); //don't timeout
}

void loop() {
  // Wait for input from serial (note that this is blocking)
  String timeStr = Serial.readStringUntil('\n');
  uint32_t timeInt32 = atoi(timeStr.c_str());
  if (timeInt32 == 0) return;
  if (timeInt32 < 1480000000) {
    Serial.println("Please enter a Unix epoch time after 01/01/2017");
  } else {
    setUnixtime(timeInt32);
  }
}
