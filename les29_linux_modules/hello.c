#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int __exit test_init(void)
{
	pr_info("HELLO WORLD is initialized\n");
	return 0;
}

static void __exit test_exit(void)
{
	pr_info("Helloworld is exit\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DESCRIPRION");
MODULE_AUTHOR("ME");
