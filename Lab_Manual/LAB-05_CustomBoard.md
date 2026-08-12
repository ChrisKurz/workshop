# Lab 5 - Bluetooth: Create a custom Board

# Overview
In this hands-on, participants will learn how to create a custom board definition for the Nordic Semiconductor _nRF54L15_ using _nRF Connect SDK (NCS) v3.4.0_. The workshop covers the required board description files, Devicetree configuration, Kconfig integration, and board validation using a sample application.

By the end of the workshop, participants will have a fully functional custom board that can be used as the target platform for future application development.

## ▶️ Step 1 - Explore the Existing Board
Before creating a custom board, it is important to understand the files that define an existing board. The nRF54L15 DK board contains all required configuration files that will be reused as a template.
1) Locate the Zephyr board directory.
2) Navigate to: __zephyr/boards/nordic/nrf54l15dk__
3) Inspect the available files.
4) Identify:
   - board.yml
   - Kconfig.board
   - Kconfig.defconfig
   - *.dts
   - *.yaml
   - board.cmake

## ▶️ Step 2 - Using _Create a new board_ Wizard
5) Click on _Create a new board_ button within the __nRF Connect__ extension.
6) Fill out the form in the Board Wizard window. Use the following settings:
   - __Board name__: lab_board
   - __Description__: my own custom board
   - __Vendor name__: nordic
   - __SoC__: nRF54L15-QFAA
   - __Board root__: c:/Nordic/myBoards

   > 💡 __Tip:__ The board root location setting does not automatically include in most cases the board to the board list. We will do this in the next step.
   >  There are different possibilities:
   > - Application-local / freestanding folder: Inside the application source tree or in a <code>boards</code> folder in application source tree.
   > - Freestanding folder elsewhere: Including path via CMakeLists.txt or VS Code Board root setting.
   > - west workspace: for example, by using Application Example Add-on
   > - In-tree (inside the Zephyr repository): only if Zephyr upstream merges are planned

## ▶️ Step 3 - Modify the DeviceTree
7) Open the board DTS file: __lab_board_nrf54l15_cpuapp.dts__
8) Review the existing peripheral configuration.
9) Add the GPIO assignments for your LED. We will use nRF54L15DK's LED3, which is on GPIO P1.14.

   ``` 
   / {
	    leds {
		     compatible = "gpio-leds";
  
	         myled: myled {
	              gpios = <&gpio1 14 GPIO_ACTIVE_HIGH>;
                  label = "My LED";
	         };
	    };
    };

    &gpio1 {
	    status = "okay";
    };

    &gpiote20 {
	    status = "okay"; 
    };
   ```

## ▶️ Step 4 - Modify KCONFIG
10) Add GPIO software module by adding following line in file __lab_board_nrf54l15_cpuapp_defconfig__.

   ``` kconfig
   CONFIG_GPIO=y
   ```

## ▶️ Step 5 - Add custom board folder to Board Root list
11) Add the path of our custom board to the boord root list within Visual Studio Code:

    File -> Preferences -> Settings -> Extensions -> nRF Connect -> Board Roots

    Add your custom board path there, e.g. __c:/Nordic/myBoards__


## ▶️ Step 6 - Use custom Board in _blinky_ sample
12) Copy Zephyr's __blinky__ sample. (__zephyr/samples/basic/blinky__)
13) Replace LED0_NODE definition in main.c file:

    ```c
	#define LED0_NODE   DT_NODELABEL(myled)
    ```
14) __Add build configuration__ and use following settings:
    - Board Target: lab_board/nrf54l15/cpuapp
15) Click on the __Generate and Build__ button.


## ▶️ Step 7 - Flash and Test
15) Download the project by clicking on __Flash__ button.
16) Does the expected LED blink?
