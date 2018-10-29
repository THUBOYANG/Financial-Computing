#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//
// Space for a standard Brownian motion realization 
//
//    B[t[i]], i=0,...,10000
//
// and time points t[i]=0,.0001,.0002,...,1.0000
//
double t[10001],B[10001],X[10001];
//
// Function to load a new realization.
// Note that the function doesn't take arguments and doesn't return anything.
//
void BrownianWithDriftAndVolatilityRealization (double mu, double sigma)
{
    int i,rnumber;
    double delta, sqrtdelta;
    delta=1./10000.; // time increment
    sqrtdelta=sqrt(delta); // position increment
    B[0]=0.;
    t[0]=0.;
    X[0]=0.;
    for (i=1;i<=10000;i++)
    {
        t[i]=((double)i)*delta;
        rnumber=rand();
        if (rnumber>RAND_MAX/2)
        {
            B[i]=B[i-1]+sqrtdelta;
        }
        else
        {
            B[i]=B[i-1]-sqrtdelta;
        }
        X[i]=mu*t[i]+sigma*B[i];
    }
    return;
}

int index_of_upper_exceedance(int I1, int I2, double threshold) 
{
    int index = 0;
    for (int i=I1;i<=I2;i++)
    {
        if (X[i]>=threshold)
        {
            index = i;
            break;
        }
    }
    return index;
}

int index_of_lower_exceedance(int I1, int I2, double threshold)
{
    int index = 0;
    for (int i=I1;i<=I2;i++)
    {
        if (X[i]<=threshold)
        {
            index = i;
            break;
        }
    }
    return index;
}

int main(int argc, char** argv) 
{
    int ntrials=10000, trial, ind, ctr, index1, index2;
  
    ctr=0;
    for (trial=0;trial<ntrials;trial++)
    {
        BrownianWithDriftAndVolatilityRealization(0.1,2.0);
        ind=0;
        index1 = index_of_upper_exceedance(0,10000,log(2));
        index2 = index_of_lower_exceedance(index1,10000,0);
        if (index2)
        {
            ind = 1;
        }
        ctr+=ind; 
    }
    printf("prob est = %lf\n", (double)ctr/(double)ntrials);
    
    return (EXIT_SUCCESS);
}
