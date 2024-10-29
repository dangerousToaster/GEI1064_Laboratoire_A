#ifndef LMS_H_
#define LMS_H_

#define N	5

#include <ap_fixed.h>

//#define FLOATING

#define W_IN    32
#define IW_IN   1
#define W_OUT   32
#define IW_OUT  1
#define W_COEF  32
#define IW_COEF 2

#ifdef FLOATING
typedef float in_t;
typedef float out_t;
typedef float coef_t;
#else
typedef ap_fixed<W_IN,IW_IN> in_t;
typedef ap_fixed<W_OUT,IW_OUT> out_t;
typedef ap_fixed<W_COEF,IW_COEF> coef_t;
#endif

void fir (
		out_t *y,
		coef_t c[N],
		in_t *x,
		in_t input[N]);

void LMS(out_t y,
		in_t ref,
		in_t mhu,
		in_t shift_reg[N],
		coef_t updated_c[N]);

void fir_LMS(in_t *x,
		out_t *y,
		in_t mhu,
		in_t ref,
		int nbTrain);

#endif
