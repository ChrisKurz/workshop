/*
 * Lab 1 - Baseline application
 *
 * A deliberately "unoptimized" application: toggles the DK's LED0 once per
 * second and logs every toggle over the default UART console. No system or
 * device power management is enabled (see prj.conf). Use this build to take
 * the "Lab 1 baseline" current measurement described in the workshop
 * (Module 3, or the datasheet typical-current figure if no PPK2 is
 * available).
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define SLEEP_TIME_MS 1000

int main(void)
{
	int ret;
	bool led_state = false;

	printk("Lab 1 baseline: PM disabled, LED toggling every %d ms\n",
	       SLEEP_TIME_MS);

	if (!gpio_is_ready_dt(&led)) {
		printk("Error: LED device not ready\n");
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error %d: failed to configure LED\n", ret);
		return 0;
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			printk("Error %d: failed to toggle LED\n", ret);
			return 0;
		}

		led_state = !led_state;
		printk("LED state: %s\n", led_state ? "ON" : "OFF");

		/*
		 * NOTE: with CONFIG_PM disabled, the idle thread only ever
		 * issues a light k_cpu_idle() between wake-ups - there is no
		 * deep sleep state selection happening here. That is exactly
		 * what Lab 2 changes.
		 */
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
