#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "LMS.h"
using namespace std;



int main (void)
{
	ifstream Fin;
	ifstream Fref;
	ifstream FoutMatlab;
	ofstream FoutHLS;

	int nbErr = 0;
	int nbTrain = 300;
	int nbData = 2000;
	double epsilon = 0.01;
	coef_t mhu = 0.25;

	in_t input[nbData];
	in_t ref[nbData];
	out_t outputHLS[nbData];
	double outputMatlab[nbData];

	Fin.open("yn.txt");

	for (int i = 0; i < nbData; ++i)
	{
		Fin >> input[i]; // conversion implicite de double vers point fixe
	}

	Fin.close();

	Fref.open("inp.txt");

	for (int i = 0; i < nbData; ++i)
	{
		Fref >> ref[i]; // conversion implicite de double vers point fixe
	}

	Fref.close();

	FoutMatlab.open("inpest.txt");

	for (int i = 0; i < nbData; ++i)
	{
		FoutMatlab >> outputMatlab[i];
	}

	FoutMatlab.close();

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
		if(fabs(double(outputHLS[i])-outputMatlab[i]) > epsilon){
			++nbErr;
			fprintf(stdout, "*******************************************\n");
			fprintf(stdout, "Erreur trouvée->total: %d\n",nbErr);
			fprintf(stdout, "*******************************************\n");
		}
	}

	if(nbErr == 0)
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "Réussite: Les résultats sont acceptables !!!\n");
		fprintf(stdout, "*******************************************\n");
	}

	else
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "Échec: %d erreurs ont été trouvées\n",nbErr);
		fprintf(stdout, "*******************************************\n");
	}

	return nbErr;
}



