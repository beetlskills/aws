The project folder is "aws".

Required Library
1. Download AWS_IOT from https://exploreembedded.com/wiki/AWS_IOT_with_Arduino_ESP32
2. Put it in the Arduino sketchbook folder (usually C:\Users\...\Documents\Arduino)

Setting up AWS IoT
1. Go to AWS IoT Management Console to create a thing.
2. Download the device certiciate, the private key and the root CA file
3. Create a policy (allow connection, publish and subscribe to *) and attach it to the thing
4. Go to C:\Users\...\Documents\Arduino\libraries\AWS_IOT\src\aws_iot_certficates.c and paste the content of the files above into the C file
4. Add \n\ at the end of every line (for the last line, it is only \n) 

Arduino Setting
 
1. Go to Arduino IDE's File/Preferences menu. Enter the following Additional boards manager URLs: 

https://dl.espressif.com/dl/package_esp32_index.json

2. Go to Tools/Board/Boards Manager menu. Search for ESP32. Install board support package for ESP32 from Espressif Systems

3. Go to Tools/Board/esp32 and select ESP32 Wrover Module

4. Go to Tools/Port and select the COM port that ESP32 is connected to

5. Open aws.ino from File/Open

6. Click on Upload button to compile, upload and run.

7. Press the side tactile button. The count will be published. The count is looped back via subscription, and the WHITE led will blink one time.