// SPDX-License-Identifier: Dual BSD/GPL
/*
 * Copyright (c) 2026 Anastasia Chukhrai
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_AUTHOR("Anastasia Chukhrai");
MODULE_DESCRIPTION("Lab 5: hello2 module using print_hello()");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");

static uint count = 1;
module_param(count, uint, 0644);
MODULE_PARM_DESC(count, "Number of times to call print_hello()");

static int __init hello2_init(void)
{
	int i;
	int ret;

	if (count > 10) {
		pr_err("hello2: error, count (%u) is greater than 10\n", count);
		return -EINVAL;
	}

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("hello2: warning, count is %u\n", count);

	for (i = 0; i < count; i++) {
		ret = print_hello();
		if (ret) {
			pr_err("hello2: print_hello() failed with %d\n", ret);
			return ret;
		}
	}

	pr_info("hello2: module loaded (count=%u)\n", count);
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
