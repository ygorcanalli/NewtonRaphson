#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gnuplot_i.h"

#define MAX_STRING 80
#define SLEEP_LGTH  2


double polyValue(double* poly, int order, double value);
double* polyDerivative(double* poly, int order);
double bissecao(double* poly, int polyOrder, double a, double b, double TOL, int N, int plot);
double doubleAbs(double a);
char* linePlotStringPoly(double x0, double y0, double x1, double y1);
double charToDouble(char* doubleInChar);
double* stringToDoublePoly(char* poly, int order);
char* doubleToStringPoly(double* poly, int order);

int main(int argc, char *argv[])
{

	double zero;	
	double *poly;
	double tol;
	double a,b;
	int order;
	int n;
	int plot;

	order = (int) strtoul(argv[2], 0, 10);
	a = charToDouble(argv[3]);
	b = charToDouble(argv[4]);
	tol = charToDouble(argv[5]);
	n = (int) strtoul(argv[6], 0, 10);
	plot = (int) strtoul(argv[7], 0, 10);

	poly = stringToDoublePoly(argv[1], order);
	
	zero = bissecao(poly, order, a, b, tol, n, plot);
	printf("\n\nf(x) = 0; x = %f\n", zero);


	return 0;
}

double doubleAbs(double a)
{
	return (a < 0) ? (-1*a) : a;
}

double polyValue(double* poly, int order, double value)
{
	int i;
	double sum  = 0;
	for (i = 0; i < order+1; i++)
		sum += poly[i]*pow(value,(double) i);
	return sum;
}

double* polyDerivative(double* poly, int order)
{
	int i;
	double* derivative = malloc(order*sizeof(double));
	for (i = 0; i < order; i++)
		derivative[i] = (i+1)*poly[i+1];	
	derivative[order] = 0;
	return derivative;
}

double bissecao(double* poly, int polyOrder, double a, double b, double TOL, int N, int plot)
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
	
	FA =  polyValue(poly, polyOrder, a);

	while (i < N)
	{
		p = a + (b-a)/2;
		FP =  polyValue(poly, polyOrder, p);
	
		if ((FP == 0) || ((b-a)/2 < TOL))
		{
			if (plot)
			{
				gnuplot_resetplot(h1);
				gnuplot_cmd(h1, "set label '(%f,0)' at %f,%f point pointtype 2",p, p, 0.00);			
		    		gnuplot_plot_equation(h1, doubleToStringPoly(poly, polyOrder), "f(x)");
				getchar();
				gnuplot_close(h1);			
			}
			printf("\nSolucao encontrada em %d iteracoes", i+1);
			return p;
		}

		if (plot)
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


double* stringToDoublePoly(char* poly, int order)
{
       double* polyInDouble;
       
       int i;
       int j;
       int beginVector;

       polyInDouble = (double*) malloc (sizeof(double) * (order+1));

       i = beginVector = j = 0;

       while (poly[i] != '\0')
       {
               if (poly[i] == ',')
               {
                       poly[i] = '\0';

                       polyInDouble[j] = charToDouble(poly + beginVector);
                       i++; //ignora espaco vazio

                       beginVector = i + 1;
                       j++; //quantidade de virgulas
               }

               i++;
       }

       polyInDouble[j] = charToDouble(poly + beginVector);

       return polyInDouble;
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
