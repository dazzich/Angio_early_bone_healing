#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream> 
#include "header.h"

using namespace std;

const int n=251;
//right
void Branch_dir1(int cells_branch1[n][n],int j_branch1, int i_branch1, int& previous_direction_branch1, int& new_j_branch1, int& new_i_branch1, int& length_branch1, int& visual_i_branch1, int& visual_j_branch1)
{            
    int r1;
    int cnt;
    
    
    r1=rand()%2;
    switch (r1)
    {
       case 0: // UP 
            cnt=1;
            while(cnt<=length_branch1 && j_branch1+cnt<n && i_branch1+cnt<n && cells_branch1[j_branch1+cnt][i_branch1+cnt]==0)
            {
              cells_branch1[j_branch1+cnt][i_branch1+cnt]=2;
              cnt=cnt+1;  
            }
            if (j_branch1+cnt<n && i_branch1+cnt<n && cells_branch1[j_branch1+cnt][i_branch1+cnt]==2)
            {
                new_i_branch1=9999;
                new_j_branch1=9999;
                visual_i_branch1=i_branch1+cnt;
                visual_j_branch1=j_branch1+cnt;
                cnt=cnt+1;
            }
            else
            {
                new_i_branch1=i_branch1+cnt-1;
                new_j_branch1=j_branch1+cnt-1;
                visual_i_branch1=i_branch1+cnt-1;
                visual_j_branch1=j_branch1+cnt-1;
            }
            previous_direction_branch1=8; //topright
            break;
       case 1: //down
            cnt=1;
            while(cnt<=length_branch1 && j_branch1-cnt>=0 && i_branch1+cnt<n && cells_branch1[j_branch1-cnt][i_branch1+cnt]==0)
            {
                 cells_branch1[j_branch1-cnt][i_branch1+cnt]=2;
                 cnt=cnt+1;
            }
            if (j_branch1-cnt>=0 && i_branch1+cnt<n && cells_branch1[j_branch1-cnt][i_branch1+cnt]==2)
            {
                new_i_branch1=9999;
                new_j_branch1=9999;
                visual_i_branch1=i_branch1+cnt;
                visual_j_branch1=j_branch1-cnt;
                cnt=cnt+1;
            }
            else
            {
                new_i_branch1=i_branch1+cnt-1;
                new_j_branch1=j_branch1-cnt+1;
                visual_i_branch1=i_branch1+cnt-1;
                visual_j_branch1=j_branch1-cnt+1;
            }
            previous_direction_branch1=2; //bottomright
            break;
            
       }
       length_branch1=cnt-1;
}    
