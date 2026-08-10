/*
 * TrustZone workshop demo — Non-secure application
 *
 * Board:  nRF54L15 DK
 * SDK:    nRF Connect SDK v3.4.0
 * Target: nrf54l15dk/nrf54l15/cpuapp/ns
 *
 * This code runs entirely in the Non-secure world. It:
 *   1. Prints a boot banner so you can see, on the console UART, that the
 *      Non-secure image is alive and separate from TF-M's own Secure-side
 *      boot log (which appears on UART1 — see boards/*.overlay).
 *   2. Calls into the Secure world via the PSA Crypto API to generate
 *      random bytes. The call crosses the Non-secure Callable veneer,
 *      executes inside Trusted Firmware-M using the CRACEN hardware
 *      accelerator, and returns a result — without this file ever
 *      touching a key, the RNG peripheral, or any Secure memory directly.
 *   3. Idles, printing a heartbeat so you can confirm the app keeps running.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(trustzone_demo, LOG_LEVEL_INF);

#if defined(CONFIG_BUILD_WITH_TFM)
#include <psa/crypto.h>

/* Demonstrates calling a Secure-world service (PSA Crypto) from
 * Non-secure code. See workshop slide "Going further: call a Secure
 * crypto service".
 */
static void secure_random_demo(void)
{
	psa_status_t status;
	uint8_t random_bytes[16];

	status = psa_crypto_init();
	if (status != PSA_SUCCESS) {
		LOG_ERR("psa_crypto_init() failed: %d", (int)status);
		return;
	}

	status = psa_generate_random(random_bytes, sizeof(random_bytes));
	if (status != PSA_SUCCESS) {
		LOG_ERR("psa_generate_random() failed: %d", (int)status);
		return;
	}

	LOG_INF("Called into the SECURE world via PSA Crypto — "
		"execution happened inside TF-M / CRACEN, not here.");
	LOG_HEXDUMP_INF(random_bytes, sizeof(random_bytes),
			 "Random bytes returned by the Secure service:");
}
#endif /* CONFIG_BUILD_WITH_TFM */

int main(void)
{
	LOG_INF("*** TrustZone workshop demo ***");
	LOG_INF("Hello World! Board: %s", CONFIG_BOARD_TARGET);
	LOG_INF("This log is coming from the NON-SECURE application image.");

#if defined(CONFIG_BUILD_WITH_TFM)
	LOG_INF("CONFIG_BUILD_WITH_TFM=y — Trusted Firmware-M is running "
		"in the Secure world on this device.");
	secure_random_demo();
#else
	LOG_WRN("CONFIG_BUILD_WITH_TFM is not set — this build has no "
		"Secure/Non-secure separation. Build for the /ns board "
		"target to enable TrustZone. See README.md.");
#endif

	while (1) {
		k_sleep(K_SECONDS(5));
		LOG_INF("Non-secure application heartbeat...");
	}

	return 0;
}
