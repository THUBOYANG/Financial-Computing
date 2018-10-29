#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <float.h>
#include <cmath>
#include <algorithm>

using namespace std;

vector<double> X(16800),t(16800),B(16800);

int read_delimited_file(string fid, char delimiter, vector<vector<string> > &v)
{
    ifstream infile;
    infile.open(fid);
    if (!infile.is_open())
    {
        cout << "can't open file for reading" << endl;
        return 1;
    }        
    stringstream ss;      // declare a string stream object
    ss << infile.rdbuf(); // make the string stream get the file data 
        
    string line;
    while(getline(ss,line)) // iterate until no more lines found 
                            // here getline only takes two arguments
    {
#if 0
       // new line has been found so append a new empty vector of strings
       vector<string> emptyvec;
       v.push_back(emptyvec);
       stringstream ss2(line);
       string field;
       while(getline(ss2,field,delimiter))
       {
           v.back().push_back(field);
       }
#endif
       stringstream ss2(line);
       vector<string> newvec;
       string field;
       while(getline(ss2,field,delimiter))
       {
           newvec.push_back(field);
       }
       v.push_back(newvec);
       
       
    }
       
    return 0;
}

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

void PriceRealization (double theta, double mu, double sigma)
{
    int i,rnumber;
    double delta, sqrtdelta;
    delta=0.01; // time increment
    sqrtdelta=sqrt(delta); // position increment
    B[0]=0.;
    t[0]=0.;
    X[0]=49.13;
    for (i=1;i<16800;i++)
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

int main(int argc, char** argv) {
    int i,j;
    string fid="CrudeOilPrices.csv";
    vector<vector<string> > v;
    if (read_delimited_file(fid,',',v))
    {
        cout << "file read not successful " << endl;
        return 0;
    }
    //cout << v.size() << endl << endl;
    vector<double> price;
    vector<double> tt;
    for (i=0;i<168;i++)
    {
        price.push_back(stod(v[168-i][1]));
        tt.push_back((double)(i));
        //cout<<price[i]<<endl;
    }
    //cout << price.size() << endl << endl;
    vector<double> para(3);
    para = fitmrp(tt,price);
    //cout<<para[0]<<' '<<para[1]<<' '<<para[2]<<endl;
    
    vector<double> mu,sigma,theta,para_1(3);
    vector<vector<double>> param(3);
    vector<double> t_test(168), X_test(168);
    for (i=0;i<1000;i++)
    {
        PriceRealization(para[1],para[0],para[2]);
        //cout<<X[50]<<endl;
        for (j=0;j<168;j++)
        {
            t_test[j] = t[j*100];
            X_test[j] = X[j*100];
        }
        //cout<<t_test[20]<<endl;
        para_1 = fitmrp(t_test,X_test);
        param[0].push_back(para_1[0]);
        param[1].push_back(para_1[2]);
        param[2].push_back(para_1[1]);
    }
    sort(param[0].begin(),param[0].end());
    sort(param[1].begin(),param[1].end());
    sort(param[2].begin(),param[2].end());
    
    FILE *fp; 
    fp=fopen("output.txt","w"); 
    fprintf(fp,"Fitted value for mu is %f and confidence interval is (%f,%f)\n",para[0],param[0][24],param[0][974]);
    fprintf(fp,"Fitted value for sigma is %f and confidence interval is (%f,%f)\n",para[2],param[1][24],param[1][974]);
    fprintf(fp,"Fitted value for theta is %f and confidence interval is (%f,%f)\n",para[1],param[2][24],param[2][974]);
    fclose(fp);
    return 0;
}

