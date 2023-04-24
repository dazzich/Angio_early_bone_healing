#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>
#include "header.h"

using namespace std;

void Direction_growth_2(int cells_dir2[N][N],int j_dir2, int i_dir2, int& new_branch_j_dir2,int& new_branch_i_dir2,int& length_increase2,int& visual_i_dir2,int& visual_j_dir2)      
{
   /***********************************
    if direction of growth DOWN RIGHT
   ************************************/
   int cnt;
   cnt=1;              
   while(cnt<=length_increase2 && i_dir2+cnt<N && j_dir2-cnt>=0 && cells_dir2[j_dir2-cnt][i_dir2+cnt]==0)
   {
      cells_dir2[j_dir2-cnt][i_dir2+cnt]=2;
      cnt=cnt+1;
   }
   if (i_dir2+cnt<N && j_dir2-cnt>=0 && cells_dir2[j_dir2-cnt][i_dir2+cnt]==2 && length_increase2!=0)
   {
       new_branch_i_dir2=9999;
       new_branch_j_dir2=9999;
       visual_i_dir2=i_dir2+cnt;
       visual_j_dir2=j_dir2-cnt;
       cnt=cnt+1;
   }
   else
   {
       new_branch_i_dir2=i_dir2+cnt-1;
       new_branch_j_dir2=j_dir2-cnt+1;
       visual_i_dir2=i_dir2+cnt-1;
       visual_j_dir2=j_dir2-cnt+1;
   }
   length_increase2=cnt-1;
}
            
       
         
