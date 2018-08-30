#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>



static struct kobject *kobj;
static int transaction_complete;

static ssize_t driver_read(struct kobject *kobj, struct kobj_attribute *attr,
	char *buffer)
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

static ssize_t driver_write(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buffer, size_t len)
{
	static char toWrite[256];
	static int i = 0;
        i = 0;
	pr_info("someone want to tell me something >:)\n");
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

static struct kobj_attribute kobjAttr =
	__ATTR(speak_to_me, 0664, driver_read, driver_write);

static int __init hello_proc_init(void)
{
	transaction_complete = 0;
	kobj = kobject_create_and_add("my_sys_mod", kernel_kobj);
	if (sysfs_create_file(kobj, &kobjAttr.attr)){
		kobject_put(kobj);
		sysfs_remove_file(kobj, &kobjAttr.attr);
		pr_info("Something wrong with my module :(\n");
		return -1;
	}
	pr_info("Sysfs module is started\n");
	return 0;
}

static void __exit hello_proc_exit(void)
{
	kobject_put(kobj);
	sysfs_remove_file(kernel_kobj, &kobjAttr.attr);
	pr_info("Module is shutted down\n");
}

MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
