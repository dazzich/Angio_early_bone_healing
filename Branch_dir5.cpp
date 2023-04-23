#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream> 
#include "header.h"

using namespace std;

const int n=251;

//LEFT
void Branch_dir5(int cells_branch5[n][n],int j_branch5, int i_branch5, int& previous_direction_branch5, int& new_j_branch5, int& new_i_branch5, int& length_branch5, int& visual_i_branch5, int& visual_j_branch5)
{            
    int r1;
    int cnt;
    
    
    r1=rand()%2;
    switch (r1)
    {
        case 0:
           cnt=1; //left up
           while(cnt<=length_branch5 && j_branch5+cnt<n && i_branch5-cnt>=0 && cells_branch5[j_branch5+cnt][i_branch5-cnt]==0)
           {
             cells_branch5[j_branch5+cnt][i_branch5-cnt]=2;
             cnt=cnt+1;  
           }
           if (j_branch5+cnt<n && i_branch5-cnt>=0 && cells_branch5[j_branch5+cnt][i_branch5-cnt]==2)
           {
               new_i_branch5=9999;
               new_j_branch5=9999;
               visual_i_branch5=i_branch5-cnt;
               visual_j_branch5=j_branch5+cnt;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch5=i_branch5-cnt+1;
               new_j_branch5=j_branch5+cnt-1;
               visual_i_branch5=i_branch5-cnt+1;
               visual_j_branch5=j_branch5+cnt-1;
           }
           previous_direction_branch5=6; //topleft
           break;
        case 1:
           cnt=1; //Down left
           while(cnt<=length_branch5 && j_branch5-cnt>=0 && i_branch5-cnt>=0 && cells_branch5[j_branch5-cnt][i_branch5-cnt]==0)
           {
             cells_branch5[j_branch5-cnt][i_branch5-cnt]=2;
             cnt=cnt+1;  
           }
           if (j_branch5-cnt>=0 && i_branch5-cnt>=0 && cells_branch5[j_branch5-cnt][i_branch5-cnt]==2)
           {
              new_i_branch5=9999;
              new_j_branch5=9999;
              visual_i_branch5=i_branch5-cnt;
              visual_j_branch5=j_branch5-cnt;
              cnt=cnt+1;
           }
           else
           {
              new_i_branch5=i_branch5-cnt+1;
              new_j_branch5=j_branch5-cnt+1;
              visual_i_branch5=i_branch5-cnt+1;
              visual_j_branch5=j_branch5-cnt+1;
           }
           previous_direction_branch5=4;  //Bottom left
           break;
        
        }
        length_branch5=cnt-1;
}
             
           
           
