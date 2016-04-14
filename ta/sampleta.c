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
#define STR_TRACE_USER_TA "SAMPLETA"

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <utee_defines.h>

#include "common.h"

/******************************************************************************/
/* Macros                                                                     */
/******************************************************************************/
#define DBUFDUMP(buf, size)\
	do {\
		DMSG("----------------------------- ##buf -----------------------------\n");\
		DHEXDUMP(buf, size);\
	} while (0)

/******************************************************************************/
/* Typedefs                                                                   */
/******************************************************************************/
/* Argument */
typedef TEE_Param TEE_PARAM_ARGV[4];

/******************************************************************************/
/* Static Function Prototypes                                                 */
/******************************************************************************/
/* Function prototype */
/* AES */
static TEE_Result ReadSecretKey(void);

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
static uint8_t	objID[] = {0x01U,0x02U,0x03U,0x04U};
static uint32_t	objID_len = 4U;

/* Secret key */
static TEE_ObjectHandle secretKey;

/* IV */
static uint8_t	iv[16];

/*
 * brief:       TA_CreateEntryPoint
 * 
 * param[in]    void
 * return       TEE_Result
 */
TEE_Result TA_CreateEntryPoint(void)
{
	TEE_Result		res;
	uint8_t			keyData[16];
	TEE_ObjectHandle	object;
	
	/* Opens a handle on an existing persistent object. */
	res = TEE_OpenPersistentObject((uint32_t)TEE_STORAGE_PRIVATE, objID, objID_len,
		(uint32_t)TEE_DATA_FLAG_ACCESS_READ, &object);
	if (res != (TEE_Result)TEE_SUCCESS) {
		IMSG("################################First Access################################\n");
		
		/* Random generation of key data. */
		TEE_GenerateRandom((void*)keyData, sizeof(keyData));
		DBUFDUMP(keyData, (int32_t)sizeof(keyData));
		
		/* Random generation of IV. */
		TEE_GenerateRandom((void*)iv, sizeof(iv));
		DBUFDUMP(iv, (int32_t)sizeof(iv));
		
		/* Create persistent object. */
		res = TEE_CreatePersistentObject((uint32_t)TEE_STORAGE_PRIVATE, objID, objID_len,
						 (uint32_t)TEE_DATA_FLAG_ACCESS_WRITE,
						 NULL, NULL, 0U, &object);
		if (res != (TEE_Result)TEE_SUCCESS) {
			EMSG("TEE_CreatePersistentObject\n");
		} else {
			/* Write the key data. */
			res = TEE_WriteObjectData(object, (void*)keyData, sizeof(keyData)); 
			if (res != (TEE_Result)TEE_SUCCESS) {
				EMSG("TEE_WriteObjectData\n");
			}
			/* Write the IV. */
			res = TEE_WriteObjectData(object, (void*)iv, sizeof(iv)); 
			if (res != (TEE_Result)TEE_SUCCESS) {
				EMSG("TEE_WriteObjectData\n");
			}
			TEE_CloseObject(object);
		}
	} else {
		TEE_CloseObject(object);
	}
	
	return res;
}

/*
 * brief:       TA_DestroyEntryPoint
 * 
 * param[in]    void
 * return       void
 */
void TA_DestroyEntryPoint(void)
{
	INMSG("\n");
	TEE_FreeTransientObject(secretKey);
	OUTMSG("\n");
}

/*
 * brief:       TA_OpenSessionEntryPoint
 * 
 * param[in]    paramTypes                      - Operation Parameters in the Internal Client API
 * param[in]    params                          - TEE_Param Data
 * param[out]   **sessionContext                - sessionContext
 * return       TEE_Result
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes,
		TEE_PARAM_ARGV params, void **sessionContext)
{
	uint32_t exp_param_types = (uint32_t)TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
							     TEE_PARAM_TYPE_NONE,
							     TEE_PARAM_TYPE_NONE,
							     TEE_PARAM_TYPE_NONE);
	TEE_Result	res = (TEE_Result)TEE_SUCCESS;
	
	INMSG("paramTypes=0x%08x, params=%p, sessionContext=%p\n", paramTypes, (void*)params, (void*)*sessionContext);

	if (paramTypes != exp_param_types) {
		EMSG("Error:paramTypes=%d \n", paramTypes);
		res = TEE_ERROR_GENERIC;
	}

	OUTMSG("res=0x%08x\n", res);

	return res;
}

/*
 * brief:       TA_CloseSessionEntryPoint
 * 
 * param[ctx]   *sessionContext                 - sessionContext
 * return       void
 */
