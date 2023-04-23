#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>
#include "header.h"

using namespace std;

const int n=251;

void Direction_growth_5(int cells_dir5[n][n],int j_dir5, int i_dir5,int& new_branch_j_dir5,int& new_branch_i_dir5,int& length_increase5,int& visual_i_dir5,int& visual_j_dir5)
{

   int cnt;
   
   /************************************
       if direction of growth LEFT
   ************************************/
   cnt=1;              
   
   while (cnt<=length_increase5 && i_dir5-cnt>=0 && cells_dir5[j_dir5][i_dir5-cnt]==0)
   {
      cells_dir5[j_dir5][i_dir5-cnt]=2;
      cnt=cnt+1;
   }
   
   
   if (i_dir5-cnt>=0 && cells_dir5[j_dir5][i_dir5-cnt]==2 && length_increase5!=0) //anastomosis
   {
      new_branch_i_dir5=9999;
      new_branch_j_dir5=9999;
      visual_i_dir5=i_dir5-cnt;
      visual_j_dir5=j_dir5;
      cnt=cnt+1;
   
   }
   else //otherwise, elongation concluded
   {
       new_branch_i_dir5=i_dir5-cnt+1;
       new_branch_j_dir5=j_dir5;
       visual_i_dir5=i_dir5-cnt+1;
       visual_j_dir5=j_dir5;
   }
   length_increase5=cnt-1;
}   
   
            
            
