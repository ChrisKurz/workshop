# Lab 1 - Board Configurator

## Overview
In this hands-on exercise, you will use the __Board Configurator__ PC application to configure the __nRF54L15DK__ development kit. The exercise guides you through the required configuration steps, allowing you to familiarize yourself with the Board Configurator interface and its available settings. By the end of this hands-on, the nRF54L15DK will be configured according to the specified requirements and ready for the subsequent exercises.

### ▶️ Step 1 - Check nRF54L15DK

1) Connect the nRF54L15DK to your PC using the USB cable. Verify that the board is powered by confirming that __LED0__ is blinking. If __LED0__ is not blinking, check the USB connection and ensure that the __POWER__ switch is in the __ON__ position before proceeding.

### ▶️ Step 2 - Connect nRF54L15DK to _Board Configurator_
2) Start _nRF Connect for Desktop_ and open the __Board Configurator__ application.
3) Click __SELECT DEVICE__.
4) In the list of available devices, select your __nRF54L15DK__ development kit. The application connects to the board.

### ▶️ Step 3 - Disable _Power to LEDs_ and check what is happening on the DK
5) Disable __Power to LEDs__, then click __Write config__. Verify that LED0 stops blinking.
6) Power cycle the development kit by either:
   - Setting the __POWER__ switch to __OFF__ and then back to __ON__, or
   - Disconnecting and reconnecting the USB cable.
7) After the board restarts, observe __LED0__. Is it blinking? Explain your observation.
   > 💡 __Tip:__ Is the GPIO still toggling, or is only the LED power disabled?

### ▶️ Step 4 - Enable _Power to LEDs_ 
8) In the __Board Configurator__, re-enable __Power to LEDs__ and click __Write config__. Verify that __LED0__ starts blinking again.
