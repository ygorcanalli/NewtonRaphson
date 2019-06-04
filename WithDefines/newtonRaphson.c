#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gnuplot_i.h"
#include "variables.h"

#define MAX_STRING 80
#define SLEEP_LGTH  2

double newtonRaphson(double p0, double TOL, int N, int plot);
double doubleAbs(double a);
char* linePlotStringPoly(double x0, double y0, double x1, double y1);
double charToDouble(char* doubleInChar);
char* doubleToStringPoly(double* poly, int order);

int main(int argc, char *argv[])
{

	double zero;	
	double tol;
	double p0;
	int n;
	int plot;

	p0 = charToDouble(argv[1]);
	tol = charToDouble(argv[2]);
	n = (int) strtoul(argv[3], 0, 10);
	plot = (int) strtoul(argv[4], 0, 10);

	zero = newtonRaphson(p0, tol, n, plot);
	printf("\n\nf(x) = 0; x = %f\n", zero);


	return 0;
}

double doubleAbs(double a)
{
	return (a < 0) ? (-1*a) : a;
}

double newtonRaphson(double p0, double TOL, int N, int plot)
{
	int i = 0;
	double p = 0;
	double f = 0;
	double fLinha = 0;
	char* functLine;
	gnuplot_ctrl *h1;


	if (plot)
	{
	    h1 = gnuplot_init();
		gnuplot_plot_slope(h1, 1.0, 0.0, "f(x)") ;
		gnuplot_plot_slope(h1, 2.0, 0.0, "f'(x)") ;

		gnuplot_setstyle(h1, "lines");
		gnuplot_cmd(h1, "set style line 1 lt 2 lw 15");
		
		gnuplot_resetplot(h1);
    	gnuplot_plot_equation(h1, SFUNCTION(x), "f(x)");
	    //sleep(SLEEP_LGTH);
	}
	
	while (i < N)
	{

		f =  FUNCTION(p0);
		fLinha =  DIFFUNCTION(p0);
		p = p0 - (f/fLinha);


		if (plot)
		{
			functLine = linePlotStringPoly(p0, f, p, 0.00);
			gnuplot_setstyle(h1, "lines");
			gnuplot_plot_equation(h1, functLine, "");
			sleep(SLEEP_LGTH);
		}

		if (doubleAbs(p - p0) < TOL)
		{
			if (plot)
			{
				gnuplot_resetplot(h1);
				gnuplot_cmd(h1, "set label '(%f,0)' at %f,%f point pointtype 2",p, p, 0.00);			
		    	gnuplot_plot_equation(h1, SFUNCTION(x), "f(x)");
				getchar();
				gnuplot_close(h1);			
			}
			printf("\nSolucao encontrada em %d iteracoes", i+1);
			return p;	
		}
		
		printf("\n%f", p);

		p0 = p;
		i++;
	}
	printf("\nLimite de %d iterecoes atingido. Nao foi possivel determinar o zero da funcao!\n", N);

	getchar();
	if (plot)
		gnuplot_close(h1);

	return p;
}

//return f(x) = ax + b
char* linePlotStringPoly(double x0, double y0, double x1, double y1)
{
		char* line;
		double* functDouble = (double*) malloc (sizeof(double) * 2);
		
		functDouble[1] = (y1 - y0)/ (x1 - x0); //a
		functDouble[0] = y1 - (functDouble[1]*x1); //b

		line = doubleToStringPoly(functDouble, 2);
		
		return line;
}


char* doubleToStringPoly(double* poly, int order)
{
	int i;
	char aux[MAX_STRING];
	char* result;

	result = (char*) malloc (sizeof(char) * MAX_STRING * (order + 1));

	for(i = 0; i < order; i++)
	{
		sprintf(aux, "%f *(x**%d) + " , poly[i], i);
		strcat(result, aux);
	}

	sprintf(aux, "%f *(x**%d)" , poly[i], i);
	strcat(result, aux);

	return result;
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
