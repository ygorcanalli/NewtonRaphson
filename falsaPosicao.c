#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gnuplot_i.h"

#define MAX_STRING 80
#define SLEEP_LGTH  2


double polyValue(double* poly, int order, double value);
double falsaPosicao(double* poly, int polyOrder, double p0,  double p1, double TOL, int N, int plot);
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
	double p0;
	double p1;
	int order;
	int n;
	int plot;

	order = (int) strtoul(argv[2], 0, 10);
	p0 = charToDouble(argv[3]);
	p1 = charToDouble(argv[4]);
	tol = charToDouble(argv[5]);
	n = (int) strtoul(argv[6], 0, 10);
	plot = (int) strtoul(argv[7], 0, 10);

	poly = stringToDoublePoly(argv[1], order);
	
	zero = falsaPosicao(poly, order, p0, p1, tol, n, plot);
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


double falsaPosicao(double* poly, int polyOrder, double p0, double p1, double TOL, int N, int plot)
{
	int i;
	double q0;
	double q1;
	double p;
	double q;

	char* functLine;
	gnuplot_ctrl *h1;

	i = 1;
	q0 =  polyValue(poly, polyOrder, p0);
	q1 =  polyValue(poly, polyOrder, p1);

	if (plot)
	{
	    h1 = gnuplot_init();
		gnuplot_plot_slope(h1, 1.0, 0.0, "f(x)") ;
		gnuplot_plot_slope(h1, 2.0, 0.0, "f'(x)") ;

		gnuplot_setstyle(h1, "lines");
		gnuplot_cmd(h1, "set style line 1 lt 2 lw 15");
		
		gnuplot_resetplot(h1);
    	gnuplot_plot_equation(h1, doubleToStringPoly(poly, polyOrder), "f(x)");
	}
	
	while (i < N)
	{

		p = p1 - (q1*(p1 - p0)/(q1- q0));
		//printf("\n\n%f %f %f %f %f", p, p0, q0, p1, q1);

		if (plot)
		{
			functLine = linePlotStringPoly(p0, q0, p1, q1);
			gnuplot_setstyle(h1, "lines");
			gnuplot_plot_equation(h1, functLine, "");
			sleep(SLEEP_LGTH);
		}

		if (doubleAbs(p - p1) < TOL)
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

		q = polyValue(poly, polyOrder, p);

		if ((q * q1) < 0)
		{
			p0 = p1;
			q0 = q1;
		}

		p1 = p;
		q1 = q; //q1 = q

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
