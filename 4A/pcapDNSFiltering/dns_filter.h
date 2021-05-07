#ifndef PCAP_EXAMPLE_H_
#define PCAP_EXAMPLE_H_

#define BUF_SIZE 65536

void process_packet(u_char *, const struct pcap_pkthdr *, const u_char *);


#endif /* PCAP_EXAMPLE_H_ */
