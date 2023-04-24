#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream> 
#include "header.h"

using namespace std;
//DOWN RIGHT
void Branch_dir2(int cells_branch2[N][N],int j_branch2, int i_branch2, int& previous_direction_branch2, int& new_j_branch2, int& new_i_branch2, int& length_branch2,int& visual_i_branch2, int& visual_j_branch2)
{            
    int r1;
    int cnt;
    
    
    r1=rand()%2;
    switch (r1)
    {
        case 0: //right i++
           cnt=1;
           while(cnt<=length_branch2 && i_branch2+cnt<N && cells_branch2[j_branch2][i_branch2+cnt]==0)
           {
             cells_branch2[j_branch2][i_branch2+cnt]=2;
             cnt=cnt+1;  
           }
           if (i_branch2+cnt<N && cells_branch2[j_branch2][i_branch2+cnt]==2)
           {
               new_i_branch2=9999;
               new_j_branch2=9999;
               visual_i_branch2=i_branch2+cnt;
               visual_j_branch2=j_branch2;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch2=i_branch2+cnt-1;
               new_j_branch2=j_branch2;
               visual_i_branch2=i_branch2+cnt-1;
               visual_j_branch2=j_branch2;
           }
           previous_direction_branch2=1; //right
           break;
        case 1: //down j--
           cnt=1;
           while(cnt<=length_branch2 && j_branch2-cnt>=0 && cells_branch2[j_branch2-cnt][i_branch2]==0)
           {
             cells_branch2[j_branch2-cnt][i_branch2]=2;
             cnt=cnt+1;  
           }
           if (j_branch2-cnt>=0 && cells_branch2[j_branch2-cnt][i_branch2]==2)
           {
               new_i_branch2=9999;
               new_j_branch2=9999;
               visual_i_branch2=i_branch2;
               visual_j_branch2=j_branch2-cnt;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch2=i_branch2;
               new_j_branch2=j_branch2-cnt+1;
               visual_i_branch2=i_branch2;
               visual_j_branch2=j_branch2-cnt+1;
           }
           previous_direction_branch2=3; //down
           break;
        }
        length_branch2=cnt-1;
}
