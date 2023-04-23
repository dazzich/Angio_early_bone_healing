#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>
#include "header.h"

using namespace std;

const int n=251;

void Direction_growth_7(int cells_dir7[n][n],int j_dir7, int i_dir7,int& new_branch_j_dir7,int& new_branch_i_dir7,int& length_increase7,int& visual_i_dir7,int& visual_j_dir7)
{
   /***************************
     if direction of growth UP
   ***************************/
   int cnt;
   
   cnt=1;
   while(cnt<=length_increase7 && j_dir7+cnt<n && cells_dir7[j_dir7+cnt][i_dir7]==0)
   {
       cells_dir7[j_dir7+cnt][i_dir7]=2;
       cnt=cnt+1;
   }
   if (j_dir7+cnt<n && cells_dir7[j_dir7+cnt][i_dir7]==2 && length_increase7!=0)
   {                                                 
       new_branch_i_dir7=9999;
       new_branch_j_dir7=9999;
       visual_i_dir7=i_dir7;
       visual_j_dir7=j_dir7+cnt;
       cnt=cnt+1;
   }
   else
   {
       new_branch_i_dir7=i_dir7;
       new_branch_j_dir7=j_dir7+cnt-1;
       visual_i_dir7=i_dir7;
       visual_j_dir7=j_dir7+cnt-1;
   }
   length_increase7=cnt-1;
}
            
            
