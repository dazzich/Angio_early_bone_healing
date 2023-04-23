#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <algorithm> 
#include <limits> // Press inter to continue
#include "header.h"

using namespace std;

const int n=251;

void Proliferation_apoptosis(int cell_matrix[n][n], int dipole[n][n],int iter, int cell_number, int& number_vessels_prol, int tip_x_prol[(n-1)*20], int tip_y_prol[(n-1)*20], double X[n*n], double Y[n*n],int previous_dir_proli [(n-1)*20])
{

    ifstream infile_num("cell_number.txt");
    infile_num>>cell_number;
	//traction force
	float f_fibro = 0.000018;
	float f_fibro_cos = 0.0000127;
	float f_endo = 0.000010;
	float f_endo_cos = 0.00000707;
	
	
/************************************************************************************************************************************
														Counting available cells 
************************************************************************************************************************************/ 	

	int cell_cnt=0;
	int i=0;
	int j=0;
	
	for (j=0;j<n;j++)
	{
	    for (i=0;i<n;i++)
	    {
			if (cell_matrix[j][i]>2)
			{
               cell_cnt=cell_cnt+1;
			}
    	}
	}
	
	cout<<"Number of fibroblasts: "<<cell_cnt<<endl; 

/************************************************************************************************************************************
													Calculate the number of cell to proliferate   
************************************************************************************************************************************/ 
	
	float proli_factor;
	proli_factor = 0.0033;
	// 55% is the proliferation rate of fibroblasts per day. if we assume 3 iter equal to 1 hour, 72 iter equal to one day, each iteration 0.76 % proliferation
	int cell_proli=0;
	// repeat with 24%: 0.33%
	
	cell_proli = ceil (cell_cnt * proli_factor); // calculate number of proliferating cells. it increases at each iter. ceil makes the round up of the value
	
	//cout<<"Number of proliferation : "<<cell_proli<<endl; 
	//cout<<"Latest cell Number is : "<<cell_number<<endl; 
	
/************************************************************************************************************************************
														Choosing cells to proliferate    
************************************************************************************************************************************/	
	//cout<<" ---> Cell proliferation started "<<endl;

	vector<int>Availability;
	Availability.clear();
	int cnt=0;
	int proliferation=0;
	
	if (cell_cnt <= 44000) // 80% of the nodes become full of cells (n*n -8000 (Cortical bone nodes))*0.8 = 44000
	{	
		while (cnt < cell_proli) // cell_proli=n of proliferating cells at each iter
		{
			i=rand()%n;
	        j=rand()%(n-2)+1; // exclusion of the first and last row
			
			/*****************************************************************
							Identifiying empty positions around cell	    
			******************************************************************/
			
			if (cell_matrix[j][i]>2) // if there is a cell here 
	        {
				if (i+1<n && cell_matrix[j][i+1]==0) // Position 0
				{
					Availability.push_back(0);
				}
				if (j-1>0 && i+1<n && cell_matrix[j-1][i+1]==0) // Position 1
				{
					Availability.push_back(1);
				}
				if (j-1>0 && cell_matrix[j-1][i]==0) // Position 2
				{
					Availability.push_back(2);
				}	
				if (j-1>0 && i-1>=0 && cell_matrix[j-1][i-1]==0) // Position 3
				{
					Availability.push_back(3);
				}
				if (i-1>=0, cell_matrix[j][i-1]==0) // Position 4
				{
					Availability.push_back(4);
				}
				if (j+1<n-1 && i-1>=0 && cell_matrix[j+1][i-1]==0) // Position 5
				{
					Availability.push_back(5);
				}
				if (j+1<n-1 && cell_matrix[j+1][i]==0) // Position 6
				{
					Availability.push_back(6);
				}
				if (j+1<n-1 && i+1<n && cell_matrix[j+1][i+1]==0) // Position 7
				{
					Availability.push_back(7);
				}
			
			/*****************************************************************
									Proliferating 	    
			******************************************************************/
				
				if (Availability.size()!=0)
				{
					sort(Availability.begin(), Availability.end());
					proliferation = Availability[rand()%Availability.size()]; // random position among those available around each selected cell
					
					switch (proliferation)
					{
						case 0:
						if (i+1<n && cell_matrix[j][i+1]==0) // check that it is not a forbidden location
						{
							cnt=cnt+1;
							cell_matrix[j][i+1]=cell_number+2; // new number for the new cell
							cell_number=cell_number+1; //increase n of cells
							if (i+1==0 || i+1==n-1) //Axial dipole at left and right boundaries
							{
								dipole[j][i+1]=3;
							}
							else //random dipole direction
							{
								dipole[j][i+1]=rand()%4+1;
							}
						}
						break;
	
						case 1:
						if (i+1<n && j-1>0 && cell_matrix[j-1][i+1]==0)
						{
							cnt=cnt+1;
							cell_matrix[j-1][i+1]=cell_number+2;
							cell_number=cell_number+1;
							if (i+1==0 || i+1==n-1) //Axial dipole at left and right boundaries
							{
								dipole[j-1][i+1]=3;
							}
							else //random dipole direction
							{
								dipole[j-1][i+1]=rand()%4+1;
							}
						}
						break;
	
						case 2:
						if (j-1>0 && cell_matrix[j-1][i]==0)
						{							
							cnt=cnt+1;
							cell_matrix[j-1][i]=cell_number+2;
							cell_number=cell_number+1;
							if (i==0 || i==n-1) //Axial dipole at left and right boundaries
							{
								dipole[j-1][i]=3;
							}
							else //random dipole direction
							{
								dipole[j-1][i]=rand()%4+1;
							}
						}
						break;
	
						case 3:
						if (j-1>0 && i-1>=0 && cell_matrix[j-1][i-1]==0)
						{							
							cnt=cnt+1;
							cell_matrix[j-1][i-1]=cell_number+2;
							cell_number=cell_number+1;
							if (i-1==0 || i-1==n-1) //Axial dipole at left and right boundaries
							{
								dipole[j-1][i-1]=3;
							}
							else //random dipole direction
							{
								dipole[j-1][i-1]=rand()%4+1;
							}
						}
						break;
	
						case 4:
						if (i-1>=0 && cell_matrix[j][i-1]==0)
						{							
							cnt=cnt+1;
							cell_matrix[j][i-1]=cell_number+2;
							cell_number=cell_number+1;
							if (i-1==0 || i-1==n-1) //Axial dipole at left and right boundaries
							{
								dipole[j][i-1]=3;
							}
							else //random dipole direction
							{
								dipole[j][i-1]=rand()%4+1;
							}
						}
						break;
	
						case 5:
						if (j+1<n-1 && i-1>=0 && cell_matrix[j+1][i-1]==0)
						{				
							cnt=cnt+1;
							cell_matrix[j+1][i-1]=cell_number+2;
							cell_number=cell_number+1;
							if (i-1==0 || i-1==n-1) //Axial dipole at left and right boundaries
							{
								dipole[j+1][i-1]=3;
							}
							else //random dipole direction
							{
								dipole[j+1][i-1]=rand()%4+1;
							}
						}
						break;
	
						case 6:
						if (j+1<n-1 && cell_matrix[j+1][i]==0)
						{							
							cnt=cnt+1;
							cell_matrix[j+1][i]=cell_number+2;
							cell_number=cell_number+1;
							if (i==0 || i==n-1) //Axial dipole at left and right boundaries
							{
								dipole[j+1][i]=3;
							}
							else //random dipole direction
							{
								dipole[j+1][i]=rand()%4+1;
							}
						}
						break;
	
						case 7:
						if (j+1<n-1 && i+1<n && cell_matrix[j+1][i+1]==0)
						{
							cnt=cnt+1;
							cell_matrix[j+1][i+1]=cell_number+2;
							cell_number=cell_number+1;
							if (i+1==0 || i+1==n-1) //Axial dipole at left and right boundaries
							{
								dipole[j+1][i+1]=3;
							}
							else //random dipole direction
							{
								dipole[j+1][i+1]=rand()%4+1;
							}
						}
						break; 
					} //end switch
				} //end if availability
			}//end if cell selected 
		}//end while
	}//end if <8500
	
			
	//cout<<" <--- Cells are proliferated "<<endl;	
	cout<<"Number of proliferated cells : "<<cnt<<endl; 
/************************************************************************************************************************************
															Apoptosis   
************************************************************************************************************************************/		
/************************************************************************************************************************************
														Counting available cells 
************************************************************************************************************************************/ 	

	cell_cnt=0;
	
	for (j=0;j<n;j++)
	{
	    for (i=0;i<n;i++)
	    {
			if (cell_matrix[j][i]>2)
			{ 
			   cell_cnt=cell_cnt+1;
			}
    	}
	}
	
	//<<"Number of cells after proliferation : "<<cell_cnt<<endl;
/************************************************************************************************************************************
													Calculate the number of cell for apoptosis   
************************************************************************************************************************************/ 
	
	float apop_factor = 0.0003;
	// 5% is the appoptosis rate of fibroblast per day. if we assume 72 iteration is equal to one day, each iteration 0.07% apoptosis
	int cell_apop=0;
	//repeat with 2%: 0.03%
	
	cell_apop = ceil (cell_cnt * apop_factor); // calculate number of cells. Ceil makes the round up of the value	

/************************************************************************************************************************************
														Choosing cells to apoptosis    
/************************************************************************************************************************************/
	//cout<<" ---> Cell apoptosis started "<<endl;
	
	int cnt_2=0;
	
	while (cnt_2 <= cell_apop)
	{
		i=rand()%n;
        j=rand()%(n-2)+1;
		
		if (cell_matrix[j][i]>2) // if there is a fibroblast here 
        {
			cell_matrix[j][i]=0;
			dipole[j][i]=0;
			cnt_2=cnt_2+1;
		}
	}
	
	//cout<<" ---> Cells appoptosis done successfully "<<endl;
	cout<<" Number of dead cells "<<cnt_2<<endl;
    
/************************************************************************************************************************************
                        		Seeding cells in empty element of periosteum
/************************************************************************************************************************************/
	
	// counting available cells in ext periosteum
	int cnt_p=0;
	float density_fibro=0.2;
	int safe=80;
	int c_ext=round(density_fibro*safe); // 20% of ext periosteum

   	i=49; // lower left
	for (j=1;j<safe+1;j++) // lower left seeding
	{
		if (cell_matrix[j][i]>2)
        	{
        		cnt_p=cnt_p+1;
        	}
    	}
    while (cnt_p<c_ext)
    {
    	j=rand()%safe+1; //left ext
    	if (cell_matrix[j][i]==0)
    	{
    		cell_matrix[j][i]=cell_number+3;
    		cell_number=cell_number+1;
    		cnt_p=cnt_p+1;
		}
	}
  
    cnt_p=0;
	for (j=(n-1-safe);j<(n-1-safe)+safe;j++) // Top left
	{
		if (cell_matrix[j][i]>2)
        {
        		cnt_p=cnt_p+1;
        }
    }
    while (cnt_p<c_ext)
	{
		j=rand()%safe+(n-1-safe);
	    if (cell_matrix[j][i]==0)
        {
            cell_matrix[j][i]=cell_number+3;
            cell_number=cell_number+1;
            cnt_p=cnt_p+1;
        }
    }

	cnt_p = 0;
	i = 48; // lower left
	for (j = 1; j < safe + 1; j++) // lower left seeding
	{
		if (cell_matrix[j][i] > 2)
		{
			cnt_p = cnt_p + 1;
		}
	}
	while (cnt_p < c_ext)
	{
		j = rand() % safe + 1; //left ext
		if (cell_matrix[j][i] == 0)
		{
			cell_matrix[j][i] = cell_number + 3;
			cell_number = cell_number + 1;
			cnt_p = cnt_p + 1;
		}
	}

	cnt_p = 0;
	for (j = (n - 1 - safe); j < (n - 1 - safe) + safe; j++) // Top left
	{
		if (cell_matrix[j][i] > 2)
		{
			cnt_p = cnt_p + 1;
		}
	}
	while (cnt_p < c_ext)
	{
		j = rand() % safe + (n - 1 - safe);
		if (cell_matrix[j][i] == 0)
		{
			cell_matrix[j][i] = cell_number + 3;
			cell_number = cell_number + 1;
			cnt_p = cnt_p + 1;
		}
	}
  
    cnt_p=0;
    i=201; // Right
	for (j=1;j<safe+1;j++) // Bottom right
	{
	    if (cell_matrix[j][i]>2)
        {
        	cnt_p=cnt_p+1;
        }
    }
    
    while (cnt_p<c_ext)
	{
		j=rand()%safe+1;
	    if (cell_matrix[j][i]==0)
        {
            cell_matrix[j][i]=cell_number+3;
            cell_number=cell_number+1;
            cnt_p=cnt_p+1;
        }
    }
   
    cnt_p=0;
	for (j=(n-1-safe);j<(n-1-safe)+safe;j++) // Upper right
	{
	    if (cell_matrix[j][i]>2)
        {
        	cnt_p=cnt_p+1;
        }
    }
    while (cnt_p<c_ext)
	{
		j=rand()%safe+(n-1-safe);
	    if (cell_matrix[j][i]==0)
        {
            cell_matrix[j][i]=cell_number+3;
            cell_number=cell_number+1;
            cnt_p=cnt_p+1;
        }
    }

	cnt_p = 0;
	i = 202; // Right
	for (j = 1; j < safe + 1; j++) // Bottom right
	{
		if (cell_matrix[j][i] > 2)
		{
			cnt_p = cnt_p + 1;
		}
	}

	while (cnt_p < c_ext)
	{
		j = rand() % safe + 1;
		if (cell_matrix[j][i] == 0)
		{
			cell_matrix[j][i] = cell_number + 3;
			cell_number = cell_number + 1;
			cnt_p = cnt_p + 1;
		}
	}

	cnt_p = 0;
	for (j = (n - 1 - safe); j < (n - 1 - safe) + safe; j++) // Upper right
	{
		if (cell_matrix[j][i] > 2)
		{
			cnt_p = cnt_p + 1;
		}
	}
	while (cnt_p < c_ext)
	{
		j = rand() % safe + (n - 1 - safe);
		if (cell_matrix[j][i] == 0)
		{
			cell_matrix[j][i] = cell_number + 3;
			cell_number = cell_number + 1;
			cnt_p = cnt_p + 1;
		}
	}
   
    int Marrow_cell_L=0;
	int Marrow_cell_U=0;
	int c_bm=round(density_fibro*safe*(179-71));
    
    for (i=71;i<180;i++) // Counting available cells in bone marrow
    {
    	for (j=1;j<safe+1;j++)
    	{	
			if (cell_matrix[j][i]>2) //Lower
        	{
            	Marrow_cell_L=Marrow_cell_L+1;
        	}
        }
    }
    for (i=71;i<180;i++) // Counting available cells in bone marrow
    {
    	for (j=(n-1-safe);j<n-1;j++)
    	{
	    	if (cell_matrix[j][i]>2) //Upper
        	{
            	Marrow_cell_U=Marrow_cell_U+1;
        	}
        }
    }
    
	while (Marrow_cell_L<c_bm) // (Lower bone marrow) filling 30% of lower bone marrow with cells
    {
        i=rand()%109+71;
        j=rand()%safe+1;
		if (cell_matrix[j][i]==0)
	    {
	        cell_matrix[j][i]=cell_number+3;
	        cell_number=cell_number+1;
	        Marrow_cell_L=Marrow_cell_L+1;
	    }
    }
    
    while (Marrow_cell_U<c_bm) // (Upper bone marrow) filling 30% of upper bone marrow with cells
    {
        i=rand()%109+71;
        j=rand()%safe+(n-1-safe);
		if (cell_matrix[j][i]==0)
	    {
	        cell_matrix[j][i]=cell_number+3;
	        cell_number=cell_number+1;
	        Marrow_cell_U=Marrow_cell_U+1;
	    }
    }	

/************************************************************************************************************************************
                    									Writing New file
************************************************************************************************************************************/
    //cout<<"--->Write new file "<<endl; 
    
    ifstream infile("FEM_template_mouse.txt");
          
    ostringstream s;
    s<<"FEM_"<<iter+1<<".inp";
     
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
    
	int node;
	node=0;
     
    for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
        {
            node=i+(j*n)+1;
            myfile<<node<<", "<<setprecision(15)<<X[node]<<", "<<setprecision(15)<<Y[node]<<endl;
        }
    }
     
    char c = infile.get();
     
    while  (!infile.eof())
    {
        myfile << c;
        c= infile.get();
    } 
     
    int node_1,node_2;
     
    for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
        {
            if (cell_matrix[j][i]>2 )
            {
            //**************************************************
            //          Calculate nodes where load is applied
            //**************************************************
				if (dipole[j][i] == 1) //90° Lateral cell
				{
					node_1 = i + 1 + j * n - 1;
					node_2 = i + 1 + j * n + 1;

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 1 << ", " << f_fibro << endl;
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 1 << ", " << -f_fibro << endl;
					myfile << "**" << endl;
				}
				else if (dipole[j][i] == 2) //-45° Diagonal Down cell
				{
					node_1 = i + 1 + j * n - (n + 1);
					node_2 = i + 1 + j * n + (n + 1);

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 1 << ", " << f_fibro_cos << endl;   // (cos45Â°*0.00000003)
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 2 << ", " << f_fibro_cos << endl;  // (sin45Â°*0.00000003)
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 1 << ", " << -f_fibro_cos << endl;   //(cos45Â°*0.00000003)
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 2 << ", " << -f_fibro_cos << endl;  // (sin45Â°*0.00000003)
					myfile << "**" << endl;
				}
				else if (dipole[j][i] == 3) //0° Axial cell
				{
					node_1 = i + 1 + j * n - n;
					node_2 = i + 1 + j * n + n;

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 2 << ", " << f_fibro << endl;
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 2 << ", " << -f_fibro << endl;
					myfile << "**" << endl;
				}
				else if (dipole[j][i] == 4) // 45° Diagonal Up dipole 
				{
					node_1 = i + 1 + j * n - (n - 1);
					node_2 = i + 1 + j * n + (n - 1);

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 1 << ", " << -f_fibro_cos << endl;   // (cos45Â°*0.00000003)
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_1 << ", " << 2 << ", " << f_fibro_cos << endl;  // (sin45Â°*0.00000003)
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 1 << ", " << f_fibro_cos << endl;   //(cos45Â°*0.00000003)
					myfile << "**" << endl;

					myfile << "*Cload" << endl;
					myfile << node_2 << ", " << 2 << ", " << -f_fibro_cos << endl;  // (sin45Â°*0.00000003)
					myfile << "**" << endl;
				}
			} // End if (cell_matrix_1[j][i]>1 )
		} // End for i 
	} // End for j

	// Apply loads of tip cells		
	int vessel;
	vessel = 0;
	i = 0;
	j = 0;
	int node_v1, node_v2, node_v3, node_v4, node_v5, node_v6, node_v7, node_v8;

	//		  Nodes towards which tip cell apply traction force
	//
	//		  6  |  7  | 8
	//      ----------------
	//		  5 | tip | 1
	//	    ----------------
	//		  4 |  3  | 2
	//

	for (vessel = 0; vessel < number_vessels_prol; vessel++)
	{
		if (tip_x_prol[vessel] != 10000 && tip_x_prol[vessel] != 9999 && tip_y_prol[vessel] != 10000 && tip_y_prol[vessel] != 9999)
		{
			j = tip_x_prol[vessel]; //select the tip node
			i = tip_y_prol[vessel];
			if (i == 0 || i == n-1) // force just up/down 
			{
				node_v3 = i + 1 + j * n - n; //down
				node_v7 = i + 1 + j * n + n; // up

				myfile << "*Cload" << endl; //down
				myfile << node_v3 << ", " << 2 << ", " << f_endo << endl; // 4nN in each direction (8 possible directions)
				myfile << "**" << endl;

				myfile << "*Cload" << endl; // up
				myfile << node_v7 << ", " << 2 << ", " << -f_endo << endl;
				myfile << "**" << endl;
			}
			else // in the direction of growth
			{
				switch (previous_dir_proli[vessel])
				{
					case 1:
						node_v1 = i + 1 + j * n + 1; // right 
						node_v5 = i + 1 + j * n - 1; // left
					
						myfile << "*Cload" << endl; // right
						myfile << node_v1 << ", " << 1 << ", " << -f_endo << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // left
						myfile << node_v5 << ", " << 1 << ", " << f_endo << endl;
						myfile << "**" << endl;
					break;
				
					case 2:
						node_v2 = i + 1 + j * n - (n - 1);// bottom-right
						node_v6 = i + 1 + j * n + (n - 1); // top-left

						myfile << "*Cload" << endl; // bottom-right
						myfile << node_v2 << ", " << 1 << ", " << -f_endo_cos << endl;   // (cos45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // bottom-right
						myfile << node_v2 << ", " << 2 << ", " << f_endo_cos << endl;  // (sin45°*0.000000004)
						myfile << "**" << endl;		
					
						myfile << "*Cload" << endl; //top-left
						myfile << node_v6 << ", " << 1 << ", " << f_endo_cos << endl;   //(cos45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; //top-left
						myfile << node_v6 << ", " << 2 << ", " << -f_endo_cos << endl;  // (sin45°*0.000000004)
						myfile << "**" << endl;	
					break;
				
					case 3:
						node_v3 = i + 1 + j * n - n; //down
						node_v7 = i + 1 + j * n + n; // up
					
						myfile << "*Cload" << endl; //down
						myfile << node_v3 << ", " << 2 << ", " << f_endo << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // up
						myfile << node_v7 << ", " << 2 << ", " << -f_endo << endl;
						myfile << "**" << endl;
					break;
				
					case 4:
						node_v4 = i + 1 + j * n - (n + 1); // bottom-left
						node_v8 = i + 1 + j * n + (n + 1); // top-right
					
						myfile << "*Cload" << endl; //bottom-left
						myfile << node_v4 << ", " << 1 << ", " << f_endo_cos << endl;   // (cos45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; //bottom-left
						myfile << node_v4 << ", " << 2 << ", " << f_endo_cos << endl;  // (sin45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // top-right
						myfile << node_v8 << ", " << 1 << ", " << -f_endo_cos << endl;   //(cos45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // top-right
						myfile << node_v8 << ", " << 2 << ", " << -f_endo_cos << endl;  // (sin45°*0.000000004)
						myfile << "**" << endl;
					break;
				
					case 5:
						node_v1 = i + 1 + j * n + 1; // right 
						node_v5 = i + 1 + j * n - 1; // left
					
						myfile << "*Cload" << endl; // right
						myfile << node_v1 << ", " << 1 << ", " << -f_endo << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // left
						myfile << node_v5 << ", " << 1 << ", " << f_endo << endl;
						myfile << "**" << endl;
					break;
				
					case 6:
						node_v2 = i + 1 + j * n - (n - 1);// bottom-right
						node_v6 = i + 1 + j * n + (n - 1); // top-left

						myfile << "*Cload" << endl; // bottom-right
						myfile << node_v2 << ", " << 1 << ", " << -f_endo_cos << endl;   // (cos45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // bottom-right
						myfile << node_v2 << ", " << 2 << ", " << f_endo_cos << endl;  // (sin45°*0.000000004)
						myfile << "**" << endl;		
					
						myfile << "*Cload" << endl; //top-left
						myfile << node_v6 << ", " << 1 << ", " << f_endo_cos << endl;   //(cos45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; //top-left
						myfile << node_v6 << ", " << 2 << ", " << -f_endo_cos << endl;  // (sin45°*0.000000004)
						myfile << "**" << endl;	
					break;
				
					case 7:
						node_v3 = i + 1 + j * n - n; //down
						node_v7 = i + 1 + j * n + n; // up
					
						myfile << "*Cload" << endl; //down
						myfile << node_v3 << ", " << 2 << ", " << f_endo << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // up
						myfile << node_v7 << ", " << 2 << ", " << -f_endo << endl;
						myfile << "**" << endl;
					break;
				
					case 8:
						node_v4 = i + 1 + j * n - (n + 1); // bottom-left
						node_v8 = i + 1 + j * n + (n + 1); // top-right
					
						myfile << "*Cload" << endl; //bottom-left
						myfile << node_v4 << ", " << 1 << ", " << f_endo_cos << endl;   // (cos45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; //bottom-left
						myfile << node_v4 << ", " << 2 << ", " << f_endo_cos << endl;  // (sin45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // top-right
						myfile << node_v8 << ", " << 1 << ", " << -f_endo_cos << endl;   //(cos45°*0.000000004)
						myfile << "**" << endl;

						myfile << "*Cload" << endl; // top-right
						myfile << node_v8 << ", " << 2 << ", " << -f_endo_cos << endl;  // (sin45°*0.000000004)
						myfile << "**" << endl;
					break;
				} //end switch
			}// end if
		} //end if
	} //end for



	int cnt_3=0;

	for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
        {
            if (cell_matrix[j][i]>2 )
            {
            	cnt_3=cnt_3+1;
            }
        }
    }
	ofstream file;
    file.open ("cell_number.txt"); 
    file<<cnt_3;
    file.close();

	cout<<"Number of fibroblasts after proliferation and appoptosis"<<cnt_3<<endl; 
    //cout<<"<--- New file Wrote Successfully"<<endl; 
     
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
	myfile<<"EP, E"<<endl;
    myfile<<"*EL PRINT, POSITION=AVERAGED AT NODES, ELSET=ALL, frequency=99999"<<endl; //before: ELSET=ALL
    myfile<<"EP, E"<<endl;
    myfile<<"*NODE PRINT, NSET=ALL, frequency=99999"<<endl;
    myfile<<"U,RF"<<endl;
    myfile<<"** HISTORY OUTPUT: H-Output-1"<<endl;
    myfile<<"*Output, history, variable=PRESELECT"<<endl;
    myfile<<"*End Step"<<endl;
    myfile.close();                      
}

