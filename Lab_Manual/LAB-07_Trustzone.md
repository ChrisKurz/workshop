# Lab 7 - Trustzone

## Overview
In this hands-on workshop, participants will explore the ARM® TrustZone® security architecture implemented on the nRF54L15. Using Trusted Firmware-M (TF-M) and the nRF Connect SDK, they will build and execute a Non-secure application that interacts with Secure-world services through the Platform Security Architecture (PSA) API.

The workshop demonstrates how the Secure and Non-secure worlds are isolated while still allowing controlled communication through secure service calls. Participants will observe both execution environments running simultaneously and verify the separation using a Secure-world cryptographic service.

## Exercise 1 - Getting familiar with Trustzone Secure/Non-Secure Separation
### ▶️ Step 1 - Explore the Project Structure
1) Download the lab project from [here](lab_projects/LAB7_trustzone.zip)
2) Open the project.
   > 💡 __Note:__ We will _Add build configuration_ later on!

3) Review the project directory structure.
4) Identify the following files:
   - prj.conf
   - src/main.c
   - boards/nrf54l15dk_nrf54l15_cpuapp_ns.overlay

5) Discuss the purpose of each file.


### ▶️ Step 2 - Select the Correct Board Target
6) Compare the following board targets:
   - nrf54l15dk/nrf54l15/cpuapp
   - nrf54l15dk/nrf54l15/cpuapp/ns
7) Discuss the differences.
8) Now, __Add build configuration__. Use following settings:

   __Board Target__: <code>nrf54l15dk/nrf54l15/cpuapp/ns</code>
   
### ▶️ Step 3 - Build the TrustZone Application
9) Open the terminal to check the outputs during build process. 
10) Observe the build output.
11) Identify the Secure and Non-secure images produced during the build.


### ▶️ Step 4 - Review the TrustZone Configuration
12) Open prj.conf.
13) Locate the TF-M related configuration options.
14) Identify the selected TF-M profile.
15) Discuss why the Secure and Non-secure worlds cannot share the same peripheral simultaneously.

### ▶️ Step 5 - Flash and Observe TrustZone
16) Flash the firmware:
17) Open two Serial Terminals.
18) Connect one terminal to the Secure UART output. (VCOM 1)
19) Connect the second terminal to the Non-secure application console. (VCOM 0)
20) Reset the board.
21) Observe the Secure boot messages.
22) Observe the Non-secure application output.


## Exercise 2 - Using Secure PSA API

### ▶️ Step 6 - Execute a Secure PSA Crypto Service
23) Inspect the PSA Crypto function call in main.c.
24) Build and flash the application if necessary.
25) Observe the application output.
26) Verify that random data is successfully returned.
27) Discuss where the cryptographic operation actually executes.

### ▶️ Step 7 - Execute a Secure PSA Crypto Service
28) Download the lab project from here
29) Open the project within VS code
30) What happens if you forget to set CONFIG_TFM_PARTITION_STORAGE=y ?
31) Which PSA Secure Storage calls are used?
