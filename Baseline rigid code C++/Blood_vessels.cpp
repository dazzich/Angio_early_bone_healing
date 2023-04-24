//		 Direction of growth blood vessel:
//
//		  6  |  7  | 8      + out of plane (9)
//      ----------------
//		  5 | tip | 1
//	    ----------------
//		  4 |  3  | 2
// 1: right
// 2: bottom-right
// 3: down
// 4: bottom-left
// 5: left
// 6: top-left
// 7: up
// 8: top-right


#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>       //To read and write data from files as input/output streams
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <math.h> 
#include "header.h"

using namespace std;

void Blood_vessels(int cells_vessels[N][N],int vessels_x[(N-1)*20][N-1],int vessels_y[(N-1)*20][N-1], int& num_vessel, int tip_x[(N-1)*20], int tip_y[(N-1)*20], int previous_direction[(N-1)*20], int v_length[(N-1)*20], int iter)
{
	int direction,direction1,direction2,direction3;
    int i,j;
    int vessel;
    int new_number_vessel;
    int prob_direction; //random number between 0 and 9
    int previous_direction_branch;
    int new_i,new_j,new_branch_i,new_branch_j;
    int length_vessel;
    int visual_i,visual_j,visual_i_branch,visual_j_branch;
     
	const int p1=4; // probability of following strain-based rule (40%)
    const int p2=6; // probability of persistence in the direction of growth (30%)
    // 1-P1-P2= probability of random growth
    
	// Out of the plane//
	const int p_out=56; //10/18 is 56%
    int count_out=0;
    int n_out; //random number between 0 and 99
	
	double EP1[N*N],EP2[N*N]; //P1=min; P2=max;
	double E11[N*N], E22[N*N], E12[N*N];
	
	float density;
    float dens_max=10.0f; //10% from Stefanowski et al. 2019
	
    
//*********************************************************************
//               Reading nodal strains
//*********************************************************************
    cout<<"Reading strains at nodes "<<endl;
    
	ostringstream s;

    s<<"FEM_"<<iter<<".dat";

    string filename=s.str();
    
    ifstream infile_strain;
    infile_strain.open(filename.c_str());
      
 	string temp;
 	string line;
        
    infile_strain>>temp;
    
    while (temp!="EP1")  //find the word EP1 in the file
    {
          infile_strain>>temp;
    }
    getline (infile_strain,line);
    
    getline (infile_strain,line);
    
    getline (infile_strain,line); // skip 3 lines
    
    int node;
    
    int k; // index of nodes
	float canc;
    float c[N*N];
    
    for (k=0;k<N*N;k++)
    {   
        infile_strain>>node;  //Node
        
        infile_strain>>EP1[k]; //min princ strain
        
		infile_strain>>canc;
		
		infile_strain>>EP2[k];
		
		if (abs(EP1[k])>abs(EP2[k]))
        {
        	if (abs(EP1[k])<=STRAIN_1)
        	{
        		c[k]=1;	
			}
			else if (abs(EP1[k])>STRAIN_1 && abs(EP1[k])<=STRAIN_2)
			{
				c[k]=(1/(STRAIN_2-STRAIN_1))*(STRAIN_1-abs(EP1[k]))+1;
			}
			else
			{
				c[k]=0;
			}
		}
		else
		{
			if (abs(EP2[k])<=STRAIN_1)
        	{
        		c[k]=1;	
			}
			else if (abs(EP2[k])>STRAIN_1 && abs(EP2[k])<=STRAIN_2)
			{
				c[k]=(1/(STRAIN_2-STRAIN_1))*(STRAIN_1-abs(EP2[k]))+1;
			}
			else
			{
				c[k]=0;
			}
		}
		
        infile_strain>>E11[k];    //E11
        
        infile_strain>>E22[k];    //E22
		infile_strain>>canc;	  //E33							
            
        infile_strain>>E12[k];    //E12
    }
    
    new_number_vessel=num_vessel;
    new_i=new_j=new_branch_i=new_branch_j=0;

     
    for (vessel=0; vessel<num_vessel; vessel++)
    {
        j=tip_x[vessel]; // select tip cell
        i=tip_y[vessel];
		
		node = i + (j * N) + 1;
		if (i!=9999 && i!=10000 && j!=9999 && j!=10000 && abs(EP1[node])<=0.3) // if neither anastomosis point nor out of plane & strains are below 30%
        {
			length_vessel=v_length[vessel]; 
         
         /**********************************************************************
             DIRECTION OF GROWTH
             direction1: strains-based rule       
             direction2: random 
             direction3: previous direction
         
         **********************************************************************/
            			
			direction1=Direction_strains(j,i,E11,E22,E12,c); // direction of principal strain
			direction2=rand()%8+1;
         	direction3=previous_direction[vessel];
								
			prob_direction=rand()%10; //0-9
         	if (prob_direction<p1) //40% strains-based rule
         	{
                direction=direction1; 
         	}
         	else
         	{ 
            	if(prob_direction>p2)
             	{
                	direction=direction3; //30% previous
             	}                      
             	else
             	{// 30% random
             		n_out=rand()%100; 
            		if (n_out<p_out && iter>15 && (iter)%16==0) // every 5.3 hours, vessels growing in a random direction can disappear from the plane
           			{
            			direction=9; 
            			previous_direction[vessel]=9;
            			tip_x[vessel]=10000; //out of plane
                		tip_y[vessel]=10000;
  						v_length[vessel]=v_length[vessel]+1;
  						vessels_x[vessel][length_vessel+1]=tip_x[vessel];
    					vessels_y[vessel][length_vessel+1]=tip_y[vessel]; 
    					count_out=count_out+1;
					}
					else
					{
	   		            direction=direction2; //in the plane - random
	   		        }
             	}                      
         	}		
         
         	if ((direction==3 && direction3==7) || (direction==7 && direction3==3) ||  
             	(direction==1 && direction3==5) || (direction==5 && direction3==1) ||   
             	(direction==2 && direction3==6) || (direction==6 && direction3==2) ||
             	(direction==4 && direction3==8) || (direction==8 && direction3==4)) 
         	{
                direction=direction3;
         	}
        
        /***********************************************************************
                                  Branching
                        Function of the length (number of cells):
                                 
                        if length <= C1: it does not branch
                        if length > C2: it branches
                        if C1 < length < C2: linear probability
        ***********************************************************************/
            int branching;
            branching=Branching(length_vessel);     //branching=1 -> it branches, if 0 no

         /**********************************************************************
                               LENGTH INCREASE=constant, 1 position every 2 iterations 
                        
         **********************************************************************/
            int length,length_branch;
            length=1; 
            length_branch=length;
                  
         /***********************************************************************
                             Blood vessel growth
         ***********************************************************************/    
		    int l;  
            switch (direction)
            {
            	case 1:  //RIGHT  
                Direction_growth_1(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                previous_direction[vessel]=1;
                tip_x[vessel]=new_j;
                tip_y[vessel]=new_i;
                v_length[vessel]=v_length[vessel]+length;
                l=v_length[vessel];
                vessels_x[vessel][l]=visual_j; // only for plot purposes
    			vessels_y[vessel][l]=visual_i;
                if (branching==1 && length_branch!=0)
                {
                    Branch_dir1(cells_vessels,j,i,previous_direction_branch,new_branch_j,new_branch_i,length_branch,visual_i_branch,visual_j_branch);
                    new_number_vessel=new_number_vessel+1;
                    tip_x[new_number_vessel-1]=new_branch_j;
                    tip_y[new_number_vessel-1]=new_branch_i;
                    vessels_x[new_number_vessel-1][0]=j;
    			 	vessels_y[new_number_vessel-1][0]=i;
                    previous_direction[new_number_vessel-1]=previous_direction_branch;
                    v_length[new_number_vessel-1]=length_branch;
                    vessels_x[new_number_vessel-1][length_branch]=visual_j_branch;
    			 	vessels_y[new_number_vessel-1][length_branch]=visual_i_branch;
                }
                break;
                 
                case 2:       
                Direction_growth_2(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                previous_direction[vessel]=2;
                tip_x[vessel]=new_j;
                tip_y[vessel]=new_i;
                v_length[vessel]=v_length[vessel]+length; 
                l=v_length[vessel];
                vessels_x[vessel][l]=visual_j;
    			vessels_y[vessel][l]=visual_i;
                if (branching==1 && length_branch!=0) 
                {
                    Branch_dir2(cells_vessels,j,i,previous_direction_branch,new_branch_j,new_branch_i,length_branch,visual_i_branch,visual_j_branch);
                    new_number_vessel=new_number_vessel+1;
                    tip_x[new_number_vessel-1]=new_branch_j;
                    tip_y[new_number_vessel-1]=new_branch_i;
                    vessels_x[new_number_vessel-1][0]=j;
    			 	vessels_y[new_number_vessel-1][0]=i;
                    previous_direction[new_number_vessel-1]=previous_direction_branch;
                    v_length[new_number_vessel-1]=length_branch;
                    vessels_x[new_number_vessel-1][length_branch]=visual_j_branch;
    			 	vessels_y[new_number_vessel-1][length_branch]=visual_i_branch;
                }
                break;
                
                case 3: //DOWN
                Direction_growth_3(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                previous_direction[vessel]=3;
                tip_x[vessel]=new_j;
                tip_y[vessel]=new_i;
                v_length[vessel]=v_length[vessel]+length;
                l=v_length[vessel];
                vessels_x[vessel][l]=visual_j; 
    			vessels_y[vessel][l]=visual_i; 
                if (branching==1 && length_branch!=0)
                {
                    Branch_dir3(cells_vessels,j,i,previous_direction_branch,new_branch_j,new_branch_i,length_branch,visual_i_branch,visual_j_branch);
                    new_number_vessel=new_number_vessel+1;
                    tip_x[new_number_vessel-1]=new_branch_j;
                    tip_y[new_number_vessel-1]=new_branch_i;
                    vessels_x[new_number_vessel-1][0]=j;
    			 	vessels_y[new_number_vessel-1][0]=i;
                    previous_direction[new_number_vessel-1]=previous_direction_branch;
                    v_length[new_number_vessel-1]=length_branch; 
                    vessels_x[new_number_vessel-1][length_branch]=visual_j_branch;
    			 	vessels_y[new_number_vessel-1][length_branch]=visual_i_branch;
                }
                break;
                
                case 4:     
                Direction_growth_4(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                previous_direction[vessel]=4;
                tip_x[vessel]=new_j;
                tip_y[vessel]=new_i;
                v_length[vessel]=v_length[vessel]+length;
                l=v_length[vessel];
                vessels_x[vessel][l]=visual_j;
    			vessels_y[vessel][l]=visual_i;
                if (branching==1 && length_branch!=0)
                {
                 	Branch_dir4(cells_vessels,j,i,previous_direction_branch,new_branch_j,new_branch_i,length_branch,visual_i_branch,visual_j_branch);
                    new_number_vessel=new_number_vessel+1;
                    tip_x[new_number_vessel-1]=new_branch_j;
                    tip_y[new_number_vessel-1]=new_branch_i;
                    vessels_x[new_number_vessel-1][0]=j;
    			 	vessels_y[new_number_vessel-1][0]=i;
                    previous_direction[new_number_vessel-1]=previous_direction_branch;
                    v_length[new_number_vessel-1]=length_branch;
                    vessels_x[new_number_vessel-1][length_branch]=visual_j_branch;
    			 	vessels_y[new_number_vessel-1][length_branch]=visual_i_branch;
                }
                break;
                
                case 5:   //LEFT      
                Direction_growth_5(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                previous_direction[vessel]=5;
                tip_x[vessel]=new_j;
                tip_y[vessel]=new_i;
                v_length[vessel]=v_length[vessel]+length;
                l=v_length[vessel];
                vessels_x[vessel][l]=visual_j;
    			vessels_y[vessel][l]=visual_i;
                if (branching==1 && length_branch!=0)
                {
                    Branch_dir5(cells_vessels,j,i,previous_direction_branch,new_branch_j,new_branch_i,length_branch,visual_i_branch,visual_j_branch);
                    new_number_vessel=new_number_vessel+1;
                    tip_x[new_number_vessel-1]=new_branch_j;
                    tip_y[new_number_vessel-1]=new_branch_i;
                    vessels_x[new_number_vessel-1][0]=j;
    			 	vessels_y[new_number_vessel-1][0]=i;
                    previous_direction[new_number_vessel-1]=previous_direction_branch;
                    v_length[new_number_vessel-1]=length_branch;
                    vessels_x[new_number_vessel-1][length_branch]=visual_j_branch;
    			 	vessels_y[new_number_vessel-1][length_branch]=visual_i_branch;
                }
				break;
				
				case 6:       
                Direction_growth_6(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                previous_direction[vessel]=6;
                tip_x[vessel]=new_j;
                tip_y[vessel]=new_i;
                v_length[vessel]=v_length[vessel]+length;
                l=v_length[vessel];
                vessels_x[vessel][l]=visual_j;
    			vessels_y[vessel][l]=visual_i;
                if (branching==1 && length_branch!=0)
                {
                 	Branch_dir6(cells_vessels,j,i,previous_direction_branch,new_branch_j,new_branch_i,length_branch,visual_i_branch,visual_j_branch);
                    new_number_vessel=new_number_vessel+1;
                    tip_x[new_number_vessel-1]=new_branch_j;
                    tip_y[new_number_vessel-1]=new_branch_i;
                    vessels_x[new_number_vessel-1][0]=j;
    			 	vessels_y[new_number_vessel-1][0]=i;
                    previous_direction[new_number_vessel-1]=previous_direction_branch;
                    v_length[new_number_vessel-1]=length_branch;
                    vessels_x[new_number_vessel-1][length_branch]=visual_j_branch;
    			 	vessels_y[new_number_vessel-1][length_branch]=visual_i_branch;
				}
                break;
            
                case 7: //UP
                Direction_growth_7(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                previous_direction[vessel]=7;
                tip_x[vessel]=new_j;
                tip_y[vessel]=new_i;
                v_length[vessel]=v_length[vessel]+length;
                l=v_length[vessel];
                vessels_x[vessel][l]=visual_j;
    			vessels_y[vessel][l]=visual_i; 
                if (branching==1 && length_branch!=0)
                {
                    Branch_dir7(cells_vessels,j,i,previous_direction_branch,new_branch_j,new_branch_i,length_branch,visual_i_branch,visual_j_branch);
                    new_number_vessel=new_number_vessel+1;
                    tip_x[new_number_vessel-1]=new_branch_j;
                    tip_y[new_number_vessel-1]=new_branch_i;
                    vessels_x[new_number_vessel-1][0]=j;
    			 	vessels_y[new_number_vessel-1][0]=i;
                    previous_direction[new_number_vessel-1]=previous_direction_branch;
                    v_length[new_number_vessel-1]=length_branch;
                    vessels_x[new_number_vessel-1][length_branch]=visual_j_branch;
    			 	vessels_y[new_number_vessel-1][length_branch]=visual_i_branch;
                }
                break;
                
				case 8:        
                Direction_growth_8(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                previous_direction[vessel]=8;
                tip_x[vessel]=new_j;
                tip_y[vessel]=new_i;
                v_length[vessel]=v_length[vessel]+length;
                l=v_length[vessel];
                vessels_x[vessel][l]=visual_j;
    			vessels_y[vessel][l]=visual_i;
                if (branching==1 && length_branch!=0)
                {
                 	Branch_dir8(cells_vessels,j,i,previous_direction_branch,new_branch_j,new_branch_i,length_branch,visual_i_branch,visual_j_branch);
                    new_number_vessel=new_number_vessel+1;
                    tip_x[new_number_vessel-1]=new_branch_j;
                    tip_y[new_number_vessel-1]=new_branch_i;
                    vessels_x[new_number_vessel-1][0]=j;
    			 	vessels_y[new_number_vessel-1][0]=i;
                    previous_direction[new_number_vessel-1]=previous_direction_branch;
                    v_length[new_number_vessel-1]=length_branch;
                    vessels_x[new_number_vessel-1][length_branch]=visual_j_branch;
    			 	vessels_y[new_number_vessel-1][length_branch]=visual_i_branch;
                }

                break;           
            }//end switch
        }//end if  
  	}//end for 
  	

	int matrix_prob; 
    int p_in;

    p_in=5; //0.5% estimated, allows to have number of vessels appearing=disappearing
		
	float dist;
	float radius;
	int r0=125;
	int r=0;
	int min,max,range;
	int count_in=0; // vessel appearing from out of plane
	
	radius=(float(r0)/(400.0f-150.0f))*(400.0f-float(iter)); //radius decreases with time
    if (radius<0)
    {
    	radius=0;
	}
	
	if (iter>15 && (iter)%16==0) // every 5.3 hours
	{
    	for (j=1;j<N-1;j++)
    	{
    		for (i=1;i<N-1;i++)
    		{
    			
    			dist=pow((i-125),2)+((j-125),2); // squared distance between point (i,j) and centre of circumference (125, 125)
    			node = i + (j * N) + 1;
    			if (dist>pow(radius,2) && cells_vessels[j][i]==0 && abs(EP1[node])<=0.3)  //if point allows a new vessel to appear and strains are below 30%
    			{
    				matrix_prob=rand()%1000;
    				if (matrix_prob<p_in) //with a probability of 0.5%
    				{
						density=count_ec(j,i,cells_vessels,iter);
    					if (density<dens_max) //and if density is below 10%
    					{
    						cells_vessels[j][i]=2;
    						new_number_vessel=new_number_vessel+1;
	 						tip_x[new_number_vessel-1]=j;
            				tip_y[new_number_vessel-1]=i;
            				vessels_x[new_number_vessel-1][0]=tip_x[new_number_vessel-1];
    						vessels_y[new_number_vessel-1][0]=tip_y[new_number_vessel-1];
            				previous_direction[new_number_vessel-1]=9;
            				v_length[new_number_vessel-1]=0;
            				count_in=count_in+1;
    					}
					}
    			}
    		}
		}// end for 
	}// end if
	
	if (iter<150 && iter>15 && (iter)%16==0) // during the first 2/3 days (callus circumference still bigger than domain) vessel coming into plane from adjacent sections of bone marrow only
	{
		while (count_in<count_out)
		{
				min=45;
				max=205;
				range=max-min;
				i=(rand()%range)+min;
				r=rand()%2; 
				switch (r) 
				{
  					case 0:
  						min=0;
  						max=90;
  						break;
  					case 1:
  						min=160;
  						max=N-1;
  						break;
  				}
  				range = max - min;
				j = (rand() % range) + min;
				node = i + (j * N) + 1;
				density=count_ec(j,i,cells_vessels,iter);
				if (cells_vessels[j][i]==0 && density<dens_max && abs(EP1[node])<=0.3)
				{
					cells_vessels[j][i]=2;
    				new_number_vessel=new_number_vessel+1;
	 				tip_x[new_number_vessel-1]=j;
            		tip_y[new_number_vessel-1]=i;
            		vessels_x[new_number_vessel-1][0]=tip_x[new_number_vessel-1];
    				vessels_y[new_number_vessel-1][0]=tip_y[new_number_vessel-1];
            		previous_direction[new_number_vessel-1]=9;
            		v_length[new_number_vessel-1]=0;
            		count_in=count_in+1;
            	}
		}
	}


//	cout<<"vessel disappearing from the plane= "<<count_out<<endl;
//	cout<<"new vessels appeared from out of plane= "<<count_in<<endl;
    num_vessel=new_number_vessel;
    cout<<"N vessels "<<num_vessel<<endl;
}
