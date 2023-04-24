#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iomanip> 
#include "header.h" 

using namespace std;

void Update_model(int cell_matrix_1[N][N], int dipole_dir_1[N][N], double cell_def_1[N][N], int dipole_dir_2[N][N], double cell_def_2[N][N],int iter, int migration[N][N])
{    
    int i,j;
     
    int Updated_matrix[N][N];
    int Updated_direction[N][N];
     
    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
        	if (cell_matrix_1[j][i]==2)
        	{
        		Updated_matrix[j][i]=2; //put ECs
			}
			else
			{
				Updated_matrix[j][i]=0;
            	Updated_direction[j][i]=0;
			}
        }
    } 
    
//****************************************************************************************************************************************************************
//                     Blocking the area that cells can not migrate in
//****************************************************************************************************************************************************************
    j=0;
    for (i=0;i<N;i++)
    {
       Updated_matrix[j][i]=1;
       Updated_direction[j][i]=99;    
    }
    
    j=N-1;
    for (i=0;i<N;i++)
    {
       Updated_matrix[j][i]=1;
       Updated_direction[j][i]=99;  
    }
    
	for (j=0;j<GAP_Down;j++)  // Bottom Cortical Bones
	{
	    for (i=CL_left;i<CL_right;i++) //Left
	    {
	        Updated_matrix[j][i]=1;
	        Updated_direction[j][i]=5;
	    }

	    for (i=CR_left;i<CR_right;i++) //Right
	    {
	        Updated_matrix[j][i]=1;
	        Updated_direction[j][i]=5;
	    }
	}
	
	for (j=GAP_Top;j<N;j++)  // Top Cortical Bones
	{
	    for (i=CL_left;i<CL_right;i++)
	    {
	        Updated_matrix[j][i]=1;
	        Updated_direction[j][i]=5;
	    }
	    
	    for (i=CR_left;i<CR_right;i++)
	    {
	       Updated_matrix[j][i]=1;
	       Updated_direction[j][i]=5;
	    }
	}
    
