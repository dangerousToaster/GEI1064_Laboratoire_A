#include "LMS.h"

void fir (
		out_t *y,
		coef_t c[N],
		in_t *x,
		in_t input[N])
{
	static in_t shift_reg[N] = {0};
	out_t acc;
	in_t data;
	int i;

	acc=0;
	Shift_Accum_Loop: for (i=N-1;i>0;i--)
	{
		shift_reg[i]=shift_reg[i-1];
		input[i] = shift_reg[i];
		data = shift_reg[i];
		acc+=data*c[i];
	}

	shift_reg[0]=*x;
	input[0] = shift_reg[0];
	data = *x;
	acc+=data*c[0];

	*y=acc;
}

void LMS(out_t y,
		in_t ref,
		in_t mhu,
		in_t shift_reg[N],
		coef_t updated_c[N])
{
	in_t e;
	static coef_t c[N];

	e = ref-y;

	for(int i = 0;i<N;++i)
	{
		c[i] = c[i]+mhu*e*shift_reg[i];
		updated_c[i] = c[i];
	}
}

void fir_LMS(in_t *x,
		out_t *y,
		in_t mhu,
		in_t ref,
		int nbTrain)
{
	in_t input[N];
	static coef_t updated_c[N] = {0};
	static int train_count = 0;
	out_t ycpy;

	fir(&ycpy,updated_c,x,input);

	if(train_count < nbTrain)
	{
		LMS(ycpy,ref,mhu,input,updated_c);
		++train_count;
	}

	*y = ycpy;
}
