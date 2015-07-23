/*
 * Copyright (C) 2015  Silvano Seva
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef W5100_DEFS_H
#define W5100_DEFS_H

//maximum number of sockets managed by the device
#define MAX_SOCK_NUM 4

#define COMMON_BASE 0x0000

#define TX_BUF_BASE (COMMON_BASE + 0x4000)  //TX buffer memory base address

#define RX_BUF_BASE (COMMON_BASE + 0x6000)  //RX buffer memory base address

/** common registers **/

#define MR              (COMMON_BASE + 0x0000)  //mode register address
#define GAR_BASE        (COMMON_BASE + 0x0001)  //Gateway IP Register base address
#define SUBR_BASE       (COMMON_BASE + 0x0005)  //Subnet mask Register base address
#define SHAR_BASE       (COMMON_BASE + 0x0009)  //Source MAC Register base address
#define SIPR_BASE       (COMMON_BASE + 0x000F)  //Source IP Register base address
#define IR              (COMMON_BASE + 0x0015)  //Interrupt Register address
#define IR_MASK         (COMMON_BASE + 0x0016)  //Interrupt mask register
#define RTR_BASE        (COMMON_BASE + 0x0017)  //retransmission Timeout register
#define RCR             (COMMON_BASE + 0x0019)  //retransmission count register

#define RMSR            (COMMON_BASE + 0x001A)  //sockets RX memory size register
#define TMSR            (COMMON_BASE + 0x001B)  //sockets TX memory size register

#define UIPR0           (COMMON_BASE + 0x002A)  //unreacheable IP address register base address
#define UPORT0          (COMMON_BASE + 0x002E)  //unreacheable port register base address

#define PPP_AUTH_REG    (COMMON_BASE + 0x001C)  //autentication type in PPPoE mode
#define PPP_TIME_REG    (COMMON_BASE + 0x0028)  //LCP Request Timer register  in PPPoE mode
#define PPP_MAGIC_REG   (COMMON_BASE + 0x0029)  //PPP LCP Magic number register  in PPPoE mode


/* MODE register values */
#define MR_RST          0x80 //reset
#define MR_PB           0x10 //ping block enable
#define MR_PPPOE        0x08 //PPPoE enable
#define MR_AI           0x02 //I/F bus auto increment, refer to datasheet
#define MR_IND          0x01 //indirect I/F bus auto increment, refer to datasheet

/* IR register values */
#define IR_CONFLICT     0x80 //IP conflict
#define IR_UNREACH      0x40 //destination host unreachable
#define IR_PPPoE        0x20 //PPPoE connection close
#define IR_SOCK(ch)     (0x01 << ch) //check socket interrupt


/** socket registers **/

#define SR_BASE         (COMMON_BASE + 0x0400)  //socket registers base address
#define SR_SIZE         0x100                   //size of each channel register map

#define SOCKn_MR            (SR_BASE + 0x0000) //socket Mode register
#define SOCKn_CR            (SR_BASE + 0x0001) //socket command register
#define SOCKn_IR            (SR_BASE + 0x0002) //socket interrupt register
#define SOCKn_SR            (SR_BASE + 0x0003) //socket status register
#define SOCKn_SPORT0        (SR_BASE + 0x0004) //socket source port register
#define SOCKn_DHAR0         (SR_BASE + 0x0006) //socket destination MAC address register
#define SOCKn_DIPR0         (SR_BASE + 0x000C) //socket destination IP address register
#define SOCKn_DPORT0        (SR_BASE + 0x0010) //socket destination port register

#define SOCKn_MSSR0         (SR_BASE + 0x0012) //socket MSS in TCP mode

#define SOCKn_PROTO         (SR_BASE + 0x0014) //socket protocol number in IPRAW mode

#define SOCKn_TOS           (SR_BASE + 0x0015) //socket's IP header's Type of Service field value
#define SOCKn_TTL           (SR_BASE + 0x0016) //socket's IP header's TTL field value

#define SOCKn_TX_FSR0       (SR_BASE + 0x0020) //socket's TX buffer free size register
#define SOCKn_TX_RD0        (SR_BASE + 0x0022) //socket's TX buffer read pointer address
#define SOCKn_TX_WR0        (SR_BASE + 0x0024) //socket's TX buffer write pointer address

#define SOCKn_RX_RSR0       (SR_BASE + 0x0026) //socket's received data size register
#define SOCKn_RX_RD0        (SR_BASE + 0x0028) //socket's RX buffer read pointer address



