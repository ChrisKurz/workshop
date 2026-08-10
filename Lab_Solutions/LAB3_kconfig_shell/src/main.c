/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

int main(void)
{
	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

	LOG_ERR("This is an ERROR level log message");
        LOG_WRN("This is a WARNING level log message");
        LOG_INF("This is an INFO level log message");
        LOG_DBG("This is a DEBUG level log message");

	while(1) {
		k_msleep(2000);
		LOG_INF("heartbeat\n");
	}


	return 0;
}
