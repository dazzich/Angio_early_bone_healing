#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>
#include "header.h"

using namespace std;

void Direction_growth_3(int cells_dir3[N][N],int j_dir3, int i_dir3,int& new_branch_j_dir3,int& new_branch_i_dir3,int& length_increase3, int& visual_i_dir3, int& visual_j_dir3)
{

   int cnt;
   
   /************************************
       if direction of growth DOWN
   ************************************/
   cnt=1;              
   
   while (cnt<=length_increase3 && j_dir3-cnt>=0 && cells_dir3[j_dir3-cnt][i_dir3]==0)
   {
      cells_dir3[j_dir3-cnt][i_dir3]=2;
      cnt=cnt+1;
   }
   
   
   if (j_dir3-cnt>=0 && cells_dir3[j_dir3-cnt][i_dir3]==2 && length_increase3!=0) 
   {
      new_branch_i_dir3=9999;
      new_branch_j_dir3=9999;
      visual_i_dir3=i_dir3;
      visual_j_dir3=j_dir3-cnt;
      cnt=cnt+1;
   
   }
   else 
   {
       new_branch_i_dir3=i_dir3;
       new_branch_j_dir3=j_dir3-cnt+1;
       visual_i_dir3=i_dir3;
       visual_j_dir3=j_dir3-cnt+1;
   }
   length_increase3=cnt-1;
   
}   
   
            
            
