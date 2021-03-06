/*
 * Copyright (c) 2015, Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include "infra/boot.h"
#include <stdbool.h>
#include <util/compiler.h>
#include "machine/soc/intel/quark_se/quark/reboot_reg.h"

static enum boot_targets _boot_target = TARGET_MAIN;

/*
 * this is application main entry point, it will decide what to boot according
 * to the boot target.
 */
__noreturn void boot(void *param)
{
	_boot_target = GET_REBOOT_REG(SCSS_GPS0, BOOT_TARGETS_MASK,
				      BOOT_TARGETS_POS);
	bool factory_mode;
	switch (_boot_target) {
	case TARGET_RECOVERY:
#ifdef CONFIG_RECOVERY_BOOT_TARGET
		recovery_task(param);
#else
		factory_mode = true;
		main_task((void *)&factory_mode);
#endif
		break;

	case TARGET_FACTORY:
		factory_mode = true;
		main_task((void *)&factory_mode);
		break;

	case TARGET_MAIN:
	default:
		factory_mode = false;
		main_task((void *)&factory_mode);
		break;
	}
	__builtin_unreachable();
}

enum boot_targets get_boot_target(void)
{
	return _boot_target;
}
