#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>
#include "header.h"

using namespace std;

void Direction_growth_6(int cells_dir6[N][N],int j_dir6, int i_dir6, int& new_branch_j_dir6,int& new_branch_i_dir6,int& length_increase6,int& visual_i_dir6,int& visual_j_dir6)      
{
   /***********************************
     if direction of growth UP Left
   ************************************/
   int cnt;
   cnt=1;              
   while(cnt<=length_increase6 && i_dir6-cnt>=0 && j_dir6+cnt<121 && cells_dir6[j_dir6+cnt][i_dir6-cnt]==0)
   {
      cells_dir6[j_dir6+cnt][i_dir6-cnt]=2;
      cnt=cnt+1;
   }
   if (i_dir6-cnt>=0 && j_dir6+cnt<N && cells_dir6[j_dir6+cnt][i_dir6-cnt]==2 && length_increase6!=0)
   {
       new_branch_i_dir6=9999;
       new_branch_j_dir6=9999;
       visual_i_dir6=i_dir6-cnt;
       visual_j_dir6=j_dir6+cnt;
       cnt=cnt+1;
   }
   else
   {
       new_branch_i_dir6=i_dir6-cnt+1;
       new_branch_j_dir6=j_dir6+cnt-1;
       visual_i_dir6=i_dir6-cnt+1;
       visual_j_dir6=j_dir6+cnt-1;
   }
   length_increase6=cnt-1;
}            
    
