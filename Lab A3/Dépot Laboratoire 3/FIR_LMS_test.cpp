#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "FIR_LMS.h"
using namespace std;

#define nbData 10

int main (void)
{
	ifstream Fin;
	ifstream Fref;
	ifstream FoutMatlab;
	ofstream FoutHLS;

	int nbErr = 0;
	int nbTrain = 300;
	double epsilon = 0.01;
	coef_t mhu = 0.25;

	in_t input[nbData] = {0.298800, 0.313165, -0.352435, 0.227485, 0.397035, -0.358665, 0.222665, 0.397035, 0.238935, 0.251395};
	in_t ref[nbData] = {0.500000, 0.500000, -0.500000, 0.500000, 0.500000, -0.500000, 0.500000, 0.500000, 0.500000, 0.500000};
	out_t outputHLS[nbData];
	double outputMatlab[nbData] = {0.000000, 0.011697, -0.015214, 0.016090, 0.058589, -0.098622, 0.071088, 0.127212, -0.039312, 0.079047};



	////////////////////////////////////////////////
	for(int i = 0;i<nbData;++i)
	{
		fir_LMS(&input[i],&outputHLS[i],mhu,ref[i],nbTrain);
	}
	///////////////////////////////////////////////

	FoutHLS.open("outHLS.txt");

	FoutHLS << fixed << setbase(10) << setprecision(18);

	for (int i = 0; i < nbData; ++i)
	{
		FoutHLS << outputHLS[i];
		FoutHLS << "\n";
	}

	FoutHLS.close();

	printf ("Comparaison des solutions HLS VS Matlab \n");

	for(int i=0;i<nbData;++i){
		fprintf(stdout, "%lf    %lf\n",double(outputHLS[i]),  outputMatlab[i]);
		if(fabs(double(outputHLS[i])-outputMatlab[i]) > epsilon){
			++nbErr;
			fprintf(stdout, "*******************************************\n");
			fprintf(stdout, "Erreur trouvee->total: %d\n",nbErr);
			fprintf(stdout, "*******************************************\n");
		}
	}

	if(nbErr == 0)
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "Reussite: Les resultats sont acceptables !!!\n");
		fprintf(stdout, "*******************************************\n");
	}

	else
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "echec: %d erreurs ont ete trouvees\n",nbErr);
		fprintf(stdout, "*******************************************\n");
	}

	return nbErr;
}



