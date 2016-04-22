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

#include "w5200.h"
#include "w5200_defs.h"
#include <unistd.h>
#include <algorithm>


W5200& w5200 = W5200::instance();

W5200::W5200()
{
    /* initialize RX and TX buffer size vectors to default value,
       which is 2kB */
    
    std::fill(txBufSize, txBufSize + MAX_SOCK_NUM, 0x02 << 10);
    std::fill(rxBufSize, rxBufSize + MAX_SOCK_NUM, 0x02 << 10);
    
    Spi_init(); //start SPI bus if needed
    
    setMacAddress(macAddress);
}

W5200& W5200::instance()
{
    static W5200 instance;
    return instance;
}


void W5200::setMacAddress(const uint8_t* address)
{
    writeRegister(SHAR_BASE, address[0]);
    writeRegister(SHAR_BASE + 1, address[1]);
    writeRegister(SHAR_BASE + 2, address[2]);
    writeRegister(SHAR_BASE + 3, address[3]);
    writeRegister(SHAR_BASE + 4, address[4]);
    writeRegister(SHAR_BASE + 5, address[5]);
}


void W5200::setIpAddress(uint8_t* address)
{
    writeRegister(SIPR_BASE, address[0]);
    writeRegister(SIPR_BASE + 1, address[1]);
    writeRegister(SIPR_BASE + 2, address[2]);
    writeRegister(SIPR_BASE + 3, address[3]);
}


void W5200::setSubnetMask(uint8_t* mask)
{
    writeRegister(SUBR_BASE, mask[0]);
    writeRegister(SUBR_BASE + 1, mask[1]);
    writeRegister(SUBR_BASE + 2, mask[2]);
    writeRegister(SUBR_BASE + 3, mask[3]);
}


void W5200::setGatewayAddress(uint8_t* address)
{
    writeRegister(GAR_BASE, address[0]);
    writeRegister(GAR_BASE + 1, address[1]);
    writeRegister(GAR_BASE + 2, address[2]);
    writeRegister(GAR_BASE + 3, address[3]);
}


void W5200::setModeReg(uint8_t value)
{
    writeRegister(MR, value);
}

void W5200::setInterruptMask(uint8_t mask)
{
    writeRegister(IR_MASK, mask);
}

uint8_t W5200::readInterruptReg()
{
    return readRegister(IR);
}

void W5200::setSocketMSS(SOCKET sockNum, uint16_t value)
{
    writeRegister(SOCKn_MSSR0 + sockNum * SR_SIZE, static_cast<uint8_t>((value & 0xff00) >> 8));
    writeRegister(SOCKn_MSSR0 + sockNum * SR_SIZE + 1, static_cast<uint8_t>(value & 0x00ff));
}

void W5200::setRetryCount(uint16_t value)
{
    writeRegister(RCR, value);
}

void W5200::setRetryTime(uint16_t value)
{
    writeRegister(RTR_BASE, static_cast<uint8_t>((value & 0xff00) >> 8));
    writeRegister(RTR_BASE + 1, static_cast<uint8_t>(value & 0x00ff));
}

void W5200::setSocketModeReg(SOCKET sockNum, uint8_t value)
{
    writeRegister(SOCKn_MR + sockNum * SR_SIZE, value);
}

uint8_t W5200::getSocketStatusReg(SOCKET sockNum)
{
    return readRegister(SOCKn_SR + sockNum * SR_SIZE);
}

uint8_t W5200::getSocketInterruptReg(SOCKET sockNum)
{
    return readRegister(SOCKn_IR + sockNum * SR_SIZE);
}

void W5200::clearSocketInterruptReg(SOCKET sockNum)
{
//     uint8_t flags = getSocketInterruptReg(sockNum);
    writeRegister(SOCKn_IR + sockNum * SR_SIZE, 0xFF);
}

void W5200::setSocketProtocolValue(SOCKET sockNum, uint8_t value)
{
    writeRegister(SOCKn_PROTO + sockNum * SR_SIZE, value);
}

uint8_t W5200::getPhyStatus()
{
    return readRegister(PHY);
}

uint8_t W5200::readSocketInterruptReg()
{
    return readRegister(SOCK_IR);
}

void W5200::setSocketInterruptMask(uint8_t mask)
{
    writeRegister(SOCK_IR_MASK, mask);
}

void W5200::setSocketInterruptMaskReg(SOCKET sockNum, uint8_t value)
{
    writeRegister(SOCKn_IMR + sockNum * SR_SIZE, value);
}

void W5200::setSocketCommandReg(SOCKET sockNum, uint8_t value)
{
    writeRegister(SOCKn_CR + sockNum * SR_SIZE, value);
}

uint8_t W5200::getSocketCommandReg(SOCKET sockNum)
{
    return readRegister(SOCKn_CR + sockNum * SR_SIZE);
}

