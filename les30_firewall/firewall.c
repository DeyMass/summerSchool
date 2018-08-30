#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/netfilter.h>
#include <linux/netfilter_decnet.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/netpoll.h>

static unsigned short int blockedPort = 443;
static struct kobject *kobj;

static unsigned int capture(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	struct iphdr *iphdr = ip_hdr(skb);
	struct udphdr *udp;
	struct tcphdr *tcp;
	pr_info("captured packet(%i) [tcp: %i udp: %i]\n", iphdr->protocol, IPPROTO_TCP, IPPROTO_UDP);
	switch(iphdr->protocol){
		case IPPROTO_UDP:
			udp = udp_hdr(skb);
			if (udp->dest == htons(blockedPort))
				return NF_DROP;
			return NF_ACCEPT;
		case IPPROTO_TCP:
			tcp = tcp_hdr(skb);
			if (tcp->dest == htons(blockedPort))
				return NF_DROP;
			return NF_ACCEPT;
		default:
			return NF_ACCEPT;
	}
}

static const struct nf_hook_ops nf_ops = {
	.hook		= capture,
	.pf		= AF_INET,
	.hooknum	= NF_DN_POST_ROUTING,
	.priority	= NF_DN_PRI_DNRTMSG,
};


static ssize_t port_get(struct kobject *kobj, struct kobj_attribute *attr,
	char *buffer)
{
	pr_info("Blocking port info requested\n");
	return sprintf(buffer, "%i\n", blockedPort);
}

static ssize_t port_update(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buffer, size_t len)
{
	sscanf(buffer, "%hu", &blockedPort);
        pr_info("Blocking port updated to %i\n", blockedPort);
        return len;
}

static struct kobj_attribute kobjAttr =
	__ATTR(blocked_port, 0664, port_get, port_update);

static int __init mod_start(void)
{
	int rv = nf_register_net_hook(&init_net, &nf_ops);
	kobj = kobject_create_and_add("my_firewall", kernel_kobj);
	if (sysfs_create_file(kobj, &kobjAttr.attr)){
		kobject_put(kobj);
		sysfs_remove_file(kobj, &kobjAttr.attr);
		pr_info("Something wrong with my module :(\n");
		return -1;
	}
	pr_info("My beautifull module is working!\n");
	return rv;
}

void __exit mod_exit(void)
{
	kobject_put(kobj);
	nf_unregister_net_hook(&init_net, &nf_ops);
	pr_info("My beautifull module is going down\n");
}

module_init(mod_start);
module_exit(mod_exit);

MODULE_AUTHOR("MICHAIL");
MODULE_DESCRIPTION("TEST_DRIVER");
MODULE_LICENSE("GPL");
