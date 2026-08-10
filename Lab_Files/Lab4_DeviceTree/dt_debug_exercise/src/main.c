/*
 * DeviceTree Debugging Exercise - nRF54L15DK
 *
 * This code itself is correct. It will not build until the
 * accompanying board overlay (boards/nrf54l15dk_nrf54l15_cpuapp.overlay)
 * has been fixed - see that file and the hands-on guide.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define WEATHER_NODE DT_NODELABEL(weather0)
#define BACKUP_NODE  DT_NODELABEL(backup0)

static const struct gpio_dt_spec weather_io = GPIO_DT_SPEC_GET(WEATHER_NODE, io_gpios);
static const struct gpio_dt_spec backup_io  = GPIO_DT_SPEC_GET(BACKUP_NODE, io_gpios);

int main(void)
{
	printk("\n=== DeviceTree Debugging Exercise ===\n");

	printk("weather0: label=%s freq=%dHz pin=%d\n",
	       DT_PROP(WEATHER_NODE, sensor_label),
	       DT_PROP(WEATHER_NODE, sampling_frequency_hz),
	       weather_io.pin);

	printk("backup0 : label=%s freq=%dHz pin=%d\n",
	       DT_PROP(BACKUP_NODE, sensor_label),
	       DT_PROP(BACKUP_NODE, sampling_frequency_hz),
	       backup_io.pin);

	if (device_is_ready(weather_io.port) && device_is_ready(backup_io.port)) {
		printk("Both DT-described devices report ready. Overlay is fixed!\n");
	}

	while (1) {
		k_msleep(1000);
	}
	return 0;
}
