#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream> 
#include "header.h"

using namespace std;

const int n=251;
//DOWN LEFT
void Branch_dir4(int cells_branch4[n][n],int j_branch4, int i_branch4, int& previous_direction_branch4, int& new_j_branch4, int& new_i_branch4, int& length_branch4,int& visual_i_branch4,int& visual_j_branch4)
{            
    int r1;
    int cnt;
    
    
    r1=rand()%2;
    switch (r1)
    {
        case 0: //left i--
           cnt=1;
           while(cnt<=length_branch4 && i_branch4-cnt>=0 && cells_branch4[j_branch4][i_branch4-cnt]==0)
           {
             cells_branch4[j_branch4][i_branch4-cnt]=2;
             cnt=cnt+1;  
           }
           if (i_branch4-cnt>=0 && cells_branch4[j_branch4][i_branch4-cnt]==2)
           {
               new_i_branch4=9999;
               new_j_branch4=9999;
               visual_i_branch4=i_branch4-cnt;
               visual_j_branch4=j_branch4;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch4=i_branch4-cnt+1;
               new_j_branch4=j_branch4;
               visual_i_branch4=i_branch4-cnt+1;
               visual_j_branch4=j_branch4;
           }
           previous_direction_branch4=5; //left
           break;
        case 1: //down j--
           cnt=1;
           while(cnt<=length_branch4 && j_branch4-cnt>=0 && cells_branch4[j_branch4-cnt][i_branch4]==0)
           {
             cells_branch4[j_branch4-cnt][i_branch4]=2;
             cnt=cnt+1;  
           }
           if (j_branch4-cnt>=0 && cells_branch4[j_branch4-cnt][i_branch4]==2)
           {
               new_i_branch4=9999;
               new_j_branch4=9999;
               visual_i_branch4=i_branch4;
               visual_j_branch4=j_branch4-cnt;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch4=i_branch4;
               new_j_branch4=j_branch4-cnt+1;
               visual_i_branch4=i_branch4;
               visual_j_branch4=j_branch4-cnt+1;
           }
           previous_direction_branch4=3; //down
           break;
        }
        length_branch4=cnt-1;
}
           
