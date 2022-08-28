# esp32 camera experiments

I'm trying to make a robot that follows objects (people, ideally) using an esp32cam.

I've used code "borrowed" from Jarkman, with his help, thank you! It uses centre of gravity, i.e. the average of where the most stuff is happening.

https://code.google.com/archive/p/theeyestheeyes/source/default/source?page=4

esp32cam_follower_ota_working is a version of this that works reasonably well for people 1-2 metres away.

BasicOTA is getting over the air updates working (which is good because esp32 cameras are annoying to flash)

simple_esp32cam is my initial stab at diffing frames

A couple of things to note
 * to use servos you need to use timers above 2 - see https://stackoverflow.com/questions/64402915/esp32-cam-with-servo-control-wont-work-arduino-ide and https://www.esp32.com/viewtopic.php?t=11379 and to do that you need this servo library https://github.com/RoboticsBrno/ServoESP32
 * To get OTA working I had to edit boards.txt, see below.
 * I used flash mode dio for serial
 * you may need to set flash mode 40hz for serial if it crashes a lot: https://github.com/espressif/arduino-esp32/issues/3033#issuecomment-517065972


# add this to boards.txt
``` 
esp32cam.menu.PartitionScheme.huge_app=Huge APP (3MB No OTA/1MB SPIFFS)
esp32cam.menu.PartitionScheme.huge_app.build.partitions=huge_app
esp32cam.menu.PartitionScheme.huge_app.upload.maximum_size=3145728
esp32cam.menu.PartitionScheme.min_spiffs=Minimal SPIFFS (Large APPS with OTA)
esp32cam.menu.PartitionScheme.min_spiffs.build.partitions=min_spiffs
esp32cam.menu.PartitionScheme.min_spiffs.upload.maximum_size=1966080
```

for me on mac os x boards.txt is in /Users/libbym/Library/Arduino15/packages/esp32/hardware/esp32/1.0.6/boards.txt

It looks like on windows it's in the Arduino directory somewhere

it may get overwritten!

I used the top answer here https://arduino.stackexchange.com/questions/75198/why-doesnt-ota-work-with-the-ai-thinker-esp32-cam-board - then you need to pick the SPIFFS one in the menu.

 
