#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream> 
#include "header.h"

using namespace std;

const int n=251;
//UPLEFT
void Branch_dir6(int cells_branch6[n][n],int j_branch6, int i_branch6, int& previous_direction_branch6, int& new_j_branch6, int& new_i_branch6, int& length_branch6,int& visual_i_branch6,int& visual_j_branch6)
{            
    int r1;
    int cnt;
    
    
    r1=rand()%2;
    switch (r1)
    {
        case 0: //left i--
           cnt=1;
           while(cnt<=length_branch6 && i_branch6-cnt>=0 && cells_branch6[j_branch6][i_branch6-cnt]==0)
           {
             cells_branch6[j_branch6][i_branch6-cnt]=2;
             cnt=cnt+1;  
           }
           if (i_branch6+cnt>=0 && cells_branch6[j_branch6][i_branch6-cnt]==2)
           {
               new_i_branch6=9999;
               new_j_branch6=9999;
               visual_i_branch6=i_branch6-cnt;
               visual_j_branch6=j_branch6;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch6=i_branch6-cnt+1;
               new_j_branch6=j_branch6;
               visual_i_branch6=i_branch6-cnt+1;
               visual_j_branch6=j_branch6;
           }
           previous_direction_branch6=5; //left
           break;
        case 1: //up j++
           cnt=1;
           while(cnt<=length_branch6 && j_branch6+cnt<n && cells_branch6[j_branch6+cnt][i_branch6]==0)
           {
             cells_branch6[j_branch6+cnt][i_branch6]=2;
             cnt=cnt+1;  
           }
           if (j_branch6+cnt<n && cells_branch6[j_branch6+cnt][i_branch6]==2)
           {
               new_i_branch6=9999;
               new_j_branch6=9999;
               visual_i_branch6=i_branch6;
               visual_j_branch6=j_branch6+cnt;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch6=i_branch6;
               new_j_branch6=j_branch6+cnt-1;
               visual_i_branch6=i_branch6;
               visual_j_branch6=j_branch6+cnt-1;
           }
           previous_direction_branch6=7; //up
           break;
        }
        length_branch6=cnt-1;        
}
