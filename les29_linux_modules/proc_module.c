#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int transaction_complete;

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
        pr_info("someone want to tell me something >:)\n");
        static char toWrite[256];
        static int i = 0;
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

static const struct file_operations hello_proc_fops = {
	.read = driver_read,
	.write = driver_write
};

static int __init hello_proc_init(void) {
	transaction_complete = 0;
	proc_create("hello_proc", 0, NULL, &hello_proc_fops);
	return 0;
}

static void __exit hello_proc_exit(void) {
	remove_proc_entry("hello_proc", NULL);
}

MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
