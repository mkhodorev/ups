#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <wiringPi.h>
#include <syslog.h>
 
#define VCC_STATUS_GPIO_PIN      22
#define DEFAULT_SHUTDOWN_TIMEOUT 300

void setupGPIO(void)
{
  wiringPiSetup();
  pinMode(VCC_STATUS_GPIO_PIN, INPUT);
}

int isBattery(void)
{
  return digitalRead(VCC_STATUS_GPIO_PIN);
}

void powerOff(void)
{
  syslog(LOG_INFO, "Power off.");
  closelog();
  sync();
  system("poweroff");
}

int readShutdownTimeout(void) {
  int waitBeforeShutdown = DEFAULT_SHUTDOWN_TIMEOUT;
  char* waitBeforeShutdownString = getenv("WAIT_BEFORE_SHUTDOWN");
  if (waitBeforeShutdownString != NULL)
  {
    waitBeforeShutdown = atoi(waitBeforeShutdownString);
    if (waitBeforeShutdown == 0)
      waitBeforeShutdown = DEFAULT_SHUTDOWN_TIMEOUT;
  }
  return waitBeforeShutdown;
}

int main(void) {
  openlog("slog", LOG_PID | LOG_CONS, LOG_USER);
  setupGPIO();
  const int shutdownTimeout = readShutdownTimeout();
  int timeToPowerOff = shutdownTimeout;
  syslog(LOG_INFO, "Power control started. Battery run time is %d seconds.", shutdownTimeout);
  while (timeToPowerOff > 0)
  {
    if (isBattery()) {
      if (timeToPowerOff == shutdownTimeout) {
        syslog(LOG_INFO, "Battery power. The system will be powered off after %d seconds.", shutdownTimeout);
      }
      if (timeToPowerOff % 10 == 0) {
        syslog(LOG_INFO, "Power off after %d sec...", timeToPowerOff);
      }
      timeToPowerOff--;
    }
    else {
      if (timeToPowerOff < shutdownTimeout) {
        syslog(LOG_INFO, "Power Supply has been restored.");
      }
      timeToPowerOff = shutdownTimeout;
    }
    sleep(1);
  }
  powerOff();
  return 0;
}
