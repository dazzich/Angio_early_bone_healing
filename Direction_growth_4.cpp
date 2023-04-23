#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>
#include "header.h"

using namespace std;

const int n=251;

void Direction_growth_4(int cells_dir4[n][n],int j_dir4, int i_dir4, int& new_branch_j_dir4,int& new_branch_i_dir4,int& length_increase4,int& visual_i_dir4,int& visual_j_dir4)      
{   
   /***********************************
   if direction of growth DOWN LEFT: 4
   ************************************/
   int cnt;
   cnt=1;              
   while(cnt<=length_increase4 && i_dir4-cnt>=0 && j_dir4-cnt>=0 && cells_dir4[j_dir4-cnt][i_dir4-cnt]==0)
   {
      cells_dir4[j_dir4-cnt][i_dir4-cnt]=2;
      cnt=cnt+1;
   }
   if (i_dir4-cnt>=0 && j_dir4-cnt>=0 && cells_dir4[j_dir4-cnt][i_dir4-cnt]==2 && length_increase4!=0)
   {
       new_branch_i_dir4=9999;
       new_branch_j_dir4=9999;
       visual_i_dir4=i_dir4-cnt;
       visual_j_dir4=j_dir4-cnt;
       cnt=cnt+1;
   }
   else
   {
       new_branch_i_dir4=i_dir4-cnt+1;
       new_branch_j_dir4=j_dir4-cnt+1;  
	   visual_i_dir4=i_dir4-cnt+1;
       visual_j_dir4=j_dir4-cnt+1;   
   }
   length_increase4=cnt-1;
}         
