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

void Proliferation_apoptosis(int cell_matrix[N][N], int dipole[N][N],int iter, int cell_number, int& number_vessels_prol, int tip_x_prol[(N-1)*20], int tip_y_prol[(N-1)*20], double X[N*N], double Y[N*N],int previous_dir_proli [(N-1)*20])
{

    ifstream infile_num("cell_number.txt");
    infile_num>>cell_number;	
	
/************************************************************************************************************************************
														Counting available cells 
************************************************************************************************************************************/ 	

	int cell_cnt=0;
	int i=0;
	int j=0;
	
	for (j=0;j<N;j++)
	{
	    for (i=0;i<N;i++)
	    {
			if (cell_matrix[j][i]>2)
			{
               cell_cnt=cell_cnt+1;
			}
    	}
	}
	
//	cout<<"Number of fibroblasts: "<<cell_cnt<<endl; 

/************************************************************************************************************************************
													Calculate the number of cell to proliferate   
************************************************************************************************************************************/ 
	/*
	Fibroblasts proliferation rate=0.55/day. Scaling because of 2D simplificatin: 0.55/2.25(18/8) = 0.24/day. 72 iters=1 day. --> 0.0033/iter.	
	*/
	float proli_factor;
	proli_factor = 0.0033; 
	int cell_proli=0;
	cell_proli = ceil(cell_cnt*proli_factor); 
	
/************************************************************************************************************************************
														Choosing cells to proliferate    
************************************************************************************************************************************/	
	vector<int>Availability;
	Availability.clear();
	int cnt=0; //proliferating cells counter
	int proliferation=0;
	
	if (cell_cnt <= 44000) // 80% of the nodes become full of cells (N*N -8000 (Cortical bone nodes))*0.8 = 44000
	{	
		while (cnt < cell_proli) 
		{
			i=rand()%N;
	        j=rand()%(N-2)+1; 
			
			/*****************************************************************
						Indentify empty positions around cell	    
			******************************************************************/
			
			if (cell_matrix[j][i]>2) 
	        {
				if (i+1<N && cell_matrix[j][i+1]==0) // Position 0
				{
					Availability.push_back(0);
				}
				if (j-1>0 && i+1<N && cell_matrix[j-1][i+1]==0) // Position 1
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
				if (j+1<N-1 && i-1>=0 && cell_matrix[j+1][i-1]==0) // Position 5
				{
					Availability.push_back(5);
				}
				if (j+1<N-1 && cell_matrix[j+1][i]==0) // Position 6
				{
					Availability.push_back(6);
				}
				if (j+1<N-1 && i+1<N && cell_matrix[j+1][i+1]==0) // Position 7
				{
					Availability.push_back(7);
				}
			
			/*****************************************************************
									Proliferating 	    
			******************************************************************/
				
				if (Availability.size()!=0)
				{
					sort(Availability.begin(), Availability.end());
					proliferation = Availability[rand()%Availability.size()]; // random position among those available around each cell
					
					switch (proliferation)
					{
						case 0:
						if (i+1<N && cell_matrix[j][i+1]==0) 
						{
							cnt=cnt+1;
							cell_matrix[j][i+1]=cell_number+2; 
							cell_number=cell_number+1; 
							if (i+1==0 || i+1==N-1) 
							{
								dipole[j][i+1]=3;
							}
							else
							{
								dipole[j][i+1]=rand()%4+1;
							}
						}
						break;
	
						case 1:
						if (i+1<N && j-1>0 && cell_matrix[j-1][i+1]==0)
						{
							cnt=cnt+1;
							cell_matrix[j-1][i+1]=cell_number+2;
							cell_number=cell_number+1;
							if (i+1==0 || i+1==N-1) 
							{
								dipole[j-1][i+1]=3;
							}
							else 
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
							if (i==0 || i==N-1) 
							{
								dipole[j-1][i]=3;
							}
							else
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
							if (i-1==0 || i-1==N-1) 
							{
								dipole[j-1][i-1]=3;
							}
							else 
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
							if (i-1==0 || i-1==N-1) 
							{
								dipole[j][i-1]=3;
							}
							else 
							{
								dipole[j][i-1]=rand()%4+1;
							}
						}
						break;
	
						case 5:
						if (j+1<N-1 && i-1>=0 && cell_matrix[j+1][i-1]==0)
						{				
							cnt=cnt+1;
							cell_matrix[j+1][i-1]=cell_number+2;
							cell_number=cell_number+1;
							if (i-1==0 || i-1==N-1) 
							{
								dipole[j+1][i-1]=3;
							}
							else 
							{
								dipole[j+1][i-1]=rand()%4+1;
							}
						}
						break;
	
						case 6:
						if (j+1<N-1 && cell_matrix[j+1][i]==0)
						{							
							cnt=cnt+1;
							cell_matrix[j+1][i]=cell_number+2;
							cell_number=cell_number+1;
							if (i==0 || i==N-1) 
							{
								dipole[j+1][i]=3;
							}
							else
							{
								dipole[j+1][i]=rand()%4+1;
							}
						}
						break;
	
						case 7:
						if (j+1<N-1 && i+1<N && cell_matrix[j+1][i+1]==0)
						{
							cnt=cnt+1;
							cell_matrix[j+1][i+1]=cell_number+2;
							cell_number=cell_number+1;
							if (i+1==0 || i+1==N-1) 
							{
								dipole[j+1][i+1]=3;
							}
							else 
							{
								dipole[j+1][i+1]=rand()%4+1;
							}
						}
						break; 
					} //end switch
				} //end if 
			}//end if  
		}//end while
	}//end if 
	
//	cout<<"Number of proliferated cells: "<<cnt<<endl; 
/************************************************************************************************************************************
															Apoptosis   
************************************************************************************************************************************/		
/************************************************************************************************************************************
														Counting available cells 
************************************************************************************************************************************/ 	

	cell_cnt=0;
	
	for (j=0;j<N;j++)
	{
	    for (i=0;i<N;i++)
	    {
			if (cell_matrix[j][i]>2)
			{ 
			   cell_cnt=cell_cnt+1;
			}
    	}
	}
	
	//<<"Number of cells after proliferation: "<<cell_cnt<<endl;
/************************************************************************************************************************************
													Calculate the number of cell for apoptosis   
************************************************************************************************************************************/ 
	/*
	Fibroblasts apoptois rate=0.05/day. Scaling because of 2D simplification: 0.05/2.25(18/8) = 0.02/day. 72 iters=1 day. --> 0.0003/iter.	
	*/	
	float apop_factor = 0.0003;
	int cell_apop=0;
	cell_apop = ceil(cell_cnt*apop_factor); // calculate number of cells. Ceil makes the round up of the value	

/************************************************************************************************************************************
														Choosing cells to apoptosis    
/************************************************************************************************************************************/
	int cnt_2=0;	
	while (cnt_2 <= cell_apop)
	{
		i=rand()%N;
        j=rand()%(N-2)+1;
		
		if (cell_matrix[j][i]>2) // if there is a fibroblast here 
        {
			cell_matrix[j][i]=0;
			dipole[j][i]=0;
			cnt_2=cnt_2+1;
		}
	}
	
//	cout<<" Number of dead cells: "<<cnt_2<<endl;
    
/************************************************************************************************************************************
                        Seeding cells in empty element of bone marrow & periosteum: countinuous source of cells
/************************************************************************************************************************************/

	int cnt_p=0;
	float density_fibro=0.2;
	int c_ext=round(density_fibro*SAFE); 

   	i=CL_left-1; 
	for (j=1;j<SAFE+1;j++) //check how many cells
	{
		if (cell_matrix[j][i]>2)
        {
        	cnt_p=cnt_p+1;
        }
    }
    while (cnt_p<c_ext) //if less than 20% of avail positions, add cells
    {
    	j=rand()%SAFE+1; 
    	if (cell_matrix[j][i]==0)
    	{
    		cell_matrix[j][i]=cell_number+3;
    		cell_number=cell_number+1;
    		cnt_p=cnt_p+1;
    		if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
		}
	}
  
    cnt_p=0;
	for (j=(N-1-SAFE);j<(N-1-SAFE)+SAFE;j++) 
	{
		if (cell_matrix[j][i]>2)
        {
        		cnt_p=cnt_p+1;
        }
    }
    while (cnt_p<c_ext)
	{
		j=rand()%SAFE+(N-1-SAFE);
	    if (cell_matrix[j][i]==0)
        {
            cell_matrix[j][i]=cell_number+3;
            cell_number=cell_number+1;
            cnt_p=cnt_p+1;
            if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
        }
    }

	cnt_p = 0;
	i = CL_left-2; 
	for (j = 1; j < SAFE + 1; j++) 
	{
		if (cell_matrix[j][i] > 2)
		{
			cnt_p = cnt_p + 1;
		}
	}
	while (cnt_p < c_ext)
	{
		j = rand() % SAFE + 1;
		if (cell_matrix[j][i] == 0)
		{
			cell_matrix[j][i] = cell_number + 3;
			cell_number = cell_number + 1;
			cnt_p = cnt_p + 1;
			if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
		}
	}

	cnt_p = 0;
	for (j = (N - 1 - SAFE); j < (N - 1 - SAFE) + SAFE; j++) 
	{
		if (cell_matrix[j][i] > 2)
		{
			cnt_p = cnt_p + 1;
		}
	}
	while (cnt_p < c_ext)
	{
		j = rand() % SAFE + (N - 1 - SAFE);
		if (cell_matrix[j][i] == 0)
		{
			cell_matrix[j][i] = cell_number + 3;
			cell_number = cell_number + 1;
			cnt_p = cnt_p + 1;
			if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
		}
	}
  
    cnt_p=0;
    i=CR_right;
	for (j=1;j<SAFE+1;j++) 
	{
	    if (cell_matrix[j][i]>2)
        {
        	cnt_p=cnt_p+1;
        }
    }
    
    while (cnt_p<c_ext)
	{
		j=rand()%SAFE+1;
	    if (cell_matrix[j][i]==0)
        {
            cell_matrix[j][i]=cell_number+3;
            cell_number=cell_number+1;
            cnt_p=cnt_p+1;
            if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
        }
    }
   
    cnt_p=0;
	for (j=(N-1-SAFE);j<(N-1-SAFE)+SAFE;j++) 
	{
	    if (cell_matrix[j][i]>2)
        {
        	cnt_p=cnt_p+1;
        }
    }
    while (cnt_p<c_ext)
	{
		j=rand()%SAFE+(N-1-SAFE);
	    if (cell_matrix[j][i]==0)
        {
            cell_matrix[j][i]=cell_number+3;
            cell_number=cell_number+1;
            cnt_p=cnt_p+1;
            if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
        }
    }

	cnt_p = 0;
	i = CR_right+1; 
	for (j = 1; j < SAFE + 1; j++) 
	{
		if (cell_matrix[j][i] > 2)
		{
			cnt_p = cnt_p + 1;
		}
	}

	while (cnt_p < c_ext)
	{
		j = rand() % SAFE + 1;
		if (cell_matrix[j][i] == 0)
		{
			cell_matrix[j][i] = cell_number + 3;
			cell_number = cell_number + 1;
			cnt_p = cnt_p + 1;
			if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
		}
	}

	cnt_p = 0;
	for (j = (N - 1 - SAFE); j < (N - 1 - SAFE) + SAFE; j++) 
	{
		if (cell_matrix[j][i] > 2)
		{
			cnt_p = cnt_p + 1;
		}
	}
	while (cnt_p < c_ext)
	{
		j = rand() % SAFE + (N - 1 - SAFE);
		if (cell_matrix[j][i] == 0)
		{
			cell_matrix[j][i] = cell_number + 3;
			cell_number = cell_number + 1;
			cnt_p = cnt_p + 1;
			if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
		}
	}
   
    int Marrow_cell_L=0;
	int Marrow_cell_U=0;
	int c_bm=round(density_fibro*SAFE*(CR_left-1-CL_right));
    
    for (i=CL_right;i<CR_left;i++)
    {
    	for (j=1;j<SAFE+1;j++)
    	{	
			if (cell_matrix[j][i]>2) 
        	{
            	Marrow_cell_L=Marrow_cell_L+1;
        	}
        }
    }
    for (i=CL_right;i<CR_left;i++) 
    {
    	for (j=(N-1-SAFE);j<N-1;j++)
    	{
	    	if (cell_matrix[j][i]>2)
        	{
            	Marrow_cell_U=Marrow_cell_U+1;
        	}
        }
    }
    
	while (Marrow_cell_L<c_bm) 
    {
        i=rand()%(CR_left-CL_right)+CL_right;
        j=rand()%SAFE+1;
		if (cell_matrix[j][i]==0)
	    {
	        cell_matrix[j][i]=cell_number+3;
	        cell_number=cell_number+1;
	        Marrow_cell_L=Marrow_cell_L+1;
	        if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
	    }
    }
    
    while (Marrow_cell_U<c_bm) 
    {
        i=rand()%(CR_left-CL_right)+CL_right;
        j=rand()%SAFE+(N-1-SAFE);
		if (cell_matrix[j][i]==0)
	    {
	        cell_matrix[j][i]=cell_number+3;
	        cell_number=cell_number+1;
	        Marrow_cell_U=Marrow_cell_U+1;
	        if (i==0 || i==N-1) 
			{
				dipole[j][i]=3;
			}
			else 
			{
				dipole[j][i]=rand()%4+1;
			}
	    }
    }	

/************************************************************************************************************************************
                    									Writing New file
************************************************************************************************************************************/
    
    ifstream infile("Control_rigid.txt");
          
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
     
    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            node=i+(j*N)+1;
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
     
    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            if (cell_matrix[j][i]>2)
            {
            //**************************************************
            //          Calculate nodes where load is applied
            //**************************************************
				if (dipole[j][i] == 1) 
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
				else if (dipole[j][i] == 2) 
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
				else if (dipole[j][i] == 3) 
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
				else if (dipole[j][i] == 4) 
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
			} // End if 
		} // End for i 
	} // End for j

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

	for (vessel = 0; vessel < number_vessels_prol; vessel++)
	{
		if (tip_x_prol[vessel] != 10000 && tip_x_prol[vessel] != 9999 && tip_y_prol[vessel] != 10000 && tip_y_prol[vessel] != 9999)
		{
			j = tip_x_prol[vessel]; 
			i = tip_y_prol[vessel];
			if (i == 0 || i == N-1) 
			{
				node_v3 = i + 1 + j * N - N; 
				node_v7 = i + 1 + j * N + N; 

				myfile << "*Cload" << endl; 
				myfile << node_v3 << ", " << 2 << ", " << F_ENDO << endl; 
				myfile << "**" << endl;

				myfile << "*Cload" << endl; 
				myfile << node_v7 << ", " << 2 << ", " << -F_ENDO << endl;
				myfile << "**" << endl;
			}
			else 
			{
				switch (previous_dir_proli[vessel])
				{
					case 1:
						node_v1 = i + 1 + j * N + 1;  
						node_v5 = i + 1 + j * N - 1; 
					
						myfile << "*Cload" << endl;
						myfile << node_v1 << ", " << 1 << ", " << -F_ENDO << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v5 << ", " << 1 << ", " << F_ENDO << endl;
						myfile << "**" << endl;
					break;
				
					case 2:
						node_v2 = i + 1 + j * N - (N - 1);
						node_v6 = i + 1 + j * N + (N - 1); 

						myfile << "*Cload" << endl; 
						myfile << node_v2 << ", " << 1 << ", " << -F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v2 << ", " << 2 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;		
					
						myfile << "*Cload" << endl;
						myfile << node_v6 << ", " << 1 << ", " << F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v6 << ", " << 2 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;	
					break;
				
					case 3:
						node_v3 = i + 1 + j * N - N; 
						node_v7 = i + 1 + j * N + N; 
					
						myfile << "*Cload" << endl; 
						myfile << node_v3 << ", " << 2 << ", " << F_ENDO << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v7 << ", " << 2 << ", " << -F_ENDO << endl;
						myfile << "**" << endl;
					break;
				
					case 4:
						node_v4 = i + 1 + j * N - (N + 1); 
						node_v8 = i + 1 + j * N + (N + 1); 
					
						myfile << "*Cload" << endl; 
						myfile << node_v4 << ", " << 1 << ", " << F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v4 << ", " << 2 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v8 << ", " << 1 << ", " << -F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v8 << ", " << 2 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;
					break;
				
					case 5:
						node_v1 = i + 1 + j * N + 1; 
						node_v5 = i + 1 + j * N - 1; 
					
						myfile << "*Cload" << endl; 
						myfile << node_v1 << ", " << 1 << ", " << -F_ENDO << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v5 << ", " << 1 << ", " << F_ENDO << endl;
						myfile << "**" << endl;
					break;
				
					case 6:
						node_v2 = i + 1 + j * N - (N - 1);
						node_v6 = i + 1 + j * N + (N - 1); 

						myfile << "*Cload" << endl; 
						myfile << node_v2 << ", " << 1 << ", " << -F_ENDOcos << endl;   
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v2 << ", " << 2 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;		
					
						myfile << "*Cload" << endl; 
						myfile << node_v6 << ", " << 1 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v6 << ", " << 2 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;	
					break;
				
					case 7:
						node_v3 = i + 1 + j * N - N; 
						node_v7 = i + 1 + j * N + N; 
					
						myfile << "*Cload" << endl; 
						myfile << node_v3 << ", " << 2 << ", " << F_ENDO << endl;
						myfile << "**" << endl;

						myfile << "*Cload" << endl;
						myfile << node_v7 << ", " << 2 << ", " << -F_ENDO << endl;
						myfile << "**" << endl;
					break;
				
					case 8:
						node_v4 = i + 1 + j * N - (N + 1);
						node_v8 = i + 1 + j * N + (N + 1);
					
						myfile << "*Cload" << endl; 
						myfile << node_v4 << ", " << 1 << ", " << F_ENDOcos << endl;  
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v4 << ", " << 2 << ", " << F_ENDOcos << endl; 
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v8 << ", " << 1 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;

						myfile << "*Cload" << endl; 
						myfile << node_v8 << ", " << 2 << ", " << -F_ENDOcos << endl;  
						myfile << "**" << endl;
					break;
				} //end switch
			}// end if
		} //end if
	} //end for



	int cnt_3=0;

	for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            if (cell_matrix[j][i]>2)
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
    myfile<<"*EL PRINT, POSITION=AVERAGED AT NODES, ELSET=ALL, frequency=99999"<<endl;
    myfile<<"EP, E"<<endl;
    myfile<<"*NODE PRINT, NSET=ALL, frequency=99999"<<endl;
    myfile<<"U"<<endl;
    myfile<<"** HISTORY OUTPUT: H-Output-1"<<endl;
    myfile<<"*Output, history, variable=PRESELECT"<<endl;
    myfile<<"*End Step"<<endl;
    myfile.close();                      
}

