#include <command.h>

static inline void delay(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}

extern int do_nand(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[]);

int read_logo_bin(unsigned char *buf, unsigned int addr, unsigned int size)
{
	int ret = 1;
	char start[32], length[32];
	char rstart[32], rlength[32], rbuffer[32];
	char read_type[32];

	char *argv_read[6]  = { NULL, NULL,  NULL, NULL, NULL, NULL, };

	argv_read[1] = read_type;
	argv_read[2] = rbuffer;
	argv_read[3] = rstart;
	argv_read[4] = rlength;

	sprintf(read_type,"read");
	sprintf(rbuffer,"0x%x",buf);
	printf("rbuffer:%s\n",rbuffer);
	sprintf(rstart,"0x%x",addr);
	printf("rstart:%s\n",rstart);
	sprintf(rlength,"0x%x",size);
	printf("rlength:%s\n",rlength);

	printf("Reading Logo\n");
	
//	delay(10000000000);

	do_nand(NULL,0,5,argv_read);

//	delay(10000000000);

	printf("Reading Logo End\n");
	
}	