void TA_CloseSessionEntryPoint(void *sessionContext)
{
	INMSG("sessionContext=%p\n", sessionContext);
	OUTMSG("\n");
}

/*
 * brief:       ReadSecretKey
 * 
 * param[in]    void
 * return       TEE_Result
 */
static TEE_Result ReadSecretKey(void)
{
	TEE_Result		res;
	TEE_Attribute		attrs[1];
	uint8_t			keyData[16];
	uint32_t		readSize;
	TEE_ObjectHandle	object;
	
	/* Opens a handle on an existing persistent object. */
	res = TEE_OpenPersistentObject((uint32_t)TEE_STORAGE_PRIVATE, objID, objID_len,
		(uint32_t)TEE_DATA_FLAG_ACCESS_READ, &object);
	if (res == (TEE_Result)TEE_SUCCESS) {
		IMSG("################################Second Access################################\n");
		
		/* Read the key data. */
		res = TEE_ReadObjectData(object, (void*)keyData, sizeof(keyData), &readSize);
		if (res != (TEE_Result)TEE_SUCCESS) {
			EMSG("TEE_ReadObjectData\n");
		}
		DBUFDUMP(keyData, (int32_t)readSize);
		
		/* Read the IV. */
		res = TEE_ReadObjectData(object, (void*)iv, sizeof(iv), &readSize);
		if (res != (TEE_Result)TEE_SUCCESS) {
			EMSG("TEE_ReadObjectData\n");
		}
		DBUFDUMP(iv, (int32_t)readSize);
		
		/* Create key handle */
		if (res == (TEE_Result)TEE_SUCCESS) {
			TEE_InitRefAttribute(&attrs[0], TEE_ATTR_SECRET_VALUE, (void*)keyData, sizeof(keyData));
			res = TEE_AllocateTransientObject(TEE_TYPE_AES, KEY_SIZE, &secretKey);
			if (res != (TEE_Result)TEE_SUCCESS) {
				EMSG("Error TEE_AllocateTransientObject\n");
			}
		}
		if (res == (TEE_Result)TEE_SUCCESS) {
			res = TEE_PopulateTransientObject(secretKey, attrs, 1U);
			if (res != (TEE_Result)TEE_SUCCESS) {
				EMSG("Error TEE_PopulateTransientObject\n");
			}
			(void)TEE_Free(attrs[0].content.ref.buffer);
			(void)TEE_Free(attrs);
		}
		TEE_CloseObject(object);
	}
	
	return res;
}

