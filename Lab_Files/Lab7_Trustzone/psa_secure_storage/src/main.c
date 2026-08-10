/*
 * PSA Protected Storage hands-on
 * ------------------------------
 * Demonstrates the PSA Certified Secure Storage API (Protected Storage
 * flavor) on the nRF54L15, running the application as the Non-Secure
 * Processing Environment (NSPE) on top of Trusted Firmware-M (TF-M).
 *
 * Every psa_ps_*() call below is an IPC request from this NS application
 * to the "Protected Storage" secure partition running inside TF-M, in
 * the Secure Processing Environment (SPE). The actual UID/data table and
 * the flash region it lives in are only accessible to that secure
 * partition; this NS binary never touches the flash area directly.
 */

#include <stdio.h>
#include <string.h>

#include <zephyr/kernel.h>

#include <psa/protected_storage.h>

/* Two arbitrary, application-chosen 64-bit UIDs identifying the assets. */
#define UID_DEVICE_TOKEN   ((psa_storage_uid_t)1)
#define UID_FACTORY_SERIAL ((psa_storage_uid_t)2)

static const char device_token[]   = "super-secret-provisioning-token-42";
static const char device_token_v2[] = "rotated-provisioning-token-99";
static const char factory_serial[] = "SN-NRF54L15-0001";

static void print_status(const char *what, psa_status_t status)
{
	printk("%-42s -> %d\n", what, (int)status);
}

static void dump_info(psa_storage_uid_t uid)
{
	struct psa_storage_info_t info;
	psa_status_t status = psa_ps_get_info(uid, &info);

	if (status != PSA_SUCCESS) {
		print_status("  psa_ps_get_info()", status);
		return;
	}

	printk("  UID 0x%llx info: size=%u capacity=%u flags=0x%x\n",
	       (unsigned long long)uid, (unsigned int)info.size,
	       (unsigned int)info.capacity, (unsigned int)info.flags);
}

int main(void)
{
	psa_status_t status;
	char read_buf[64];
	size_t read_len;

	printk("\n=== PSA Protected Storage hands-on (nRF54L15 + TF-M) ===\n\n");

	/* 1. Store a regular (rewritable) secret in Protected Storage. */
	printk("Step 1: Write device token (no special flags)\n");
	status = psa_ps_set(UID_DEVICE_TOKEN, sizeof(device_token),
			    device_token, PSA_STORAGE_FLAG_NONE);
	print_status("  psa_ps_set(UID_DEVICE_TOKEN)", status);
	dump_info(UID_DEVICE_TOKEN);

	/* 2. Read it back and verify the round-trip. */
	printk("\nStep 2: Read the device token back\n");
	memset(read_buf, 0, sizeof(read_buf));
	status = psa_ps_get(UID_DEVICE_TOKEN, 0, sizeof(device_token),
			    read_buf, &read_len);
	print_status("  psa_ps_get(UID_DEVICE_TOKEN)", status);
	if (status == PSA_SUCCESS) {
		printk("  Read back (%u bytes): %s\n", (unsigned int)read_len,
		       read_buf);
		printk("  Matches original: %s\n",
		       memcmp(read_buf, device_token, sizeof(device_token)) == 0
			       ? "yes" : "NO (!)");
	}

	/* 3. Overwrite it (allowed, since it has no write-once flag). */
	printk("\nStep 3: Rotate/overwrite the device token\n");
	status = psa_ps_set(UID_DEVICE_TOKEN, sizeof(device_token_v2),
			    device_token_v2, PSA_STORAGE_FLAG_NONE);
	print_status("  psa_ps_set(UID_DEVICE_TOKEN, v2)", status);

	memset(read_buf, 0, sizeof(read_buf));
	status = psa_ps_get(UID_DEVICE_TOKEN, 0, sizeof(device_token_v2),
			    read_buf, &read_len);
	if (status == PSA_SUCCESS) {
		printk("  Read back after rotation: %s\n", read_buf);
	}

	/* 4. Store the factory serial as WRITE_ONCE: tamper/overwrite
	 *    protection enforced by the secure partition, not by this
	 *    (untrusted) NS application.
	 */
	printk("\nStep 4: Write factory serial with PSA_STORAGE_FLAG_WRITE_ONCE\n");
	status = psa_ps_set(UID_FACTORY_SERIAL, sizeof(factory_serial),
			    factory_serial, PSA_STORAGE_FLAG_WRITE_ONCE);
	print_status("  psa_ps_set(UID_FACTORY_SERIAL, WRITE_ONCE)", status);
	dump_info(UID_FACTORY_SERIAL);

	/* 5. Try to overwrite the write-once asset: TF-M must refuse this
	 *    with PSA_ERROR_NOT_PERMITTED, even though the NS application
	 *    code asking for it is fully under our control.
	 */
	printk("\nStep 5: Attempt to overwrite the write-once factory serial\n");
	status = psa_ps_set(UID_FACTORY_SERIAL, sizeof("HACKED"), "HACKED",
			    PSA_STORAGE_FLAG_NONE);
	print_status("  psa_ps_set(UID_FACTORY_SERIAL, again)", status);
	printk("  Expected PSA_ERROR_NOT_PERMITTED (%d): %s\n",
	       (int)PSA_ERROR_NOT_PERMITTED,
	       status == PSA_ERROR_NOT_PERMITTED ? "confirmed" : "UNEXPECTED");

	/* 6. Try to remove the write-once asset: also refused. */
	printk("\nStep 6: Attempt to remove the write-once factory serial\n");
	status = psa_ps_remove(UID_FACTORY_SERIAL);
	print_status("  psa_ps_remove(UID_FACTORY_SERIAL)", status);

	/* 7. Clean up the rewritable asset (this one IS removable). */
	printk("\nStep 7: Remove the (rewritable) device token\n");
	status = psa_ps_remove(UID_DEVICE_TOKEN);
	print_status("  psa_ps_remove(UID_DEVICE_TOKEN)", status);

	{
		struct psa_storage_info_t info;

		status = psa_ps_get_info(UID_DEVICE_TOKEN, &info);
		printk("  psa_ps_get_info() after removal -> %d (expect %d = DOES_NOT_EXIST)\n",
		       (int)status, (int)PSA_ERROR_DOES_NOT_EXIST);
	}

	printk("\n=== Example finished ===\n");

	return 0;
}
