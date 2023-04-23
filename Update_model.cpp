#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iomanip>  

using namespace std;

const int n=251;

void Update_model(int cell_matrix_1[n][n], int dipole_dir_1[n][n], double cell_def_1[n][n],int cell_matrix_2[n][n], int dipole_dir_2[n][n], double cell_def_2[n][n],int iter, int migration[n][n])
{    
    int i,j;
     
    int Updated_matrix[n][n];
    int Updated_direction[n][n];
     
    for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
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
     
    j=0;
    for (i=0;i<n;i++)
    {
       Updated_matrix[j][i]=1;
       Updated_direction[j][i]=99;    
    }
    
    j=n-1;
    for (i=0;i<n;i++)
    {
       Updated_matrix[j][i]=1;
       Updated_direction[j][i]=99;  
    }
    
//****************************************************************************************************************************************************************
//                     Blocking the area that cells can not migrate in
//****************************************************************************************************************************************************************
    
	for (j=0;j<101;j++)  // Bottom Cortical Bones
	{
	    for (i=50;i<71;i++) //Left
	    {
	        Updated_matrix[j][i]=1;
	        Updated_direction[j][i]=5;
	    }

	    for (i=180;i<201;i++) //Riht
	    {
	        Updated_matrix[j][i]=1;
	        Updated_direction[j][i]=5;
	    }
	}
	
	for (j=150;j<n;j++)  // Top Cortical Bones
	{
	    for (i=50;i<71;i++)
	    {
	        Updated_matrix[j][i]=1;
	        Updated_direction[j][i]=5;
	    }
	    
	    for (i=180;i<201;i++)
	    {
	       Updated_matrix[j][i]=1;
	       Updated_direction[j][i]=5;
	    }
	}
    
//****************************************************************************************************************************************************************
//                														Updating cells position
//****************************************************************************************************************************************************************  
	int No_Dens=0;
	int No_Stiff=0;
	int No_Space=0; // No_space counts the number of cells that can't move
    int Dens_Migration=0;
    int Stif_Migration=0;
    int cnt_cell_1=0;
    int cnt_upd_cell=0;
    int Mig_X=0;
    int Mig_Y=0;
    vector<int>Migration;
	Migration.clear(); 

	//while 80% of fibroblasts not moved --> i=randn()%n
    for (j=round((n-1)/2);j>=0;j--)
    {
        for (i=0;i<n;i++)
        {
//			node=i+(j*121)+1;
            if (cell_matrix_1[j][i]>2) //just fibroblasts
            {
                cnt_cell_1=cnt_cell_1+1; //count all fibro in the first half
                 
                switch (migration[j][i])
                {
                    case 99: // never?
                        Updated_matrix[j][i]=cell_matrix_1[j][i]; 
                        Updated_direction[j][i]=dipole_dir_1[j][i]; 
                    break;
                    
                    case 88: // no available positions around the current location
                        Updated_matrix[j][i]=cell_matrix_1[j][i];  //cell stays there
                        Updated_direction[j][i]=dipole_dir_1[j][i]; //dipole direction doesn't change
						No_Space=No_Space+1; 
                    break;
                     
                    case 0: // migration established in direction 0 and it is the direction with lower density of cells
                        if (Updated_matrix[j][i+1]==0) // if the pos in that direction is empty and not forbidden
                        {
                           	Updated_matrix[j][i+1]=cell_matrix_1[j][i]; //put the number of the cell in the new position
                           	Updated_direction[j][i+1]=dipole_dir_2[j][i+1]; //use the new random dipole direction
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<<setw(15)<<"xxxxx"<<left << setw(15)<<dipole_dir_1[j][i]<<left << setw(15)<<"xxxxx"<<left << setw(15)<<Updated_direction[j][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i]; //otherwise, leave in the last pos and
                           	Updated_direction[j][i]=dipole_dir_1[j][i]; //leave the previous dipole orient
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<<setw(15)<<"xxxxx"<<left << setw(15)<<dipole_dir_1[j][i]<<left << setw(15)<<"xxxxx"<<left << setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;
                     
                    case 1:
                        if (Updated_matrix[j-1][i+1]==0)
                        {
                           	Updated_matrix[j-1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i+1]=dipole_dir_2[j-1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j-1][i+1]<<endl;
                        }
                        else
                        {
                           Updated_matrix[j][i]=cell_matrix_1[j][i];
                           Updated_direction[j][i]=dipole_dir_1[j][i];
                           No_Dens = No_Dens+1;
						   //myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 2:
                        if (Updated_matrix[j-1][i]==0)
                        {
                           	Updated_matrix[j-1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i]=dipole_dir_2[j-1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						  	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j-1][i]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 3:
                        if (Updated_matrix[j-1][i-1]==0)
                        {
                           	Updated_matrix[j-1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i-1]=dipole_dir_2[j-1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j-1][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 4:
                        if (Updated_matrix[j][i-1]==0)
                        {
                           	Updated_matrix[j][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i-1]=dipole_dir_2[j][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }	
                    break;
                     
                    case 5:
                        if (Updated_matrix[j+1][i-1]==0)
                        {
                           	Updated_matrix[j+1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i-1]=dipole_dir_2[j+1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j+1][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;
                     
                    case 6:
                        if (Updated_matrix[j+1][i]==0)
                        {
                           	Updated_matrix[j+1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i]=dipole_dir_2[j+1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_Y=Mig_Y+1;
						   	Dens_Migration = Dens_Migration+1;
						   	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j+1][i]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 7:
                        if (Updated_matrix[j+1][i+1]==0)
                        {
                           	Updated_matrix[j+1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i+1]=dipole_dir_2[j+1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j+1][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break; 
                    
                    case 8: // if there wasn't a great difference in cell density, the difference in stiffness will guide migration or not
                        if (cell_def_1[j][i]>cell_def_2[j][i+1] && Updated_matrix[j][i+1]==0)
                        {
                           	Updated_matrix[j][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i+1]=dipole_dir_2[j][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j][i+1]<<left<< setw(15)<<Updated_direction[j][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j][i+1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }  
                    break;
                     
                    case 9:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i+1] && Updated_matrix[j-1][i+1]==0)
                        {
                           	Updated_matrix[j-1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i+1]=dipole_dir_2[j-1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i+1]<<left<< setw(15)<<Updated_direction[j-1][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i+1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 10:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i] && Updated_matrix[j-1][i]==0)
                        {
                           	Updated_matrix[j-1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i]=dipole_dir_2[j-1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i]<<left<< setw(15)<<Updated_direction[j-1][i]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 11:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i-1] && Updated_matrix[j-1][i-1]==0)
                        {
                           	Updated_matrix[j-1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i-1]=dipole_dir_2[j-1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i-1]<<left<< setw(15)<<Updated_direction[j-1][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i-1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 12:
                        if (cell_def_1[j][i]>cell_def_2[j][i-1] && Updated_matrix[j][i-1]==0)
                        {
                           	Updated_matrix[j][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i-1]=dipole_dir_2[j][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j][i-1]<<left<< setw(15)<<Updated_direction[j][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j][i-1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;
                     
                    case 13:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i-1] && Updated_matrix[j+1][i-1]==0)
                        {
                           	Updated_matrix[j+1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i-1]=dipole_dir_2[j+1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i-1]<<left<< setw(15)<<Updated_direction[j+1][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i-1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;
                     
                    case 14:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i] && Updated_matrix[j+1][i]==0)
                        {
                           	Updated_matrix[j+1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i]=dipole_dir_2[j+1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i]<<left<< setw(15)<<Updated_direction[j+1][i]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 15:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i+1] && Updated_matrix[j+1][i+1]==0)
                        {
                           	Updated_matrix[j+1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i+1]=dipole_dir_2[j+1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i+1]<<left<< setw(15)<<Updated_direction[j+1][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i+1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break; 	
                } // End switch                                   
            } // if (cell_matrix_1[j][i]>1)
        } // End for i
    } // End for j
     
    for (j=round((n-1)/2+1);j<n;j++)
    {
        for (i=0;i<n;i++)
        {
//			node=i+(j*121)+1;
            if (cell_matrix_1[j][i]>2)
            {
                cnt_cell_1=cnt_cell_1+1;
                 
                switch (migration[j][i])
                {
                    case 99:
                        Updated_matrix[j][i]=cell_matrix_1[j][i]; 
                        Updated_direction[j][i]=dipole_dir_1[j][i]; 
                    break;
                    
                    case 88:
                        Updated_matrix[j][i]=cell_matrix_1[j][i]; 
                        Updated_direction[j][i]=dipole_dir_1[j][i];
						No_Space=No_Space+1; 
                    break;
                     
                    case 0:
                        if (Updated_matrix[j][i+1]==0)
                        {
                           	Updated_matrix[j][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i+1]=dipole_dir_2[j][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<<setw(15)<<"xxxxx"<<left << setw(15)<<dipole_dir_1[j][i]<<left << setw(15)<<"xxxxx"<<left << setw(15)<<Updated_direction[j][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<<setw(15)<<"xxxxx"<<left << setw(15)<<dipole_dir_1[j][i]<<left << setw(15)<<"xxxxx"<<left << setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;
                     
                    case 1:
                        if (Updated_matrix[j-1][i+1]==0)
                        {
                           	Updated_matrix[j-1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i+1]=dipole_dir_2[j-1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j-1][i+1]<<endl;
                        }
                        else
                        {
                           Updated_matrix[j][i]=cell_matrix_1[j][i];
                           Updated_direction[j][i]=dipole_dir_1[j][i];
                           No_Dens = No_Dens+1;
						   //myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 2:
                        if (Updated_matrix[j-1][i]==0)
                        {
                           	Updated_matrix[j-1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i]=dipole_dir_2[j-1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						  	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j-1][i]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 3:
                        if (Updated_matrix[j-1][i-1]==0)
                        {
                           	Updated_matrix[j-1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i-1]=dipole_dir_2[j-1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j-1][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 4:
                        if (Updated_matrix[j][i-1]==0)
                        {
                           	Updated_matrix[j][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i-1]=dipole_dir_2[j][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }	
                    break;
                     
                    case 5:
                        if (Updated_matrix[j+1][i-1]==0)
                        {
                           	Updated_matrix[j+1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i-1]=dipole_dir_2[j+1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j+1][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;
                     
                    case 6:
                        if (Updated_matrix[j+1][i]==0)
                        {
                           	Updated_matrix[j+1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i]=dipole_dir_2[j+1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_Y=Mig_Y+1;
						   	Dens_Migration = Dens_Migration+1;
						   	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j+1][i]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 7:
                        if (Updated_matrix[j+1][i+1]==0)
                        {
                           	Updated_matrix[j+1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i+1]=dipole_dir_2[j+1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Dens_Migration = Dens_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j+1][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Dens = No_Dens+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<"xxxxx"<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break; 
                    
                    case 8:
                        if (cell_def_1[j][i]>cell_def_2[j][i+1] && Updated_matrix[j][i+1]==0)
                        {
                           	Updated_matrix[j][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i+1]=dipole_dir_2[j][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j][i+1]<<left<< setw(15)<<Updated_direction[j][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j][i+1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }  
                    break;
                     
                    case 9:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i+1] && Updated_matrix[j-1][i+1]==0)
                        {
                           	Updated_matrix[j-1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i+1]=dipole_dir_2[j-1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i+1]<<left<< setw(15)<<Updated_direction[j-1][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i+1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 10:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i] && Updated_matrix[j-1][i]==0)
                        {
                           	Updated_matrix[j-1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i]=dipole_dir_2[j-1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i]<<left<< setw(15)<<Updated_direction[j-1][i]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 11:
                        if (cell_def_1[j][i]>cell_def_2[j-1][i-1] && Updated_matrix[j-1][i-1]==0)
                        {
                           	Updated_matrix[j-1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j-1][i-1]=dipole_dir_2[j-1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i-1]<<left<< setw(15)<<Updated_direction[j-1][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j-1][i-1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 12:
                        if (cell_def_1[j][i]>cell_def_2[j][i-1] && Updated_matrix[j][i-1]==0)
                        {
                           	Updated_matrix[j][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j][i-1]=dipole_dir_2[j][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j][i-1]<<left<< setw(15)<<Updated_direction[j][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j][i-1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;
                     
                    case 13:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i-1] && Updated_matrix[j+1][i-1]==0)
                        {
                           	Updated_matrix[j+1][i-1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i-1]=dipole_dir_2[j+1][i-1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i-1]<<left<< setw(15)<<Updated_direction[j+1][i-1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i-1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;
                     
                    case 14:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i] && Updated_matrix[j+1][i]==0)
                        {
                           	Updated_matrix[j+1][i]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i]=dipole_dir_2[j+1][i];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i]<<left<< setw(15)<<Updated_direction[j+1][i]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break;

                    case 15:
                        if (cell_def_1[j][i]>cell_def_2[j+1][i+1] && Updated_matrix[j+1][i+1]==0)
                        {
                           	Updated_matrix[j+1][i+1]=cell_matrix_1[j][i];
                           	Updated_direction[j+1][i+1]=dipole_dir_2[j+1][i+1];
                           	cnt_upd_cell=cnt_upd_cell+1;
                           	Mig_X=Mig_X+1;
                           	Mig_Y=Mig_Y+1;
                           	Stif_Migration = Stif_Migration+1;
                           	//Migration.push_back(cell_matrix_1[j][i]);
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i+1]<<left<< setw(15)<<Updated_direction[j+1][i+1]<<endl;
                        }
                        else
                        {
                           	Updated_matrix[j][i]=cell_matrix_1[j][i];
                           	Updated_direction[j][i]=dipole_dir_1[j][i];
                           	No_Stiff = No_Stiff+1;
						   	//myfile_Def_Com<<left<< setw(15)<<node<<left<< setw(15)<<cell_def_1[j][i]<<left<< setw(15)<<dipole_dir_1[j][i]<<left<< setw(15)<<cell_def_2[j+1][i+1]<<left<< setw(15)<<Updated_direction[j][i]<<endl;
                        }
                    break; 	
                } // End switch                                   
            } // if (cell_matrix_1[j][i]>1)
        } // End for i
    } // End for j
     
//	 myfile_Def_Com.close();  
	 
//    cout<<"number selected cells : "<<cnt_cell_1<<endl;
//    cout<<"No Density: "<<No_Dens<<endl;
//    cout<<"No Stiffness: "<<No_Stiff<<endl;
//    cout<<"No Space: "<<No_Space<<endl;
//    cout<<"Density driven: "<<Dens_Migration<<endl;
//    cout<<"Stiffness driven "<<Stif_Migration<<endl;
//    cout<<"number_updated cells: "<<cnt_upd_cell<<endl;
     
//****************************************************************************************************************************************************************
//                														Writing cells migration direction
//**************************************************************************************************************************************************************** 
//
//	cout<<" ---> Writing migration information "<<endl;
//
//    ofstream myfile_Mig;
//    myfile_Mig.open("Migration_info.txt", ios::app);
//    
//	if (iter==0)
//    {     
//    	myfile_Mig<<"iter"<<" "<<"Mig_X"<<" "<<"Mig_Y"<<" "<<"Updated_Cell"<<" "<<"Dens_Migration"<<" "<<"Stif_Migration"<<" "<<"No_Space"<<endl;
//		myfile_Mig<<iter<<" "<<Mig_X<<" "<<Mig_Y<<" "<<cnt_upd_cell<<" "<<Dens_Migration<<" "<<Stif_Migration<<" "<<No_Space<<endl;
//	}
//	else
//    {
//		myfile_Mig<<iter<<" "<<Mig_X<<" "<<Mig_Y<<" "<<cnt_upd_cell<<" "<<Dens_Migration<<" "<<Stif_Migration<<" "<<No_Space<<endl; 
//	}   
//	
//	myfile_Mig.close(); 
//	
// 	cout<<" <--- Migration information calculated and wrote successfully "<<endl;
 
//****************************************************************************************************************************************************************
//                     											Writing migrated cells in a file
//****************************************************************************************************************************************************************
//       ostringstream M1;
//       if (iter<10)
//     	{
//	 		M1<<"Migration_000"<<iter<<".txt";
// 	 	}
// 	   else if (iter<100)
//     	{
//	 		M1<<"Migration_00"<<iter<<".txt";
// 	 	} 
// 	   else if (iter<1000)
//     	{
//	 		M1<<"Migration_0"<<iter<<".txt";
// 	 	} 
// 	   else 
//     	{
//	 		M1<<"Migration_"<<iter<<".txt";
// 	 	} 
//    
//       string Mig1=M1.str();
//       ofstream myfile_Mig1;

//       myfile_Mig1.open(Mig1.c_str());
//	   int Mi;
//	   sort( Migration.begin(), Migration.end() );  // sorting the elements of Mig_posibility vector
//	   for (vector<int>::iterator Mi=Migration.begin(); Mi != Migration.end(); ++Mi)
//        {
//          myfile_Mig1<<*Mi<<" "<<endl;
//          }

//****************************************************************************************************************************************************************
//                     										Writing nodes Deformations
//****************************************************************************************************************************************************************
//cout<<" ---> Writing node deformation "<<endl;

//    int node2;
//    ostringstream D1;
//    if (iter<10)
//    {
//	 D1<<"Deformation_000"<<iter<<".txt";
// 	}
// 	else if (iter<100)
//    {
//	 D1<<"Deformation_00"<<iter<<".txt";
// 	} 
// 	else if (iter<1000) 
//    {
//	 D1<<"Deformation_0"<<iter<<".txt";
// 	}
//	else
//    {
//	 D1<<"Deformation_"<<iter<<".txt";
// 	} 
//    
//    string Def=D1.str();
//    ofstream myfile_Def;
//    myfile_Def.open(Def.c_str());
//    
//    
//     for (j=0;j<121;j++)
//     {
//         for (i=0;i<121;i++)
//         {
//              node2=i+(j*121)+1;
//              myfile_Def<<node2<<" "<<j<<" "<<i<<" "<<Updated_matrix[j][i]<<" "<<cell_def_2[j][i]<<endl;
//         }
//     }
//     
//    myfile_Def.close();  
//     
//cout<<" <--- Node deformations wrote successfully "<<endl;
               
//****************************************************************************************************************************************************************
//                    											Check number of cells
//****************************************************************************************************************************************************************
    
	int cells_1, cells_2;
    cells_1=0;
    cells_2=0;
     
    for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
        {
            if (cell_matrix_1[j][i]>2)
            {
                cells_1=cells_1+1;             
            }
            if (Updated_matrix[j][i]>2)
            {
                cells_2=cells_2+1;             
            }
        }
    }
     
    cout<<"Iteration: "<<iter<<endl;  
    cout<<"Number of fibroblasts in FEM_"<<iter<<":"<<cells_1<<endl;
    cout<<"Number of fibroblasts in Updated Model for Iteration - No."<<iter+1<<":"<<cells_2<<endl;  
      
//****************************************************************************************************************************************************************
//                 												Update the cell matrix
//****************************************************************************************************************************************************************
    
	cout<<"Update the Cell Matrix "<<endl; 

    for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
        {
            cell_matrix_1[j][i]=Updated_matrix[j][i];
            dipole_dir_1[j][i]=Updated_direction[j][i];
             
        }
    }

    cout<<"Cell Matrix Updated Successfully"<<endl;
     
//*********************************************************************************************************************************************************************************************************
//               													Writing Cell Orientations of whole model
//*********************************************************************************************************************************************************************************************************

//	cout<<" ---> Writing cells orientation information "<<endl;
//
//    int LcellW, AcellW, DUcellW, DDcellW; // Cell in whole model
//    int LcellG, AcellG, DUcellG, DDcellG; // Cell in gap area
//    int Lcell1, Acell1, DUcell1, DDcell1; // Cell in region 1
//    int Lcell2, Acell2, DUcell2, DDcell2; // Cell in region 2
//    int Lcell3, Acell3, DUcell3, DDcell3; // Cell in region 3
//    int Lcell4, Acell4, DUcell4, DDcell4; // Cell in region 4
//    int Lcell5, Acell5, DUcell5, DDcell5; // Cell in region 5
//    int Lcell6, Acell6, DUcell6, DDcell6; // Cell in region 6
//    int Lcell7, Acell7, DUcell7, DDcell7; // Cell in region 7
//
//    LcellW=0;    // Number of Lateral cell "-"
//    DDcellW=0;   // Number of Diagonal Right cell "/"
//	AcellW=0;    // Number of Axial cell "|"
//    DUcellW=0;   // Number of Diagonal Left cell "\"
//    
//    for (j=0;j<121;j++)
//	{
//	    for (i=0;i<121;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               LcellW=LcellW+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcellW=DDcellW+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               AcellW=AcellW+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcellW=DUcellW+1;                      
//            }
//	    }
//	}
//	
//    ofstream myfile_Ori;    
//    myfile_Ori.open("Orientation_info.txt", ios::app);
//    
//    if (iter==0)
//    {
//    	myfile_Ori<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori<<iter<<"      "<<AcellW<<"   "<<DUcellW+DDcellW<<"     "<<LcellW<<"    "<<DDcellW<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori<<iter<<"      "<<AcellW<<"   "<<DUcellW+DDcellW<<"     "<<LcellW<<"    "<<DDcellW<<"    "<<endl;
//    }
//    
//    myfile_Ori.close(); 
//
////*********************************************************************************************************************************************************************************************************
////                              												 Writing Cell Orientations of the gap area
////*********************************************************************************************************************************************************************************************************
//    
//    LcellG=0;    // Number of Lateral cell "-"
//    DDcellG=0;   // Number of Diagonal Right cell "/"
//	AcellG=0;    // Number of Axial cell "|"
//    DUcellG=0;   // Number of Diagonal Left cell "\"
//    
//    for (j=11;j<110;j++)
//	{
//	    for (i=0;i<121;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               LcellG=LcellG+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcellG=DDcellG+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               AcellG=AcellG+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcellG=DUcellG+1;                      
//            }
//	    }
//	}
//	
//    ofstream myfile_Ori_gap;    
//    myfile_Ori_gap.open("Orientation_info_gap.txt", ios::app);
//    
//       if (iter==0)
//    {
//    	myfile_Ori_gap<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_gap<<iter<<"      "<<AcellG<<"   "<<DUcellG+DDcellG<<"     "<<LcellG<<"    "<<DDcellG<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_gap<<iter<<"      "<<AcellG<<"   "<<DUcellG+DDcellG<<"     "<<LcellG<<"    "<<DDcellG<<"    "<<endl;
//    }
//    
//    
//    myfile_Ori_gap.close(); 
////********************************************************************************************************************************************************************************************************
////                   															Defining Region of Interest (ROI)
////									
////              				  110   -------------------------
////					 					|                        |	
////					 					|         ROI 7          |	
////								  94    |------------------------|	
////					 					|                        |	
////					 					|         ROI 6          |	
////								  78    |------------------------|	
////					 					|ROI 3  | ROI 4  | ROI 5 |	
////					 					|       |        |       |	
////					 			  43	|------------------------|	
////					 					|         ROI 2          |	
////					 					|                        |
////					 			  27	|------------------------|
////					 					|         ROI 1          |	
////					 					|                        |	
////				     		      11  	--------------------------
////                           					40       80       
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of ROI No.1
////*********************************************************************************************************************************************************************************************************
//
//	cout<<" ---> Writing cells orientation information of ROIs "<<endl;
//
//    Lcell1=0;    // Number of Lateral cell "-"
//    DDcell1=0;   // Number of Diagonal Right cell "/"
//	Acell1=0;    // Number of Axial cell "|"
//    DUcell1=0;   // Number of Diagonal Left cell "\"
//    
//    for (j=11;j<27;j++)
//	{
//	    for (i=0;i<121;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               Lcell1=Lcell1+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcell1=DDcell1+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               Acell1=Acell1+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcell1=DUcell1+1;                      
//            }
//	    }
//	}
//	
//    ofstream myfile_Ori_ROI1;    
//    myfile_Ori_ROI1.open("Orientation_info_ROI1.txt", ios::app);
//    
//    if (iter==0)
//    {
//    	myfile_Ori_ROI1<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_ROI1<<iter<<"      "<<Acell1<<"   "<<DUcell1+DDcell1<<"     "<<Lcell1<<"    "<<DDcell1<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_ROI1<<iter<<"      "<<Acell1<<"   "<<DUcell1+DDcell1<<"     "<<Lcell1<<"    "<<DDcell1<<"    "<<endl;
//    }
//    
//    myfile_Ori_ROI1.close(); 
//     
//
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of ROI No.2
////*********************************************************************************************************************************************************************************************************
//
//    Lcell2=0;    // Number of Lateral cell "-"
//    DDcell2=0;   // Number of Diagonal Right cell "/"
//	Acell2=0;    // Number of Axial cell "|"
//    DUcell2=0;   // Number of Diagonal Left cell "\"
//    
//    for (j=27;j<43;j++)
//	{
//	    for (i=0;i<121;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               Lcell2=Lcell2+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcell2=DDcell2+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               Acell2=Acell2+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcell2=DUcell2+1;                      
//            }
//	    }
//	}
//
//    ofstream myfile_Ori_ROI2;    
//    myfile_Ori_ROI2.open("Orientation_info_ROI2.txt", ios::app);
//     
//    if (iter==0)
//    {
//    	myfile_Ori_ROI2<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_ROI2<<iter<<"      "<<Acell2<<"   "<<DUcell2+DDcell2<<"     "<<Lcell2<<"    "<<DDcell2<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_ROI2<<iter<<"      "<<Acell2<<"   "<<DUcell2+DDcell2<<"     "<<Lcell2<<"    "<<DDcell2<<"    "<<endl;
//    }
//     
//    myfile_Ori_ROI2.close(); 
//     
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of ROI No.3
////*********************************************************************************************************************************************************************************************************
//
//    Lcell3=0;    // Number of Lateral cell "-"
//    DDcell3=0;   // Number of Diagonal Right cell "/"
//	Acell3=0;    // Number of Axial cell "|"
//    DUcell3=0;   // Number of Diagonal Left cell "\"
//	    
//    for (j=43;j<78;j++)
//	{
//	    for (i=0;i<41;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               Lcell3=Lcell3+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcell3=DDcell3+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               Acell3=Acell3+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcell3=DUcell3+1;                      
//            }
//	    }
//	}
//    
//    ofstream myfile_Ori_ROI3;    
//    myfile_Ori_ROI3.open("Orientation_info_ROI3.txt", ios::app);   
//    
//    if (iter==0)
//    {
//    	myfile_Ori_ROI3<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_ROI3<<iter<<"      "<<Acell3<<"   "<<DUcell3+DDcell3<<"     "<<Lcell3<<"    "<<DDcell3<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_ROI3<<iter<<"      "<<Acell3<<"   "<<DUcell3+DDcell3<<"     "<<Lcell3<<"    "<<DDcell3<<"    "<<endl;
//    }
//    
//    myfile_Ori_ROI3.close(); 
//
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of ROI No.4
////*********************************************************************************************************************************************************************************************************
//
//    Lcell4=0;    // Number of Lateral cell "-"
//    DDcell4=0;   // Number of Diagonal Right cell "/"
//	Acell4=0;    // Number of Axial cell "|"
//    DUcell4=0;   // Number of Diagonal Left cell "\"
//    
//    for (j=43;j<78;j++)
//	{
//	    for (i=41;i<81;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               Lcell4=Lcell4+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcell4=DDcell4+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               Acell4=Acell4+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcell4=DUcell4+1;                      
//            }
//	    }
//	}
//	
//    ofstream myfile_Ori_ROI4;    
//    myfile_Ori_ROI4.open("Orientation_info_ROI4.txt", ios::app);
//    
//    if (iter==0)
//    {
//    	myfile_Ori_ROI4<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_ROI4<<iter<<"      "<<Acell4<<"   "<<DUcell4+DDcell4<<"     "<<Lcell4<<"    "<<DDcell4<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_ROI4<<iter<<"      "<<Acell4<<"   "<<DUcell4+DDcell4<<"     "<<Lcell4<<"    "<<DDcell4<<"    "<<endl;
//    }
//     
//    myfile_Ori_ROI4.close(); 
//
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of ROI No.5
////*********************************************************************************************************************************************************************************************************
//
//    Lcell5=0;    // Number of Lateral cell "-"
//    DDcell5=0;   // Number of Diagonal Right cell "/"
//	Acell5=0;    // Number of Axial cell "|"
//    DUcell5=0;   // Number of Diagonal Left cell "\"
//    
//    for (j=43;j<78;j++)
//	{
//	    for (i=81;i<121;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               Lcell5=Lcell5+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcell5=DDcell5+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               Acell5=Acell5+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcell5=DUcell5+1;                      
//            }
//	    }
//	}
//	
//    ofstream myfile_Ori_ROI5;    
//    myfile_Ori_ROI5.open("Orientation_info_ROI5.txt", ios::app);
//    
//    if (iter==0)
//    {
//    	myfile_Ori_ROI5<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_ROI5<<iter<<"      "<<Acell5<<"   "<<DUcell5+DDcell5<<"     "<<Lcell5<<"    "<<DDcell5<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori<<iter<<"      "<<Acell5<<"   "<<DUcell5+DDcell5<<"     "<<Lcell5<<"    "<<DDcell5<<"    "<<endl;
//    }
//   
//    myfile_Ori_ROI5.close();   
//
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of ROI No.6
////*********************************************************************************************************************************************************************************************************
//    Lcell6=0;    // Number of Lateral cell "-"
//    DDcell6=0;   // Number of Diagonal Right cell "/"
//	Acell6=0;    // Number of Axial cell "|"
//    DUcell6=0;   // Number of Diagonal Left cell "\"
//    
//    for (j=78;j<94;j++)
//	{
//	    for (i=0;i<121;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               Lcell6=Lcell6+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcell6=DDcell6+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               Acell6=Acell6+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcell6=DUcell6+1;                      
//            }
//	    }
//	}
//
//    ofstream myfile_Ori_ROI6;    
//    myfile_Ori_ROI6.open("Orientation_info_ROI6.txt", ios::app);
//    
//    if (iter==0)
//    {
//    	myfile_Ori_ROI6<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_ROI6<<iter<<"      "<<Acell6<<"   "<<DUcell6+DDcell6<<"     "<<Lcell6<<"    "<<DDcell6<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_ROI6<<iter<<"      "<<Acell6<<"   "<<DUcell6+DDcell6<<"     "<<Lcell6<<"    "<<DDcell6<<"    "<<endl;
//    }
//  
//    myfile_Ori_ROI6.close(); 
//
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of ROI No.7
////*********************************************************************************************************************************************************************************************************
//
//    Lcell7=0;    // Number of Lateral cell "-"
//    DDcell7=0;   // Number of Diagonal Right cell "/"
//	Acell7=0;    // Number of Axial cell "|"
//    DUcell7=0;   // Number of Diagonal Left cell "\"
//    
//    for (j=94;j<110;j++)
//	{
//	    for (i=0;i<121;i++)
//	    {
//	        if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==1 )
//	        {
//               Lcell7=Lcell7+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==2 )
//	        {
//               DDcell7=DDcell7+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==3 )
//	        {
//               Acell7=Acell7+1;                      
//            }
//            else if (cell_matrix_1[j][i]>2 && dipole_dir_1[j][i]==4 )
//	        {
//               DUcell7=DUcell7+1;                      
//            }
//	    }
//	}
//
//    ofstream myfile_Ori_ROI7;    
//    myfile_Ori_ROI7.open("Orientation_info_ROI7.txt", ios::app);
//    
//    if (iter==0)
//    {
//    	myfile_Ori_ROI7<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_ROI7<<iter<<"      "<<Acell7<<"   "<<DUcell7+DDcell7<<"     "<<Lcell7<<"    "<<DDcell7<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_ROI7<<iter<<"      "<<Acell7<<"   "<<DUcell7+DDcell7<<"     "<<Lcell7<<"    "<<DDcell7<<"    "<<endl;
//    }
//    
//    myfile_Ori_ROI7.close(); 
//
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of Periphery regions
////*********************************************************************************************************************************************************************************************************
//
//    ofstream myfile_Ori_Periphery;    
//    myfile_Ori_Periphery.open("Orientation_info_Periphery.txt", ios::app);
//    
//    if (iter==0)
//    {
//    	myfile_Ori_Periphery<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_Periphery<<iter<<"      "<<Acell1+Acell7<<"   "<<DUcell1+DUcell7+DDcell1+DDcell7<<"     "<<Lcell1+Lcell7<<"    "<<DDcell1+DDcell7<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_Periphery<<iter<<"      "<<Acell1+Acell7<<"   "<<DUcell1+DUcell7+DDcell1+DDcell7<<"     "<<Lcell1+Lcell7<<"    "<<DDcell1+DDcell7<<"    "<<endl;
//    }
//    
//    myfile_Ori_Periphery.close(); 
//    
////*********************************************************************************************************************************************************************************************************
////                    														 Writing Cell Orientations of Middle regions
////*********************************************************************************************************************************************************************************************************
//
//
//    ofstream myfile_Ori_Middle;    
//    myfile_Ori_Middle.open("Orientation_info_Middle.txt", ios::app);
//    
//    if (iter==0)
//    {
//    	myfile_Ori_Middle<<"iter"<<"  "<<"Axial"<<"  "<<"Diagonal"<<"  "<<"Lateral"<<"  "<<"-45"<<" "<<endl;
//    	myfile_Ori_Middle<<iter<<"      "<<Acell3+Acell4+Acell5<<"   "<<DUcell3+DUcell4+DUcell5+DDcell3+DDcell4+DDcell5<<"     "<<Lcell3+Lcell4+Lcell5<<"    "<<DDcell3+DDcell4+DDcell5<<"    "<<endl;
//    }
//    else
//    {
//    	myfile_Ori_Middle<<iter<<"      "<<Acell3+Acell4+Acell5<<"   "<<DUcell3+DUcell4+DUcell5+DDcell3+DDcell4+DDcell5<<"     "<<Lcell3+Lcell4+Lcell5<<"    "<<DDcell3+DDcell4+DDcell5<<"    "<<endl;
//    }
//    
//    myfile_Ori_Middle.close();      
//     
//cout<<" <--- Cells orientation information wrote successfully "<<endl;
//        
}

