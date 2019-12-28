#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <wiringPi.h>
 
#define VCC_STATUS_GPIO_PIN      22
#define DEFAULT_SHUTDOWN_TIMEOUT 300

void setupGPIO()
{
  wiringPiSetup();
  pinMode(VCC_STATUS_GPIO_PIN, INPUT);
}

int isBattery()
{
  return digitalRead(VCC_STATUS_GPIO_PIN);
}

void powerOff()
{
  sync();
  system("poweroff");
}

int shutdownTimeout() {
  int waitBeforeShutdown = DEFAULT_SHUTDOWN_TIMEOUT;
  char* waitBeforeShutdownString = getenv("WAIT_BEFORE_SHUTDOWN");
  if(waitBeforeShutdownString != NULL)
  {
    waitBeforeShutdown = atoi(waitBeforeShutdownString);
    if(waitBeforeShutdown == 0)
      waitBeforeShutdown = DEFAULT_SHUTDOWN_TIMEOUT;
  }
  return waitBeforeShutdown;
}

int main(void)
{
  setupGPIO();
  int timeToReboot = shutdownTimeout();
  while(timeToReboot > 0)
  {
    if(isBattery())
      timeToReboot--;
    else
      timeToReboot = shutdownTimeout();
    sleep(1);
  }
  powerOff();
  return 0;
}
