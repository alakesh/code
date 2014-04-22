#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/mman.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "regs.h"

#define DEBUG 0

uint8_t *addr;
int proc_mem_fd = 0;

struct igb_reg_info {
	char name[256];
	uint32_t reg_offset;
} igb_regs[] = {
	{"Transmit Control Register - TCTL", 0x400},
	{"DMA Tx Control DTXCTL ", 0x3590},
	{"DMA Tx TCP Flags Control Low ", 0x359c },
	{"DMA Tx TCP Flags Control High", 0x35A0 },
	{"DMA Tx Max Total Allow Size Requests ", 0x3540 },
	{"DMA Tx Maximum Packet Size ", 0x355c },
	{"Transmit Descriptor Base Address Low [0] ", 0xE000 }, 
	{"Transmit Descriptor Base Address Low [1] ", 0xE000 + 0x40*1}, 
	{"Transmit Descriptor Base Address Low [2] ", 0xE000 + 0x40*2}, 
	{"Transmit Descriptor Base Address Low [3] ", 0xE000 + 0x40*3}, 
	{"Transmit Descriptor Base Address Low [4] ", 0xE000 + 0x40*4}, 
	{"Transmit Descriptor Base Address Low [5] ", 0xE000 + 0x40*5}, 
	{"Transmit Descriptor Base Address Low [6] ", 0xE000 + 0x40*6}, 
	{"Transmit Descriptor Base Address Low [7] ", 0xE000 + 0x40*7}, 

	{"Transmit Descriptor Base Address High [0] ", 0xE004 + 0x40*0}, 
	{"Transmit Descriptor Base Address High [1] ", 0xE004 + 0x40*1}, 
	{"Transmit Descriptor Base Address High [2] ", 0xE004 + 0x40*2}, 
	{"Transmit Descriptor Base Address High [3] ", 0xE004 + 0x40*3}, 
	{"Transmit Descriptor Base Address High [4] ", 0xE004 + 0x40*4}, 
	{"Transmit Descriptor Base Address High [5] ", 0xE004 + 0x40*5}, 
	{"Transmit Descriptor Base Address High [6] ", 0xE004 + 0x40*6}, 
	{"Transmit Descriptor Base Address High [7] ", 0xE004 + 0x40*7}, 

	{"Transmit Descriptor Ring Length - TDLEN [0] ", 0xE008 + 0x40*0},
	{"Transmit Descriptor Ring Length - TDLEN [1] ", 0xE008 + 0x40*1},
	{"Transmit Descriptor Ring Length - TDLEN [2] ", 0xE008 + 0x40*2},
	{"Transmit Descriptor Ring Length - TDLEN [3] ", 0xE008 + 0x40*3},
	{"Transmit Descriptor Ring Length - TDLEN [4] ", 0xE008 + 0x40*4},
	{"Transmit Descriptor Ring Length - TDLEN [5] ", 0xE008 + 0x40*5},
	{"Transmit Descriptor Ring Length - TDLEN [6] ", 0xE008 + 0x40*6},
	{"Transmit Descriptor Ring Length - TDLEN [7] ", 0xE008 + 0x40*7},

	{"Transmit Descriptor Head - TDH [0] ", 0xE010 + 0x40*0},
	{"Transmit Descriptor Head - TDH [1] ", 0xE010 + 0x40*1},
	{"Transmit Descriptor Head - TDH [2] ", 0xE010 + 0x40*2},
	{"Transmit Descriptor Head - TDH [3] ", 0xE010 + 0x40*3},
	{"Transmit Descriptor Head - TDH [4] ", 0xE010 + 0x40*4},
	{"Transmit Descriptor Head - TDH [5] ", 0xE010 + 0x40*5},
	{"Transmit Descriptor Head - TDH [6] ", 0xE010 + 0x40*6},
	{"Transmit Descriptor Head - TDH [7] ", 0xE010 + 0x40*7},

	{"Transmit Descriptor Tail - TDT [0] ", 0xE018 + 0x40*0},
	{"Transmit Descriptor Tail - TDT [1] ", 0xE018 + 0x40*1},
	{"Transmit Descriptor Tail - TDT [2] ", 0xE018 + 0x40*2},
	{"Transmit Descriptor Tail - TDT [3] ", 0xE018 + 0x40*3},
	{"Transmit Descriptor Tail - TDT [4] ", 0xE018 + 0x40*4},
	{"Transmit Descriptor Tail - TDT [5] ", 0xE018 + 0x40*5},
	{"Transmit Descriptor Tail - TDT [6] ", 0xE018 + 0x40*6},
	{"Transmit Descriptor Tail - TDT [7] ", 0xE018 + 0x40*7},

	{"Transmit Descriptor Control - TXDCTL [0]", 0xE028 + 0x40*0},
	{"Transmit Descriptor Control - TXDCTL [1]", 0xE028 + 0x40*1},
	{"Transmit Descriptor Control - TXDCTL [2]", 0xE028 + 0x40*2},
	{"Transmit Descriptor Control - TXDCTL [3]", 0xE028 + 0x40*3},
	{"Transmit Descriptor Control - TXDCTL [4]", 0xE028 + 0x40*4},
	{"Transmit Descriptor Control - TXDCTL [5]", 0xE028 + 0x40*5},
	{"Transmit Descriptor Control - TXDCTL [6]", 0xE028 + 0x40*6},
	{"Transmit Descriptor Control - TXDCTL [7]", 0xE028 + 0x40*7},

	{"Transmit Descriptor Completion Write Back Addr Low [0]", 
		0xE038 + 0x40*0},
	{"Transmit Descriptor Completion Write Back Addr Low [1]", 
		0xE038 + 0x40*1},
	{"Transmit Descriptor Completion Write Back Addr Low [2]", 
		0xE038 + 0x40*2},
	{"Transmit Descriptor Completion Write Back Addr Low [3]", 
		0xE038 + 0x40*3},
	{"Transmit Descriptor Completion Write Back Addr Low [4]", 
		0xE038 + 0x40*4},
	{"Transmit Descriptor Completion Write Back Addr Low [5]", 
		0xE038 + 0x40*5},
	{"Transmit Descriptor Completion Write Back Addr Low [6]", 
		0xE038 + 0x40*6},
	{"Transmit Descriptor Completion Write Back Addr Low [7]", 
		0xE038 + 0x40*7},

	{"Transmit Descriptor Completion Write Back Addr High [0]", 
		0xE03C + 0x40*0},
	{"Transmit Descriptor Completion Write Back Addr High [1]", 
		0xE03C + 0x40*1},
	{"Transmit Descriptor Completion Write Back Addr High [2]", 
		0xE03C + 0x40*2},
	{"Transmit Descriptor Completion Write Back Addr High [3]", 
		0xE03C + 0x40*3},
	{"Transmit Descriptor Completion Write Back Addr High [4]", 
		0xE03C + 0x40*4},
	{"Transmit Descriptor Completion Write Back Addr High [5]", 
		0xE03C + 0x40*5},
	{"Transmit Descriptor Completion Write Back Addr High [6]", 
		0xE03C + 0x40*6},
	{"Transmit Descriptor Completion Write Back Addr High [7]", 
		0xE03C + 0x40*7},

	{"Receive Control Register - RCTRL ", 0x0100 },
	{"Split and Replication Receive CTRL SRRCTL [0]",0xC00C + 0x40*0},
	{"Split and Replication Receive CTRL SRRCTL [1]",0xC00C + 0x40*1},
	{"Split and Replication Receive CTRL SRRCTL [2]",0xC00C + 0x40*2},
	{"Split and Replication Receive CTRL SRRCTL [3]",0xC00C + 0x40*3},
	{"Split and Replication Receive CTRL SRRCTL [4]",0xC00C + 0x40*4},
	{"Split and Replication Receive CTRL SRRCTL [5]",0xC00C + 0x40*5},
	{"Split and Replication Receive CTRL SRRCTL [6]",0xC00C + 0x40*6},
	{"Split and Replication Receive CTRL SRRCTL [7]",0xC00C + 0x40*7},

	{"Packet Split Receive Type - PSRTYPE [0]",0x5480 + 4*0},
	{"Packet Split Receive Type - PSRTYPE [1]",0x5480 + 4*1},
	{"Packet Split Receive Type - PSRTYPE [2]",0x5480 + 4*2},
	{"Packet Split Receive Type - PSRTYPE [3]",0x5480 + 4*3},
	{"Packet Split Receive Type - PSRTYPE [4]",0x5480 + 4*4},
	{"Packet Split Receive Type - PSRTYPE [5]",0x5480 + 4*5},
	{"Packet Split Receive Type - PSRTYPE [6]",0x5480 + 4*6},
	{"Packet Split Receive Type - PSRTYPE [7]",0x5480 + 4*7},

	{"Replicated Packet Split Receive Type ",0x54C0},

	{"Receive Descriptor Base Address Low - RDBAL [0]",0xC000 + 0x40*0},
	{"Receive Descriptor Base Address Low - RDBAL [1]",0xC000 + 0x40*1},
	{"Receive Descriptor Base Address Low - RDBAL [2]",0xC000 + 0x40*2},
	{"Receive Descriptor Base Address Low - RDBAL [3]",0xC000 + 0x40*3},
	{"Receive Descriptor Base Address Low - RDBAL [4]",0xC000 + 0x40*4},
	{"Receive Descriptor Base Address Low - RDBAL [5]",0xC000 + 0x40*5},
	{"Receive Descriptor Base Address Low - RDBAL [6]",0xC000 + 0x40*6},
	{"Receive Descriptor Base Address Low - RDBAL [7]",0xC000 + 0x40*7},

	{"Receive Descriptor Base Address High - RDBAH [0]",0xC004 + 0x40*0},
	{"Receive Descriptor Base Address High - RDBAH [1]",0xC004 + 0x40*1},
	{"Receive Descriptor Base Address High - RDBAH [2]",0xC004 + 0x40*2},
	{"Receive Descriptor Base Address High - RDBAH [3]",0xC004 + 0x40*3},
	{"Receive Descriptor Base Address High - RDBAH [4]",0xC004 + 0x40*4},
	{"Receive Descriptor Base Address High - RDBAH [5]",0xC004 + 0x40*5},
	{"Receive Descriptor Base Address High - RDBAH [6]",0xC004 + 0x40*6},
	{"Receive Descriptor Base Address High - RDBAH [7]",0xC004 + 0x40*7},

	{"Receive Descriptor Ring Length - RDLEN [0]",0xC008 + 0x40*0},
	{"Receive Descriptor Ring Length - RDLEN [1]",0xC008 + 0x40*1},
	{"Receive Descriptor Ring Length - RDLEN [2]",0xC008 + 0x40*2},
	{"Receive Descriptor Ring Length - RDLEN [3]",0xC008 + 0x40*3},
	{"Receive Descriptor Ring Length - RDLEN [4]",0xC008 + 0x40*4},
	{"Receive Descriptor Ring Length - RDLEN [5]",0xC008 + 0x40*5},
	{"Receive Descriptor Ring Length - RDLEN [6]",0xC008 + 0x40*6},
	{"Receive Descriptor Ring Length - RDLEN [7]",0xC008 + 0x40*7},

	{"Receive Descriptor Head RDH [0]", 0xC010 + 0x40*0},
	{"Receive Descriptor Head RDH [1]", 0xC010 + 0x40*1},
	{"Receive Descriptor Head RDH [2]", 0xC010 + 0x40*2},
	{"Receive Descriptor Head RDH [3]", 0xC010 + 0x40*3},
	{"Receive Descriptor Head RDH [4]", 0xC010 + 0x40*4},
	{"Receive Descriptor Head RDH [5]", 0xC010 + 0x40*5},
	{"Receive Descriptor Head RDH [6]", 0xC010 + 0x40*6},
	{"Receive Descriptor Head RDH [7]", 0xC010 + 0x40*7},

	{"Receive Descriptor Tail RDT [0]", 0xC018 + 0x40*0},
	{"Receive Descriptor Tail RDT [1]", 0xC018 + 0x40*1},
	{"Receive Descriptor Tail RDT [2]", 0xC018 + 0x40*2},
	{"Receive Descriptor Tail RDT [3]", 0xC018 + 0x40*3},
	{"Receive Descriptor Tail RDT [4]", 0xC018 + 0x40*4},
	{"Receive Descriptor Tail RDT [5]", 0xC018 + 0x40*5},
	{"Receive Descriptor Tail RDT [6]", 0xC018 + 0x40*6},
	{"Receive Descriptor Tail RDT [7]", 0xC018 + 0x40*7},

	{"Receive Descriptor Control - RXDCTL [0]", 0xC028 + 0x40*0},
	{"Receive Descriptor Control - RXDCTL [1]", 0xC028 + 0x40*1},
	{"Receive Descriptor Control - RXDCTL [2]", 0xC028 + 0x40*2},
	{"Receive Descriptor Control - RXDCTL [3]", 0xC028 + 0x40*3},
	{"Receive Descriptor Control - RXDCTL [4]", 0xC028 + 0x40*4},
	{"Receive Descriptor Control - RXDCTL [5]", 0xC028 + 0x40*5},
	{"Receive Descriptor Control - RXDCTL [6]", 0xC028 + 0x40*6},
	{"Receive Descriptor Control - RXDCTL [7]", 0xC028 + 0x40*7},

	{"Receive Queue Drop Paacket Count RQDPC [0] ", 0xC030 + 0x40*0},
	{"Receive Queue Drop Paacket Count RQDPC [1] ", 0xC030 + 0x40*1},
	{"Receive Queue Drop Paacket Count RQDPC [2] ", 0xC030 + 0x40*2},
	{"Receive Queue Drop Paacket Count RQDPC [3] ", 0xC030 + 0x40*3},
	{"Receive Queue Drop Paacket Count RQDPC [4] ", 0xC030 + 0x40*4},
	{"Receive Queue Drop Paacket Count RQDPC [5] ", 0xC030 + 0x40*5},
	{"Receive Queue Drop Paacket Count RQDPC [6] ", 0xC030 + 0x40*6},
	{"Receive Queue Drop Paacket Count RQDPC [7] ", 0xC030 + 0x40*7},

	{"Receive Checksum Control - RXCSUM ",0x5000},
	{"Receive Filter Control Register - RFCTL ", 0x5008},

	{"Receive Address Low RAL [0] ", 0x5400 + 8*0},
	{"Receive Address Low RAL [1] ", 0x5400 + 8*1},
	{"Receive Address Low RAL [2] ", 0x5400 + 8*2},
	{"Receive Address Low RAL [3] ", 0x5400 + 8*3},
	{"Receive Address Low RAL [4] ", 0x5400 + 8*4},
	{"Receive Address Low RAL [5] ", 0x5400 + 8*5},
	{"Receive Address Low RAL [6] ", 0x5400 + 8*6},
	{"Receive Address Low RAL [7] ", 0x5400 + 8*7},

	{"Receive Address High RAH [0] ", 0x5404 + 8*0},
	{"Receive Address High RAH [1] ", 0x5404 + 8*1},
	{"Receive Address High RAH [2] ", 0x5404 + 8*2},
	{"Receive Address High RAH [3] ", 0x5404 + 8*3},
	{"Receive Address High RAH [4] ", 0x5404 + 8*4},
	{"Receive Address High RAH [5] ", 0x5404 + 8*5},
	{"Receive Address High RAH [6] ", 0x5404 + 8*6},
	{"Receive Address High RAH [7] ", 0x5404 + 8*7},
};


