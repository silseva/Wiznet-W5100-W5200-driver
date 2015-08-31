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

#include "w5100.h"
#include "w5100_defs.h"
#include <unistd.h>
#include <algorithm>


// W5100& w5100 = W5100::instance();

W5100::W5100()
{
    /* initialize RX and TX buffer size vectors to default value,
       which is 2kB */
    
    std::fill(txBufSize, txBufSize + MAX_SOCK_NUM, 0x02 << 10);
    std::fill(rxBufSize, rxBufSize + MAX_SOCK_NUM, 0x02 << 10);
    
    Spi_init(); //start SPI bus if needed
}

W5100& W5100::instance()
{
    static W5100 instance;
    return instance;
}


void W5100::setMacAddress(uint8* address)
{
    writeRegister(SHAR_BASE, address[0]);
    writeRegister(SHAR_BASE + 1, address[1]);
    writeRegister(SHAR_BASE + 2, address[2]);
    writeRegister(SHAR_BASE + 3, address[3]);
    writeRegister(SHAR_BASE + 4, address[4]);
    writeRegister(SHAR_BASE + 5, address[5]);
}


void W5100::setIpAddress(uint8* address)
{
    writeRegister(SIPR_BASE, address[0]);
    writeRegister(SIPR_BASE + 1, address[1]);
    writeRegister(SIPR_BASE + 2, address[2]);
    writeRegister(SIPR_BASE + 3, address[3]);
}


void W5100::setSubnetMask(uint8* mask)
{
    writeRegister(SUBR_BASE, mask[0]);
    writeRegister(SUBR_BASE + 1, mask[1]);
    writeRegister(SUBR_BASE + 2, mask[2]);
    writeRegister(SUBR_BASE + 3, mask[3]);
}


void W5100::setGatewayAddress(uint8* address)
{
    writeRegister(GAR_BASE, address[0]);
    writeRegister(GAR_BASE + 1, address[1]);
    writeRegister(GAR_BASE + 2, address[2]);
    writeRegister(GAR_BASE + 3, address[3]);
}


void W5100::setModeReg(uint8 value)
{
    writeRegister(MR, value);
}

void W5100::setInterruptMask(uint8 mask)
{
    writeRegister(IR_MASK, mask);
}

uint8 W5100::readInterruptReg()
{
    return readRegister(IR);
}

void W5100::setSocketMSS(SOCKET sockNum, uint16 value)
{
    writeRegister(SOCKn_MSSR0 + sockNum * SR_SIZE, (uint8) ((value & 0xff00) >> 8));
    writeRegister(SOCKn_MSSR0 + sockNum * SR_SIZE + 1, (uint8) (value & 0x00ff));
}

void W5100::setRetryCount(uint16 value)
{
    writeRegister(RCR, value);
}

void W5100::setRetryTime(uint16 value)
{
    writeRegister(RTR_BASE, (uint8) ((value & 0xff00) >> 8));
    writeRegister(RTR_BASE + 1, (uint8) (value & 0x00ff));
}

void W5100::setSocketModeReg(SOCKET sockNum, uint8 value)
{
    writeRegister(SOCKn_MR + sockNum * SR_SIZE, value);
}

uint8 W5100::getSocketStatusReg(SOCKET sockNum)
{
    return readRegister(SOCKn_SR + sockNum * SR_SIZE);
}

uint8 W5100::getSocketInterruptReg(SOCKET sockNum)
{
    return readRegister(SOCKn_IR + sockNum * SR_SIZE);
}

void W5100::setSocketProtocolValue(SOCKET sockNum, uint8 value)
{
    writeRegister(SOCKn_PROTO + sockNum * SR_SIZE, value);
}

void W5100::setSocketCommandReg(SOCKET sockNum, uint8 value)
{
    writeRegister(SOCKn_CR + sockNum * SR_SIZE, value);
}

void W5100::setSocketDestIp(SOCKET sockNum, uint8* destIP)
{
    writeRegister(SOCKn_DIPR0 + sockNum * SR_SIZE, destIP[0]);
    writeRegister(SOCKn_DIPR0 + sockNum * SR_SIZE + 1, destIP[1]);
    writeRegister(SOCKn_DIPR0 + sockNum * SR_SIZE + 2, destIP[2]);
    writeRegister(SOCKn_DIPR0 + sockNum * SR_SIZE + 3, destIP[3]);
}

