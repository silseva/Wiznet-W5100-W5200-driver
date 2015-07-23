/*
 * Driver class for Wiznet W5100 chip
 * 
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

#ifndef W5100_H
#define W5100_H

#include "w5100_defs.h"
#include "spi_impl.h"

typedef uint8 SOCKET;

extern W5100& w5100; //a W5100 driver class instance

class W5100
{
public:
   
    /**
     * \return a singleton instance of W5100 class
     */
    static W5100& instance();
    
    /**
     * Set chip's MAC address
     */
    void setMacAddress(uint8 *address);
    
    /**
     * Set chip's IP address
     */
    void setIpAddress(uint8 *address);
    
    /**
     * Set network's subnet mask
     */
    void setSubnetMask(uint8 *mask);
    
    /**
     * Set network's gateway IP address
     */
    void setGatewayAddress(uint8 *address);
    
    /**
     * Set Mode register flags
     */
    void setModeReg(uint8 value);
    
    /**
     * Set retransmission timeout period in units of 100us each
     * for example to set timeout period to 400ms you have to set
     * this register to 0x4000
     */
    void setRetryTime(uint16 value);
    
    /**
     * Configures the number of retransmissions
     */
    void setRetryCount(uint16 value);
    
    /**
     * Configures the interrupt mask for socket interrupts
     */    
    void setInterruptMask(uint8 mask);
    
    /**
     * \return chip's interrupt register value
     */    
    uint8 readInterruptReg();    
        
    /**
     * Writes socket's mode register
     * \param sockNum: socket number, between 0 and 3
     * \param value: value to be written
     */
    void setSocketModeReg(SOCKET sockNum, uint8 value);
    
    /**
     * Used to send a command to a socket through its command register
     * \param sockNum: socket number, between 0 and 3
     * \param value: command opcode
     */
    void setSocketCommandReg(SOCKET sockNum, uint8 value);
    
    
    /**
     * Reads socket's interrupt register
     * \param sockNum: socket number, between 0 and 3
     * \return socket's interrupt register value
     */
    uint8 getSocketInterruptReg(SOCKET sockNum);
    
    /**
     * Reads socket's status register
     * \param sockNum: socket number, between 0 and 3
     * \return socket's status register value
     */
    uint8 getSocketStatusReg(SOCKET sockNum);
    
    /**
     * Sets socket's source port value, both for TCP and UDP
     * \param sockNum: socket number, between 0 and 3
     * \param port: socket's source port number
     */
    uint8 setSocketSourcePort(SOCKET sockNum, uint16 port);
    
    /**
     * Sets socket's destination MAC address
     * \param sockNum: socket number, between 0 and 3
     * \param destMAC: socket's destination MAC address
     */
    void setSocketDestMac(SOCKET sockNum, uint8* destMAC);
    
    /**
     * Sets socket's destination IP address 
     * \param sockNum: socket number, between 0 and 3
     * \param destIP: socket's destination IP address
     */
    void setSocketDestIp(SOCKET sockNum, uint8 *destIP);
    
    /**
     * Sets socket's destination port number
     * \param sockNum: socket number, between 0 and 3
     * \param value: socket's destination port number
     */
    void setSocketDestPort(SOCKET sockNum, uint16 destPort);
    
    /**
     * Set socket's Maximum Segment Size for TCP mode
     * \param sockNum: socket number, between 0 and 3
     * \param value: MSS value
     */
    void setSocketMSS(SOCKET sockNum, uint16 *value);
    
    /**
     * Set socket's protocol number when used in IPraw mode
     * \param sockNum: socket number, between 0 and 3
     * \param value: protocol number
     */
    void setSocketProtocolValue(SOCKET sockNum, uint8 value);
    
    /**
     * Sets Type Of Service field value in socket's IP header
     * \param sockNum: socket number, between 0 and 3
     * \param TOSvalue: field value
     */
    void setSocketTos(SOCKET sockNum, uint8 TOSvalue);
    
    /**
     * Sets Time To Live field value in socket's IP header
     * \param sockNum: socket number, between 0 and 3
     * \param TTLvalue: field value
     */
    void setSocketTtl(SOCKET sockNum, uint8 TTLvalue);
    
    /**
     * Configures socket's internal RX memory size
     * Accepted values are: 1, 2, 4 and 8kB
     * \param sockNum: socket number, between 0 and 3
     * \param memSize: desired RX memory size
     */
    void setSocketRxMemSize(SOCKET sockNum, uint8 memSize);
    
    /**
     * Configures socket's internal TX memory size
     * Accepted values are: 1, 2, 4 and 8kB
     * \param sockNum: socket number, between 0 and 3
     * \param memSize: desired TX memory size
     */
    void setSocketTxMemSize(SOCKET sockNum, uint8 memSize);
    
    /**
     * Writes data into socket TX buffer and updates in-chip pointer
     * \param sockNum: socket number, between 0 and 3
     * \param data: pointer to buffer containing data to be written
     * \param len: number of bytes to be written
     */
    void writeData(SOCKET sockNum, uint8 *data, uint16 len);
    
    /**
     * Reads data from socket RX buffer and updates in-chip pointer
     * \param sockNum: socket number, between 0 and 3
     * \param data: pointer to buffer in which write data
     * \param len: number of bytes to be read
     */
    void readData(SOCKET sockNum, uint8 *data, uint16 len);
    
    /**
     * \param sockNum: socket number, between 0 and 3
     * \return the received data size in byte
     */
    uint16 getReceivedSize(SOCKET sockNum);
    
private:
    
    W5100();
    
    /**
     * Write one byte into chip's register
     * \param address: register's address
     * \param data: data to be written
     */
    void writeRegister(uint16 address, uint8 data);
    
    /**
     * Write multiple bytes into chip's memory
     * \param address: writing process start point address
     * \param data: pointer to the data to be written
     * \param len: number of bytes to be written
     */
    void writeBuffer(uint16 address, uint8 *data, uint16 len);
    
    /**
     * Read one byte from chip's register
     * \param address: register's address
     * \return data read
     */
    uint8 readRegister(uint16 address);
    
    /**
     * Read multiple bytes into chip's memory
     * \param address: reading process start point address
     * \param data: pointer to the data to be read
     * \param len: number of bytes to be read
     */
    void readBuffer(uint16 address, uint8 *data, uint16 len);
    
    /**
     * This function is used to copy data from application buffer to socket's 
     * in-chip TX buffer
     * 
     * TODO: better description
     *     
     * \param socket: socket number, between 0 and 3
     * \param src: pointer to source buffer
     * \param dst: pointer to destination buffer
     * \param len: number of bytes to be copied
     */
    void writeTxBuf(SOCKET socket, volatile uint8 *src, volatile uint8 *dst, uint16 len);
    
    /**
     * This function is used to copy data from socket's in-chip TX buffer
     * to application buffer
     * 
     * TODO: better description
     * 
     * \param socket: socket number, between 0 and 3
     * \param src: pointer to source buffer
     * \param dst: pointer to destination buffer
     * \param len: number of bytes to be copied
     */
    void readRxBuf(SOCKET socket, volatile uint8 *src, volatile uint8 *dst, uint16 len);
    
    uint16 txBufSize[MAX_SOCK_NUM];   //sockets TX buffer size in byte
    uint16 rxBufSize[MAX_SOCK_NUM];   //sockets RX buffer size in byte
};

#endif // W5100_H