void dump_ctrl_reg()
{	
	uint32_t ctrl = *((uint32_t *)(addr));
	printf("0x00000: CTRL (Device Control Register)  0x%08x \n",ctrl);
	printf("\tFull-Duplex           %s\n", (ctrl & 0x0001) ? "yes" : "no");
	printf("\tSLU                   %s\n", (ctrl & 0x0040) ? "yes" : "no");
	printf("\tInvert Loss-of-signal %s\n", (ctrl & 0x0080) ? "yes" : "no");
	printf("\tReceive flow control  %s\n", (ctrl & 0x1<<27) ? "enabled" : "disabled");
	printf("\tTransmit flow control %s\n", (ctrl & 0x1<<28) ? "enabled" : "disabled");
	printf("\tVLAN mode             %s\n", (ctrl & 0x1<<30) ? "enabled" : "disabled");
	printf("\tSpeed select:         %s\n", (ctrl & 0x1 << 9) ? 
			((ctrl & 0x1 << 8) ? "not used" : "1000 Mb/s") :
			((ctrl & 0x1 << 8) ? "100 Mb/s" : "10 Mb/s"));
}

void dump_dev_status_reg()
{

	uint32_t status = *((uint32_t *)(addr + 0x00008));
	printf("0x00008: STATUS (Device Status Register) 0x%08x \n", status);
	printf("\tDuplex                %s\n", (status & 0x1)? "Full Duplex":
			"Half Duplex");
	printf("\tLink Up               %s\n", (status & 0x1<<1)? "link config":			"no");
	printf("\tTransmission          %s\n", (status & 0x1<<4)?"Paused":"on");
	printf("\tMAC clock gating      %s\n", (status & 0x1<<31)?"Enabled":
			"Disabled");
}

