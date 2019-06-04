#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gnuplot_i.h"

#define MAX_STRING 80
#define SLEEP_LGTH  2

#include "variables.h"

double bissecao(double a, double b, double TOL, int N, int plot);
double doubleAbs(double a);
double charToDouble(char* doubleInChar);

int main(int argc, char *argv[])
{

	double zero;	
	double tol;
	double a,b;
	int n;
	int plot;

	a = charToDouble(argv[1]);
	b = charToDouble(argv[2]);
	tol = charToDouble(argv[3]);
	n = (int) strtoul(argv[4], 0, 10);
	plot = (int) strtoul(argv[5], 0, 10);
	
	zero = bissecao(a, b, tol, n, plot);
	printf("\n\nf(x) = 0; x = %f\n", zero);


	return 0;
}

double doubleAbs(double a)
{
	return (a < 0) ? (-1*a) : a;
}

double bissecao(double a, double b, double TOL, int N, int plot)
{
	int i = 0;
	double p = 0;
	double FA = 0;
	double FP = 0;
	gnuplot_ctrl *h1;
	
	if (plot)
	{
		h1 = gnuplot_init();	
		gnuplot_plot_slope(h1, 1.0, 0.0, "f(x)") ;

		gnuplot_setstyle(h1, "lines");
		gnuplot_cmd(h1, "set style line 1 lt 2 lw 15");
		
	}
	
	FA =  FUNCTION(a);

	while (i < N)
	{
		p = a + (b-a)/2;
		FP =  FUNCTION(p);
	
		if ((FP == 0) || ((b-a)/2 < TOL))
		{
			if (plot)
			{
				gnuplot_resetplot(h1);
				gnuplot_cmd(h1, "set label '(%f,0)' at %f,%f point pointtype 2",p, p, 0.00);			
	    		gnuplot_plot_equation(h1, SFUNCTION(p), "f(x)");
				getchar();
				gnuplot_close(h1);			
			}
			printf("\nSolucao encontrada em %d iteracoes", i+1);
			return p;
		}

		printf("\n%f", p);

		if (FA*FP > 0)
		{
			a = p;
			FA = FP;
		}
		else
			b = p;
		i++;
	}
	printf("\nLimite de %d iterecoes atingido. Nao foi possivel determinar o zero da funcao!\n", N);

	getchar();
	if (plot)
		gnuplot_close(h1);

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

