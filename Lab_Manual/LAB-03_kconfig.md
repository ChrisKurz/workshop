# Lab 3 - KCONFIG

# Overview
Kconfig is the configuration system shared by Zephyr and the nRF Connect SDK. It is how you turn features on and off, tune buffer sizes, select drivers, and — critically — how the build system enforces that a feature is only enabled when everything it depends on is also present. Reading and reasoning about Kconfig correctly is one of the most important skills for working productively in the nRF Connect SDK, because almost every subsystem (logging, Bluetooth, power management, security) is gated behind Kconfig symbols with non-trivial dependency chains.

In this workshop, **logging** and **shell** are used as examples to practise this skill.

## ▶️ Step 1 — Create and Flash the `hello_world` Project

1) The __hello_world__ sample ships with Zephyr and is included in every NCS checkout. Rather than building directly inside the SDK tree, copy it out into a workspace folder so you can edit it freely.

   > 💡 __Tip:__ Instead of manually copying the __hello world__ project using File Explorer, you can also use the __Create a new application__ -> __Copy a sample__ feature in the __nRF Connect__ view.  

2) _Add build configuration_ with following settings:
   - Board target: nRF54L15DK/nRF54L15/cpuapp

3) Flash the project and check Serial Terminal output. 


## ▶️ Step 2 — Add Logging With the Default Configuration
4) Enable Logging subsystem by adding following line to __prj.conf__ file:

  ``` kconfig
  CONFIG_LOG=y
  ```
5) Register a Log module by adding following lines in __main.c__ file:

  ``` C
  #include <zephyr/kernel.h>
  #include <zephyr/logging/log.h>

  LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);
  ```

> 💡 __Tip:__ `LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG)` declares a logging *module* named `app` and sets its **compiled-in** maximum severity to `DEBUG`. This is a source-level macro, not a Kconfig symbol, but it interacts with Kconfig: if the global logging level configured via Kconfig (`CONFIG_LOG_MAX_LEVEL` / `CONFIG_LOG_DEFAULT_LEVEL`) were set lower than `DEBUG`, your `LOG_DBG()` calls would be compiled out regardless of what you pass to `LOG_MODULE_REGISTER`. This is a first, small example of a **dependency between a Kconfig symbol and code you write**.
> Further Log levels are <code>LOG_LEVEL_INF</code>, <code>LOG_LEVEL_WRN</code>, <code>LOG_LEVEL_ERR</code> and <code>LOG_LEVEL_NONE</code>.

6) Add log calls in <code>main()</code> function:

   ``` c
       LOG_ERR("This is an ERROR level log message");
       LOG_WRN("This is a WARNING level log message");
       LOG_INF("This is an INFO level log message");
       LOG_DBG("This is a DEBUG level log message");
   ```

7) Build, flash and check Serial Terminal output.

## ▶️ Step 3 — Enable Logging commands in the Zephyr Shell

8) Update __prj_conf__ and change logging configuration to:

   ``` kconfig
   CONFIG_LOG=y

   # Enable LOG commands in Shell
   CONFIG_LOG_CMDS=y
   ```

9) Rebuild, flash and check Serial Terminal

10) Why doesn't it work? What is the warning message in the terminal log?

## ▶️ Step 4 — Using the nRF Kconfig GUI to Trace Dependencies
> 💡 Manually editing `prj.conf` and grepping `.config` is a valid workflow, but it doesn't show you the *reasoning* behind a symbol — its help text, its `depends on` clause, what it `select`s, and what its `default` condition is. That reasoning is exactly what the **nRF Kconfig GUI**, built into nRF Connect for VS Code, is designed to surface.

11) Open _nRF Kconfig GUI_ tool
12) Find <code>CONFIG_LOG_CMDS</code> and check its dependencies.
13) Which dependency is blocking here?
14) Fix it, rebuild, flash.
15) Is the <code>log</code> instruction available in the shell?

    > 💡 _Tip_: Check by entering <code>help</code> in the Serial Terminal.

## ▶️ Step 5 — Add Logging via RTT
16) Let's add the snippet <code>rtt_console</code> to our __Build Configuration__.
17) Please disable the Zephyr __Shell__ in the project.
18) Add a heart beat message to the application by adding following lines:

    ``` c
    while(1) {
        k_msleep(2000);
        LOG_INF("heartbeat");
    }      
    ```
19) Check the RTT output.
