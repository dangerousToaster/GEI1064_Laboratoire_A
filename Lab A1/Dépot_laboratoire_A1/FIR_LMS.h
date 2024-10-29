
#ifndef FIR__LMS_H_
#define FIR_LMS_H_



/******************************************************************************
 * DEFINES
******************************************************************************/
#define N					2000	/* total number of input data points */
#define N_TRAIN 			200		/* total number of training points */
#define N_FAST_CONVERGENCE	19		/* number of iteration made with mu = (mu * 2) */
#define CHANNEL_ORDER 		5		/* order of the channel */



/******************************************************************************
 * FUNCTION DECLARATIONS
******************************************************************************/
void LMS(float *input,
		 float *inputEstimate,
		 float *yn_input,
		 float *w,
		 float mu);

void FIR_LMS(float *inputEstimate,
			 float *inputNoise,
			 float mu,
			 float *input,
			 bool isTraining);



#endif
