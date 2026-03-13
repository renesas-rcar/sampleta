#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <rsipm_trng_ta.h>

#define RANDOM_SIZE_1 16
#define RANDOM_SIZE_2 25
#define RANDOM_SIZE_3 32

void generate_and_show_trng( uint8_t* buf, uint32_t buf_len);

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	IMSG("RSIPM TEST: True Random Number Generaion service!\n");

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	IMSG("Goodbye!\n");
}

void generate_and_show_trng( uint8_t* buf, uint32_t buf_len)
{
	IMSG("Generate an true random number with %ld bytes:\n", buf_len);

	/* Prepare buffer */
	TEE_MemFill(buf, 0, buf_len);

	/* Generate true random value */
	TEE_RSIPM_TRNG(buf, buf_len);

	/* Show true random value */
	for(uint32_t i = 0; i < buf_len; i++) {
		MSG("random_number[%d] = 0x%x\n", i, buf[i]);
	}
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id __attribute__((unused)),
			uint32_t param_types __attribute__((unused)), TEE_Param params[4] __attribute__((unused)))
{
	(void)&sess_ctx; /* Unused parameter */

	uint8_t	random_number1[RANDOM_SIZE_1];
	uint8_t random_number2[RANDOM_SIZE_2];
	uint8_t random_number3[RANDOM_SIZE_3];

	IMSG("================== START TEST TRNG RSIPM ==================\n");
	generate_and_show_trng(random_number1, sizeof(random_number1));

	generate_and_show_trng(random_number2, sizeof(random_number2));

	generate_and_show_trng(random_number3, sizeof(random_number3));

	IMSG("=================== END TEST TRNG RSIPM =====================\n");

	return TEE_SUCCESS;
}