void dump_rctl_reg()
{
	uint32_t rctl = *((uint32_t *)(addr + 0x0100));
	printf("0x00100: RCTL (Recieve Control Register) 0x%08x \n", rctl);
	printf("\tReceiver            %s\n", (rctl & 0x2)? "Enabled":"Disabled");
	printf("\tStore Bad Pkt       %s\n", (rctl & 0x1<<2)? "Enabled":"Disabled");
	printf("\tUnicast Promiscous  %s\n", (rctl & 0x1<<3)? "Enabled":"Disabled");
	printf("\tMulticast Promisc   %s\n", (rctl & 0x1<<4)? "Enabled":"Disabled");
	printf("\tLong Packet         %s\n", (rctl & 0x1<<5)? "Enabled":"Disabled");
	printf("\tBCast Accept Mode   %s\n", (rctl & 0x1<<15)? "Enabled":"Disabled");
	printf("\tVLAN Filter         %s\n", (rctl & 0x1<<18)? "Enabled":"Disabled");
	printf("\tDiscard Pause Frame %s\n", (rctl & 0x1<<22)? "Discarded":"Forwarded");
	printf("\tReceive Buffer Size %s\n", (rctl & 0x1<<17)? 
			((rctl & 0x1 << 16) ? "256 Bytes" : "512 Bytes") :
			((rctl & 0x1 << 16) ? "1024 Bytes" : "2048 Bytes"));
}

