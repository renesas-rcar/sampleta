/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <tee_client_api.h>
#include "common.h"

#define myFree(p)\
	do {\
		if ((p) != NULL) {\
		free(p); \
		(p)=NULL;} \
	} while (0)

int main(int argc, char *argv[])
{
	const uint8_t	ARGV_ECB[3] = "ecb";
	const uint8_t	ARGV_CBC[3] = "cbc";
	const uint8_t	ARGV_ENC[3] = "enc";
	const uint8_t	ARGV_DEC[3] = "dec";
	
	TEEC_Result	res = (TEEC_Result)TEEC_SUCCESS;
	TEEC_Context	ctx;
	TEEC_Session	sess;
	TEEC_Operation	op;
	TEEC_UUID	uuid = SAMPLE_TA_UUID;
	uint32_t	err_origin;
	
	uint32_t	algo;
	uint32_t	mode;
	FILE		*fp_in;
	FILE		*fp_out;
	size_t		readSize;
	size_t		writeSize;
	uint8_t		*input_buf	= NULL;
	uint8_t		*output_buf	= NULL;
	struct	stat	st;
	TEEC_SharedMemory	input_SharedMemory;
	
	
	if (argc != 5) {
		res = TEEC_ERROR_GENERIC;
		(void)printf("Error argc=%d\n", argc);
	}
	
	/* Initialize a context. */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		res = TEEC_InitializeContext(NULL, &ctx);
		if (res != (TEEC_Result)TEEC_SUCCESS) {
			(void)printf("TEEC_InitializeContext failed with code 0x%08x\n", res);
		}
	}

	/* Open a session to the "sample ta". */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		res = TEEC_OpenSession(&ctx, &sess, &uuid, (uint32_t)TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
		if (res != (TEEC_Result)TEEC_SUCCESS) {
			(void)printf("TEEC_Opensession failed with code 0x%08x origin 0x%08x\n", res, err_origin);
		}
	}

	/* Algo */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		if (memcmp(argv[1], ARGV_ECB, sizeof(ARGV_ECB)) == 0U) {
			algo = ALGO_ECB;
		} else if (memcmp(argv[1], ARGV_CBC, sizeof(ARGV_CBC)) == 0U) {
			algo = ALGO_CBC;
		} else {
			res = TEEC_ERROR_GENERIC;
			(void)printf("Unsupported algorithm %s\n", argv[1]);
		}
	}
	
	/* Mode */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		if (memcmp(argv[2], ARGV_ENC, sizeof(ARGV_ENC)) == 0U) {
			mode = MODE_ENC;
		} else if (memcmp(argv[2], ARGV_DEC, sizeof(ARGV_DEC)) == 0U) {
			mode = MODE_DEC;
		} else {
			res = TEEC_ERROR_GENERIC;
			(void)printf("Unsupported mode %s\n", argv[2]);
		}
	}
	
	/* Get the stat of the file. */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		if (stat(argv[3], &st) != 0) {
			res = TEEC_ERROR_GENERIC;
			(void)printf("File access error %s\n", argv[3]);
		}
	}
	
	/* Check the file size. */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		if ((st.st_size % 16U) != 0U || (st.st_size > 262144U)) {
			res = TEEC_ERROR_GENERIC;
			(void)printf("File size is not a multiple of 16 bytes. Or file size is larger than 262144 bytes %s\n", argv[3]);
		}
	}
	
	/* Allocates a buffer. */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		input_SharedMemory.size = st.st_size;
		input_SharedMemory.flags = TEEC_MEM_INPUT;
		res = TEEC_AllocateSharedMemory(&ctx, &input_SharedMemory);
		if (res != (TEEC_Result)TEEC_SUCCESS) {
			(void)printf("TEEC_AllocateSharedMemory error 0x%08x\n", res);
		} else {
			input_buf = input_SharedMemory.buffer;
		}
	}
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		output_buf = malloc((size_t)st.st_size);
		if (output_buf == NULL) {
			res = TEEC_ERROR_GENERIC;
			(void)printf("Memory allocate error\n");
		}
	}
	
	/* Input file */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		fp_in = fopen(argv[3], "rb");
		if (fp_in == NULL) {
			res = TEEC_ERROR_GENERIC;
			(void)printf("fopen error in_file=%s\n", argv[3]);
		} else {
			readSize = fread(input_buf, 1U, (size_t)st.st_size, fp_in);
			(void)fclose(fp_in);
			
			if (readSize != st.st_size) {
				res = TEEC_ERROR_GENERIC;
				(void)printf("File read error %ld\n", readSize);
			}
		}
	}
	
	/* Invoke command. */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		(void)memset(&op, 0, sizeof(op));
		op.paramTypes = (uint32_t)TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_MEMREF_PARTIAL_INPUT, TEEC_MEMREF_TEMP_OUTPUT);
			op.params[0].value.a       = algo;
			op.params[1].value.a       = mode;
			op.params[2].memref.offset = 0U;
			op.params[2].memref.size   = readSize;
			op.params[2].memref.parent = &input_SharedMemory;
			op.params[3].tmpref.buffer = (void*)output_buf;
			op.params[3].tmpref.size   = readSize;
		
		res = TEEC_InvokeCommand(&sess, (uint32_t)E_TEEC_CMD_AES, &op, &err_origin);
		if (res != (TEEC_Result)TEEC_SUCCESS) {
			(void)printf("TEEC_InvokeCommand failed with code 0x%08x origin 0x%08x\n", res, err_origin);
		}
	}
	
	/* Output file */
	if (res == (TEEC_Result)TEEC_SUCCESS) {
		fp_out = fopen(argv[4], "wb");
		if (fp_out == NULL) {
			(void)printf("fopen error out_file=%s\n", argv[4]);
		} else {
			writeSize = fwrite(output_buf, 1U, op.params[3].tmpref.size, fp_out);
			(void)fclose(fp_out);
			
			if (writeSize != op.params[3].tmpref.size) {
				(void)printf("File write error %ld\n", writeSize);
			}
		}
	}
	/* Release */
	TEEC_ReleaseSharedMemory(&input_SharedMemory);
	myFree(output_buf);
	
	/*
	 * We're done with the TA, close the session and
	 * destroy the context.
	 */
	TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);

	return 0;
}
