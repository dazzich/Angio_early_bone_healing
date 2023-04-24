#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <string>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <vector>
#include <iomanip>
#include <limits> // Press inter to continue
#include "header.h"

using namespace std;

void Create_temporal_FEM(int cell_1[N][N], int cell_2[N][N], int direction_2[N][N], int iter, int migration_dir[N][N],int& number_vessels_temp, int tip_x_temp[(N-1)*20], int tip_y_temp[(N-1)*20], double X_temp[N*N],double Y_temp[N*N],int previous_dir_temp [(N-1)*20])
{
    int i,j;

//*******************************************************************************************************************************************************
//                    										Initialize new cell matrix
//*******************************************************************************************************************************************************
    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            cell_2[j][i]=cell_1[j][i]; 
            direction_2[j][i]=0; 
            migration_dir[j][i]=99; 
        }
    }

    int cell_selected[N][N]; 
    int cnt_selected;
    cnt_selected=0; //number of migrating cells

    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            cell_selected[j][i]=0; //initialize matrix that will contain a 3 when a cell is selected for migration
        }
    }

    int migration, Cell_density, Cell_dens0, Cell_dens1, Cell_dens2, Cell_dens3, Cell_dens4, Cell_dens5, Cell_dens6, Cell_dens7, random;
    int Dens_Case, Stiff_Case;
    Dens_Case = 0;
    Stiff_Case=0;

    for (j=0;j<N;j++)
	{
	    for (i=0;i<N;i++)
	    {
           if (cell_2[j][i]>2 && cell_selected[j][i]==0) // We are selecting ALL fibroblasts to move 
           {
               cnt_selected=cnt_selected+1; //number of migrating cells
               cell_selected[j][i]=3; //cell already selected
           }
    	}
	}

	cout<<"Number of cells selected:  "<<cnt_selected<<endl;
//*******************************************************************************************************************************************************
//                														Calculating cell density around the current position
//                          5 | 6 | 7
//			               ------------
//                          4 | C | 0
//		             	   ------------
//			                3 | 2 | 1
//
//      0 = j,   i+1
// 		1 = j-1, i+1
// 		2 = j-1,  i
// 		3 = j-1, i-1
// 		4 = j,   i-1
// 		5 = j+1, i-1
// 		6 = j+1,  i
// 		7 = j+1, i+1
//*******************************************************************************************************************************************************
		
//***************************************************************************************************************************************************
//                              						Moving first half (j=0 to j=(N-1)/2) of the model 
//***************************************************************************************************************************************************	
	
	for (j=round((N-1)/2);j>=0;j--) // rows from the middle towords the bottom of the model 
	{
	    for (i=0;i<N;i++) // all the columns
	    {
		    if (cell_selected[j][i]==3) //if that cell was selected to migrate
            {
	         	Cell_density = 0; //number of cells around current position
			 	Cell_dens0 = 0; //number of cells around position 0 (excluding current pos C)
			 	Cell_dens1 = 0; //and so on for the other positions
			 	Cell_dens2 = 0;
			 	Cell_dens3 = 0;
			 	Cell_dens4 = 0;
			 	Cell_dens5 = 0;
			 	Cell_dens6 = 0;
			 	Cell_dens7 = 0;
			 	migration=0;
			 	vector<int>Availability;
	         	Availability.clear();
	         	vector<int>Min_dens;
	         	Min_dens.clear();
//*******************************************************************************************************************************************************
//      												Calculating cell density around the current position of selected cell
//*******************************************************************************************************************************************************
				if (i+1 && cell_2[j][i+1]>2) // Position 0
            	{
           	   		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	   		Availability.push_back(0); 
            	}
            	if (j-1>0 && i+1<N && cell_2[j-1][i+1]>2 ) // Position 1
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(1);
            	}
            	if (j-1>0 && cell_2[j-1][i]>2) // Position 2
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(2);
            	}	
            	if (j-1>0 && i-1>=0 && cell_2[j-1][i-1]>2) // Position 3
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(3);
            	}
            	if (i-1>=0, cell_2[j][i-1]>2) // Position 4
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(4);
            	}
            	if (j+1<N && i-1>=0 && cell_2[j+1][i-1]>2) // Position 5
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(5);
            	}
            	if (j+1<N && cell_2[j+1][i]>2) // Position 6
            	{
	           	  	Cell_density = Cell_density+1;
	            }
	            else
	            {
	           	  	Availability.push_back(6);
	            }
	            if (j+1<N && i+1<N && cell_2[j+1][i+1]>2) // Position 7
	            {
	           	  	Cell_density = Cell_density+1;
	            }
	            else
	            {
	           	  	Availability.push_back(7);
	            }
	            
//*******************************************************************************************************************************************************
//      											Calculating cell density around position 0 - cell_2[j][i+1] "Cell_dens0"
//*******************************************************************************************************************************************************

        		if (i+2<N && cell_2[j][i+2]>2) //Position 0 for Position 0
            	{
           	  		Cell_dens0 = Cell_dens0+1;
            	}
	            if (j-1>0 && i+2<N && cell_2[j-1][i+2]>2 )  //Position 1 for Position 0
	            {
	           	  	Cell_dens0 = Cell_dens0+1;
	            }
	            if (j-1>0 && i+1<N && cell_2[j-1][i+1]>2)  //Position 2 for Position 0
	            {
		       	  	Cell_dens0 = Cell_dens0+1;
		        }
		       	if (j-1>0 && cell_2[j-1][i]>2) //Position 3 for Position 0
		       	{
		          	Cell_dens0 = Cell_dens0+1;
		        }