void dump_tctl_reg()
{
	uint32_t tctl = *((uint32_t *)(addr + 0x0400));
	printf("0x00400: TCTL (Transmit Control Register) 0x%08x \n", tctl);
	printf("\tEnable                      %s\n", 
			(tctl & 0x2)?"Enabled":"Disabled");
	printf("\tPad Short Packets:          %s\n", 
			(tctl & 0x1<<3)?"Yes":"No");
	printf("\tSoftware XOFF Transmission: %s\n",
			(tctl & 0x1<<22)?"Yes":"No");
	printf("\tRe-transmit on late collission:   %s\n",(tctl & 0x1<<24)?
			"Yes":"No");
}
	
void dump_internal_reg(void)
{
	char buf[32];
	uint32_t irpbs = *((uint32_t *)(addr + 0x2404));
	uint32_t itpbs = *((uint32_t *)(addr + 0x3404));
	printf("0x02404: Internal Recv Pkt Buffer Size Reg 0x%08x \n", irpbs);
	switch(irpbs & 0x000F){
		case 0x0:
			snprintf(buf,32,"36KB");
			break;
		case 0x1:
			snprintf(buf,32,"72KB");
			break;
		case 0x2:
			snprintf(buf,32,"72KB");
			break;
		case 0x3:
			snprintf(buf,32,"72KB");
			break;
		case 0x4:
			snprintf(buf,32,"72KB");
			break;
		case 0x5:
			snprintf(buf,32,"72KB");
			break;
		case 0x6:
			snprintf(buf,32,"72KB");
			break;
		case 0x7:
			snprintf(buf,32,"72KB");
			break;
		case 0x8:
			snprintf(buf,32,"72KB");
			break;
		case 0x9:
			snprintf(buf,32,"72KB");
			break;
		case 0xA:
			snprintf(buf,32,"72KB");
			break;
		default:
			snprintf(buf,32,"reserved");
			break;
	}
	printf("\tRXPbSize             %s\n",buf);

	printf("0x03404: Internal Tx Pkt Buffer Size Reg 0x%08x \n", itpbs);
	switch(irpbs & 0x000F){
		case 0x0:
			snprintf(buf,32,"20KB");
			break;
		case 0x1:
			snprintf(buf,32,"40KB");
			break;
		case 0x2:
			snprintf(buf,32,"80KB");
			break;
		case 0x3:
			snprintf(buf,32,"1KB");
			break;
		case 0x4:
			snprintf(buf,32,"2KB");
			break;
		case 0x5:
			snprintf(buf,32,"4KB");
			break;
		case 0x6:
			snprintf(buf,32,"8KB");
			break;
		case 0x7:
			snprintf(buf,32,"16KB");
			break;
		case 0x8:
			snprintf(buf,32,"19KB");
			break;
		case 0x9:
			snprintf(buf,32,"38KB");
			break;
		case 0xA:
			snprintf(buf,32,"76KB");
			break;
		default:
			snprintf(buf,32,"reserved");
			break;
	}
	printf("\tTXPbSize             %s\n",buf);

}

