
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
 * @param[in]		yn_input		noised input
 * @param[inout]	w				weights to adjust
 * @param[inout]	mu				adaptation step
 *
 * @warning     none
 ******************************************************************************/
void LMS(T_FIR_LMS_data *input,
		 T_FIR_LMS_data *inputEstimate,
		 T_FIR_LMS_data *yn_input,
		 T_FIR_LMS_weights *w,
		 T_FIR_LMS_mu mu)
{
	T_FIR_LMS_data error = *input - *inputEstimate;

	/* adjust weights */
	T_FIR_LMS_int i;
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
void FIR_LMS(T_FIR_LMS_data *inputEstimate,
			 T_FIR_LMS_data *inputNoise,
			 T_FIR_LMS_mu mu,
			 T_FIR_LMS_data *input,
			 T_FIR_LMS_bool isTraining)
{
	static T_FIR_LMS_weights w[CHANNEL_ORDER] = {0};
	static T_FIR_LMS_data yn_input[CHANNEL_ORDER] = {0};

	T_FIR_LMS_data error = 0;
	T_FIR_LMS_int i;

	/* shift values inside yn_input[] */
	shift_loop: for(i = (CHANNEL_ORDER - 1u); i > 0; i--)
	{
		yn_input[i] = yn_input[i - 1u];
	}
	yn_input[0] = *inputNoise;

	/* compute the input's estimate */
	*inputEstimate = 0;
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
