// No Header
// 26/11/2015 - 14:08:26

#ifndef PROTOTYPES_H__
#define PROTOTYPES_H__

#include "yellow.h"

int		main(int, char**);
t_opt		*set_opt(t_opt*, int, char**);
int		release_packet(t_packet*);
void		process_dump(t_packet*, t_display*);
int		packet_dump(t_packet *, t_display *);
void		no_op(t_packet*, t_yellow*);
void		data_process(t_packet*, t_yellow*);
int		packet_process(t_packet**, t_yellow*, void*, int);
int		recv_packet(t_packet**, t_yellow*);
int		read_cli(t_packet**, t_yellow*);
int		write_out(t_packet**, t_yellow*);
int		process_cmd(t_packet **p, t_yellow *y);

/* FILE loop.c */
/* Static Functions */
//int		_do_actions(t_packet**, t_yellow*, int);
//action	_exec_action(t_net*);
int		reset_select(t_net*);
int		loop_io(t_packet**, t_yellow*);

/* FILE init.c */
t_fd		*init_fd(int, action, action);
int		init(t_yellow*, int, char**);
/* Static Functions */
//t_net		*_init_net();
//t_input	*_init_input(int, int, char*, int);
//t_window	*_init_win(int, int, int, int);
//t_window	*_init_input_win();
//char		**_init_entry();
//t_menu	*_init_menu(int, int, char**, int);
//t_window	*_init_menu_win();
//t_output	*_init_dump();
//t_form	*_init_form();
//t_window	**_init_output_win();
//t_display	*_init_display();
//t_opt		*_init_opt();

/* FILE display.c */
int		display(t_yellow*);
int		glob_refresh(t_display*);
void		print_icmp_header(struct icmphdr*, t_display*);
void		print_ip_header(struct iphdr*, t_display*);
void		print_tcp_header(unsigned char*, t_display*);
void		print_eth_header(t_ether*, t_display*);

/* FILE socket.c */
int		get_sock();
t_net		*register_sock(t_net*, int);
void		unregister_sock(t_net*, int);
int		unset_promiscuous(int);
/* TODO */
int		send_packet(t_packet**, t_yellow*);
/* Static Functions */
//int		set_promiscuous(int);

/* FILE destroy.c */
int		destroy_the_yell(t_yellow*);
void		destroy_fd(t_net*, t_fd *);
/* Static Functions */
//void		_destroy_input(t_window*);
//void		_destroy_entry(char**);
//void		_destroy_menu(t_window*);
//void		_destroy_display(t_display*);
//void		_destroy_dump(t_output*);
//void		_destroy_form(t_form*);
//void		_destroy_output(t_window**);

/* FILE eth_process.c */
void		eth_process_ip(t_packet*, t_yellow*);
void		eth_process_ipv6(t_packet*, t_yellow*);
void		eth_process_arp(t_packet*,  t_yellow*);
t_packet	*process_ether(t_packet*, t_yellow*);

/* FILE ip_process.c */
void		ip_process_icmp(t_packet*, t_yellow*);
void		ip_process_tcp(t_packet*, t_yellow*);
void		ip_process_secure(t_packet*, t_yellow*);
void		ip_process_udp(t_packet*, t_yellow*);

/* FILE utils.c */
void		*xmalloc(size_t);
void		*xrealloc(void *, size_t);
char		*xstrdup(char*);

/* FILE pcap.c */
void		pcap_export(const char *fname, t_packet *p);

#endif /* !PROTOTYPES_H__ */
