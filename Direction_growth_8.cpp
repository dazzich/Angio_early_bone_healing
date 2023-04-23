#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>
#include "header.h"

using namespace std;

const int n=251;

void Direction_growth_8(int cells_dir8[n][n],int j_dir8, int i_dir8, int& new_branch_j_dir8,int& new_branch_i_dir8,int& length_increase8,int& visual_i_dir8,int& visual_j_dir8)      
{
   /***********************************
    if direction of growth UP RIGHT: 8
   ************************************/
   int cnt;
   cnt=1;              
   while(cnt<=length_increase8 && i_dir8+cnt<n && j_dir8+cnt<n && cells_dir8[j_dir8+cnt][i_dir8+cnt]==0)
   {
      cells_dir8[j_dir8+cnt][i_dir8+cnt]=2;
      cnt=cnt+1;
   }
   if (i_dir8+cnt<n && j_dir8+cnt<n && cells_dir8[j_dir8+cnt][i_dir8+cnt]==2 && length_increase8!=0)
   {
       new_branch_i_dir8=9999;
       new_branch_j_dir8=9999;
       visual_i_dir8=i_dir8+cnt;
       visual_j_dir8=j_dir8+cnt;
       cnt=cnt+1;
   }
   else
   {
       new_branch_i_dir8=i_dir8+cnt-1;
       new_branch_j_dir8=j_dir8+cnt-1;
       visual_i_dir8=i_dir8+cnt-1;
       visual_j_dir8=j_dir8+cnt-1;
   }
   length_increase8=cnt-1;
}
            
       
