
#ifndef FIR__LMS_H_
#define FIR_LMS_H_

/******************************************************************************
 * INCLUDES
******************************************************************************/
#include <ap_fixed.h>



/******************************************************************************
 * DEFINES
******************************************************************************/
#define N						2000	/* total number of input data points */
#define N_TRAIN 				200		/* total number of training points */
#define N_FAST_CONVERGENCE		19		/* number of iteration made with greater mu */
#define CHANNEL_ORDER 			5		/* order of the channel */

#define FIR_DATA_WIDTH			11		/* data total width */
#define FIR_DATA_INT_BITS		1		/* data integer width */

#define FIR_WEIGHTS_WIDTH		14		/* weights total width */
#define FIR_WEIGHTS_INT_BITS	2		/* weights integer width */

#define FIR_MU_WIDTH			14		/* mu total width */
#define FIR_MU_INT_BITS			2		/* mu integer width */

#define FIR_BOOL_WIDTH			1		/* boolean total width */
#define FIR_BOOL_INT_BITS		1		/* boolean integer width */

#define FIR_INTEGER_WIDTH		4		/* integer total width */
#define FIR_INTEGER_INT_BITS	4		/* integer total width */



/******************************************************************************
 * TYPE DEFINITIONS
******************************************************************************/
typedef ap_fixed<FIR_DATA_WIDTH, FIR_DATA_INT_BITS> 		T_FIR_LMS_data;
typedef ap_fixed<FIR_WEIGHTS_WIDTH,	FIR_WEIGHTS_INT_BITS> 	T_FIR_LMS_weights;
typedef ap_fixed<FIR_MU_WIDTH, FIR_MU_INT_BITS> 			T_FIR_LMS_mu;
typedef ap_fixed<FIR_BOOL_WIDTH, FIR_BOOL_INT_BITS> 		T_FIR_LMS_bool;
typedef ap_fixed<FIR_INTEGER_WIDTH, FIR_INTEGER_INT_BITS> 	T_FIR_LMS_int;



/******************************************************************************
 * FUNCTION DECLARATIONS
******************************************************************************/
void LMS(T_FIR_LMS_data *input,
		 T_FIR_LMS_data *inputEstimate,
		 T_FIR_LMS_data *yn_input,
		 T_FIR_LMS_weights *w,
		 T_FIR_LMS_mu mu);

void FIR_LMS(T_FIR_LMS_data *inputEstimate,
			 T_FIR_LMS_data *inputNoise,
			 T_FIR_LMS_weights mu,
			 T_FIR_LMS_data *input,
			 T_FIR_LMS_bool isTraining);



#endif
