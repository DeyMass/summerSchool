#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>

static ssize_t driver_read(struct file *file, char *buffer, size_t length, loff_t *offset);
static ssize_t driver_write(struct file *filp, const char *buff, size_t len, loff_t * off);

static int transaction_complete = 0;

static struct file_operations fops = {
	.read = driver_read,
	.write = driver_write
};

static ssize_t driver_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
	if (transaction_complete == 0){
		pr_info("someone just tried to read my power >:)\n");
		sprintf(buffer, "you are unworthy! >:)\n");
		transaction_complete = 1;
		return 22;
	}
	transaction_complete = 0;
	return 0;
}

static ssize_t driver_write(struct file *file, const char *buffer, size_t len, loff_t * off)
{
	char toWrite[256];
	static int i = 0;
	pr_info("someone wants to say something >:)");
	i = 0;
	while(buffer[i] != '\n'){
		toWrite[i] = buffer[i];
		i++;
	}
	toWrite[i++] = '\n';
	toWrite[i] = 0;
	pr_info("he wants to\n");
	printk(toWrite);
	return len;
}

static int __init mod_start(void)
{
	int majorId = register_chrdev(333, "my_super_name", &fops);
	if (majorId < 0){
		pr_info("My beautifull module... It's not working\n");
		return -1;
	}
	pr_info("My beautifull module is working!\n");
	return 0;
}

void __exit mod_exit(void)
{
	unregister_chrdev(333, "my_super_name");
	pr_info("My beautifull module is going down\n");
}

module_init(mod_start);
module_exit(mod_exit);

MODULE_AUTHOR("MICHAIL");
MODULE_DESCRIPTION("TEST_DRIVER");
MODULE_LICENSE("GPL");