void W5100::setSocketDestMac(SOCKET sockNum, uint8* destMAC)
{
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE, destMAC[0]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 1, destMAC[1]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 2, destMAC[2]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 3, destMAC[3]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 4, destMAC[4]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 5, destMAC[5]);
}

void W5100::setSocketDestPort(SOCKET sockNum, uint16 destPort)
{
    writeRegister(SOCKn_DPORT0 + sockNum * SR_SIZE, (uint8) ((destPort & 0xff00) >> 8));
    writeRegister(SOCKn_DPORT0 + sockNum * SR_SIZE + 1, (uint8) (destPort & 0x00ff));
}

void W5100::setSocketSourcePort(SOCKET sockNum, uint16 port)
{
    writeRegister(SOCKn_SPORT0 + sockNum * SR_SIZE, (uint8) ((port & 0xff00) >> 8));
    writeRegister(SOCKn_SPORT0 + sockNum * SR_SIZE + 1, (uint8) (port & 0x00ff));
}

void W5100::setSocketTos(SOCKET sockNum, uint8 TOSvalue)
{
    writeRegister(SOCKn_TOS + sockNum * SR_SIZE, TOSvalue);
}

void W5100::setSocketTtl(SOCKET sockNum, uint8 TTLvalue)
{
    writeRegister(SOCKn_TTL + sockNum * SR_SIZE, TTLvalue);
}

void W5100::setSocketRxMemSize(SOCKET sockNum, uint8 memSize)
{
    uint8 bits = 0;
    uint8 regVal = readRegister(RMSR);      //get actual register value
    
    /* clear bits that set sockNum's register value 
       leaving the others untouched */
    regVal &= ~(0x03 << sockNum);          
    
    /* the memory configuration bits value is symply the 
       base 2 logarithm of desired size in kB. In other
       words is the base 2 logarithm of memSize, that
       is simply obtained with a while loop */
    while(memSize>>1) bits++;
    
    regVal |= bits << sockNum;      //update value
    writeRegister(RMSR, regVal);
    
    rxBufSize[sockNum] = memSize << 10;
}

void W5100::setSocketTxMemSize(SOCKET sockNum, uint8 memSize)
{
    uint8 bits = 0;
    uint8 regVal = readRegister(RMSR);

    regVal &= ~(0x03 << sockNum);          
    while(memSize>>1) bits++;
    regVal |= bits << sockNum;      //update value
    writeRegister(RMSR, regVal);
    
    txBufSize[sockNum] = memSize << 10;
}

uint16 W5100::getReceivedSize(SOCKET sockNum)
{
    uint16 len;
    len = readRegister(SOCKn_RX_RSR0 + sockNum * SR_SIZE) << 8;
    len += readRegister(SOCKn_RX_RSR0 + sockNum * SR_SIZE + 1);
    
    return len;
}


void W5100::readData(SOCKET sockNum, uint8* data, uint16 len)
{
    uint16 readPtr;
    readPtr = readRegister(SOCKn_RX_RD0 + sockNum * SR_SIZE) << 8;  //read read pointer's upper byte
    readPtr += readRegister(SOCKn_RX_RD0 + sockNum * SR_SIZE + 1);  //read read pointer's lower byte
    
    readRxBuf(sockNum, reinterpret_cast<uint8 *>(readPtr), data, len);
    
    readPtr += len;
    writeRegister(SOCKn_RX_RD0 + sockNum * SR_SIZE, (uint8)((readPtr & 0xFF00) >> 8)); //update read pointer value
    writeRegister(SOCKn_RX_RD0 + sockNum * SR_SIZE + 1, (uint8)(readPtr & 0x00FF));
}

void W5100::writeData(SOCKET sockNum, uint8* data, uint16 len)
{
    uint16 writePtr;
    writePtr = readRegister(SOCKn_TX_WR0 + sockNum * SR_SIZE) << 8;  //read write pointer's upper byte
    writePtr += readRegister(SOCKn_TX_WR0 + sockNum * SR_SIZE + 1);  //read write pointer's lower byte
    
    readRxBuf(sockNum, data, reinterpret_cast<uint8 *>(writePtr), len);
    
    writePtr += len;
    writeRegister(SOCKn_TX_WR0 + sockNum * SR_SIZE, (uint8)((writePtr & 0xFF00) >> 8)); //update write pointer value
    writeRegister(SOCKn_TX_WR0 + sockNum * SR_SIZE + 1, (uint8)(writePtr & 0x00FF));
}


