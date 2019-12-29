# ups
for detect power failure use gpio 100, odroid-c1 pin 31  

## Install packets
```Bash
sudo apt install software-properties-common
sudo add-apt-repository ppa:hardkernel/ppa
sudo apt update
sudo apt install odroid-wiringpi libwiringpi-dev libwiringpi2
```

## Compile and install safe-shutdown
```Bash
gcc safe-shutdown.c -lwiringPi -lwiringPiDev -lm -lpthread -lrt -lcrypt
mv a.out /bin/safe-shutdown
cp safe-shutdown.service /etc/systemd/system/
systemctl enable safe-shutdown.service
systemctl start safe-shutdown.service
```
