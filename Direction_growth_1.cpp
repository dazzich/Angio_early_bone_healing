#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>
#include "header.h"

using namespace std;

const int n=251;

void Direction_growth_1(int cells_dir1[n][n],int j_dir1, int i_dir1,int& new_branch_j_dir1,int& new_branch_i_dir1,int& length_increase1, int& visual_i_dir1, int& visual_j_dir1) 
{ 
   /***************************
   if direction of growth RIGHT
   ***************************/
   int cnt;
   cnt=1;
   while(cnt<=length_increase1 && i_dir1+cnt<n && cells_dir1[j_dir1][i_dir1+cnt]==0)
   {
       cells_dir1[j_dir1][i_dir1+cnt]=2;
       cnt=cnt+1;
   }
   if (i_dir1+cnt<n && cells_dir1[j_dir1][i_dir1+cnt]==2 && length_increase1!=0)
   {
       new_branch_i_dir1=9999;
       new_branch_j_dir1=9999;
       visual_i_dir1=i_dir1+cnt;
   	   visual_j_dir1=j_dir1;
   	   cnt=cnt+1;
   }
   else
   {
       new_branch_i_dir1=i_dir1+cnt-1;
       new_branch_j_dir1=j_dir1;
       visual_i_dir1=i_dir1+cnt-1;
   	   visual_j_dir1=j_dir1;
   }
   length_increase1=cnt-1;
}            
