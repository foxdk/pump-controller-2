#include <Arduino.h>
#include <WebServer.h>
#include "logger.h"

const int ROW_SIZE = 64;
const int NUMBER_ROWS = 100;
const int LOG_SIZE = ROW_SIZE * NUMBER_ROWS;

const int LINE_END_SIZE = 2;   // size for ending LF and NULL
const int MAX_TIME_SIZE = 15;  // "mm/dd hh:mm:ss "
const int MAX_MESSAGE_SIZE = ROW_SIZE - MAX_TIME_SIZE - LINE_END_SIZE;

char logBuffer[LOG_SIZE];

int rowOffset = 0;

int nextRow(int currentOffset) {
  int newOffset = currentOffset + ROW_SIZE;
  if (newOffset >= LOG_SIZE) {
    newOffset -= LOG_SIZE;
  }
  return newOffset;
}

int previousRow(int currentOffset) {
  int newOffset = currentOffset - ROW_SIZE;
  if (newOffset < 0) {
    newOffset += LOG_SIZE;
  }
  return newOffset;
}

size_t addTimestampAtLineStart(char *buf) {
  if (WiFi.status() != WL_CONNECTED) {
    return 0;
  }
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return 0;
  }
  return strftime(buf, MAX_TIME_SIZE + 1, "%m/%d %H:%M:%S ", &timeinfo);
}

void copyCharsAddNewline(char *dest, const char *s, size_t max) {
  size_t maxChars = max - LINE_END_SIZE;
  while (maxChars > 0 && *s != 0) {
    *dest++ = *s++;
    maxChars--;
  }
  *dest++ = '\n';
  *dest = 0;
  if (dest >= (logBuffer + LOG_SIZE)) {
    Serial.println("ERROR - BUFFER OVERRUN in logger.cpp");
  }
}

// PUBLIC METHODS

void initializeLog() {
  // set the first char of each row to null
  for (int i = 0; i < NUMBER_ROWS; i++) {
    logBuffer[i * ROW_SIZE] = 0;
  }
  rowOffset = 0;
}

void logPrintln(const char *s) {
  char *destPtr = logBuffer + rowOffset;
  size_t timeSize = addTimestampAtLineStart(destPtr);
  copyCharsAddNewline(destPtr + timeSize, s, ROW_SIZE - timeSize);
  Serial.print(destPtr);  // does a println because there is already a LF at the end of the line
  rowOffset = nextRow(rowOffset);
}

const char *getProgramNameAndVersion() {
  return "Pump Controller v 1.9.2";
}

void sendHowLongRunning(WebServer &server) {
  long secondsSinceReboot = (long)(esp_timer_get_time() / 1000000LL);
  long hoursSinceReboot = secondsSinceReboot / 3600L;
  long daysSinceReboot = hoursSinceReboot / 24L;
  long hours = hoursSinceReboot % 24L;

  char tempBuffer[ROW_SIZE];

  snprintf(tempBuffer, sizeof(tempBuffer), "Running for %ld days %ld hours\n", daysSinceReboot, hours);
  server.sendContent(tempBuffer);
  snprintf(tempBuffer, sizeof(tempBuffer), "millis() counter: %lu\n", millis());
  server.sendContent(tempBuffer);
}

void sendToWebServer(WebServer &server) {
  // write rows beginning with row most recently added first and then working backwards
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/plain", getProgramNameAndVersion());
  server.sendContent("\n");
  sendHowLongRunning(server);
  server.sendContent("----------\n");
  int offset = previousRow(rowOffset);
  for (int i = 0; i < NUMBER_ROWS; i++) {
    char *row = logBuffer + offset;
    if (*row != 0) {
      server.sendContent(row);
    }
    offset = previousRow(offset);
  }
  server.sendContent("");
}
