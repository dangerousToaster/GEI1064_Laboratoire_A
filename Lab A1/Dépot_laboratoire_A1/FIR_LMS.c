
/******************************************************************************
 * INCLUDES
******************************************************************************/
#include <stdbool.h>
#include "FIR_LMS.h"



/******************************************************************************
 * LMS
 *
 * @param[in]		input 			input signal
 * @param[in]   	inputEstimate	estimation of the input signal
 * @param[inout]	w				weights to adjust
 *
 * @warning     none
 ******************************************************************************/
void LMS(float *input,
		 float *inputEstimate,
		 float *yn_input,
		 float *w,
		 float mu)
{
	float error = *input - *inputEstimate;

	/* adjust weights */
	int i;
	weights_loop: for(i = 0; i < CHANNEL_ORDER; i++)
	{
		w[i] = w[i] + (mu * error * yn_input[i]);
	}
}
/* End of LMS() */
/******************************************************************************/



/******************************************************************************
 * FIR_LMS
 *
 * @param[out]	inputEstimate 	estimation of the input signal
 * @param[in]   inputNoise		noised input signal
 * @param[in] 	mu				adaptation step
 * @param[in] 	input			clean input
 * @param[in]	isTraining		flag to train the filter
 *
 * @warning     none
 ******************************************************************************/
void FIR_LMS(float *inputEstimate,
			 float *inputNoise,
			 float mu,
			 float *input,
			 bool isTraining)
{
	static float w[CHANNEL_ORDER] = {0};
	static float yn_input[CHANNEL_ORDER] = {0};
	float error = 0;

	int i;

	/* shift values inside yn_input[] */
	shift_loop: for(i = (CHANNEL_ORDER - 1u); i > 0; i--)
	{
		yn_input[i] = yn_input[i - 1u];
	}
	yn_input[0] = *inputNoise;

	/* compute the input's estimate */
	compute_loop: for(i = 0; i < CHANNEL_ORDER; i++)
	{
		*inputEstimate += w[i] * yn_input[i];
	}

	/* compute error */
	error = *input - *inputEstimate;

	/* train weights if filter is in training session */
	if (isTraining)
	{
		LMS(input, inputEstimate, yn_input, w, mu);
	}
}
/* End of FIR_LMS() */
/******************************************************************************/
