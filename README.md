# ups
for detect power failure use gpio 100, odroid-c1 pin 31  

##Install packets
```sudo apt install software-properties-common
sudo add-apt-repository ppa:hardkernel/ppa
sudo apt update
sudo apt install odroid-wiringpi libwiringpi-dev libwiringpi2

## Install safe-shutdown
1. compile safe-shutdown  
   `gcc safe-shutdown.c -lwiringPi -lwiringPiDev -lm -lpthread -lrt -lcrypt`
2. `mv a.out /bin/safe-shutdown`
3. `cp safe-shutdown.service /etc/systemd/system/`
4. `systemctl enable safe-shutdown.service`
5. `systemctl start safe-shutdown.service`