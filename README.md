# roller-shutter
Somfy roller shutter controlled by an ESP32

## Materials

### ESP32 from azdelivery

![](/images/esp32.png)

### RF433 module

![](/images/rf433-module.jpg)

### 433.42MHz oscillator

![](/images/oscillator.jpg)

## First run or rescue mode

Keep pressed on the rescue button (pin 14) and press the ESP32 reset button.

The ESP32 start in rescue mode as an access point.

* Access point : "VoletRoulants"

* Default IP : 10.10.10.1

Configuration URL : http://192.168.1.64/config

Set your wifi credentials in the wifi page and restart the ESP32.

* Home page URL : http://X.X.X.X

* Configuration page URL : http://X.X.X.X/config

## Attach a roller shutter

![](/images/somfy-rts.jpg)

Press the "Prog" button 3 seconds

Go to the "Attacher" page and click on the desired roller shutter


## Screenshots

* Main page

![](/images/main.png)

* Configuration page

![](/images/config.png)


* Programs page

![](/images/programs.png)

* Wifi page

![](/images/wifi.png)

* Clock page

![](/images/clock.png)

* Schema

![](/images/schema.png)

## Photos

![](/images/pcb_composants.jpg)

![](/images/pcb_pistes.jpg)

![](/images/pcb_dans_boitier.jpg)

![](/images/boitier_ferme.jpg)
