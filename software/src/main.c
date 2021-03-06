/* nfc-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * main.c: Initialization for NFC Bricklet
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdbool.h>

#include "configs/config.h"

#include "bricklib2/bootloader/bootloader.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/logging/logging.h"
#include "communication.h"
#include "pn7150.h"

#include "bricklib2/os/coop_task.h"
#include "xmc_gpio.h"

CoopTask pn7150_task;

uint32_t count = 0;

void HardFault_Handler() {
	loge("HardFault_Handler\n\r");
	while(true);
}

int main(void) {
	logging_init();
	logd("Start NFC Bricklet\n\r");

	communication_init();
	coop_task_init(&pn7150_task, pn7150_tick);

	uint32_t t = system_timer_get_ms();
	while(true) {
		if(system_timer_is_time_elapsed_ms(t, 1000)) {
			t = system_timer_get_ms();

			logi("tick: %u\n\r", count);
			count = 0;
		}

		bootloader_tick();
		communication_tick();
		coop_task_tick(&pn7150_task);
	}
}
