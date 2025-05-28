// #include <err.h>
#include <stdio.h>
#include <string.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* For the UUID (found in the TA's h-file(s)) */
#include <icumfw_trng_ta.h>

int main(void)
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_UUID uuid = TA_ICUMFW_TRNG_UUID;
	uint32_t err_origin;

	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		(void)printf("TEEC_InitializeContext failed with code 0x%x", res);

	/* Open a session to the "ICUMFW TRNG" TA */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		(void)printf("TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);
	/* Invoking TA */
	(void)printf("Invoking TA to generate true random number\n");
	res = TEEC_InvokeCommand(&sess, 0, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		(void)printf("TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);

	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);

	return 0;
}
