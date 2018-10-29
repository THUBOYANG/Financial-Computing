#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>
#include <float.h>

using namespace std;

vector<double> fitmrp(vector<double> t, vector<double> X)
{
    int n = t.size();
    int i,j;
    double theta,mu,sigma,likelihood=-DBL_MAX;
    vector <double> para(3);
    for(i=1;i<1000;i++)
    {
        double theta_i,mu_i,sigma_i,S_i=0,
                mu_numerator=0.,mu_denominator=0.,likelihood_i=0.;
        theta_i = i*0.01;
        vector <double> w(n),m(n);
        w[0]=1.;
        m[0]=0.;
        for(j=1;j<n;j++)
        {
            w[j]=exp(-theta_i*(t[j]-t[j-1]));
            mu_numerator += (X[j]-X[j-1]*w[j])/(1.+w[j]);
            mu_denominator += (1.-w[j])/(1.+w[j]);
        }
        mu_i = mu_numerator/mu_denominator;
        for(j=1;j<n;j++)
        {
            m[j]=w[j]*X[j-1]+mu_i*(1.-w[j]);
            S_i += 2*theta_i*(X[j]-m[j])*(X[j]-m[j])/(1.-w[j]*w[j]);
        }
        sigma_i = sqrt(S_i/(double)n);
        likelihood_i = -(n-1)*log(sigma_i)-1./2.*S_i/(sigma_i*sigma_i);
        for(j=1;j<n;j++)
        {
            likelihood_i -= 1./2.*log((1.-w[j]*w[j])/(2*theta_i)); 
        }
        if(likelihood_i > likelihood)
        {
            likelihood = likelihood_i;
            mu = mu_i;
            theta = theta_i;
            sigma = sigma_i;
        }  
    }
    para[0] = mu;
    para[1] = theta;
    para[2] = sigma;
    return para;   
}