/*
 * brief:       TA_InvokeCommandEntryPoint
 * 
 * param[in]    *sessionContext                 - sessionContext
 * param[in]    commandID                       - commandID
 * param[in]    paramTypes                      - parameter type
 * param[in]    params                          - TEE_Param Data
 * return       TEE_Result
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sessionContext, uint32_t commandID,
			uint32_t paramTypes, TEE_PARAM_ARGV params)
{
	TEE_Result		res;
	TEE_OperationHandle	op = TEE_HANDLE_NULL;
	TEE_ObjectInfo		info;

	uint32_t	Algo;
	uint32_t	Mode;
	uint8_t		*inbuf;
	uint32_t	inbuf_size;
	uint8_t		*outbuf;
	uint32_t	outbuf_size;
	
	uint32_t	pos;
	uint32_t	Remainder;
	
	INMSG("sessionContext=%p, commandID=0x%08x, paramTypes=0x%08x, params=%p\n", (void*)sessionContext, commandID, paramTypes, (void*)params);
	
	Algo		= params[0].value.a;
	Mode		= params[1].value.a;
	inbuf		= (uint8_t*)params[2].memref.buffer;
	inbuf_size	= (uint32_t)params[2].memref.size;
	outbuf		= (uint8_t*)params[3].memref.buffer;
	outbuf_size	= (uint32_t)params[3].memref.size;
	
	/* Reads the secret key */
	res = ReadSecretKey();
	
	/* Algo */
	if (res == (TEE_Result)TEE_SUCCESS) {
		switch(Algo) {
		case ALGO_ECB:
			Algo = (uint32_t)TEE_ALG_AES_ECB_NOPAD;
			break;
		case ALGO_CBC:
			Algo = (uint32_t)TEE_ALG_AES_CBC_NOPAD;
			break;
		default:
			EMSG("Unsupported algorithm=%d\n", Algo);
			res = TEE_ERROR_GENERIC;
			break;
		}
	}
	
	/* Mode */
	if (res == (TEE_Result)TEE_SUCCESS) {
		switch(Mode) {
		case MODE_ENC:
			Mode = (uint32_t)TEE_MODE_ENCRYPT;
			break;
		case MODE_DEC:
			Mode = (uint32_t)TEE_MODE_DECRYPT;
			break;
		default:
			EMSG("Unsupported operation mode=%d\n", Mode);
			res = TEE_ERROR_GENERIC;
			break;
		}
	}
	
	/* To encrypt or decrypt the input data with the secret key */
	if (res == (TEE_Result)TEE_SUCCESS) {
		TEE_GetObjectInfo1(secretKey, &info);
		IMSG("maxKeySize=%d\n", info.maxKeySize);
		
		res = TEE_AllocateOperation(&op, Algo, Mode, info.maxKeySize);
		if (res != (TEE_Result)TEE_SUCCESS) {
			EMSG("Error TEE_AllocateOperation maxKeySize=%d\n", info.maxKeySize);
		}
	}
	if (res == (TEE_Result)TEE_SUCCESS) {
		res = TEE_SetOperationKey(op, secretKey);
		if (res != (TEE_Result)TEE_SUCCESS) {
			EMSG("Error TEE_SetOperationKey\n");
		}
	}
	if (res == (TEE_Result)TEE_SUCCESS) {
		TEE_CipherInit(op, iv, sizeof(iv));
		
		pos = 0U;
		Remainder = inbuf_size;
		/*
		 * TEE_CipherUpdate since the update process is called in TEE_CipherDoFinal is not required.
		 * The following comment out is an example when carry out TEE_CipherUpdate.
		 * Big data can not send at one time to Secure World from Normal World.
		 * If this is the case, you will need to implement separately in TEE_CipherUpdate and TEE_CipherDoFinal.
		 */
		//while ((Remainder % BLOCK_SIZE) == 0U) {
		//	res = TEE_CipherUpdate(op, &inbuf[pos], BLOCK_SIZE, &outbuf[pos], &outbuf_size);
		//	if (res != (TEE_Result)TEE_SUCCESS) {
		//		EMSG("Error TEE_CipherUpdate\n");
		//		break;
		//	}
		//	pos += BLOCK_SIZE;
		//	Remainder -= BLOCK_SIZE;
		//	
		//	/* Exit to leave the last of the block */
		//	if (Remainder == BLOCK_SIZE) {
		//		break;
		//	}
		//}
		
		/* Last block */
		if (res == (TEE_Result)TEE_SUCCESS) {
			res = TEE_CipherDoFinal(op, &inbuf[pos], Remainder, &outbuf[pos], &outbuf_size);
			if (res != (TEE_Result)TEE_SUCCESS) {
				EMSG("Error TEE_CipherDoFinal\n");
			}
		}
	}

	/* Free OperationHandle */
	if (op != TEE_HANDLE_NULL) {
		TEE_FreeOperation(op);
	}
	OUTMSG("res=0x%08x\n", res);
	return res;
}