void dump_interrupt_reg()
{
	uint32_t picause = *((uint32_t *)(addr + 0x5B88));
	printf("0x5B88: PCIe Interrupt Cause 0x%08x \n", picause);
	printf("\t PCI Completion Abort: %s\n", (picause & 0x1) ? "Yes":"no");
	printf("\t Unsupported IO address exception: %s\n",
			(picause & 0x2) ? "Yes" : "No");
	printf("\t Wrong Byte-Enable exception in FUNC unit: %s\n",
			(picause & 0x4) ? "Yes" : "No");
	printf("\t Bus Master enable of PF de-asserted: %s \n",
			(picause & (0x1 << 4)) ? "Yes" : "No");

	uint32_t piena = *((uint32_t *)(addr + 0x5B8C));
	printf("0x5B8C: PCIe Interrupt Enable: 0x%08x \n", piena);
	printf("\t Completion Abort interrupt enabled:  %s\n",
			(piena & 0x1) ? "Yes": "No");
	printf("\t Unsupported IO Address intr enabled:  %s\n",
			(piena & 0x2) ? "Yes": "No");
	printf("\t Wrong Byte-Enable interrupt enabled:  %s\n",
			(piena & 0x4) ? "Yes": "No");
	printf("\t PCI Timeout interrupt enabled: %s\n",
			(piena & 0x8) ? "Yes": "No");
	printf("\t PCI Bus Master Enable interrupt enabled: %s\n",
			(piena & 0x1 << 4) ? "Yes" : "No");
	printf("\t PCI Completion abort received intr enabled: %s\n",
			(piena & 0x1 << 5) ? "Yes" : "No");

}

