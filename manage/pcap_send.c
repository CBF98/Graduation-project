#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <pcap/pcap.h>
#include <net/if.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "pcap_send.h"

int port = 8000;
int pack_len = 20;
static char errbuf[PCAP_ERRBUF_SIZE];

static void net_dev_close(pcap_t *dev)
{
	printf("Close Device\n");
	if(dev){
	    pcap_close(dev);
	}
}

void pcap_send(char *buff, char *dmac, char *dip, int num){
    char dev_name[10] = {0};
    int len;
    int ret;
    int i,j;
    
    sprintf(dev_name, DEV_NAME);
    if((ret = get_local_mac(dev_name)) == -1){
        perror("get_local_mac fail...") ;
        printf("can't grt mac\n");
        return;
    }
    memset(dstmac, 0, sizeof(dstmac));
    memset(dstip, 0, sizeof(dstip));
    
    if(dmac == NULL){
        strcpy(dstmac, DEFAULT_MAC);
    }
    else{
        strcpy(dstmac, dmac);
    }

    if(dip == NULL){
        strcpy(dstip, DEFAULT_IP);
    }
    else{
        strcpy(dstip, dip);
    }

    pcap_snd_udp_pack(buff, dev_name, num);

}


int get_local_mac(char *ifname){
    int s, ret;
	struct ifreq ifr;
    char s_local_mac[6] = {0};
	s = socket(AF_INET, SOCK_STREAM, 0);
	if( s < 0 ) {
	    perror("srv_cmd_get_local_mac : scoket() error.\n");
	    return -1;
	}
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, ifname);
	ret = ioctl(s, SIOCGIFHWADDR, &ifr, sizeof(ifr));
	if( ret == 0 ) {
		memcpy(s_local_mac, ifr.ifr_hwaddr.sa_data, 6);
        printf( "get local mac(%c:%c:%c:%c:%c:%c)\n",s_local_mac[0],s_local_mac[1],s_local_mac[2],s_local_mac[3],s_local_mac[4],s_local_mac[5]);
	}else{
		printf(" get local mac error.\n");
	}
	close(s);
	return ret;
}

int pcap_snd_udp_pack(char *udp_pack, char *dev_name ,int num){
    int ret;
    ret = bld_udp_pack(udp_pack);
    printf("pcap_snd_udp_pack()...bld_udp_pack complete.\n");
    if((ret = pcap_snd_pack(udp_pack, dev_name , num)) == -1){
        perror("pcap_snd_udp_pack()...pcap_snd_pack fail.\n");
        return -1;
    }
    return 0;
}

int bld_udp_pack(char *pack){
    struct ethhdr *peth;
    struct iphdr *pip;
    struct udphdr *udp;
    int hdr_size;
    char buf[pack_len];
    int i;
    
    bzero(pack, sizeof(pack));
    
    peth = (struct ethhdr *)pack;
    pip = (struct iphdr *)(pack + sizeof(struct ethhdr));
    udp = (struct udphdr *)(pack + sizeof(struct ethhdr) + sizeof(struct iphdr));
    hdr_size = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
    printf("the long of the udp is %d\n", hdr_size);
    
    for(i = 0 ; i < 6 ; i++)
	{
		peth->h_dest[i] = dstmac[i];
		peth->h_source[i] = srcmac[i];
	}
	peth->h_proto = htons(ETH_P_IP);
	
    pip->ihl = 5;
    pip->version = 4;
    pip->tos = 0;
    pip->id = 0;
    pip->frag_off = 0;
    pip->ttl = 64;
    pip->protocol = IPPROTO_UDP;          //ip->protocol = IPPROTO_UDP
    pip->saddr = inet_addr(dstip);
    pip->daddr = inet_addr("127.0.0.1");
    pip->check = 0;
    
    udp->source= htons(port);
    udp->dest  = htons(port);
    udp->check = 0;
    udp->len   = sizeof(struct udphdr);
    
    printf("Input UDP content:\n") ;
    scanf("%s" , buf) ;
    for(i = 0 ; (i < pack_len) && (i < strlen(buf)) ; i++){
        pack[hdr_size+i] = buf[i] ;
    }
    
    return hdr_size+strlen(buf) ;
}

int pcap_snd_pack(char *pkt, char *dev_name , int num){
    
    int i ;
    pcap_t *fp = NULL ;

    printf("dev_name = : %s", dev_name);
    
    fp=pcap_open_live(dev_name, 8000, 1, 500, errbuf);
    if(fp == NULL){
        perror("error in creating dev fp\n");
        return -1 ;
    }

    for(i = 0 ; i < num ; i++){
        printf("begin to pcap_sendpacket----");
        pcap_sendpacket(fp, (const u_char *)pkt, sizeof(pkt));
        printf("success\n") ;
        sleep(1) ;
    }
    net_dev_close(fp) ;
    
    return 0 ;
}