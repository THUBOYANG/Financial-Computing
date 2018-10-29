#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    //(1)
     printf("I love programming!\n");
     
     printf("size of short is %d\n", sizeof(short));
     printf("size of int is %d\n", sizeof(int));
     printf("size of long is %d\n", sizeof(long));
     printf("size of float is %d\n", sizeof(float));
     printf("size of double is %d\n", sizeof(double));
     printf("size of long double is %d\n", sizeof(long double));
     
     //(2)
     short int is = 0x8000;
     printf("The smallest decimal value of is = %hd\n", is);
     is = 0x7fff;
     printf("The largest decimal value of is = %hd\n", is);
     int i = 0x80000000;
     printf("The smallest decimal value of i = %d\n", i);
     i = 0x7fffffff;
     printf("The largest decimal value of i = %d\n", i);
     long long int ill = 0x8000000000000000;
     printf("The smallest decimal value of ill = %lld\n", ill);
     ill = 0x7fffffffffffffff;
     printf("The largest decimal value of ill = %lld\n", ill);
     unsigned short int uis = 0x0000;
     printf("The smallest decimal value of uis = %hu\n", uis);
     uis = 0xffff;
     printf("The largest decimal value of uis = %hu\n", uis);
     unsigned int ui = 0x00000000;
     printf("The smallest decimal value of ui = %u\n", ui);
     ui = 0xffffffff;
     printf("The largest decimal value of ui = %u\n", ui);
     unsigned long long int ull = 0x0000000000000000;
     printf("The smallest decimal value of ull = %llu\n", ull);
     ull = 0xffffffffffffffff;
     printf("The largest decimal value of ull = %llu\n", ull);
     return (EXIT_SUCCESS);
}

