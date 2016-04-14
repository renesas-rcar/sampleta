/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef COMMON_H
#define COMMON_H

/******************************************************************************/
/* TA Communication Interfaces                                                */
/******************************************************************************/
/* ************************************************** */
/* Define                                             */
/* ************************************************** */
/* This UUID is generated with uuidgen
   the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html */
#define SAMPLE_TA_UUID { 0x2FA77580U, 0xEC0AU, 0x11E5U, \
		{ 0xA4U, 0x7FU, 0x00U, 0x02U, 0xA5U, 0xD5U, 0xC5U, 0x1BU} }

/******************************************************************************/
/* Enumerators                                                                */
/******************************************************************************/
enum TEEC_CommandId {
	E_TEEC_CMD_AES = 0U,
	E_TEEC_CMD_NUM_OF_COMMANDID
};

#define ALGO_ECB	0U
#define ALGO_CBC	1U
#define MODE_ENC	0U
#define MODE_DEC	1U

#define BLOCK_SIZE	16U
#define KEY_SIZE	128U

#endif /* COMMON_H */
