/******************************************************************************
 *
 * Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Use of the Software is limited solely to applications:
 * (a) running on a Xilinx device, or
 * (b) that interact with a Xilinx device through a bus or interconnect.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 *
 ******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "RSA.h"
#include "xil_io.h"
#include "xuartlite_l.h"

#define RSA_writeMsg(msg) \
		RSA_mWriteReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG0_OFFSET,msg)

#define RSA_writeExp(exp) \
		RSA_mWriteReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG1_OFFSET,exp)

#define RSA_readMsg() \
		RSA_mReadReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG2_OFFSET)

#define RSA_done() \
		RSA_mReadReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG3_OFFSET)

#define LEN 255

s32 read(char *buf, u32 len)
{
#ifdef STDIN_BASEADDRESS
	u32 i;
	s32 numbytes = 0;

	if(buf != NULL) {
		for (i = 0; i < len; i++) {
			*(buf + i) = inbyte();
			numbytes++;
			if ((*(buf + i) == '\n' ) ||
					(*(buf + i) == '\r')) {
				break;
			}
		}
	}

	return numbytes;
#endif
	return 0;
}

void encrypt(char *msg, s32 msgLen) {
	RSA_writeExp(17);
	char resultBuf;
	sint32 result;
	for(int i = 0; i < msgLen; i++) {
		RSA_writeMsg(*(msg + i));
		while(!RSA_done()) {

		}
		result = RSA_readMsg();
		for(int j = 0; j < 4; j++) {
			resultBuf = (result & 0x0000000F);
			resultBuf += '0';
			if(resultBuf > '9') {
				resultBuf += ('A' - '9' - 1);
			}
			outbyte(resultBuf);
			result = result >> 4;
		}
	}
	outbyte('\n');
}

void decrypt(char *msg, s32 msgLen) {
	RSA_writeExp(413);
	sint32 toWrite;
	for(int i = 0; i < msgLen/4; i++) {
		toWrite = 0;
		for(int j = 3; j > -1; j--) {
			msg[4*i + j] -= '0';
			if(msg[4*i + j] > 9) {
				msg[4*i + j] -= ('A' - '9' - 1);
			}
			toWrite += msg[4*i + j];
			toWrite = toWrite << 4;
		}
		toWrite = toWrite >> 4;
		RSA_writeMsg(toWrite);
		while(!RSA_done()) {

		}
		outbyte((char)RSA_readMsg());
	}
	outbyte('\n');
}

int main()
{
	init_platform();

	xil_printf("Hello World\n\r");
	xil_printf("Successfully ran Hello World application\n\r");

	char message[LEN];
	//char result[2*LEN];

	char *encryptMsg = "Encrypt";
	char *decryptMsg = "Decrypt";
	s32 msgLen = 0;
	for(;;) {
		msgLen = read(message,LEN);
		if(msgLen > 0) {
			for(int i = 0; i < 7; i++) {
				if(encryptMsg[i] != message[i])
					break;
				if(i == 6) {
					print("Encrypting\n\r");
					encrypt(message + 8,msgLen - 9);
				}
			}
			for(int i = 0; i < 7; i++) {
				if(decryptMsg[i] != message[i])
					break;
				if(i == 6) {
					print("Decrypting\n\r");
					decrypt(message + 8,msgLen - 9);
				}
			}
		}
	}



	/*
    for(;;) {

    s32 msgLen = read(message,LEN);

    if(msgLen > 0) {
    	printf(message);

    }
    }
	 */
	/*

    if(msgLen > 8) {
    	for(int i = 0; i < 7; i++) {
    		if(encryptMsg[i] != message[i])
    			break;
    		if(i == 6) {
    			print("Encrypting\n\r");
    			//encrypt(message + 8,msgLen - 8);
    		}
    	}
    	for(int i = 0; i < 7; i++) {
    	    if(decryptMsg[i] != message[i])
    	    	break;
    	    if(i == 6) {
    	    	print("Decrypting\n\r");
    	    	//decrypt(message + 8,msgLen - 8);
    	    }
        }
    }
    msgLen = 0;
    }
	 */
	RSA_mWriteReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG0_OFFSET,0x6C6C6548);
	RSA_mWriteReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG1_OFFSET,0x55555555);
	sint32 msg = RSA_mReadReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG2_OFFSET);

	message[0] = (char)(msg & 0x000000FF);
	message[1] = (char)((msg >> 8) & 0x000000FF);
	message[2] = (char)((msg >> 16) & 0x000000FF);
	message[3] = (char)((msg >> 24) & 0x000000FF);
	message[4] = '\000';
	xil_printf(message);

	RSA_mWriteReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG0_OFFSET,msg);
	msg = RSA_mReadReg(XPAR_RSA_0_S00_AXI_BASEADDR,RSA_S00_AXI_SLV_REG2_OFFSET);

	message[0] = (char)(msg & 0x000000FF);
	msg = msg >> 8;
	message[1] = (char)(msg & 0x000000FF);
	msg = msg >> 8;
	message[2] = (char)(msg & 0x000000FF);
	msg = msg >> 8;
	message[3] = (char)(msg & 0x000000FF);
	message[4] = '\000';
	xil_printf(message);


	cleanup_platform();
	return 0;
}


