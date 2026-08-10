/*
 * Lab 3 - Device Power Management
 *
 * Demonstrates Device Runtime PM: pm_device_runtime_get()/put() bracketing
 * an intermittent UART transaction on a peripheral marked
 * "zephyr,pm-device-runtime-auto" in the board overlay, with a
 * PPK2-visible marker GPIO toggled around each transaction.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/pm/device_runtime.h>
#include <zephyr/sys/printk.h>

#define LED0_NODE    DT_ALIAS(led0)
#define MARKER_NODE  DT_PATH(zephyr_user)
#define DEMO_UART    DT_NODELABEL(uart21)

static const struct gpio_dt_spec led    = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec marker = GPIO_DT_SPEC_GET(MARKER_NODE, marker_gpios);
static const struct device *demo_uart   = DEVICE_DT_GET(DEMO_UART);

#define IDLE_PERIOD_MS    1500
#define BURST_LEN_BYTES   8
#define VISIBILITY_HOLD_MS 60

static void send_demo_burst(void)
{
	static uint8_t seq;
	int ret;

	/* Mark the start of the transaction on the PPK2 trace. */
	gpio_pin_set_dt(&marker, 1);

	/*
	 * Device Runtime PM: ask for the device. If it was suspended, it is
	 * resumed here (synchronously) before the call returns.
	 */
	ret = pm_device_runtime_get(demo_uart);
	if (ret < 0) {
		printk("pm_device_runtime_get failed: %d\n", ret);
		gpio_pin_set_dt(&marker, 0);
		return;
	}

	for (int i = 0; i < BURST_LEN_BYTES; i++) {
		uart_poll_out(demo_uart, (uint8_t)(seq + i));
	}
	seq++;

	/*
	 * The actual burst above is well under 1 ms (8 bytes at 115200
	 * baud) - too short to see on a PPK2 trace at a normal, multi-second
	 * zoom level. Hold the marker high (and the device held active via
	 * runtime PM) for a bit longer purely so the pulse is easy to spot
	 * without having to zoom in to a few milliseconds at exactly the
	 * right moment. Remove this hold once you're comfortable finding
	 * the real, much shorter pulse in a zoomed-in view.
	 */
	k_msleep(VISIBILITY_HOLD_MS);

	/*
	 * Release our usage reference. If nobody else holds one, the device
	 * is suspended right away.
	 */
	ret = pm_device_runtime_put(demo_uart);
	if (ret < 0) {
		printk("pm_device_runtime_put failed: %d\n", ret);
	}

	/* Mark the end of the transaction on the PPK2 trace. */
	gpio_pin_set_dt(&marker, 0);

	printk("Sent %d-byte burst over %s (runtime PM get/put around it, "
	       "marker held %d ms for visibility)\n",
	       BURST_LEN_BYTES, demo_uart->name, VISIBILITY_HOLD_MS);
}

int main(void)
{
	printk("Lab 3: PM_DEVICE_RUNTIME=%d\n",
	       IS_ENABLED(CONFIG_PM_DEVICE_RUNTIME));

	if (!gpio_is_ready_dt(&led) || !gpio_is_ready_dt(&marker)) {
		printk("Error: GPIO device(s) not ready\n");
		return 0;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&marker, GPIO_OUTPUT_INACTIVE);

	if (!device_is_ready(demo_uart)) {
		printk("Error: demo UART device not ready\n");
		return 0;
	}

	/*
	 * With "zephyr,pm-device-runtime-auto" set in the overlay, the
	 * device already starts suspended - no explicit
	 * pm_device_runtime_enable() call is needed here.
	 */

	while (1) {
		gpio_pin_toggle_dt(&led);

		send_demo_burst();

		/*
		 * Idle window between bursts: with Device Runtime PM, the
		 * UART instance is suspended for this entire window even
		 * though the LED toggle loop itself keeps running.
		 */
		k_msleep(IDLE_PERIOD_MS);
	}

	return 0;
}