void W5200::setSocketDestIp(SOCKET sockNum, uint8_t* destIP)
{
    writeRegister(SOCKn_DIPR0 + sockNum * SR_SIZE, destIP[0]);
    writeRegister(SOCKn_DIPR0 + sockNum * SR_SIZE + 1, destIP[1]);
    writeRegister(SOCKn_DIPR0 + sockNum * SR_SIZE + 2, destIP[2]);
    writeRegister(SOCKn_DIPR0 + sockNum * SR_SIZE + 3, destIP[3]);
}

void W5200::setSocketDestMac(SOCKET sockNum, uint8_t* destMAC)
{
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE, destMAC[0]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 1, destMAC[1]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 2, destMAC[2]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 3, destMAC[3]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 4, destMAC[4]);
    writeRegister(SOCKn_DHAR0 + sockNum * SR_SIZE + 5, destMAC[5]);
}

void W5200::setSocketDestPort(SOCKET sockNum, uint16_t destPort)
{
    writeRegister(SOCKn_DPORT0 + sockNum * SR_SIZE, static_cast<uint8_t>((destPort & 0xff00) >> 8));
    writeRegister(SOCKn_DPORT0 + sockNum * SR_SIZE + 1, static_cast<uint8_t>(destPort & 0x00ff));
}

void W5200::setSocketSourcePort(SOCKET sockNum, uint16_t port)
{
    writeRegister(SOCKn_SPORT0 + sockNum * SR_SIZE, static_cast<uint8_t>((port & 0xff00) >> 8));
    writeRegister(SOCKn_SPORT0 + sockNum * SR_SIZE + 1, static_cast<uint8_t>(port & 0x00ff));
}

void W5200::setSocketFragmentValue(SOCKET sockNum, uint16_t value)
{
    writeRegister(SOCKn_FRAG0 + sockNum * SR_SIZE, static_cast<uint8_t>((value & 0xff00) >> 8));
    writeRegister(SOCKn_FRAG0 + sockNum * SR_SIZE + 1, static_cast<uint8_t>(value & 0x00ff));
}

void W5200::setSocketTos(SOCKET sockNum, uint8_t TOSvalue)
{
    writeRegister(SOCKn_TOS + sockNum * SR_SIZE, TOSvalue);
}

void W5200::setSocketTtl(SOCKET sockNum, uint8_t TTLvalue)
{
    writeRegister(SOCKn_TTL + sockNum * SR_SIZE, TTLvalue);
}

void W5200::setSocketRxMemSize(SOCKET sockNum, uint8_t memSize)
{
    writeRegister(SOCKn_RXMEM_SIZE + sockNum * SR_SIZE, memSize);
    rxBufSize[sockNum] = memSize << 10;
}

void W5200::setSocketTxMemSize(SOCKET sockNum, uint8_t memSize)
{
    writeRegister(SOCKn_TXMEM_SIZE + sockNum * SR_SIZE, memSize);
    txBufSize[sockNum] = memSize << 10;
}

uint16_t W5200::getReceivedSize(SOCKET sockNum)
{
    uint16_t len;
    len = readRegister(SOCKn_RX_RSR0 + sockNum * SR_SIZE) << 8;
    len += readRegister(SOCKn_RX_RSR0 + sockNum * SR_SIZE + 1);
    
    return len;
}


void W5200::readData(SOCKET sockNum, uint8_t* data, uint16_t len)
{
    uint16_t readPtr;          
    readPtr = readRegister(SOCKn_RX_RD0 + sockNum * SR_SIZE) << 8;  //read read pointer's upper byte
    readPtr += readRegister(SOCKn_RX_RD0 + sockNum * SR_SIZE + 1);  //read read pointer's lower byte    

    readRxBuf(sockNum, readPtr, data, len);
    
    readPtr += len;
    writeRegister(SOCKn_RX_RD0 + sockNum * SR_SIZE, static_cast<uint8_t>((readPtr & 0xFF00) >> 8)); //update read pointer value
    writeRegister(SOCKn_RX_RD0 + sockNum * SR_SIZE + 1, static_cast<uint8_t>(readPtr & 0x00FF));
}

void W5200::writeData(SOCKET sockNum, uint8_t* data, uint16_t len)
{
    uint16_t writePtr;
    writePtr = readRegister(SOCKn_TX_WR0 + sockNum * SR_SIZE) << 8;  //read write pointer's upper byte
    writePtr += readRegister(SOCKn_TX_WR0 + sockNum * SR_SIZE + 1);  //read write pointer's lower byte
    
    writeTxBuf(sockNum, data, writePtr, len);
    
    writePtr += len;    
    writeRegister(SOCKn_TX_WR0 + sockNum * SR_SIZE, static_cast<uint8_t>((writePtr & 0xFF00) >> 8)); //update write pointer value
    writeRegister(SOCKn_TX_WR0 + sockNum * SR_SIZE + 1, static_cast<uint8_t>(writePtr & 0x00FF));
}


