// SPDX-License-Identifier: Dual BSD/GPL
/*
 * Copyright (c) 2026 Anastasia Chukhrai
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_AUTHOR("Anastasia Chukhrai");
MODULE_DESCRIPTION("Lab 5: hello1 module exporting print_hello()");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");

struct my_event {
	struct list_head list;
	ktime_t start_time;
	ktime_t end_time;
};

static LIST_HEAD(event_list_head);

int print_hello(void)
{
	struct my_event *element;

	element = kmalloc(sizeof(*element), GFP_KERNEL);
	if (!element) {
		pr_err("hello1: memory allocation failed\n");
		return -ENOMEM;
	}

	element->start_time = ktime_get();
	pr_info("Hello, world!\n");
	element->end_time = ktime_get();

	list_add_tail(&element->list, &event_list_head);

	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct my_event *md, *tmp;
	s64 duration_ns;

	list_for_each_entry_safe(md, tmp, &event_list_head, list) {
		duration_ns = ktime_to_ns(ktime_sub(md->end_time,
						    md->start_time));
		pr_info("hello1: print duration: %lld ns\n", duration_ns);
		list_del(&md->list);
		kfree(md);
	}

	pr_info("hello1: module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
