/*
 * DeviceTree Workshop - nRF54L15DK / nRF Connect SDK v3.4.0
 *
 * This app is used across Exercises 1 and 2 of the hands-on guide:
 *
 *  Exercise 1 (Explore): build as-is, then go read
 *      build/dt_workshop_app/zephyr/zephyr.dts
 *      build/dt_workshop_app/zephyr/include/generated/zephyr/devicetree_generated.h
 *  and match what you find there to the DT_ macros below.
 *
 *  Exercise 2 (Extend): the board overlay (boards/nrf54l15dk_nrf54l15_cpuapp.overlay)
 *  adds an alias ("my-led") and a custom node ("fake_sensor0") using the
 *  "workshop,fake-sensor" binding. This file reads both back out.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* ---- Node identifiers -----------------------------------------------
 * DT_ALIAS()     resolves an /aliases entry     (led0, sw0 ship with the
 *                board files; my-led is added by our overlay)
 * DT_NODELABEL() resolves a devicetree node label directly
 * These all resolve entirely at *compile time* - there is no runtime
 * parsing of devicetree in Zephyr.
 * ---------------------------------------------------------------------
 */
#define LED0_NODE    DT_ALIAS(led0)
#define BUTTON0_NODE DT_ALIAS(sw0)
#define SENSOR_NODE  DT_NODELABEL(fake_sensor0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay) && DT_NODE_HAS_STATUS(BUTTON0_NODE, okay)
static const struct gpio_dt_spec led0    = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios);
#else
#error "led0 / sw0 aliases not found - check that you are building for " \
       "nrf54l15dk/nrf54l15/cpuapp and that the board files are intact."
#endif

static void print_led0_info(void)
{
	printk("\n-- LED0 (alias 'led0') --------------------------------\n");
	printk("  full node path : %s\n", DT_NODE_PATH(LED0_NODE));
	printk("  GPIO controller: %s\n", led0.port->name);
	printk("  GPIO pin       : %d\n", led0.pin);
	printk("  active flags   : 0x%02x\n", led0.dt_flags);
}

static void print_my_led_info(void)
{
#if DT_NODE_EXISTS(DT_ALIAS(my_led))
	/* Note: this branch only compiles once the Exercise 2 overlay
	 * (which adds the "my-led" alias) has been applied. */
	#define MY_LED_NODE DT_ALIAS(my_led)
	static const struct gpio_dt_spec my_led = GPIO_DT_SPEC_GET(MY_LED_NODE, gpios);

	printk("\n-- my-led (alias added in Exercise 2) -----------------\n");
	printk("  resolves to    : %s\n", DT_NODE_PATH(MY_LED_NODE));
	printk("  GPIO pin       : %d\n", my_led.pin);
#else
	printk("\n-- my-led alias not found ------------------------------\n");
	printk("  Apply the Exercise 2 overlay (boards/%s.overlay) and rebuild.\n",
	       CONFIG_BOARD);
#endif
}

static void print_sensor_info(void)
{
	printk("\n-- fake_sensor0 (custom binding, Exercise 2) ----------\n");
#if DT_NODE_EXISTS(SENSOR_NODE)
	printk("  has compatible \"workshop,fake-sensor\": %s\n",
	       DT_NODE_HAS_COMPAT(SENSOR_NODE, workshop_fake_sensor) ? "yes" : "no");
	printk("  sensor-label   : %s\n", DT_PROP(SENSOR_NODE, sensor_label));
	printk("  sample freq    : %d Hz\n", DT_PROP(SENSOR_NODE, sampling_frequency_hz));
	printk("  status         : %s\n",
	       DT_NODE_HAS_STATUS(SENSOR_NODE, okay) ? "okay" : "disabled");

#if DT_NODE_HAS_STATUS(SENSOR_NODE, okay)
	static const struct gpio_dt_spec sensor_io = GPIO_DT_SPEC_GET(SENSOR_NODE, io_gpios);
	printk("  io-gpios pin   : %d (on %s)\n", sensor_io.pin, sensor_io.port->name);
#endif
#else
	printk("  Node not found - apply the Exercise 2 overlay and rebuild.\n");
#endif
}

int main(void)
{
	printk("\n=== DeviceTree Workshop - nRF54L15DK (NCS v3.4.0) ===\n");

	if (!device_is_ready(led0.port)) {
		printk("ERROR: LED0 GPIO controller is not ready!\n");
		return 0;
	}
	if (!device_is_ready(button0.port)) {
		printk("ERROR: Button0 GPIO controller is not ready!\n");
		return 0;
	}

	gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&button0, GPIO_INPUT);

	print_led0_info();
	print_my_led_info();
	print_sensor_info();

	printk("\nHold Button0 to light LED0. Try `device list` on the shell.\n\n");

	while (1) {
		bool pressed = gpio_pin_get_dt(&button0) > 0;

		gpio_pin_set_dt(&led0, pressed);
		k_msleep(50);
	}
	return 0;
}