void dump_tx_dma_ring(void)
{
	uint32_t tx_base_low[8];
	uint32_t tx_base_high[8];
	uint32_t tx_ring_length[8];
	uint32_t tx_ring_head[8];
	uint32_t tx_ring_tail[8];
	uint64_t dma = 0;

	int i=0,j;
	for(i=0; i<8; i++)
	{
		tx_base_low[i] = *((uint32_t *)(addr + 0xE000 + 0x40 * i));	
		tx_base_high[i] = *((uint32_t *)(addr + 0xE004 + 0x40 * i));	
		tx_ring_length[i] = *((uint32_t *)(addr + 0xE008 + 0x40 * i));
		tx_ring_head[i] = *((uint32_t *)(addr + 0xE010 + 0x40 * i));
		tx_ring_tail[i] = *((uint32_t *)(addr + 0xE018 + 0x40 * i));
	}
	printf ("\t\t\t\tTX RING DUMP \n");
	printf("=======================================================\n");

	for (i=0;i<8;i++){
		uint32_t len = tx_ring_length[i];
		if (!len)
			continue;
		printf("\t\t\tDUMP OF TX RING [%d] \n", i);
		printf("Tx Descriptor Ring [%d] Length [bytes] 0x%x \n",i, tx_ring_length[i]);
		printf("ring->dma: 0x%016lX \n", 
				dma = ((uint64_t)tx_base_high[i])<<32 | 
					(uint64_t)(tx_base_low[i]));

		printf("Tx Ring[%d] Head:0x%08x Tail:0x%08x \n", i,
				*((uint32_t *)(addr + 0xE010 + 0x40 * i)),
				*((uint32_t *)(addr + 0xE018 + 0x40 * i)));
	}
}