//           	if (cell_2[j][i]>2)  //Position 4 for Position 0
//           	{
//           	 	Cell_dens0 = Cell_dens0+1;
//           	}
	        	if (j+1<N && cell_2[j+1][i]>2) //Position 5 for Position 0
	        	{
	       	  		Cell_dens0 = Cell_dens0+1;
	        	}
	        	if (j+1<N && i+1<N && cell_2[j+1][i+1]>2) //Position 6 for Position 0
	        	{
	       	  		Cell_dens0 = Cell_dens0+1;
	        	}
	        	if (j+1<N && i+2<N && cell_2[j+1][i+2]>2)  //Position 7 for Position 0
	        	{
	       	  		Cell_dens0 = Cell_dens0+1;
	        	}
	        	
//*******************************************************************************************************************************************************
//      											Calculating cell density of position 1 - cell_2[j-1][i+1] "Cell_dens1"
//*******************************************************************************************************************************************************	        	

		        if (j-1>0 && i+2<N && cell_2[j-1][i+2]>2)  //Position 0 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
		        if (j-2>0 && i+2<N && cell_2[j-2][i+2]>2 ) //Position 1 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
		        if (j-2>0 && i+1<N && cell_2[j-2][i+1]>2)  //Position 2 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
		        if (j-2>0 && cell_2[j-2][i]>2) //Position 3 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
		        if (j-1>0 && cell_2[j-1][i]>2)  //Position 4 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
//	        	if (cell_2[j][i]>2)  //Position 5 for Position 1 //it's the current position C
//	        	{
//	       	  		Cell_dens1 = Cell_dens1+1;
//	        	}
		        if (i+1<N && cell_2[j][i+1]>2) //Position 6 for Position 1
		        {
		         	Cell_dens1 = Cell_dens1+1;
		        }
		        if (i+2<N && cell_2[j][i+2]>2) //Position 7 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }

//*******************************************************************************************************************************************************
//      											Calculating cell density of position 2 - cell_2[j-1][i] "Cell_dens2"
//*******************************************************************************************************************************************************
		        
	            if (j-1>0 && i+1<N && cell_2[j-1][i+1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (j-2>0 && i+1<N && cell_2[j-2][i+1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (j-2>0 && cell_2[j-2][i]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (j-2>0 && i-1>=0 && cell_2[j-2][i-1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (j-1>0 && i-1>=0 && cell_2[j-1][i-1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (i-1>=0 && cell_2[j][i-1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
//	            if (cell_2[j][i]>2)
//	            {
//	           	  	Cell_dens2 = Cell_dens2+1;
//	            }
	            if (i+1<N && cell_2[j][i+1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
//*******************************************************************************************************************************************************
//      											Calculating cell density of position 3 - cell_2[j-1][i-1] "Cell_dens3"
//*******************************************************************************************************************************************************

	            if (j-1>0 &&  cell_2[j-1][i]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
	            if (j-2>0 && cell_2[j-2][i]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
	            if (j-2>0 && i-1>=0 && cell_2[j-2][i-1]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
	            if (j-2>0 && i-2>=0 && cell_2[j-2][i-2]>2)
	            {
	           		Cell_dens3 = Cell_dens3+1;
	            }
	            if (j-1>0 && i-2>=0 && cell_2[j-1][i-2]>2)
	            {
	              	Cell_dens3 = Cell_dens3+1;
	            }
	            if (i-2>=0 && cell_2[j][i-2]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
	            if (i-1>=0 && cell_2[j][i-1]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
//            	if (cell_2[j][i]>2)
//            	{
//           	  	Cell_dens3 = Cell_dens3+1;
//            	}

//*******************************************************************************************************************************************************
//      											Calculating cell density of position 4 - cell_2[j][i-1] "Cell_dens4"
//*******************************************************************************************************************************************************            

//            	if (cell_2[j][i]>2) //Position 0 for Position 4
//            	{
//           	  	Cell_dens4 = Cell_dens4+1;
//            	}
	            if (j-1>0 && cell_2[j-1][i]>2) //Position 1 for Position 4
	            {
	              Cell_dens4 = Cell_dens4+1;
	            }
	            if (j-1>0 && i-1>=0 && cell_2[j-1][i-1]>2) //Position 2 for Position 4
	            {
	           	  	Cell_dens4 = Cell_dens4+1;
            	}
             	if (j-1>0 && i-2>=0 && cell_2[j-1][i-2]>2) //Position 3 for Position 4
             	{
           	 	 	Cell_dens4 = Cell_dens4+1;
             	}
             	if (i-2>=0 && cell_2[j][i-2]>2) //Position 4 for Position 4
             	{
           	 		Cell_dens4 = Cell_dens4+1;
             	}
             	if (j+1<N && i-2>=0 && cell_2[j+1][i-2]>2) //Position 5 for Position 4
             	{
           	  		Cell_dens4 = Cell_dens4+1;
             	}
             	if (j+1<N && i-1>=0 && cell_2[j+1][i-1]>2) //Position 6 for Position 4
             	{
           	  		Cell_dens4 = Cell_dens4+1;
             	}
             	if (j+1<N && cell_2[j+1][i]>2) //Position 7 for Position 4
             	{
           	  		Cell_dens4 = Cell_dens4+1;
             	}
             	
//*******************************************************************************************************************************************************
//      											Calculating cell density of position 5 - cell_2[j+1][i-1] "Cell_dens5"
//*******************************************************************************************************************************************************              

             	if (j+1<N && cell_2[j+1][i]>2) //Position 0 for Position 5
             	{
           	  	Cell_dens5 = Cell_dens5+1;
             	}
//            	if (cell_2[j][i]>2) //Position 1 for Position 5
//            	{
//           	  	Cell_dens5 = Cell_dens5+1;
//            	}
             	if (i-1>=0 && cell_2[j][i-1]>2) //Position 2 for Position 5
             	{
           	  		Cell_dens5 = Cell_dens5+1;
             	}
             	if (i-2>=0 && cell_2[j][i-2]>2) //Position 3 for Position 5
             	{
              		Cell_dens5 = Cell_dens5+1;
             	}
             	if (j+1<N && i-2>=0 && cell_2[j+1][i-2]>2) //Position 4 for Position 5
             	{
           	  		Cell_dens5 = Cell_dens5+1;
             	}
             	if (j+2<N && i-2>=0 && cell_2[j+2][i-2]>2) //Position 5 for Position 5
             	{
              		Cell_dens5 = Cell_dens5+1;
             	}
             	if (j+2<N && i-1>=0 && cell_2[j+2][i-1]>2) //Position 6 for Position 5
             	{
           	  		Cell_dens5 = Cell_dens5+1;
             	}
             	if (j+2<N && cell_2[j+2][i]>2) //Position 7 for Position 5
             	{
           	  	Cell_dens5 = Cell_dens5+1;
             	}

//*******************************************************************************************************************************************************
//      											Calculating cell density of position 6 - cell_2[j+1][i] "Cell_dens6"
//*******************************************************************************************************************************************************              	

             	if (j+1<N && i+1<N && cell_2[j+1][i+1]>2) //Position 0 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	if (i+1<N && cell_2[j][i+1]>2) //Position 1 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
//            	if (cell_2[j][i]>2) //Position 2 for Position 6
//            	{
//              	Cell_dens6 = Cell_dens6+1;
//            	}
             	if (i-1>=0 && cell_2[j][i-1]>2) //Position 3 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	if (j+1<N && i-1>=0 && cell_2[j+1][i-1]>2)  //Position 4 for Position 6
             	{
              		Cell_dens6 = Cell_dens6+1;
             	}
             	if (j+2<N && i-1>=0 && cell_2[j+2][i-1]>2) //Position 5 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	if (j+2<N && cell_2[j+2][i]>2) //Position 6 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	if (j+2<N && i+1<N && cell_2[j+2][i+1]>2) //Position 7 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	
//*******************************************************************************************************************************************************
//      											Calculating cell density of position 7 - cell_2[j+1][i+1] "Cell_dens7"
//*******************************************************************************************************************************************************              	

             	if (j+1<N && i+2<N && cell_2[j+1][i+2]>2) //Position 0 for Position 7
             	{
              		Cell_dens7 = Cell_dens7+1;
             	}
             	if (i+2<N && cell_2[j][i+2]>2) //Position 1 for Position 7
             	{
           	  		Cell_dens7 = Cell_dens7+1;
             	}
             	if (i+1<N && cell_2[j][i+1]>2) //Position 2 for Position 7
             	{
           	  		Cell_dens7 = Cell_dens7+1;
             	}
//            	if (cell_2[j][i]>2) //Position 3 for Position 7
//            	{
//           	  	Cell_dens7 = Cell_dens7+1;
//            	}
             	if (j+1<N && cell_2[j+1][i]>2) //Position 4 for Position 7
             	{
           	  		Cell_dens7 = Cell_dens7+1;
             	}
             	if (j+2<N && cell_2[j+2][i]>2) //Position 5 for Position 7
             	{
              		Cell_dens7 = Cell_dens7+1;
             	}
             	if (j+2<N && i+1<N && cell_2[j+2][i+1]>2) //Position 6 for Position 7
             	{
           	  	Cell_dens7 = Cell_dens7+1;
             	}
             	if (j+2<N && i+2<N && cell_2[j+2][i+2]>2) //Position 7 for Position 7
             	{
           	  	Cell_dens7 = Cell_dens7+1;
             	}
    
//***************************************************************************************************************************************************
//                              				Migration cases for first half of the model 
//***************************************************************************************************************************************************

				sort(Availability.begin(), Availability.end()); 
				if (Availability.size()!= 0) // if there is at least one free position around current location
				{
					if (Cell_dens0<Cell_density)
					{
						Min_dens.push_back(0);
					}
					if (Cell_dens1<Cell_density)
					{
						Min_dens.push_back(1);
					}
					if (Cell_dens2<Cell_density)
					{
						Min_dens.push_back(2);
					}
					if (Cell_dens3<Cell_density)
					{
						Min_dens.push_back(3);
					}
					if (Cell_dens4<Cell_density)
					{
						Min_dens.push_back(4);
					}
					if (Cell_dens5<Cell_density)
					{
						Min_dens.push_back(5);
					}
					if (Cell_dens6<Cell_density)
					{
						Min_dens.push_back(6);
					}
					if (Cell_dens7<Cell_density)
					{
						Min_dens.push_back(7);
					}
					sort(Min_dens.begin(), Min_dens.end()); 
					
					if (Min_dens.size()!= 0) // if there is at least one position with a lower density around current location
					{
				   		migration = Min_dens[rand()%Min_dens.size()]; //migr is a random number representing an available location with lower density
					}
					else
					{
						migration = Availability[rand()%Availability.size()]; //migr is a random number representing an available location
					}				
				}
				else // if no available locations around current cell, migration = 88
				{
				   	migration = 88;
					migration_dir[j][i]=88;
				}
	
				random=rand()%6; //include stochasticity 
	            switch (migration)
	            {			
					case 0: 
	                if (i+1<N && cell_2[j][i+1]==0) 
	                {
	                    cell_2[j][i+1]=cell_2[j][i]; 
	                    cell_2[j][i]=0; 
	                    if ((random <= abs(Cell_density - Cell_dens0) && Cell_density>Cell_dens0 && iter>DAY4) || iter<=DAY4) //if cell is moving towords a position with less cells around and a random component is satisfied (whose probability increase if a big difference between the two densities)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=8; 
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 1:
	                if (i+1<N && j-1>0 && cell_2[j-1][i+1]==0)
	                {
	                    cell_2[j-1][i+1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens1) && Cell_density>Cell_dens1 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=9;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 2:
	                if (j-1>0 && cell_2[j-1][i]==0)
	                {
	                    cell_2[j-1][i]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens2) && Cell_density>Cell_dens2 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=10;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 3:
	                if (j-1>0 && i-1>=0 && cell_2[j-1][i-1]==0)
	                {
	                    cell_2[j-1][i-1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens3) && Cell_density>Cell_dens3 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=11;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 4:
	                if (i-1>=0 && cell_2[j][i-1]==0)
	                {
	                    cell_2[j][i-1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    migration_dir[j][i]=migration;
	                    if ((random <= abs(Cell_density - Cell_dens4) && Cell_density>Cell_dens4 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=12;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 5:
	                if (j+1<N && i-1>=0 && cell_2[j+1][i-1]==0)
	                {
	                    cell_2[j+1][i-1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens5) && Cell_density>Cell_dens5 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=13;
						 	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 6:
	                if (j+1<N && cell_2[j+1][i]==0)
	                {
	                    cell_2[j+1][i]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens6) && Cell_density>Cell_dens6 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=14;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 7:
	                if (j+1<N && i+1<N && cell_2[j+1][i+1]==0)
	                {
	                    cell_2[j+1][i+1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens7) && Cell_density>Cell_dens7 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=15;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
               } //end switch
           } //end if
        }//end for i
    }//end for j

//***************************************************************************************************************************************************
//                              				Migration of second half of the model 
//***************************************************************************************************************************************************  

   for (j=round((N-1)/2+1);j<N;j++)
	{
	    for (i=0;i<N;i++)
	    {
		   if (cell_selected[j][i]==3)
        	{
	         	Cell_density = 0;
			 	Cell_dens0 = 0;
			 	Cell_dens1 = 0;
			 	Cell_dens2 = 0;
			 	Cell_dens3 = 0;
			 	Cell_dens4 = 0;
			 	Cell_dens5 = 0;
			 	Cell_dens6 = 0;
			 	Cell_dens7 = 0;
			 	migration =0;
			 	vector<int>Availability;
	         	Availability.clear();
	         	vector<int>Min_dens;
	         	Min_dens.clear();
//*******************************************************************************************************************************************************
//      												Calculating cell density around the current position
//*******************************************************************************************************************************************************
		    	if (i+1<N && cell_2[j][i+1]>2) // Position 0
            	{
           	   		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	   		Availability.push_back(0);
            	}
            	if (j-1>0 && i+1<N && cell_2[j-1][i+1]>2 ) // Position 1
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(1);
            	}
            	if (j-1>0 && cell_2[j-1][i]>2) // Position 2
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(2);
            	}	
            	if (j-1>0 && i-1>=0 && cell_2[j-1][i-1]>2) // Position 3
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(3);
            	}
            	if (i-1>=0, cell_2[j][i-1]>2) // Position 4
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(4);
            	}
            	if (j+1<N && i-1>=0 && cell_2[j+1][i-1]>2) // Position 5
            	{
           	  		Cell_density = Cell_density+1;
            	}
            	else
            	{
           	  		Availability.push_back(5);
            	}
            	if (j+1<N && cell_2[j+1][i]>2) // Position 6
            	{
	           	  	Cell_density = Cell_density+1;
	            }
	            else
	            {
	           	  	Availability.push_back(6);
	            }
	            if (j+1<N && i+1<N && cell_2[j+1][i+1]>2) // Position 7
	            {
	           	  	Cell_density = Cell_density+1;
	            }
	            else
	            {
	           	  	Availability.push_back(7);
	            }
	            
//*******************************************************************************************************************************************************
//      											Calculating cell density around position 0 - cell_2[j][i+1] "Cell_dens0"
//*******************************************************************************************************************************************************

        		if (i+2<N && cell_2[j][i+2]>2) //Position 0 for Position 0
            	{
           	  		Cell_dens0 = Cell_dens0+1;
            	}
	            if (j-1>0 && i+2<N && cell_2[j-1][i+2]>2 )  //Position 1 for Position 0
	            {
	           	  	Cell_dens0 = Cell_dens0+1;
	            }
	            if (j-1>0 && i+1<N && cell_2[j-1][i+1]>2)  //Position 2 for Position 0
	            {
		       	  	Cell_dens0 = Cell_dens0+1;
		        }
		       	if (j-1>0 && cell_2[j-1][i]>2) //Position 3 for Position 0
		       	{
		          	Cell_dens0 = Cell_dens0+1;
		        }
//           	if (cell_2[j][i]>2)  //Position 4 for Position 0
//           	{
//           	 	Cell_dens0 = Cell_dens0+1;
//           	}
	        	if (j+1<N && cell_2[j+1][i]>2) //Position 5 for Position 0
	        	{
	       	  		Cell_dens0 = Cell_dens0+1;
	        	}
	        	if (j+1<N && i+1<N && cell_2[j+1][i+1]>2) //Position 6 for Position 0
	        	{
	       	  		Cell_dens0 = Cell_dens0+1;
	        	}
	        	if (j+1<N && i+2<N && cell_2[j+1][i+2]>2)  //Position 7 for Position 0
	        	{
	       	  		Cell_dens0 = Cell_dens0+1;
	        	}
	        	
//*******************************************************************************************************************************************************
//      											Calculating cell density of position 1 - cell_2[j-1][i+1] "Cell_dens1"
//*******************************************************************************************************************************************************	        	

		        if (j-1>0 && i+2<N && cell_2[j-1][i+2]>2)  //Position 0 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
		        if (j-2>0 && i+2<N && cell_2[j-2][i+2]>2 ) //Position 1 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
		        if (j-2>0 && i+1<N && cell_2[j-2][i+1]>2)  //Position 2 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
		        if (j-2>0 && cell_2[j-2][i]>2) //Position 3 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
		        if (j-1>0 && cell_2[j-1][i]>2)  //Position 4 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }
//	        	if (cell_2[j][i]>2)  //Position 5 for Position 1
//	        	{
//	       	  		Cell_dens1 = Cell_dens1+1;
//	        	}
		        if (i+1<N && cell_2[j][i+1]>2) //Position 6 for Position 1
		        {
		         	Cell_dens1 = Cell_dens1+1;
		        }
		        if (i+2<N && cell_2[j][i+2]>2) //Position 7 for Position 1
		        {
		       	  	Cell_dens1 = Cell_dens1+1;
		        }

//*******************************************************************************************************************************************************
//      											Calculating cell density of position 2 - cell_2[j-1][i] "Cell_dens2"
//*******************************************************************************************************************************************************
		        
	            if (j-1>0 && i+1<N && cell_2[j-1][i+1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (j-2>0 && i+1<N && cell_2[j-2][i+1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (j-2>0 && cell_2[j-2][i]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (j-2>0 && i-1>=0 && cell_2[j-2][i-1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (j-1>0 && i-1>=0 && cell_2[j-1][i-1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
	            if (i-1>=0 && cell_2[j][i-1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
//	            if (cell_2[j][i]>2)
//	            {
//	           	  	Cell_dens2 = Cell_dens2+1;
//	            }
	            if (i+1<N && cell_2[j][i+1]>2)
	            {
	           	  	Cell_dens2 = Cell_dens2+1;
	            }
//*******************************************************************************************************************************************************
//      											Calculating cell density of position 3 - cell_2[j-1][i-1] "Cell_dens3"
//*******************************************************************************************************************************************************

	            if (j-1>0 &&  cell_2[j-1][i]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
	            if (j-2>0 && cell_2[j-2][i]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
	            if (j-2>0 && i-1>=0 && cell_2[j-2][i-1]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
	            if (j-2>0 && i-2>=0 && cell_2[j-2][i-2]>2)
	            {
	           		Cell_dens3 = Cell_dens3+1;
	            }
	            if (j-1>0 && i-2>=0 && cell_2[j-1][i-2]>2)
	            {
	              	Cell_dens3 = Cell_dens3+1;
	            }
	            if (i-2>=0 && cell_2[j][i-2]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
	            if (i-1>=0 && cell_2[j][i-1]>2)
	            {
	           	  	Cell_dens3 = Cell_dens3+1;
	            }
//            	if (cell_2[j][i]>2)
//            	{
//           	  	Cell_dens3 = Cell_dens3+1;
//            	}

//*******************************************************************************************************************************************************
//      											Calculating cell density of position 4 - cell_2[j][i-1] "Cell_dens4"
//*******************************************************************************************************************************************************            

//             if (cell_2[j][i]>2) //Position 0 for Position 4
//             {
//           	  	Cell_dens4 = Cell_dens4+1;
//             }
	            if (j-1>0 && cell_2[j-1][i]>2) //Position 1 for Position 4
	            {
	              Cell_dens4 = Cell_dens4+1;
	            }
	            if (j-1>0 && i-1>=0 && cell_2[j-1][i-1]>2) //Position 2 for Position 4
	            {
	           	  	Cell_dens4 = Cell_dens4+1;
            	}
             	if (j-1>0 && i-2>=0 && cell_2[j-1][i-2]>2) //Position 3 for Position 4
             	{
           	 	 	Cell_dens4 = Cell_dens4+1;
             	}
             	if (i-2>=0 && cell_2[j][i-2]>2) //Position 4 for Position 4
             	{
           	 		Cell_dens4 = Cell_dens4+1;
             	}
             	if (j+1<N && i-2>=0 && cell_2[j+1][i-2]>2) //Position 5 for Position 4
             	{
           	  		Cell_dens4 = Cell_dens4+1;
             	}
             	if (j+1<N && i-1>=0 && cell_2[j+1][i-1]>2) //Position 6 for Position 4
             	{
           	  		Cell_dens4 = Cell_dens4+1;
             	}
             	if (j+1<N && cell_2[j+1][i]>2) //Position 7 for Position 4
             	{
           	  		Cell_dens4 = Cell_dens4+1;
             	}
             	
//*******************************************************************************************************************************************************
//      											Calculating cell density of position 5 - cell_2[j+1][i-1] "Cell_dens5"
//*******************************************************************************************************************************************************              
             	if (j+1<N && cell_2[j+1][i]>2) //Position 0 for Position 5
             	{
           	  	Cell_dens5 = Cell_dens5+1;
             	}
//            	if (cell_2[j][i]>2) //Position 1 for Position 5
//            	{
//           	  	Cell_dens5 = Cell_dens5+1;
//            	}
             	if (i-1>=0 && cell_2[j][i-1]>2) //Position 2 for Position 5
             	{
           	  		Cell_dens5 = Cell_dens5+1;
             	}
             	if (i-2>=0 && cell_2[j][i-2]>2) //Position 3 for Position 5
             	{
              		Cell_dens5 = Cell_dens5+1;
             	}
             	if (j+1<N && i-2>=0 && cell_2[j+1][i-2]>2) //Position 4 for Position 5
             	{
           	  		Cell_dens5 = Cell_dens5+1;
             	}
             	if (j+2<N && i-2>=0 && cell_2[j+2][i-2]>2) //Position 5 for Position 5
             	{
              		Cell_dens5 = Cell_dens5+1;
             	}
             	if (j+2<N && i-1>=0 && cell_2[j+2][i-1]>2) //Position 6 for Position 5
             	{
           	  		Cell_dens5 = Cell_dens5+1;
             	}
             	if (j+2<N && cell_2[j+2][i]>2) //Position 7 for Position 5
             	{
           	  	Cell_dens5 = Cell_dens5+1;
             	}

//*******************************************************************************************************************************************************
//      											Calculating cell density of position 6 - cell_2[j+1][i] "Cell_dens6"
//*******************************************************************************************************************************************************              	

             	if (j+1<N && i+1<N && cell_2[j+1][i+1]>2) //Position 0 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	if (i+1<N && cell_2[j][i+1]>2) //Position 1 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
//            	if (cell_2[j][i]>2) //Position 2 for Position 6
//            	{
//              	Cell_dens6 = Cell_dens6+1;
//            	}
             	if (i-1>=0 && cell_2[j][i-1]>2) //Position 3 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	if (j+1<N && i-1>=0 && cell_2[j+1][i-1]>2)  //Position 4 for Position 6
             	{
              		Cell_dens6 = Cell_dens6+1;
             	}
             	if (j+2<N && i-1>=0 && cell_2[j+2][i-1]>2) //Position 5 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	if (j+2<N && cell_2[j+2][i]>2) //Position 6 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	if (j+2<N && i+1<N && cell_2[j+2][i+1]>2) //Position 7 for Position 6
             	{
           	  		Cell_dens6 = Cell_dens6+1;
             	}
             	
//*******************************************************************************************************************************************************
//      											Calculating cell density of position 7 - cell_2[j+1][i+1] "Cell_dens7"
//*******************************************************************************************************************************************************              	

             	if (j+1<N && i+2<N && cell_2[j+1][i+2]>2) //Position 0 for Position 7
             	{
              		Cell_dens7 = Cell_dens7+1;
             	}
             	if (i+2<N && cell_2[j][i+2]>2) //Position 1 for Position 7
             	{
           	  		Cell_dens7 = Cell_dens7+1;
             	}
             	if (i+1<N && cell_2[j][i+1]>2) //Position 2 for Position 7
             	{
           	  		Cell_dens7 = Cell_dens7+1;
             	}
//            	if (cell_2[j][i]>2) //Position 3 for Position 7
//            	{
//           	  	Cell_dens7 = Cell_dens7+1;
//            	}
             	if (j+1<N && cell_2[j+1][i]>2) //Position 4 for Position 7
             	{
           	  		Cell_dens7 = Cell_dens7+1;
             	}
             	if (j+2<N && cell_2[j+2][i]>2) //Position 5 for Position 7
             	{
              		Cell_dens7 = Cell_dens7+1;
             	}
             	if (j+2<N && i+1<N && cell_2[j+2][i+1]>2) //Position 6 for Position 7
             	{
           	  	Cell_dens7 = Cell_dens7+1;
             	}
             	if (j+2<N && i+2<N && cell_2[j+2][i+2]>2) //Position 7 for Position 7
             	{
           	  	Cell_dens7 = Cell_dens7+1;
             	}
    
//***************************************************************************************************************************************************
//                              				Migration cases of second half (j=126 to j=250) of the model 
//***************************************************************************************************************************************************
				
				sort(Availability.begin(), Availability.end()); 
				
				if (Availability.size()!= 0) 
				{
					if (Cell_dens0<Cell_density)
					{
						Min_dens.push_back(0);
					}
					if (Cell_dens1<Cell_density)
					{
						Min_dens.push_back(1);
					}
					if (Cell_dens2<Cell_density)
					{
						Min_dens.push_back(2);
					}
					if (Cell_dens3<Cell_density)
					{
						Min_dens.push_back(3);
					}
					if (Cell_dens4<Cell_density)
					{
						Min_dens.push_back(4);
					}
					if (Cell_dens5<Cell_density)
					{
						Min_dens.push_back(5);
					}
					if (Cell_dens6<Cell_density)
					{
						Min_dens.push_back(6);
					}
					if (Cell_dens7<Cell_density)
					{
						Min_dens.push_back(7);
					}
					sort(Min_dens.begin(), Min_dens.end()); 
					
					if (Min_dens.size()!= 0) 
					{
				   		migration = Min_dens[rand()%Min_dens.size()]; 
					}
					else
					{
						migration = Availability[rand()%Availability.size()]; 
					}				
				}
				else 
				{
				   	migration = 88;
					migration_dir[j][i]=88;
				}
	
				random=rand()%6; 
	            switch (migration)
	            {			
					case 0: 
	                if (i+1<N && cell_2[j][i+1]==0) 
	                {
	                    cell_2[j][i+1]=cell_2[j][i]; 
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens0) && Cell_density>Cell_dens0 && iter>DAY4) || iter<=DAY4) 
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=8; 
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 1:
	                if (i+1<N && j-1>0 && cell_2[j-1][i+1]==0)
	                {
	                    cell_2[j-1][i+1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens1) && Cell_density>Cell_dens1 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=9;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 2:
	                if (j-1>0 && cell_2[j-1][i]==0)
	                {
	                    cell_2[j-1][i]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens2) && Cell_density>Cell_dens2 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=10;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 3:
	                if (j-1>0 && i-1>=0 && cell_2[j-1][i-1]==0)
	                {
	                    cell_2[j-1][i-1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens3) && Cell_density>Cell_dens3 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=11;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 4:
	                if (i-1>=0 && cell_2[j][i-1]==0)
	                {
	                    cell_2[j][i-1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    migration_dir[j][i]=migration;
	                    if ((random <= abs(Cell_density - Cell_dens4) && Cell_density>Cell_dens4 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=12;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 5:
	                if (j+1<N && i-1>=0 && cell_2[j+1][i-1]==0)
	                {
	                    cell_2[j+1][i-1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens5) && Cell_density>Cell_dens5 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=13;
						 	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 6:
	                if (j+1<N && cell_2[j+1][i]==0)
	                {
	                    cell_2[j+1][i]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens6) && Cell_density>Cell_dens6 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=14;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
	
	                case 7:
	                if (j+1<N && i+1<N && cell_2[j+1][i+1]==0)
	                {
	                    cell_2[j+1][i+1]=cell_2[j][i];
	                    cell_2[j][i]=0;
	                    if ((random <= abs(Cell_density - Cell_dens7) && Cell_density>Cell_dens7 && iter>DAY4) || iter<=DAY4)
	                    {
	                      	migration_dir[j][i]=migration;
	                      	Dens_Case=Dens_Case+1;
	                    }
						else
						{
						  	migration_dir[j][i]=15;
						  	Stiff_Case=Stiff_Case+1;
						}
	                }
	                break;
               } //end switch
           } //end if
        }//end for i
    }


   	cout<<"Cells moved successfully"<<endl;
//   	cout<<"Density cases : "<<Dens_Case<<endl;
//   	cout<<"Stiffness cases : "<<Stiff_Case<<endl;
//*******************************************************************************************************************************************************
//                  											Create new FEM model
//*******************************************************************************************************************************************************
    cout<<"Create new FEM model"<<endl;

	ifstream infile("Control_rigid.txt");

    ostringstream s;
    s<<"Temporal_FEM_"<<iter<<".inp";

    string filename1=s.str();

    ofstream myfile;
    myfile.open(filename1.c_str(),ios::app);

// *************  Writing heading of the inp file******************

    myfile<<"*Heading"<<endl;
    myfile<<"** Job name: Job-1 Model name: Model-1"<<endl;
    myfile<<"** Generated by: Abaqus/CAE 6.9-2"<<endl;
    myfile<<"*Preprint, echo=NO, model=NO, history=NO, contact=NO"<<endl;
    myfile<<"**"<<endl;
    myfile<<"** PARTS"<<endl;
    myfile<<"**"<<endl;
    myfile<<"*Node, nset=node_all"<<endl;

// *************  Writing nodes   **********************************
    
	
	int node=0;

    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            node=i+(j*N)+1;
            myfile<<node<<", "<<setprecision(15)<<X_temp[node]<<", "<<setprecision(15)<<Y_temp[node]<<endl;
        }
    }

    char c = infile.get();

    while  (!infile.eof())
    {
        myfile << c;
        c= infile.get();
    }

    int node_1,node_2;

    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            if (cell_2[j][i]>2) 
            {
            //*****************************************
            // dipole direction
            //*****************************************

		        if (i==0 || i==N-1) 
		        {
		            direction_2[j][i]=3;
		        }
		        else 
		        {
		            direction_2[j][i]=rand()%4+1;
		        }
            //****************************************
            // Calculate nodes where load is applied
            //****************************************
				if (direction_2[j][i] == 1) //Lateral dipole 0°
				{
					node_1 = i + 1 + j * N - 1;
					node_2 = i + 1 + j * N + 1;

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 1 << ", " << F_FIBRO << endl;
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 1 << ", " << -F_FIBRO << endl;
					myfile << "**" << endl;
				}
				else if (direction_2[j][i] == 2) //Diagonal right dipole 45°
				{
					node_1 = i + 1 + j * N - (N + 1);
					node_2 = i + 1 + j * N + (N + 1);

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 1 << ", " << F_FIBROcos << endl;  
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 2 << ", " << F_FIBROcos << endl;  
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 1 << ", " << -F_FIBROcos << endl;   
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 2 << ", " << -F_FIBROcos << endl;  
					myfile << "**" << endl;
				}
				else if (direction_2[j][i] == 3) //Axial dipole 90°
				{
					node_1 = i + 1 + j * N - N;
					node_2 = i + 1 + j * N + N;

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 2 << ", " << F_FIBRO << endl;
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 2 << ", " << -F_FIBRO << endl;
					myfile << "**" << endl;
				}
				else if (direction_2[j][i] == 4) //Diagonal left dipole 135°(-45°)
				{
					node_1 = i + 1 + j * N - (N - 1);
					node_2 = i + 1 + j * N + (N - 1);

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 1 << ", " << -F_FIBROcos << endl;  
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 2 << ", " << F_FIBROcos << endl;  
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 1 << ", " << F_FIBROcos << endl;   
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 2 << ", " << -F_FIBROcos << endl;  
					myfile << "**" << endl;
				}
			} 
		} //End for i
	} //End for j

//*********************************************************************************************************
//                         Applying forces at the sprout tip (tip cell behaves like a dipole)
//*********************************************************************************************************		
	int vessel;
	vessel = 0;
	i = 0;
	j = 0;
	int node_v1, node_v2, node_v3, node_v4, node_v5, node_v6, node_v7, node_v8;

	//		  
	//
	//		  6  |  7  | 8
	//      ----------------
	//		  5 | tip | 1
	//	    ----------------
	//		  4 |  3  | 2
	//
	
	for (vessel = 0; vessel < number_vessels_temp; vessel++)
	{
		if (tip_x_temp[vessel] != 10000 && tip_x_temp[vessel] != 9999 && tip_y_temp[vessel] != 10000 && tip_y_temp[vessel] != 9999)
		{
			j = tip_x_temp[vessel]; //select the tip node
			i = tip_y_temp[vessel];
			
			if (i == 0 || i == N-1) // force just up/down 
			{
				node_v3 = i + 1 + j * N - N; //down
				node_v7 = i + 1 + j * N + N; // up

				myfile << "*Cload" << endl; //down
				myfile << node_v3 << ", " << 2 << ", " << F_ENDO << endl; 
				myfile << "**" << endl;

				myfile << "*Cload" << endl; // up
				myfile << node_v7 << ", " << 2 << ", " << -F_ENDO << endl;
				myfile << "**" << endl;
			}
			else // in the direction of growth
			{
				switch (previous_dir_temp[vessel])
				{
					case 1:
						node_v1 = i + 1 + j * N + 1; // right 
						node_v5 = i + 1 + j * N - 1; // left
					
						myfile << "*Cload" << endl; // right
						myfile << node_v1 << ", " << 1 << ", " << -F_ENDO << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // left
						myfile << node_v5 << ", " << 1 << ", " << F_ENDO << endl;
						myfile << "**" << endl;
					break;
				
					case 2:
						node_v2 = i + 1 + j * N - (N - 1);// bottom-right
						node_v6 = i + 1 + j * N + (N - 1); // top-left

						myfile << "*Cload" << endl; // bottom-right
						myfile << node_v2 << ", " << 1 << ", " << -F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // bottom-right
						myfile << node_v2 << ", " << 2 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;		
					
						myfile << "*Cload" << endl; //top-left
						myfile << node_v6 << ", " << 1 << ", " << F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; //top-left
						myfile << node_v6 << ", " << 2 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;	
					break;
				
					case 3:
						node_v3 = i + 1 + j * N - N; //down
						node_v7 = i + 1 + j * N + N; // up
					
						myfile << "*Cload" << endl; //down
						myfile << node_v3 << ", " << 2 << ", " << F_ENDO << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // up
						myfile << node_v7 << ", " << 2 << ", " << -F_ENDO << endl;
						myfile << "**" << endl;
					break;
				
					case 4:
						node_v4 = i + 1 + j * N - (N + 1); // bottom-left
						node_v8 = i + 1 + j * N + (N + 1); // top-right
					
						myfile << "*Cload" << endl; //bottom-left
						myfile << node_v4 << ", " << 1 << ", " << F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; //bottom-left
						myfile << node_v4 << ", " << 2 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // top-right
						myfile << node_v8 << ", " << 1 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // top-right
						myfile << node_v8 << ", " << 2 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;
					break;
				
					case 5:
						node_v1 = i + 1 + j * N + 1; // right 
						node_v5 = i + 1 + j * N - 1; // left
					
						myfile << "*Cload" << endl; // right
						myfile << node_v1 << ", " << 1 << ", " << -F_ENDO << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // left
						myfile << node_v5 << ", " << 1 << ", " << F_ENDO << endl;
						myfile << "**" << endl;
					break;
				
					case 6:
						node_v2 = i + 1 + j * N - (N - 1);// bottom-right
						node_v6 = i + 1 + j * N + (N - 1); // top-left

						myfile << "*Cload" << endl; // bottom-right
						myfile << node_v2 << ", " << 1 << ", " << -F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // bottom-right
						myfile << node_v2 << ", " << 2 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;		
					
						myfile << "*Cload" << endl; //top-left
						myfile << node_v6 << ", " << 1 << ", " << F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; //top-left
						myfile << node_v6 << ", " << 2 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;	
					break;
				
					case 7:
						node_v3 = i + 1 + j * N - N; //down
						node_v7 = i + 1 + j * N + N; // up
					
						myfile << "*Cload" << endl; //down
						myfile << node_v3 << ", " << 2 << ", " << F_ENDO << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // up
						myfile << node_v7 << ", " << 2 << ", " << -F_ENDO << endl;
						myfile << "**" << endl;
					break;
				
					case 8:
						node_v4 = i + 1 + j * N - (N + 1); // bottom-left
						node_v8 = i + 1 + j * N + (N + 1); // top-right
					
						myfile << "*Cload" << endl; //bottom-left
						myfile << node_v4 << ", " << 1 << ", " << F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; //bottom-left
						myfile << node_v4 << ", " << 2 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // top-right
						myfile << node_v8 << ", " << 1 << ", " << -F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // top-right
						myfile << node_v8 << ", " << 2 << ", " << -F_ENDOcos << endl; 
						myfile << "**" << endl;
					break;
				} //end switch
			}// end if
		}//end else
	} //end for


    myfile<<"*************************************"<<endl;
    myfile<<"** OUTPUT REQUESTS"<<endl;
    myfile<<"*************************************"<<endl;
    myfile<<"**"<<endl;
    myfile<<"*Restart, write, frequency=0"<<endl;
    myfile<<"**"<<endl;
    myfile<<"** FIELD OUTPUT: F-Output-1"<<endl;
    myfile<<"**"<<endl;
    myfile<<"*Output, field, variable=PRESELECT"<<endl;
    myfile<<"**"<<endl;
    myfile<<"*Element Output, directions=YES"<<endl;
    myfile<<"*EL PRINT, POSITION=CENTROID, ELSET=ALL, frequency=99999"<<endl;
    myfile<<"*NODE PRINT, NSET=ALL, frequency=99999"<<endl;
    myfile<<"U"<<endl;
    myfile<<"** HISTORY OUTPUT: H-Output-1"<<endl;
    myfile<<"*Output, history, variable=PRESELECT"<<endl;
    myfile<<"*End Step"<<endl;
    myfile.close();

   	cout<<" <--- New FEM model created successfully"<<endl;

//*******************************************************************************************************************************************************
// 																	Check number of cells
//*******************************************************************************************************************************************************
    
//	int ncell_1,ncell_2;
//    ncell_1=ncell_2=0;
//
//    for (j=0;j<N;j++)
//    {
//        for (i=0;i<N;i++)
//        {	
//			if (cell_1[j][i]>2 )
//            {
//                ncell_1=ncell_1+1;
//            }
//            if (cell_2[j][i]>2)
//            {
//                ncell_2=ncell_2+1;
//            }
//        }
//    }
//
//    cout<<"TEMPORAL"<<endl;
//    cout<<"Number of Fibroblasts in FEM_"<<iter<<":"<<ncell_1<<endl;
//    cout<<"Number of Fibroblasts in Temporal_FEM_"<<iter<<":"<<ncell_2<<endl;
}
