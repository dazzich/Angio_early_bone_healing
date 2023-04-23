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

using namespace std;
void Update_model(int cell_matrix_upd[121][81], int dipole_dir_upd[121][81],int iter, double X_upd[9801],double Y_upd[9801])
{

/************************************************************************************************************************************
														Counting available cells 
************************************************************************************************************************************/ 	

	int cells_num=0;
	int i=0;
	int j=0;
	
	for (j=0;j<121;j++)
	{
	    for (i=0;i<81;i++)
	    {
			if (cell_matrix_upd[j][i]>1)
			{
               cells_num=cells_num+1;
			}
    	}
	}
	
/************************************************************************************************************************************
													Calculate the number of cell to proliferate   
************************************************************************************************************************************/ 
	
	int proli_factor = 0.01;
	// 55% is the proliferation rate of fibroblast per day. if we assume 550 iteration is equal to one day, each iteration 1 % proliferation
	int cell_proli=0;
	
	cell_proli = floor (cells_num * proli_factor); // calculate number of cells. floor makes the round down of the value
	
/************************************************************************************************************************************
														Choosing cells to proliferate    
************************************************************************************************************************************/	
	cout<<" ---> Cell proliferation started "<<endl;

	vector<int>Availability;
	Availability.clear();
	int cnt=0;
	int proliferation=0;
		
	while (cnt < cell_proli)
	{
		i=rand()%81;
        j=rand()%119+1;
		
		/*****************************************************************
						Identifiying empty positions around cell	    
		******************************************************************/
		
		if (cell_matrix_upd[j][i]>1) // if there is a cell here 
        {
			if (i+1<81 && cell_matrix_upd[j][i+1]==0) // Position 0
			{
				Availability.push_back(0);
			}
			if (j-1>0 && i+1<81 && cell_matrix_upd[j-1][i+1]==0) // Position 1
			{
				Availability.push_back(1);
			}
			if (j-1>0 && cell_matrix_upd[j-1][i]==0) // Position 2
			{
				Availability.push_back(2);
			}	
			if (j-1>0 && i-1>=0 && cell_matrix_upd[j-1][i-1]==0) // Position 3
			{
				Availability.push_back(3);
			}
			if (i-1>=0, cell_matrix_upd[j][i-1]==0) // Position 4
			{
				Availability.push_back(4);
			}
			if (j+1<120 && i-1>=0 && cell_matrix_upd[j+1][i-1]==0) // Position 5
			{
				Availability.push_back(5);
			}
			if (j+1<120 && cell_matrix_upd[j+1][i]==0) // Position 6
			{
				Availability.push_back(6);
			}
			if (j+1<120 && i+1<81 && cell_matrix_upd[j+1][i+1]==0) // Position 7
			{
				Availability.push_back(7);
			}
		
		/*****************************************************************
								Proliferating 	    
		******************************************************************/
			
			if (Availability.size()!=0)
			{
				sort(Availability.begin(), Availability.end());
				proliferation = Availability[rand()%Availability.size()];
				
				switch (proliferation)
				{
					case 0:
					if (i+1<81 && cell_matrix_upd[j][i+1]==0)
					{
						cell_matrix_upd[j][i+1]=cells_num+cnt;
						cnt=cnt+1;
						if (i==0 || i==80) //Axial dipole at left and right boundaries
						{
							dipole_dir_upd[j][i]=3;
						}
						else //random dipole direction
						{
							dipole_dir_upd[j][i]=rand()%4+1;
						}
					}
					break;

					case 1:
					if (i+1<81 && j-1>0 && cell_matrix_upd[j-1][i+1]==0)
					{
						cell_matrix_upd[j-1][i+1]=cells_num+cnt;
						cnt=cnt+1;
						if (i==0 || i==80) //Axial dipole at left and right boundaries
						{
							dipole_dir_upd[j-1][i+1]=3;
						}
						else //random dipole direction
						{
							dipole_dir_upd[j-1][i+1]=rand()%4+1;
						}
					}
					break;

					case 2:
					if (j-1>0 && cell_matrix_upd[j-1][i]==0)
					{
						cell_matrix_upd[j-1][i]=cells_num+cnt;
						cnt=cnt+1;
						if (i==0 || i==80) //Axial dipole at left and right boundaries
						{
							dipole_dir_upd[j-1][i]=3;
						}
						else //random dipole direction
						{
							dipole_dir_upd[j-1][i]=rand()%4+1;
						}
					}
					break;

					case 3:
					if (j-1>0 && i-1>=0 && cell_matrix_upd[j-1][i-1]==0)
					{
						cell_matrix_upd[j-1][i-1]=cells_num+cnt;
						cnt=cnt+1;
						if (i==0 || i==80) //Axial dipole at left and right boundaries
						{
							dipole_dir_upd[j-1][i-1]=3;
						}
						else //random dipole direction
						{
							dipole_dir_upd[j-1][i-1]=rand()%4+1;
						}
					}
					break;

					case 4:
					if (i-1>=0 && cell_matrix_upd[j][i-1]==0)
					{
						cell_matrix_upd[j][i-1]=cells_num+cnt;
						cnt=cnt+1;
						if (i==0 || i==80) //Axial dipole at left and right boundaries
						{
							dipole_dir_upd[j][i-1]=3;
						}
						else //random dipole direction
						{
							dipole_dir_upd[j][i-1]=rand()%4+1;
						}
					}
					break;

					case 5:
					if (j+1<120 && i-1>=0 && cell_matrix_upd[j+1][i-1]==0)
					{
						cell_matrix_upd[j+1][i-1]=cells_num+cnt;
						cnt=cnt+1;
						if (i==0 || i==80) //Axial dipole at left and right boundaries
						{
							dipole_dir_upd[j+1][i-1]=3;
						}
						else //random dipole direction
						{
							dipole_dir_upd[j+1][i-1]=rand()%4+1;
						}
					}
					break;

					case 6:
					if (j+1<120 && cell_matrix_upd[j+1][i]==0)
					{
						cell_matrix_upd[j+1][i]=cells_num+cnt;
						cnt=cnt+1;
						if (i==0 || i==80) //Axial dipole at left and right boundaries
						{
							dipole_dir_upd[j+1][i]=3;
						}
						else //random dipole direction
						{
							dipole_dir_upd[j+1][i]=rand()%4+1;
						}
					}
					break;

					case 7:
					if (j+1<120 && i+1<81 && cell_matrix_upd[j+1][i+1]==0)
					{
						cell_matrix_upd[j+1][i+1]=cells_num+cnt;
						cnt=cnt+1;
						if (i==0 || i==80) //Axial dipole at left and right boundaries
						{
							dipole_dir_upd[j+1][i+1]=3;
						}
						else //random dipole direction
						{
							dipole_dir_upd[j+1][i+1]=rand()%4+1;
						}
					}
					break; 
				} //end switch
			} //end if availability
		}//end if cell selected 
	}//end while
		
	cout<<" <--- Cells are proliferated "<<endl;	
	cout<<"Number of proliferated cells : "<<cnt<<endl; 
/************************************************************************************************************************************
															Apoptosis   
************************************************************************************************************************************/		
/************************************************************************************************************************************
														Counting available cells 
************************************************************************************************************************************/ 	

	cells_num=0;
	
	for (j=0;j<121;j++)
	{
	    for (i=0;i<81;i++)
	    {
			if (cell_matrix_upd[j][i]>1)
			{
               cells_num=cells_num+1;
			}
    	}
	}
	
/************************************************************************************************************************************
													Calculate the number of cell for apoptosis   
************************************************************************************************************************************/ 
	
	int apop_factor = 0.001;
	// 5% is the appoptosis rate of fibroblast per day. if we assume 550 iteration is equal to one day, each iteration 0.001 % proliferation
	int cell_apop=0;
	
	cell_apop = ceil (cells_num * apop_factor); // calculate number of cells. Ceil makes the round up of the value	

/************************************************************************************************************************************
														Choosing cells to apoptosis    
************************************************************************************************************************************/	
	cout<<" ---> Cell apoptosis started "<<endl;
	
	int cnt_2=0;
	
	
	while (cnt_2 < cell_apop)
	{
		i=rand()%81;
        j=rand()%119+1;
		
		if (cell_matrix_upd[j][i]>1) // if there is a cell here 
        {
			cell_matrix_upd[j][i]=0;
			dipole_dir_upd[j][i]=0;
			cnt_2=cnt_2+1;
		}
	}
	
	cout<<" ---> Cells appoptosis done successfully "<<endl;


/************************************************************************************************************************************
                    									Writing New file
************************************************************************************************************************************/
    cout<<"--->Write new file "<<endl; 
    
    ifstream infile("6mm_template_Elastic.txt");
          
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
     
    for (j=0;j<121;j++)
    {
        for (i=0;i<81;i++)
        {
            node=i+(j*81)+1;
            myfile<<node<<", "<<setprecision(15)<<X_upd[node]<<", "<<setprecision(15)<<Y_upd[node]<<endl;
        }
    }
     
    char c = infile.get();
     
    while  (!infile.eof())
    {
        myfile << c;
        c= infile.get();
    } 
     
    int node_1,node_2;
     
    for (j=0;j<121;j++)
    {
        for (i=0;i<81;i++)
        {
            if (cell_matrix_upd[j][i]>1 )
            {
            //***************************************************
            //        Calculate nodes where load is applied
            //***************************************************
                if (dipole_dir_upd[j][i]==1) //Lateral dipole 0°
            		{
             		  	node_1=i+1+j*81-1;
               		 	node_2=i+1+j*81+1;
            		}
	            else if (dipole_dir_upd[j][i]==2) //Diagonal right dipole 45°
	               {
	                  	node_1=i+1+j*81-82;
	                  	node_2=i+1+j*81+82;
	               }
	            else if (dipole_dir_upd[j][i]==3) //Axial dipole 90°
	                {
	                  	node_1=i+1+j*81-81;
	                  	node_2=i+1+j*81+81;
	                }
	            else if (dipole_dir_upd[j][i]==4) //Diagonal left dipole 135°(-45°)
                     {
	                  	node_1=i+1+j*81-80;
	                  	node_2=i+1+j*81+80;
	                 } 

            //**************************************************
            //          Write dipole forces to file
            //**************************************************
                if (dipole_dir_upd[j][i]==1) //90° Lateral cell
	            {
	                myfile<<"*Cload"<<endl;
	                myfile<<node_1<<", "<<1<<", 0.00000003"<<endl;
		            myfile<<"**"<<endl;
					
		            myfile<<"*Cload"<<endl;
		            myfile<<node_2<<", "<<1<<", -0.00000003"<<endl;
					myfile<<"**"<<endl;
	        	}
	            else if (dipole_dir_upd[j][i]==2) //-45° Diagonal Down cell
	        	{
		        	myfile<<"*Cload"<<endl;
		            myfile<<node_1<<", "<<1<<", 0.0000000212"<<endl;   // (cos45°*0.00000003)
					myfile<<"**"<<endl;
					
					myfile<<"*Cload"<<endl;
		            myfile<<node_1<<", "<<2<<", 0.0000000212"<<endl;  // (sin45°*0.00000003)
		            myfile<<"**"<<endl;
					
		            myfile<<"*Cload"<<endl;
		            myfile<<node_2<<", "<<1<<", -0.0000000212"<<endl;   //(cos45°*0.00000003)
					myfile<<"**"<<endl;
					
					myfile<<"*Cload"<<endl;
		            myfile<<node_2<<", "<<2<<", -0.0000000212"<<endl;  // (sin45°*0.00000003)
					myfile<<"**"<<endl;
	        	}
	        	else if (dipole_dir_upd[j][i]==3) //0° Axial cell
	            {
	                myfile<<"*Cload"<<endl;
	                myfile<<node_1<<", "<<2<<", 0.00000003"<<endl;
		            myfile<<"**"<<endl;
					
		            myfile<<"*Cload"<<endl;
		            myfile<<node_2<<", "<<2<<", -0.00000003"<<endl;
					myfile<<"**"<<endl;
	        	}
	        	else if (dipole_dir_upd[j][i]==4) // 45° Diagonal Up dipole 
	            {
	                myfile<<"*Cload"<<endl;
          		    myfile<<node_1<<", "<<1<<", -0.0000000212"<<endl;   // (cos45°*0.00000003)
					myfile<<"**"<<endl;
					
					myfile<<"*Cload"<<endl;
            	    myfile<<node_1<<", "<<2<<", 0.0000000212"<<endl;  // (sin45°*0.00000003)
            	    myfile<<"**"<<endl;
					
            		myfile<<"*Cload"<<endl;
            		myfile<<node_2<<", "<<1<<", 0.0000000212"<<endl;   //(cos45°*0.00000003)
					myfile<<"**"<<endl;
					
					myfile<<"*Cload"<<endl;
            		myfile<<node_2<<", "<<2<<", -0.0000000212"<<endl;  // (sin45°*0.00000003)
					myfile<<"**"<<endl;
	        	} 
            } // End if (cell_matrix_1[j][i]>1 )
        } // End for i 
    } // End for j

    cout<<"<--- New file Wrote Successfully"<<endl; 
     
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
//     myfile<<"SDV, E"<<endl;
    myfile<<"*EL PRINT, POSITION=CENTROID, ELSET=ALL, frequency=99999"<<endl;
//     myfile<<"SDV, E"<<endl;
    myfile<<"*NODE PRINT, NSET=ALL, frequency=99999"<<endl;
    myfile<<"U,RF"<<endl;
    myfile<<"** HISTORY OUTPUT: H-Output-1"<<endl;
    myfile<<"*Output, history, variable=PRESELECT"<<endl;
    myfile<<"*End Step"<<endl;
    myfile.close();                      
}
