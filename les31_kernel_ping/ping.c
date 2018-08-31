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

#ifndef OPER_PING
#define OPER_PING 1
#endif
#ifndef OPER_UPDATE
#define OPER_UPDATE 2
#endif

static struct kobject *kobj;
unsigned char my_ip[5] = {192, 168, 0, 60, 0};
unsigned char ip[5] = {127, 0, 0, 1, 0};
char response[256] = "unknown";

u32 create_address(u8 *ip){
	u32 addr = 0;
	int i;

	for(i=0; i<4; i++)
	{
		addr += ip[i];
		if(i==3)
			break;
		addr <<= 8;
	}
	return addr;
}

static u16 gen_crc16(const u16 *data, u16 size)
{
	int i = 0;
	u32 crc = 0;
	while (i < size) {
		crc = crc + data[i];
		if (crc > 0xFFFF) {
			crc -= 0x10000;
		crc += 0x1;
		}
		++i;
	}

	return ~crc;
}

int ping_create(void)
{
	struct socket *sock;
	struct sockaddr_in addr;
	struct sockaddr_in my_addr;
	struct kvec vec;
	struct msghdr msg;
	struct icmphdr icmph;
	int status;
	memset((void*)&icmph, 0, sizeof(icmph));

	if (sock_create_kern(&init_net, AF_INET, SOCK_RAW, IPPROTO_ICMP, &sock) < 0){
		pr_info("socket creation error\n");
		return -1;
	}
	addr.sin_addr.s_addr = htonl(create_address(ip));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	
	
	
//----------------------------------------
	msg.msg_name = &addr;
	msg.msg_namelen = sizeof(addr);
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_flags = 0;

	vec.iov_len = sizeof(struct icmphdr);
	vec.iov_base = &icmph;
//----------------------------------------

	
		
	
	
	icmph.type = 8;//8 - ICMP request
	icmph.code = 0;
	icmph.checksum = gen_crc16((u16*)&icmph, sizeof(icmph)/2);
	kernel_sendmsg(sock, &msg, &vec, 1, sizeof(struct icmphdr));
	sock->ops->release(sock);
	return 0;
}

//static unsigned short int blockedPort = 443;

static unsigned int capture(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	struct iphdr *iphdr = ip_hdr(skb);
	struct icmphdr *icmph;
	pr_info("received packet\n");
	if (iphdr->protocol == IPPROTO_ICMP){
		icmph = icmp_hdr(skb);
		sprintf(response, "got answer: %i:%i\n", icmph->type, icmph->code);
		return NF_ACCEPT;
	}
	return NF_ACCEPT;
}


static const struct nf_hook_ops nf_ops = {
	.hook		= capture,
	.pf		= AF_INET,
	.hooknum	= NF_DN_PRE_ROUTING,
	.priority	= NF_DN_PRI_DNRTMSG,
};



static ssize_t addr_get(struct kobject *kobj, struct kobj_attribute *attr,
	char *buffer)
{
	pr_info("Pinging address info requested\n");
	return sprintf(buffer, "Pinging %i.%i.%i.%i\nLast ping ended: %s\n", ip[0], ip[1], ip[2], ip[3], response);
	return 0;
}

static ssize_t addr_oper(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buffer, size_t len)
{
	int A,B,C,D;
	int operType;
	sscanf(buffer, "%i", &operType);
	switch (operType) {
		case OPER_UPDATE:
			sscanf(buffer,"%i %i %i %i %i",
				&operType, &A, &B, &C, &D);
			pr_info("IP updated to %i.%i.%i.%i\n", A, B, C, D);
			ip[0] = A & 0xFF;
			ip[1] = B & 0xFF;
			ip[2] = C & 0xFF;
			ip[3] = D & 0xFF;

			break;
		case OPER_PING:
			ping_create();
			break;
		default:
			return len;
	}
        //pr_info("Blocking port updated to %i\n", blockedPort);
        return len;
}


static struct kobj_attribute kobjAttr =
	__ATTR(ping_ctl, 0664, addr_get, addr_oper);

static int __init mod_start(void)
{
	nf_register_net_hook(&init_net, &nf_ops);
	kobj = kobject_create_and_add("my_ping", kernel_kobj);

	if (sysfs_create_file(kobj, &kobjAttr.attr)){
		kobject_put(kobj);
		sysfs_remove_file(kobj, &kobjAttr.attr);
		pr_info("Something wrong with my module :(\n");
		return -1;
	}

	pr_info("My beautifull module is working!\n");
	return 0;
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