/* SOCKn_MR values */
#define SOCKn_MR_CLOSE     0x00        //socket closed
#define SOCKn_MR_TCP       0x01        //TCP mode
#define SOCKn_MR_UDP       0x02        //UDP mode
#define SOCKn_MR_IPRAW     0x03        //IP layer raw socket
#define SOCKn_MR_MACRAW    0x04        //MAC layer raw socket
#define SOCKn_MR_PPPOE     0x05        //PPPoE mode
#define SOCKn_MR_ND        0x20        //No delayed ACK enable
#define SOCKn_MR_MULTI     0x80        //enable multicasting (only in UDP mode)

/* SOCKn_CR values */
#define SOCKn_CR_OPEN      0x01        //initialize and open socket
#define SOCKn_CR_LISTEN    0x02        //wait connection request in TCP mode (Server mode)
#define SOCKn_CR_CONNECT   0x04        //send connection request in TCP mode (Client mode)
#define SOCKn_CR_DISCON    0x08        //disconnect request in TCP mode
#define SOCKn_CR_CLOSE     0x10        //close socket
#define SOCKn_CR_SEND      0x20        //send all data stored in TX buffer
#define SOCKn_CR_SEND_MAC  0x21        //send data with MAC address without ARP process (only in UDP mode)
#define SOCKn_CR_SEND_KEEP 0x22        //check if TCP connection is still alive
#define SOCKn_CR_RECV      0x40        //receive data

// #ifdef __DEF_IINCHIP_PPP__
//     #define SOCKn_CR_PCON      0x23         
//     #define SOCKn_CR_PDISCON       0x24         
//     #define SOCKn_CR_PCR       0x25         
//     #define SOCKn_CR_PCN       0x26        
//     #define SOCKn_CR_PCJ       0x27        
// #endif

/* SOCKn_IR values */
// #ifdef __DEF_IINCHIP_PPP__
//     #define SOCKn_IR_PRECV     0x80        
//     #define SOCKn_IR_PFAIL     0x40        
//     #define SOCKn_IR_PNEXT     0x20        
// #endif
#define SOCKn_IR_CON       0x01        //connection established
#define SOCKn_IR_DISCON    0x02        //disconnected (TCP mode)
#define SOCKn_IR_RECV      0x04        //some data received
#define SOCKn_IR_TIMEOUT   0x08        //Timeout occurred in ARP or TCP
#define SOCKn_IR_SEND_OK   0x10        //SEND command completed

/* SOCKn_SR values */
#define SOCK_CLOSED        0x00        //socket closed
#define SOCK_INIT          0x13        //TCP init state
#define SOCK_LISTEN        0x14        //TCP server listen for connection state
#define SOCK_SYNSENT       0x15        //TCP connection request sent to server
#define SOCK_SYNRECV       0x16        //TCP connection request received from client
#define SOCK_ESTABLISHED   0x17        //TCP connection established
#define SOCK_FIN_WAIT      0x18        //TCP closing state
#define SOCK_CLOSING       0x1A        //TCP closing state
#define SOCK_TIME_WAIT     0x1B        //TCP closing state
#define SOCK_CLOSE_WAIT    0x1C        //TCP closing state
#define SOCK_LAST_ACK      0x1D        //TCP closing state
#define SOCK_UDP           0x22        //socket opened in UDP mode
#define SOCK_IPRAW         0x32        //socket opened in IP raw mode
#define SOCK_MACRAW        0x42        //socket opened in MAC raw mode
#define SOCK_PPPOE         0x5F        //socket opened in PPPoE mode

/* IP PROTOCOL */
#define IPPROTO_IP         0           // Dummy for IP
#define IPPROTO_ICMP       1           // ICMP protocol
#define IPPROTO_IGMP       2           // IGMP protocol
#define IPPROTO_GGP        3           // GGP protocol
#define IPPROTO_TCP        6           // TCP
#define IPPROTO_PUP        12          // PUP
#define IPPROTO_UDP        17          // UDP
#define IPPROTO_IDP        22          // XNS idp
#define IPPROTO_RAW        255         // Raw IP packet */


/*** data types definition ***/

#ifdef CPLUSPLUS
extern "C" {
#endif

/**
 * 8-bit unsigned data type.
 */
typedef unsigned char uint8;

/**
 * 16-bit unsigned data type.
 */
typedef unsigned short uint16;


#ifdef CPLUSPLUS
}
#endif

#endif