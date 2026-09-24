#ifndef logger_h
#define logger_h

#include <Arduino.h>
#include <WebServer.h>

void initializeLog();
void logPrintln(const char *s);
void logFlowln(unsigned long duration);
const char *getProgramNameAndVersion();
void sendToWebServer(WebServer &server);

#endif  // logger_h