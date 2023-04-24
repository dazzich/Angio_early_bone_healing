#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include "header.h"

int cell_lattice [N][N];
int dipole_direction[N][N];
double cell_deformation[N][N];

double Nodes_x[N*N];
double Nodes_y[N*N];

//Temporal model
int new_cell_lattice[N][N]; 
int new_dipole_direction[N][N];
double new_cell_deformation[N][N];

int cell_migration [N][N];

int vessels_lattice_x [(N-1)*20][N-1]; // rows are vessels and each row contains the x coordinates of nodes composing the vessel
int vessels_lattice_y [(N-1)*20][N-1]; // rows are vessels and each row contains the y coordinates of nodes composing the vessel
int tip_x [(N-1)*20];
int tip_y [(N-1)*20];
int previous_growth_dir [(N-1)*20];
int vessel_length [(N-1)*20];


/*   Type of cell in each position (array cells)
       0: empty and availabe
       1: forbidden
       2: ECs
       3 ++ : fibroblasts
*/


using namespace std;

int main()
{
    srand((unsigned)time(0));
    
    int model;
    int iteration;
    int fibro_cell_number;
    int endo_cell_number;
    int number_vessels;
	int i,j;
	
    for (iteration=0;iteration<505;iteration++) //504 iters = 7 DAYS
    {
	 	
	    /**************************************************************************************
	                                              Read iteration
	    *****************************************************************************************/    
	    cout<<"READ ITERATION"<<endl;
	    ifstream infile("iteration.txt");
	    infile>>iteration;
	  
	    cout<<"Iteration number: "<<iteration<<endl;
	    
	    infile.close();  
	
	    /**************************************************************************************
		                              Initialize FEM and blood vessels
		*****************************************************************************************/ 
		if (iteration==0)
	    {
		    cout<<"INITIALIZATION"<<endl;
		    Initialize_model(cell_lattice,vessels_lattice_x, vessels_lattice_y, dipole_direction, fibro_cell_number, number_vessels, tip_x, tip_y, previous_growth_dir, vessel_length, Nodes_x, Nodes_y);
		    
		    /**************************************************************************************
		                                              Write batch file
		    *****************************************************************************************/                   
		    cout<<"WRITE BATCH FILE"<<endl; 
		    model=1;
		    Write_batch_file(iteration,model); 
		    
		    /**************************************************************************************
		                                              Run abaqus
		    *****************************************************************************************/  
		    cout<<"RUN ABAQUS"<<endl;
		    system("./run.bat");    
	    }
	    
	    /**************************************************************************************
	                                             Read cell matrix
	    *****************************************************************************************/ 
	    if (iteration>0)
	    {
	    	cout<<"READ CELL LATTICE"<<endl;
	    	Read_cell_matrix(cell_lattice,dipole_direction,iteration);
	    }
		
	    /**************************************************************************************
	                                             Read displacements
	    *****************************************************************************************/ 
	    cout<<"READ DISPLACEMENTS AND COMPUTE DEFORMATIONS"<<endl;  
	    model=1; //not temporal
	    Read_displacements(cell_lattice, dipole_direction, cell_deformation, iteration, model);           
	
	    /**************************************************************************************
	                                        Create temporary FEM
	    *****************************************************************************************/
	    cout<<"CREATE TEMPORARY FEM"<<endl;
	    Create_temporal_FEM(cell_lattice, new_cell_lattice, new_dipole_direction, iteration, cell_migration, number_vessels, tip_x, tip_y, Nodes_x, Nodes_y, previous_growth_dir); 
	        
	    /**************************************************************************************
	                                          Write batch file
	    *****************************************************************************************/                   
	    cout<<"WRITE BATCH FILE"<<endl;  
	    model=2; //temporal model
	    Write_batch_file(iteration,model);  
	        
	    /**************************************************************************************
	                                            Run abaqus
	    *****************************************************************************************/  
	    cout<<"RUN ABAQUS"<<endl;
	    system("./run.bat");
	        
	    /**************************************************************************************
	                                            Read_displacements
	    *****************************************************************************************/                 
	    cout<<"READ DISPLACMENTS TEMPORAL MODEL AND COMPUTE DEFORMATIONS"<<endl;
	    model=2;  //temporal model
	    Read_displacements(new_cell_lattice, new_dipole_direction, new_cell_deformation, iteration, model);           
	      
		/**************************************************************************************
                                                 Vessel growth
        *****************************************************************************************/ 
    	if ((iteration)%2==0) //every 2 iteration ( growth speed = 15 micron per hour, Kenyon et al. 1996)
    	{
    		cout<<"BLOOD VESSELS GROWTH"<<endl;   
   			Blood_vessels(cell_lattice,vessels_lattice_x, vessels_lattice_y,number_vessels,tip_x,tip_y,previous_growth_dir,vessel_length, iteration);
    
    		cout<<"WRITING RESULTS BLOOD VESSELS"<<endl;
    		Write_data_vessels(iteration, vessels_lattice_x, vessels_lattice_y, number_vessels);

		}  
	    /**************************************************************************************
	                                         Update_model
	    *****************************************************************************************/ 
	    cout<<"UPDATE MODEL"<<endl;
	    Update_model(cell_lattice, dipole_direction, cell_deformation, new_dipole_direction, new_cell_deformation, iteration, cell_migration);
	    
		/**************************************************************************************
	                                         Proliferation_apoptosis
	    *****************************************************************************************/ 
	    cout<<"PROLIFERATION AND APOPTOSIS"<<endl;
		
	    Proliferation_apoptosis(cell_lattice, dipole_direction, iteration, fibro_cell_number, number_vessels, tip_x, tip_y, Nodes_x, Nodes_y,previous_growth_dir);      
		

		/*************************************************************************************
	                                          Write results
	    *****************************************************************************************/                 
	    cout<<"WRITE RESULTS"<<endl;
	    Write_results(cell_lattice, dipole_direction,iteration);
	     
	          
	    /**************************************************************************************
	                                              Remove abaqus filed
	    *****************************************************************************************/ 
	    if (iteration>0)
		{
		cout<<"REMOVING ABAQUS FILES"<<endl;
		
		
			ostringstream s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11;
	    	s1<<"Temporal_FEM_"<<iteration-1<<".inp";
	    	s2<<"Temporal_FEM_"<<iteration-1<<".dat";
	    	s3<<"Temporal_FEM_"<<iteration-1<<".odb";
	    	s4<<"Temporal_FEM_"<<iteration-1<<".prt";
	    	s5<<"Temporal_FEM_"<<iteration-1<<".stt";
	    	s6<<"Temporal_FEM_"<<iteration-1<<".com";
	    	s7<<"Temporal_FEM_"<<iteration-1<<".mdl";
	    	s8<<"Temporal_FEM_"<<iteration-1<<".msg";
	    	s9<<"Temporal_FEM_"<<iteration-1<<".sta";
	    	s10<<"Temporal_FEM_"<<iteration-1<<".res";
	    	s11<<"Temporal_FEM_"<<iteration-1<<".sim";
	    
	    	string filename1=s1.str(); 
	    	string filename2=s2.str(); 
	    	string filename3=s3.str(); 
	    	string filename4=s4.str(); 
	    	string filename5=s5.str(); 
	    	string filename6=s6.str(); 
	    	string filename7=s7.str(); 
	    	string filename8=s8.str(); 
	    	string filename9=s9.str(); 
	    	string filename10=s10.str();
	    	string filename11=s11.str();
	    
	    	remove(filename1.c_str());
	    	remove(filename2.c_str());
	    	remove(filename3.c_str());
	    	remove(filename4.c_str());
	    	remove(filename5.c_str());
	    	remove(filename6.c_str());
	    	remove(filename7.c_str());
	    	remove(filename8.c_str());
	    	remove(filename9.c_str());
	    	remove(filename10.c_str());
	    	remove(filename11.c_str());
	    
	    
	    	ostringstream r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11;
	    	r1<<"FEM_"<<iteration-1<<".inp";
	    	r2<<"FEM_"<<iteration-1<<".dat";
	    	r3<<"FEM_"<<iteration-1<<".odb";
	    	r4<<"FEM_"<<iteration-1<<".prt";
	    	r5<<"FEM_"<<iteration-1<<".stt";
	    	r6<<"FEM_"<<iteration-1<<".com";
	    	r7<<"FEM_"<<iteration-1<<".mdl";
	    	r8<<"FEM_"<<iteration-1<<".msg";
	    	r9<<"FEM_"<<iteration-1<<".sta";
	   		r10<<"FEM_"<<iteration-1<<".res";
	    	r11<<"FEM_"<<iteration-1<<".sim";
	
	    
	    	string file1=r1.str(); 
	        string file2=r2.str(); 
	        string file3=r3.str(); 
	        string file4=r4.str(); 
	        string file5=r5.str(); 
	        string file6=r6.str();
	        string file7=r7.str();
	        string file8=r8.str();
	        string file9=r9.str();
	        string file10=r10.str();
	        string file11=r11.str();
	        
	        remove(file1.c_str());
	        remove(file2.c_str());
	        if ((iteration-1)%50 != 0 || iteration == 1)
	    	{ 
	        	remove(file3.c_str());
	    	}
	        remove(file4.c_str());
	        remove(file5.c_str());
	        remove(file6.c_str());
	        remove(file7.c_str());
	        remove(file8.c_str());
	        remove(file9.c_str());
	        remove(file10.c_str());
	        remove(file11.c_str());
		}
			
	    cout<<"ABAQUS FILES REMOVED SUCCESSFULLY"<<endl;         
	        
	               
	    /**************************************************************************************
	                                              Write batch file
	    *****************************************************************************************/                   
	    cout<<"WRITE BATCH FILE"<<endl; 
	    int iter;
	    iter=iteration+1;
	    model=1;
	    Write_batch_file(iter,model); 
	        
	    /**************************************************************************************
	                                              Run abaqus
	    *****************************************************************************************/  
	    cout<<"RUN ABAQUS"<<endl;
	    system("./run.bat");
	    
	    
	    /**************************************************************************************
	                                          Update iteration
	    *****************************************************************************************/    
	    cout<<"UPDATE ITERATION"<<endl;
	    ofstream file;
	    file.open ("iteration.txt"); 
	    file<<iteration+1;
	    file.close();  
	     
	    cout<<"ITERATION UPDATED"<<endl;
	   
	    //system("PAUSE");
    }
    return 0;
}

