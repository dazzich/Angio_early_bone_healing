#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream> 
#include "header.h"

using namespace std;

const int n=251;
//UP RIGHT
void Branch_dir8(int cells_branch8[n][n],int j_branch8, int i_branch8, int& previous_direction_branch8, int& new_j_branch8, int& new_i_branch8, int& length_branch8,int& visual_i_branch8,int& visual_j_branch8)
{            
    int r1;
    int cnt;
    
    
    r1=rand()%2;
    switch (r1)
    {
        case 0: //right i++
           cnt=1;
           while(cnt<=length_branch8 && i_branch8+cnt<n && cells_branch8[j_branch8][i_branch8+cnt]==0)
           {
             cells_branch8[j_branch8][i_branch8+cnt]=2;
             cnt=cnt+1;  
           }
           if (i_branch8+cnt<n && cells_branch8[j_branch8][i_branch8+cnt]==2)
           {
               new_i_branch8=9999;
               new_j_branch8=9999;
               visual_i_branch8=i_branch8+cnt;
               visual_j_branch8=j_branch8;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch8=i_branch8+cnt-1;
               new_j_branch8=j_branch8;
               visual_i_branch8=i_branch8+cnt-1;
               visual_j_branch8=j_branch8;
           }
           previous_direction_branch8=1; //right
           break;
        case 1: //up j++
           cnt=1;
           while(cnt<=length_branch8 && j_branch8+cnt<n && cells_branch8[j_branch8+cnt][i_branch8]==0)
           {
             cells_branch8[j_branch8+cnt][i_branch8]=2;
             cnt=cnt+1;  
           }
           if (j_branch8+cnt<n && cells_branch8[j_branch8+cnt][i_branch8]==2)
           {
               new_i_branch8=9999;
               new_j_branch8=9999;
               visual_i_branch8=i_branch8;
               visual_j_branch8=j_branch8+cnt;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch8=i_branch8;
               new_j_branch8=j_branch8+cnt-1;
               visual_i_branch8=i_branch8;
               visual_j_branch8=j_branch8+cnt-1;
           }
           previous_direction_branch8=7; //up
           break;
        }
        length_branch8=cnt-1;        
}
