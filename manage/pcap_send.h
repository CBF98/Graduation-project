#define _PCAP_SEND_H_
#define _SEND_UDP_H_
#define _GET_PACK_CONTENT_H_
#define _PACK_BLD_H_
#define _PCAP_SEND_H_

void pcap_send(char *buf, char *dmac, char *dip, int num);
int get_local_mac(char *ifname);
int pcap_snd_udp_pack(char *udp_pack, char *dev_name, int num);
int bld_udp_pack(char *pack);
int pcap_snd_pack(char *pkt, char *dev_name , int num);

#define MAX 1000
#define DEV_NAME "eth2"
#define DEFAULT_MAC "111111"
#define DEFAULT_IP "10.0.0.1"

char    dstmac[6];
char    srcmac[6];
char    dstip[16];
char    srcip[16];


