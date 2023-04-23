#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream> 
#include "header.h"

using namespace std;

const int n=251;

//DOWN
void Branch_dir3(int cells_branch3[n][n],int j_branch3, int i_branch3, int& previous_direction_branch3, int& new_j_branch3, int& new_i_branch3,int& length_branch3,int& visual_i_branch3,int& visual_j_branch3)
{            
            
    int r1;
    int cnt;
    
    
    r1=rand()%2;  //random number between 0 and 1: il vaso sta crescendo verso il basso ma può fare branching in 2 direzioni (bottomright/bottomleft)
    switch (r1)
    {
       case 0: //right
           cnt=1;
           while (cnt<=length_branch3 && j_branch3-cnt>=0 && i_branch3+cnt<n && cells_branch3[j_branch3-cnt][i_branch3+cnt]==0)
           {
                 cells_branch3[j_branch3-cnt][i_branch3+cnt]=2;
                 cnt=cnt+1;
           }
           
           if (j_branch3-cnt>=0 && i_branch3+cnt<n && cells_branch3[j_branch3-cnt][i_branch3+cnt]==2) //anastomosis
           {
               new_i_branch3=9999;
               new_j_branch3=9999;
               visual_i_branch3=i_branch3+cnt;
               visual_j_branch3=j_branch3-cnt;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch3=i_branch3+cnt-1;
               new_j_branch3=j_branch3-cnt+1;
               visual_i_branch3=i_branch3+cnt-1;
               visual_j_branch3=j_branch3-cnt+1;
           }
           previous_direction_branch3=2; //bottom right
           break;
       case 1: //left
           cnt=1; 
           while (cnt<=length_branch3 && j_branch3-cnt>=0 && i_branch3-cnt>=0 && cells_branch3[j_branch3-cnt][i_branch3-cnt]==0)
           {
                 cells_branch3[j_branch3-cnt][i_branch3-cnt]=2;
                 cnt=cnt+1;
           }
           if (j_branch3-cnt>=0 && i_branch3-cnt>=0 && cells_branch3[j_branch3-cnt][i_branch3-cnt]==2)
           {
               new_i_branch3=9999;
               new_j_branch3=9999;
               visual_i_branch3=i_branch3-cnt;
               visual_j_branch3=j_branch3-cnt;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch3=i_branch3-cnt+1;
               new_j_branch3=j_branch3-cnt+1;
               visual_i_branch3=i_branch3-cnt+1;
               visual_j_branch3=j_branch3-cnt+1;
           }
           previous_direction_branch3=4; //bottom left
           break;
                
    }
    
    length_branch3=cnt-1;

}
