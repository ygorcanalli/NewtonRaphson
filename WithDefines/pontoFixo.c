#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gnuplot_i.h"
#include "variables.h"

#define MAX_STRING 80
#define SLEEP_LGTH  2


double pontoFixo(double p0, double TOL, int N);
double doubleAbs(double a);
double charToDouble(char* doubleInChar);

int main(int argc, char *argv[])
{

	double fixedPoint;	
	double tol;
	double p0;
	int n;

	p0 = charToDouble(argv[1]);
	tol = charToDouble(argv[2]);
	n = (int) strtoul(argv[3], 0, 10);
	
	fixedPoint = pontoFixo(p0, tol, n);
	printf("\n\nf(x) = x = %f\n", fixedPoint);


	return 0;
}

double doubleAbs(double a)
{
	return (a < 0) ? (-1*a) : a;
}

double pontoFixo(double p0, double TOL, int N)
{
	int i = 0;
	double p = 0;
	
	while (i < N)
	{

		p = FUNCTION(p0);

		if (doubleAbs(p - p0) < TOL)
		{
			printf("\nPonto fixo encontrado em %d iteracoes", i+1);
			return p;	
		}
			
		printf("\n%f", p);
		p0 = p;
		i++;
	}
	printf("\nLimite de %d iterecoes atingido. Nao foi possivel encontrar um ponto fixo da funcao!\n", N);

	return p;
}

double charToDouble(char* doubleInChar)
{
       int i = 0;
       int j = 1;
       int signal = 1;
       double integer = 0;
       double decimal = 0;
       
       if (doubleInChar == NULL)
               return 0;


       if (doubleInChar[0] == '-') //Se o numero for negativo
       {
               signal = -1;
               i = 1;  
       }

       //Define a parte inteira
       while ((doubleInChar[i] != '\0') && (doubleInChar[i] != ',') && (doubleInChar[i] != '.'))
       {               
               integer = (integer * 10) + (doubleInChar[i] - 48);
               i++;
       }       

               if (doubleInChar[i] != '\0')
               i++; //pula a virgula

       //Define a parte decimal
       while (doubleInChar[i] != '\0')
       {
               decimal += (doubleInChar[i] - 48) / pow(10, j);
               i++;
               j++;
       }
       
       return signal*(integer + decimal);
}