void dump_rx_dma_ring(void)
{
	int i=0,j=0;
	uint32_t rx_base_high=0;
	uint32_t rx_base_low=0;
	uint32_t len=0;
	uint64_t dma=0;

	printf ("\t\t\t\tRX RING DUMP \n");
	printf("=======================================================\n");
	for (i=0;i<8;i++){
		len = *((uint32_t *)(addr + 0xC008 + 0x40 * i));
		if (!len)
			continue;
		printf("\t\t\tDUMP OF RX RING [%d] \n", i);
		printf("Rx Descriptor Ring[%d] Length [bytes] 0x%x \n",i, len);
		rx_base_high = *((uint32_t *)(addr + 0xC004 + 0x40 * i));
		rx_base_low = *((uint32_t *)(addr + 0xC000 + 0x40 * i));
		printf("ring->dma: 0x%016lX \n",
				dma = ((uint64_t) rx_base_high) << 32 |
				(uint64_t) rx_base_low );
		printf("Rx Ring[%d] Head:0x%08x Tail:0x%08x \n", i,
				*((uint32_t *)(addr + 0xC010 + 0x40 * i)),
				*((uint32_t *)(addr + 0xC018 + 0x40 * i)));
			
	}
}


void dump_reg(char *name, uint32_t offset)
{
	printf("%s Value: 0x%x \n", name, *((uint32_t *)(addr + offset)));
}

void dump_byte(uint32_t offset)
{
	printf("0x%x", *((uint8_t *)(addr + offset)));
}

void dump_all_reg(int flag)
{
	int i=0,n;
	/* Print registers */
	dump_ctrl_reg();
	dump_dev_status_reg();
	dump_rctl_reg();
	dump_tctl_reg();
	dump_internal_reg();
	dump_interrupt_reg();
	dump_rx_dma_ring();
	dump_tx_dma_ring();
	if (flag){
		printf("\t\tGENERAL REGISTER DUMP BEGINS\n"); 
		printf("==========================================\n\n");
		for (i=0,n=sizeof(igb_regs)/sizeof(*igb_regs);i<n; i++)
		{
			dump_reg(igb_regs[i].name, igb_regs[i].reg_offset);
		}
		printf("==========================================\n\n");
	}
}

int main(int argc, char *argv[])
{
	int fd = 0, opt;
	char device[256];
	int dumpall = 0;
	uint32_t size = 20*4096;
	char **ptr;
	uint32_t offset = 0xFFFFFFFF;
	struct ifreq ifr;
	int s;
	uint32_t start_mem_addr;
	uint32_t end_mem_addr;

	while ((opt = getopt(argc, argv, "d:har:")) != -1) {
		switch (opt) {
			case 'a':
				dumpall = 1;
				break;
			case 'd':
				strncpy(device, optarg,
						sizeof device);
				break;
			case 'r':
				offset = strtol(optarg,NULL,16);
				break;
			case 'h':
				fprintf(stderr, 
						"Usage: %s [-r reg] or [-a to dump all]\n",
						argv[0]);
				break;
			default: /* '?' */
				fprintf(stderr, 
						"Usage: %s [-r reg] or [-a]\n",
						argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	if (strlen(device)==1)
		return -1;

	if ((fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0) {
		printf("Error opening /dev/mem \n");
		close(fd);
		return (-1);
	}
	proc_mem_fd = fd;

	/* Obtain memory mapping */
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, device, sizeof ifr.ifr_name);

	if ((s = socket(ifr.ifr_addr.sa_family, SOCK_DGRAM, 0)) < 0)
	{
		printf("socket() failed with error %d",errno);
		return -1;
	}

	ioctl(s, SIOCGIFMAP, (caddr_t)&ifr);
	printf("Device : %s \n", ifr.ifr_name);
	printf("Mem Start: 0x%x \n", 
			start_mem_addr = (uint32_t)(ifr.ifr_map.mem_start));
	printf("Mem End: 0x%x \n", 
			end_mem_addr = (uint32_t)(ifr.ifr_map.mem_end));

	addr = (uint8_t *)mmap(0, size, PROT_READ, MAP_PRIVATE,
			fd, start_mem_addr);

        if (dumpall == 0){
		if ( offset != 0xFFFFFFFF)
		{
			char str[32];
			snprintf(str, 15, "REG: 0x%x", offset);
			dump_reg(str, offset);
		}
		dump_all_reg(0);
	}
	if (dumpall == 1){
		dump_all_reg(1);
	}
	munmap(addr, size);
	close(s);
	return 0;
}
