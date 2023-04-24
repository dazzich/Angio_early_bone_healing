#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>       //To read and write data from files as input/output streams
#include <math.h>
#include "header.h"


using namespace std;

int Branching(int length)
{
     
       const int C1=9; //Lmin=100 Checa et al. 2009
       const int C2=29; //Lmax=300 Checa et al. 2009
       int r1;
       float p;
       
       if (length<=C1)
       {
           return 0; //it doesn't branch
       }
       else
       {
           if (length>C2)
           {
              return 1;
           }
           else
           {
               float a,b;
               a=1/(C2-C1);
               b=(-1*C1)/(C2-C1);
               p=(a*length)+b;
               
               r1=rand()%10;
               if (r1>p)
               {
                   return 0;
               }
               else
               {
                   return 1; //it branches
               }
           }
       }
             
             
}