void W5100::readRxBuf(SOCKET socket, volatile uint8* src, volatile uint8* dst, uint16 len)
{
    
    /* compute socket's buffer base address as a sum of RX_BUF_BASE and
       the sizes of buffers allotted for sockets before this */

    uint16 sockBufBase = RX_BUF_BASE;
    
    for(int i = 0; i < socket-1; i++)
        sockBufBase += rxBufSize[i];
    
    /* the address mask value is equal to socket's size in byte minus one */
    
    uint16 mask = rxBufSize[socket] - 1;
    
    /* the physical address at which reading process begins is base address plus
       the logical and between src pointer and address mask */
    
    uint16 startAddress = (reinterpret_cast<unsigned int>(src) & mask) + sockBufBase;
    
    if(startAddress + len > rxBufSize[socket])
    {
        uint16 size = rxBufSize[socket] - startAddress;
        readBuffer(startAddress,const_cast<uint8 *>(dst), size);
        dst += size;
        size = len - size;
        readBuffer(sockBufBase, const_cast<uint8 *>(dst), size);
    
    }else{
        
        readBuffer(startAddress, const_cast<uint8 *>(dst), len);
    }
}

void W5100::writeTxBuf(SOCKET socket, volatile uint8* src, volatile uint8* dst, uint16 len)
{
    
    /* compute socket's buffer base address as a sum of RX_BUF_BASE and
       the sizes of buffers allotted for sockets before this */

    uint16 sockBufBase = TX_BUF_BASE;
    
    for(int i = 0; i < socket-1; i++)
        sockBufBase += txBufSize[i];
    
    /* the address mask value is equal to socket's size in byte minus one */
    
    uint16 mask = txBufSize[socket] - 1;
    
    /* the physical address at which reading process begins is base address plus
       the logical and between src pointer and address mask */
    
    uint16 startAddress = (reinterpret_cast<unsigned int>(dst) & mask) + sockBufBase;
    
    if(startAddress + len > txBufSize[socket])
    {
        uint16 size = txBufSize[socket] - startAddress;
        writeBuffer(startAddress, const_cast<uint8 *>(src), size);
        dst += size;
        size = len - size;
        writeBuffer(sockBufBase, const_cast<uint8 *>(src), size);
    
    }else{
        
        writeBuffer(startAddress, const_cast<uint8 *>(src), len);
    }
}

uint8 W5100::readRegister(uint16 address)
{
    uint8 data;
    
    Spi_CS_low();
    
    Spi_sendRecv(0x0F);                     // read opcode
    Spi_sendRecv(address >> 8);             // Address byte 1
    Spi_sendRecv(address & 0x00FF);         // Address byte 2
    data = Spi_sendRecv(0x00);              // Data read

    Spi_CS_high();                         
    
    return data;
}

void W5100::readBuffer(uint16 address, uint8* data, uint16 len)
{
    if(len == 0)
        return;
    
    for(uint16 i=0; i < len; i++)
    {
        Spi_CS_low();
    
        Spi_sendRecv(0x0F);                     // read opcode
        Spi_sendRecv(address >> 8);             // Address byte 1
        Spi_sendRecv(address & 0x00FF);         // Address byte 2
        address++;
        data[i] = Spi_sendRecv(0x00);           // Data read

        Spi_CS_high();
    }
}

void W5100::writeRegister(uint16 address, uint8 data)
{
    Spi_CS_low();
    
    Spi_sendRecv(0xF0);                     // write opcode
    Spi_sendRecv(address >> 8);             // Address byte 1
    Spi_sendRecv(address & 0x00FF);         // Address byte 2
    Spi_sendRecv(data);                     // Data write

    Spi_CS_high(); 
}

void W5100::writeBuffer(uint16 address, uint8* data, uint16 len)
{
    if(len == 0)
        return;
    
    for(uint16 i=0; i < len; i++)
    {
        Spi_CS_low();
    
        Spi_sendRecv(0xF0);                     // write opcode
        Spi_sendRecv(address >> 8);             // Address byte 1
        Spi_sendRecv(address & 0x00FF);         // Address byte 2
        address++;
        Spi_sendRecv(data[i]);                  // Data write

        Spi_CS_high();
    }
}




