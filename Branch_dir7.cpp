#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream> 
#include "header.h"

using namespace std;

const int n=251;
//UP
void Branch_dir7(int cells_branch7[n][n],int j_branch7, int i_branch7, int& previous_direction_branch7, int& new_j_branch7, int& new_i_branch7,int& length_branch7,int& visual_i_branch7,int& visual_j_branch7)
{            
               
    int r1;
    int cnt;
    
    
    r1=rand()%2;
    switch (r1)
    {
       case 0: //right
            cnt=1;
            while(cnt<=length_branch7 && j_branch7+cnt<n && i_branch7+cnt<n && cells_branch7[j_branch7+cnt][i_branch7+cnt]==0)
            {
               cells_branch7[j_branch7+cnt][i_branch7+cnt]=2;
               cnt=cnt+1;  
            }
            
            if (j_branch7+cnt<n && i_branch7+cnt<n && cells_branch7[j_branch7+cnt][i_branch7+cnt]==2)
            {
               new_i_branch7=9999;
               new_j_branch7=9999;
               visual_i_branch7=i_branch7+cnt;
               visual_j_branch7=j_branch7+cnt;
               cnt=cnt+1;
            }
            else
            {
               new_i_branch7=i_branch7+cnt-1;
               new_j_branch7=j_branch7+cnt-1;
               visual_i_branch7=i_branch7+cnt-1;
               visual_j_branch7=j_branch7+cnt-1;
            }
            previous_direction_branch7=8; //topright
            break;
      case 1: //left
           cnt=1;   
           while(cnt<=length_branch7 && j_branch7+cnt<n && i_branch7-cnt>=0 && cells_branch7[j_branch7+cnt][i_branch7-cnt]==0)
           {
              cells_branch7[j_branch7+cnt][i_branch7-cnt]=2;
              cnt=cnt+1;
           }
           if (j_branch7+cnt<n && i_branch7-cnt>=0 && cells_branch7[j_branch7+cnt][i_branch7-cnt]==2)
           {
               new_i_branch7=9999;
               new_j_branch7=9999;
               visual_i_branch7=i_branch7-cnt;
               visual_j_branch7=j_branch7+cnt;
               cnt=cnt+1;
           }
           else
           {
               new_i_branch7=i_branch7-cnt+1;
               new_j_branch7=j_branch7+cnt-1;
               visual_i_branch7=i_branch7-cnt+1;
               visual_j_branch7=j_branch7+cnt-1;
           }
           previous_direction_branch7=6; // top left
           break;
      
      }
      length_branch7=cnt-1;
}
         
