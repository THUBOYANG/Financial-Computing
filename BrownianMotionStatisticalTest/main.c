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
double t[1000001],B[1000001],X[1000001];
//
// Function to load a new realization.
// Note that the function doesn't take arguments and doesn't return anything.
//
void PriceRealization (double theta, double mu, double sigma)
{
    int i,rnumber;
    double delta, sqrtdelta;
    delta=1./10000.; // time increment
    sqrtdelta=sqrt(delta); // position increment
    B[0]=0.;
    t[0]=0.;
    X[0]=100.;
    for (i=1;i<=1000000;i++)
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
        X[i]=X[i-1]+theta*(mu-X[i-1])*delta+sigma*(B[i]-B[i-1]);
    }
    return;
}

int compare_doubles(const void *t1, const void *t2)
{
    double *p1, *p2;

    p1=(double *)t1; // cast the first pointer
    p2=(double *)t2; // cast the second pointer
    // now dereference to get the values
    if (*p1<*p2) return(-1);

#if 0
    // this doesn't work!!!
    if (*t1<*t2) return(-1);
#endif
    return(1);
}

int main(int argc, char** argv) 
{
    // question 1
    int ntrials = 1000,Ncount,i,j,signal;
    double sum25=0,average25=0,std25=0,
            sum50=0,average50=0,std50=0,
            sum75=0,average75=0,std75=0,
            sum100=0,average100=0,std100=0,
            sumvar25=0,sumvar50=0,sumvar75=0,sumvar100=0,
            summax=0,summin=0,sumindex=0,summaxvar=0,sumncount=0,sumnvar=0,
            leftbound,rightbound;
    double X25[ntrials],X50[ntrials],X75[ntrials],X100[ntrials],
            max[ntrials],min[ntrials];
    int nmax[ntrials],nmin[ntrials],N[ntrials];
    
    for (i=0;i<ntrials;i++)
    {
        PriceRealization(1.,100.,10.);
        X25[i] = X[250000];
        sum25 += X25[i];
        X50[i] = X[500000];
        sum50 += X50[i];
        X75[i] = X[750000];
        sum75 += X75[i];
        X100[i] = X[1000000];
        sum100 += X100[i];
        
        max[i] = X[0];
        min[i] = X[0];
        nmax[i] = 0;
        nmin[i] = 0;
        Ncount = 0;
        signal = 0;
        for (j=1;j<=1000000;j++)
        {
            //for question 3-4
            if (X[j]>max[i]) 
            {
                max[i] = X[j];
                nmax[i] = j;
            }
            if (X[j]<min[i]) 
            {
                min[i] = X[j];
                nmin[i] = j;
            }
            
            // for question 5

            if (X[j]>=110.)
            {
                signal = 1;
            }
            else if (X[j]<=90. && signal == 1)
            {
                Ncount++;
                signal = 0;
            }
        }
        N[i] = Ncount;
    }
    average25 = sum25 / (double)ntrials;
    average50 = sum50 / (double)ntrials;
    average75 = sum75 / (double)ntrials;
    average100 = sum100 / (double)ntrials;
    printf("The estimated expected value of the price at time 25 = %f\n", average25);
    printf("The estimated expected value of the price at time 50 = %f\n", average50);
    printf("The estimated expected value of the price at time 75 = %f\n", average75);
    printf("The estimated expected value of the price at time 100 = %f\n", average100);
    for (i=0;i<ntrials;i++)
    {
        sumvar25 += pow((X25[i]-average25),2);
        sumvar50 += pow((X50[i]-average50),2);
        sumvar75 += pow((X75[i]-average75),2);
        sumvar100 += pow((X100[i]-average100),2);                
    }
    printf("The estimated standard deviation of the price at time 25 = %f\n",sqrt(sumvar25/(double)(ntrials-1)));
    printf("The estimated standard deviation of the price at time 50 = %f\n",sqrt(sumvar50/(double)(ntrials-1)));
    printf("The estimated standard deviation of the price at time 75 = %f\n",sqrt(sumvar75/(double)(ntrials-1)));
    printf("The estimated standard deviation of the price at time 100 = %f\n",sqrt(sumvar100/(double)(ntrials-1)));
    //question 2
    qsort((void *)X25,ntrials,sizeof(double),&compare_doubles);
    qsort((void *)X50,ntrials,sizeof(double),&compare_doubles);
    qsort((void *)X75,ntrials,sizeof(double),&compare_doubles);
    qsort((void *)X100,ntrials,sizeof(double),&compare_doubles);
    printf("The estimated median price at time 25 = %f\n",(X25[ntrials/2-1]+X25[ntrials/2])/2.0);
    printf("The estimated median price at time 50 = %f\n",(X50[ntrials/2-1]+X50[ntrials/2])/2.0);
    printf("The estimated median price at time 75 = %f\n",(X75[ntrials/2-1]+X75[ntrials/2])/2.0);
    printf("The estimated median price at time 100 = %f\n",(X100[ntrials/2-1]+X100[ntrials/2])/2.0);
    //question 3
    for (i=0;i<ntrials;i++)
    {
        summax += max[i];
        summin += min[i];        
    }
    printf("The estimated minimum value of the price between times 0 and 100 = %f\n", summin/(double)ntrials);
    printf("The estimated maximum value of the price between times 0 and 100 = %f\n", summax/(double)ntrials);
    
    //question 4
    for (i=0;i<ntrials;i++) sumindex += nmax[i];        
    printf("The estimated expected value of the time at which the maximum price occurs = %f\n", sumindex/((10000.*(double)ntrials)));
    for (i=0;i<ntrials;i++) summaxvar += pow((double)nmax[i]/10000.-sumindex/((10000.*(double)ntrials)),2);
    printf("The estimated standard deviation of the time at which the maximum price occurs = %f\n", sqrt(summaxvar/(double)(ntrials-1)));
    
    //question 5
    for (i=0;i<ntrials;i++) sumncount += N[i];
    printf("The estimated expected value of N = %f\n", sumncount/(double)ntrials);
    for (i=0;i<ntrials;i++) sumnvar += pow((double)N[i]-sumncount/(double)ntrials,2);
    printf("The estimated standard deviation of N = %f\n", sqrt(sumnvar/(double)(ntrials-1)));
    
    //question 6
    leftbound = sumncount/(double)ntrials - 1.96*sqrt(sumnvar/(double)(ntrials-1))/sqrt((double)ntrials);
    rightbound = sumncount/(double)ntrials + 1.96*sqrt(sumnvar/(double)(ntrials-1))/sqrt((double)ntrials);
    printf("The 95 percent confidence interval is [%f,%f]\n", leftbound,rightbound);
    
    return (EXIT_SUCCESS);
}
