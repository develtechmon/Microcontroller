# Getting Started

This userguide explain on how to enable a communication from `Arduino` to `RPI` using `RF24` module

You may need to open this [link](https://medium.com/@anujdev11/communication-between-arduino-and-raspberry-pi-using-nrf24l01-818687f7f363) alongside to help you understand better the installation and setup process

## Configuration
To start, we need enable `SPI` in `RPI` using below command

```
sudo raspi-config

Then go to `Interfacing options` and select `SPI` and enabled it.

sudo reboot
```

## Library (Raspberry PI and Arduino)
Next we're going to install `RF24` python library for `RPI` as follow

```
sudo apt-get update
sudo apt-get upgrade

wget http://tmrh20.github.io/RF24Installer/RPi/install.sh

chmod +x install.sh

Run below script. In this case, i select only the `RF24 Core`
./install.sh

```

After installation sucessfully done. Then navigate to `RF24` directory as follow

```
cd rf24libs/RF24/examples_linux
make
```
Bear it mind, it's worth to install the `Python` wrapper based on this [python_wrapper.d](https://github.com/nRF24/RF24/blob/master/docs/python_wrapper.md) that allow you to install `getting_started.py` version. Default is made only for `c++`

Next you can below command to ensure `RF24` installed correctly. 

```
cd /home/jlukas/rf24libs/RF24

Invoke python interpreter
python3

Python 3.9.2 (default, Mar 12 2021, 04:06:34) 
[GCC 10.2.1 20210110] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> from RF24 import RF24
>>> RF24(0,22)
<RF24.RF24 object at 0x7668c8c0>

You should able to see above result.
```

Next, we're going to install `RF24` library in arduino. You can follow below steps

```
1. Open Arduino IDE
2. Under Skecth --> Manage Libraries --> RF24
3. You should be able to see new RF24 libraries in the examples list
```
## Schematic Conection

### Arduino (UNO) with NRF (Transmitter)
```
Connect the VCC pin to 5 Volts
Connect the GND pin to ground (GND)
Connect the CE pin to Arduino 7
Connect the CSN pin to Arduino 8
Connect the SCK pin to Arduino 13
Connect the MOSI pin to Arduino 11
Connect the MISO pin to Arduino 12
```

### Rpi (Zero) with NRF (Receiver)
```
Connect the VCC pin to 5 Volts (Pin 1)
Connect the GND pin to ground (GND) (Pin 6)
Connect the CE pin to Raspberry GPIO 22
Connect the CSN pin to Raspberry GPIO 8
Connect the SCK pin to Raspberry GPIO 11
Connect the MOSI pin to Raspberry GPIO 10
Connect the MISO pin to Raspberry GPIO 09
```

## Run
```
Upload arduino_to_rpi_transmitter.ino to Arduino. Here, Arduino act as a transmitter

run arduino_to_rpi_receiver.py 
```
[arduino_to_rpi_receiver.py](https://gist.github.com/anujdev11/2becc0717a5e071c1831d9c2b9871869#file-arduinoreceive-py) code


[arduino_to_rpi_transmitter.ino](https://gist.github.com/anujdev11/a1e480112e7f5f7ce1dd2a2e6c74e80a) code

[Github Owner link for above code](https://gist.github.com/anujdev11)
## Result
```
You should be able to see the incoming data from Arduino. Else, fix or redo the wire connection to solve this.
```