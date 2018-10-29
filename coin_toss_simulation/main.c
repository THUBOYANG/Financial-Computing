#include <stdio.h>
#include <stdlib.h>
//
// How likely to see a run of at least some size when 
// when we flip a coin a bunch of times.
//
//   ntrials = number of monte-carlo trials
//   ntosses = number of coin flips
//   threshold = determine probability of a run of at least this size
//
int main(int argc, char** argv) {
    int trial, toss, current_runsize, runsize, ntosses, ntrials, 
            max_runsize, count, target;
    float rnumber;
    
    ntrials=100000;
    ntosses=1000;
    target=5;
    srand(1);    // set the random number generator seed
    count=0;     // number of trials in which we get a run of size exactly the target
    
    // loop over the trials
    for (trial=0;trial<ntrials;trial++) 
    {
        current_runsize=0; // initialize the current run size for this trial
        max_runsize=0; // initialize the maximum run size for this trial
        
        // loop over ntosses coin tosses
        for (toss=0;toss<ntosses;toss++)
        {
            // generate the next coin toss
            // and update the current runsize
            rnumber=(float)rand()/32768; // random number from 0,...,32767
            if (rnumber > 2.0/3.0) // note slight bias
            {
               current_runsize++; // increment current run size
            }
            else
            {
                current_runsize=0; // reset the current run size
            }
            // check whether we get a new maximum in this trial
            // and if so update the maximum
            if (current_runsize>max_runsize)
            {
                max_runsize=current_runsize;
            }
        }
        if (max_runsize == target)
        {
            count++; // increment count
        }
    }
    printf("probability estimate = %lf\n",(double)count/(double)ntrials);
    
    return (EXIT_SUCCESS);
}