//****************************************************************************************************************************************************************
//                														Updating cells position
//****************************************************************************************************************************************************************  
    int cnt_cell=0;
    int cnt_upd_cell=0;
	/*
	First half
	*/
    for (j=round((N-1)/2);j>=0;j--) 
    {
        for (i=0;i<N;i++)
        {
            if (cell_matrix_1[j][i]>2)
            {
                cnt_cell=cnt_cell+1; //count all fibro in the first half
                 
                switch (migration[j][i])
                {
                    case 99: 
                        Updated_matrix[j][i]=cell_matrix_1[j][i]; 
                        Updated_direction[j][i]=dipole_dir_1[j][i]; 
                    break;
                    
                    case 88: // no available positions around the current location
                        Updated_matrix[j][i]=cell_matrix_1[j][i];  
                        Updated_direction[j][i]=dipole_dir_1[j][i]; 
                    break;
                     
                    case 0: 
                        if (Updated_matrix[j][i+1]==0)
                        {
                           	Updated_matrix[j][i+1]=cell_matrix_1[j][i]; 
                           	Updated_direction[j][i+1]=dipole_dir_2[j][i+1]; 
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i]; 
                        }
                    break;
                     
                    case 1:
                        if (Updated_matrix[j-1][i+1]==0)
                        {
                           	Updated_matrix[j-1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i+1]=dipole_dir_2[j-1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           Updated_matrix[j][i]=cell_matrix_1[j][i];
                           Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 2:
                        if (Updated_matrix[j-1][i]==0)
                        {
                           	Updated_matrix[j-1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i]=dipole_dir_2[j-1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 3:
                        if (Updated_matrix[j-1][i-1]==0)
                        {
                           	Updated_matrix[j-1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i-1]=dipole_dir_2[j-1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 4:
                        if (Updated_matrix[j][i-1]==0)
                        {
                           	Updated_matrix[j][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i-1]=dipole_dir_2[j][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }	
                    break;
                     
                    case 5:
                        if (Updated_matrix[j+1][i-1]==0)
                        {
                           	Updated_matrix[j+1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i-1]=dipole_dir_2[j+1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;
                     
                    case 6:
                        if (Updated_matrix[j+1][i]==0)
                        {
                           	Updated_matrix[j+1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i]=dipole_dir_2[j+1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 7:
                        if (Updated_matrix[j+1][i+1]==0)
                        {
                           	Updated_matrix[j+1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i+1]=dipole_dir_2[j+1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break; 
                    
                    case 8: 
                        if (cell_def_1[j][i]>cell_def_2[j][i+1] && Updated_matrix[j][i+1]==0)
                        {
                           	Updated_matrix[j][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i+1]=dipole_dir_2[j][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }  
                    break;
                     
                    case 9:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i+1] && Updated_matrix[j-1][i+1]==0)
                        {
                           	Updated_matrix[j-1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i+1]=dipole_dir_2[j-1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 10:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i] && Updated_matrix[j-1][i]==0)
                        {
                           	Updated_matrix[j-1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i]=dipole_dir_2[j-1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 11:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i-1] && Updated_matrix[j-1][i-1]==0)
                        {
                           	Updated_matrix[j-1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i-1]=dipole_dir_2[j-1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 12:
                        if (cell_def_1[j][i]>cell_def_2[j][i-1] && Updated_matrix[j][i-1]==0)
                        {
                           	Updated_matrix[j][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i-1]=dipole_dir_2[j][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;
                     
                    case 13:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i-1] && Updated_matrix[j+1][i-1]==0)
                        {
                           	Updated_matrix[j+1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i-1]=dipole_dir_2[j+1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;
                     
                    case 14:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i] && Updated_matrix[j+1][i]==0)
                        {
                           	Updated_matrix[j+1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i]=dipole_dir_2[j+1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 15:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i+1] && Updated_matrix[j+1][i+1]==0)
                        {
                           	Updated_matrix[j+1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i+1]=dipole_dir_2[j+1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break; 	
                } // End switch                                   
            } // if (cell_matrix_1[j][i]>1)
        } // End for i
    } // End for j
    
	/*
	Second half
	*/
    for (j=round((N-1)/2+1);j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            if (cell_matrix_1[j][i]>2)
            {
                cnt_cell=cnt_cell+1;
                 
                switch (migration[j][i])
                {
                    case 99:
                        Updated_matrix[j][i]=cell_matrix_1[j][i]; 
                        Updated_direction[j][i]=dipole_dir_1[j][i]; 
                    break;
                    
                    case 88:
                        Updated_matrix[j][i]=cell_matrix_1[j][i]; 
                        Updated_direction[j][i]=dipole_dir_1[j][i]; 
                    break;
                     
                    case 0:
                        if (Updated_matrix[j][i+1]==0)
                        {
                           	Updated_matrix[j][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i+1]=dipole_dir_2[j][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;
                     
                    case 1:
                        if (Updated_matrix[j-1][i+1]==0)
                        {
                           	Updated_matrix[j-1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i+1]=dipole_dir_2[j-1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           Updated_matrix[j][i]=cell_matrix_1[j][i];
                           Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 2:
                        if (Updated_matrix[j-1][i]==0)
                        {
                           	Updated_matrix[j-1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i]=dipole_dir_2[j-1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 3:
                        if (Updated_matrix[j-1][i-1]==0)
                        {
                           	Updated_matrix[j-1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i-1]=dipole_dir_2[j-1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 4:
                        if (Updated_matrix[j][i-1]==0)
                        {
                           	Updated_matrix[j][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i-1]=dipole_dir_2[j][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }	
                    break;
                     
                    case 5:
                        if (Updated_matrix[j+1][i-1]==0)
                        {
                           	Updated_matrix[j+1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i-1]=dipole_dir_2[j+1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;
                     
                    case 6:
                        if (Updated_matrix[j+1][i]==0)
                        {
                           	Updated_matrix[j+1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i]=dipole_dir_2[j+1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 7:
                        if (Updated_matrix[j+1][i+1]==0)
                        {
                           	Updated_matrix[j+1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i+1]=dipole_dir_2[j+1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break; 
                    
                    case 8:
                        if (cell_def_1[j][i]>cell_def_2[j][i+1] && Updated_matrix[j][i+1]==0)
                        {
                           	Updated_matrix[j][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i+1]=dipole_dir_2[j][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }  
                    break;
                     
                    case 9:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i+1] && Updated_matrix[j-1][i+1]==0)
                        {
                           	Updated_matrix[j-1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i+1]=dipole_dir_2[j-1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 10:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i] && Updated_matrix[j-1][i]==0)
                        {
                           	Updated_matrix[j-1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i]=dipole_dir_2[j-1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 11:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i-1] && Updated_matrix[j-1][i-1]==0)
                        {
                           	Updated_matrix[j-1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i-1]=dipole_dir_2[j-1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 12:
                        if (cell_def_1[j][i]>cell_def_2[j][i-1] && Updated_matrix[j][i-1]==0)
                        {
                           	Updated_matrix[j][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i-1]=dipole_dir_2[j][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;
                     
                    case 13:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i-1] && Updated_matrix[j+1][i-1]==0)
                        {
                           	Updated_matrix[j+1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i-1]=dipole_dir_2[j+1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;
                     
                    case 14:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i] && Updated_matrix[j+1][i]==0)
                        {
                           	Updated_matrix[j+1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i]=dipole_dir_2[j+1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break;

                    case 15:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i+1] && Updated_matrix[j+1][i+1]==0)
                        {
                           	Updated_matrix[j+1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i+1]=dipole_dir_2[j+1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                        }
                    break; 	
                } // End switch                                   
            } // End if 
        } // End i
    } // End j
     

               
//****************************************************************************************************************************************************************
//                    											Check number of cells
//****************************************************************************************************************************************************************
    
//	int cells_1, cells_2;
//    cells_1=0;
//    cells_2=0;
//     
//    for (j=0;j<N;j++)
//    {
//        for (i=0;i<N;i++)
//        {
//            if (cell_matrix_1[j][i]>2)
//            {
//                cells_1=cells_1+1;             
//            }
//            if (Updated_matrix[j][i]>2)
//            {
//                cells_2=cells_2+1;             
//            }
//        }
//    }   
//    cout<<"Iteration: "<<iter<<endl;  
//    cout<<"Number of fibroblasts in FEM_"<<iter<<":"<<cells_1<<endl;
//    cout<<"Number of fibroblasts in Updated Model for Iteration - No."<<iter+1<<":"<<cells_2<<endl;  
      
//****************************************************************************************************************************************************************
//                 												Update the cell matrix
//****************************************************************************************************************************************************************
    
	cout<<"Update the Cell Matrix "<<endl; 

    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            cell_matrix_1[j][i]=Updated_matrix[j][i];
            dipole_dir_1[j][i]=Updated_direction[j][i];
             
        }
    }

    cout<<"Cell Matrix Updated Successfully"<<endl;       
}

