#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gnuplot_i.h"

#define MAX_STRING 80
#define SLEEP_LGTH  2


double polyValue(double* poly, int order, double value);
double* polyDerivative(double* poly, int order);
double pontoFixo(double* poly, int polyOrder, double p0, double TOL, int N);
double doubleAbs(double a);
char* linePlotStringPoly(double x0, double y0, double x1, double y1);
double charToDouble(char* doubleInChar);
double* stringToDoublePoly(char* poly, int order);
char* doubleToStringPoly(double* poly, int order);

int main(int argc, char *argv[])
{

	double fixedPoint;	
	double *poly;
	double tol;
	double p0;
	int order;
	int n;

	order = (int) strtoul(argv[2], 0, 10);
	p0 = charToDouble(argv[3]);
	tol = charToDouble(argv[4]);
	n = (int) strtoul(argv[5], 0, 10);

	poly = stringToDoublePoly(argv[1], order);
	
	fixedPoint = pontoFixo(poly, order, p0, tol, n);
	printf("\n\nf(x) = x = %f\n", fixedPoint);


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

double pontoFixo(double* poly, int polyOrder, double p0, double TOL, int N)
{
	int i = 0;
	double p = 0;
	
	while (i < N)
	{

		p = polyValue(poly, polyOrder, p0);

		if (doubleAbs(p - p0) < TOL)
		{
			printf("\nPonto fixo encontrado em %d iteracoes", i+1);
			return p;	
		}

		p0 = p;
		i++;
	}
	printf("\nLimite de %d iterecoes atingido. Nao foi possivel encontrar um ponto fixo da funcao!\n", N);

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
                       i++; //ignora espaço vazio

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
