# LEDCode


A variety of LED effects. Mainly coded for two 4 meter strips. 

## Quick start

1. Install Arduino 1.6.5
2. Install Teensy LC software (you will need to navigate to the Arduino code for Teensy LC to link with it, v. 1.6.5 of Arduino satisfies the Teensy software).
3. Pull the code from Git (check out source tree if you want a GUI for this)
4. Open code (FastLEDCode/Nov21-code is a good one to start with)
5. Under Tools --> Board ; select "TeensyLC"
6. Under Sketch --> Compile/Verify. Make sure code compiles.
7. Make sure Teensy is hooked to your computer by now (it can be hooked at any of the prev. steps)
8. Under Sketch --> Upload code. Wait for teensy to reboot. You may need to power cycle it/try a couple of times if it does not respond to the request to reboot. 
9. Once you've successfully loaded the code you can start editting the code/changing what it does. Most variables are self explanatory. setup() does initialization and is where you should look to change the number of agents/colors for random agents. loop() is the loop that decides what to display. See switchclock() to see how to change the color palettes for the agent clock. ad_palettes.h contains a bunch of prebuilt palettes you can pick from. 
10. To see your changes repeat step 8 above. It compiles before uploading so will also tell you if you have errors. If you get a random/nonsensical seeming error it might be that your code is trying to use more memory than is available on Teensy.