// void W5200::readRxBuf(SOCKET socket, volatile uint16_t src, volatile uint8_t* dst, uint16_t len)
void W5200::readRxBuf(SOCKET socket, volatile uint16_t src, uint8_t* dst, uint16_t len)
{
    
    /* compute socket's buffer base address as a sum of RX_BUF_BASE and
       the sizes of buffers allotted for sockets before this */

    uint16_t sockBufBase = RX_BUF_BASE;
    
    for(int i = 0; i < socket; i++)
        sockBufBase += rxBufSize[i];
    
    /* the address mask value is equal to socket's size in byte minus one */
    
    uint16_t mask = rxBufSize[socket] - 1;
    
    /* the physical address at which reading process begins is base address plus
       the logical and between src pointer and address mask */
    
    uint16_t startAddress = (src & mask) + sockBufBase;

    if((src & mask) + len > rxBufSize[socket])
    {
        uint16_t size = rxBufSize[socket] - (src & mask);
        readBuffer(startAddress, dst, size);
        dst += size;
        size = len - size;
        readBuffer(sockBufBase, dst, size);
    
    }else{
        
        readBuffer(startAddress, dst, len);
    }
}

// void W5200::writeTxBuf(SOCKET socket, volatile uint8_t* src, uint16_t dst, uint16_t len)
void W5200::writeTxBuf(SOCKET socket, uint8_t* src, uint16_t dst, uint16_t len)
{
    
    /* compute socket's buffer base address as a sum of RX_BUF_BASE and
       the sizes of buffers allotted for sockets before this */

    uint16_t sockBufBase = TX_BUF_BASE;
    
    for(int i = 0; i < socket; i++)
        sockBufBase += txBufSize[i];
    
    /* the address mask value is equal to socket's size in byte minus one */
    
    uint16_t mask = txBufSize[socket] - 1;
    
    /* the physical address at which reading process begins is base address plus
       the logical and between src pointer and address mask */
    
    uint16_t startAddress = (dst & mask) + sockBufBase;
            
    if((dst & mask) + len > txBufSize[socket])
    {
        uint16_t size = txBufSize[socket] - (dst & mask);
        writeBuffer(startAddress, src, size);
        src += size;
        size = len - size;
        writeBuffer(sockBufBase, src, size);
        
    }else{
        
        writeBuffer(startAddress, src, len);
    }    
}

uint8_t W5200::readRegister(uint16_t address)
{
    uint8_t data;
    
    Spi_CS_low();
    
    Spi_sendRecv((address & 0xFF00) >> 8);  // Address byte 1
    Spi_sendRecv(address & 0x00FF);         // Address byte 2
    Spi_sendRecv(0x00);                     // Data read command and read data length 1
    Spi_sendRecv(0x01);                     // Read data length 2
    data = Spi_sendRecv(0x00);              // Data read

    Spi_CS_high();                         
    
    return data;
}

void W5200::writeBuffer(uint16_t address, uint8_t* data, uint16_t len)
{
    if(len == 0)
        return;
    
    Spi_CS_low();
    
    Spi_sendRecv((address & 0xFF00) >> 8);          // Address byte 1
    Spi_sendRecv(address & 0x00FF);                 // Address byte 2
    Spi_sendRecv(0x80 | ((len & 0x7F00) >> 8));     // Data write command and Write data length 1
    Spi_sendRecv(len & 0x00FF);                     // Write data length 2
    
    for(uint16_t i = 0; i < len; i++)                       
        Spi_sendRecv(data[i]);
    
    Spi_CS_high();
}


void W5200::writeRegister(uint16_t address, uint8_t data)
{
    Spi_CS_low();
    
    Spi_sendRecv((address & 0xFF00) >> 8);  // Address byte 1
    Spi_sendRecv(address & 0x00FF);         // Address byte 2
    Spi_sendRecv(0x80);                     // Data write command and Write data length 1
    Spi_sendRecv(0x01);                     // Write data length 2
    Spi_sendRecv(data);                     // Data write

    Spi_CS_high();    
}


void W5200::readBuffer(uint16_t address, uint8_t* data, uint16_t len)
{
    if(len == 0)
        return;
    
    Spi_CS_low();
    
    Spi_sendRecv((address & 0xFF00) >> 8);          // Address byte 1
    Spi_sendRecv(address & 0x00FF);                 // Address byte 2
    Spi_sendRecv(0x00 | ((len & 0x7F00) >> 8));     // Data write command and Write data length 1
    Spi_sendRecv(len & 0x00FF);                     // Write data length 2
    
    for(uint16_t i = 0; i < len; i++)                       
        data[i] = Spi_sendRecv(0x00);
    
    Spi_CS_high();

}


