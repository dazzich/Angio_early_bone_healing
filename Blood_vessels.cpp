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
#include <math.h> 
#include "header.h"

using namespace std;

const int n=251;

void Blood_vessels(int cells_vessels[n][n],int vessels_x[(n-1)*20][n-1],int vessels_y[(n-1)*20][n-1], int& num_vessel, int tip_x[(n-1)*20], int tip_y[(n-1)*20], int previous_direction[(n-1)*20], int v_length[(n-1)*20], int iter)
{
	int direction,direction1,direction2,direction3;
    int i,j,i1,j1;
    int vessel;
    int cont;
    int new_number_vessel;
    int r,r1,r2,prob_direction,prob_disappear;
    int branching;
    int previous_direction_branch;
    int new_i,new_j,new_branch_i,new_branch_j;
    int length_vessel;
    int visual_i,visual_j;
    int visual_i_branch,visual_j_branch;
     
      
    const int p0=10; // 10% of probability of going in the out of plane direction every n iteration
	const int p1=4; // probability of strain guided growth (40%)
    const int p2=6; // 9-p2= probability of persistence (30%)
    // 1-P1-P2= probability of random growth
   
	const int p_out=56; //10/18 since the check to avoid tortuous path is after. At this level the potential positions are 18.
    int count_out=0;
    int n_out;
	
	double EP1[n*n],EP2[n*n]; //P1=min; P3=max;
	double E11[n*n], E22[n*n], E12[n*n]; 
    ostringstream s;

    s<<"FEM_"<<iter<<".dat";


    string filename=s.str();
    
    ifstream infile_strain;
    infile_strain.open(filename.c_str());
      
 	string temp;
 	string line;
    
//*********************************************************************
//               Reading nodal strains
//*********************************************************************
    cout<<"Reading strains at nodes "<<endl;
        
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
    float c[n*n];
    float strain1, strain2;
    strain1=0.05;
    strain2=0.13;
    
    for (k=0;k<n*n;k++)
    {   
        infile_strain>>node;  //Node
        
        infile_strain>>EP1[k]; //min princ strain
        
        if (abs(EP1[k])<=strain1)
        {
        	c[k]=1;	
		}
		else if (abs(EP1[k])>strain1 && abs(EP1[k])<=strain2)
		{
			c[k]=(1/(strain2-strain1))*(strain1-abs(EP1[k]))+1;
		}
		else
		{
			c[k]=0;
		}
		infile_strain>>EP2[k];

    
        infile_strain>>E11[k];    //E11
        
        infile_strain>>E22[k];    //E22
            
        infile_strain>>E12[k];    //E12
    }

    
    new_number_vessel=num_vessel;
    new_i=new_j=new_branch_i=new_branch_j=0;
    float density;
    float dens_max=10.0f;
     
    for (vessel=0; vessel<num_vessel; vessel++)
    {
        j=tip_x[vessel]; // select tip cell
        i=tip_y[vessel];

		
		node = i + (j * n) + 1;
		if (i!=9999 && i!=10000 && j!=9999 && j!=10000 && abs(EP1[node])<=0.3) // if not anastomosis point or out of plane
        {
			length_vessel=v_length[vessel]; 
         
         /**********************************************************************
             DIRECTION OF GROWTH
             direction1: towards principal strains       
             direction2: random 
             direction3: previous direction
         
         **********************************************************************/
            
			// growth in the plane
			
			direction1=Direction_strains(j,i,E11,E22,E12,c); // direction of principal strain
			direction2=rand()%8+1;
         	direction3=previous_direction[vessel];
								
			prob_direction=rand()%10; //1-9
//            density=count_ec(j,i,cells_vessels,iter);
         	if (prob_direction<p1) //40% in direction of principal strain
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
             	{//random
             		n_out=rand()%100; 
            		if (n_out<p_out && iter>15 && (iter)%16==0) 
           			{
            			direction=9; //disappear out of plane
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
	   		            direction=direction2; //30% random
	   		        }
             	}                      
         	}		
         
         	if ((direction==3 && direction3==7) || (direction==7 && direction3==3) ||   //tipo per evitare vasi contorti UP/DOWN
             	(direction==1 && direction3==5) || (direction==5 && direction3==1) ||   //LEFT/RIGHT
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
         
         branching=Branching(length_vessel);     //branching=1 -> it branches, if 0 no

         /**********************************************************************
                               LENGTH INCREASE
                               
                         Function of the mechanical environment
                             shear strain and fluid flow
                        
         **********************************************************************/
         int length,length_branch;
         length=1; //every four iter, the vessel grow of 10 microns
         length_branch=length;
                  
         /***********************************************************************
                             Blood vessel growth
         ***********************************************************************/    
		 int l;  
         switch (direction)
         {
            case 3: //DOWN
                 Direction_growth_3(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                 previous_direction[vessel]=3;
                 tip_x[vessel]=new_j;
                 tip_y[vessel]=new_i;
                 v_length[vessel]=v_length[vessel]+length;
                 l=v_length[vessel];
                 vessels_x[vessel][l]=visual_j; // just for plot purposes
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
            case 1:  //RIGHT  
                 Direction_growth_1(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                 previous_direction[vessel]=1;
                 tip_x[vessel]=new_j;
                 tip_y[vessel]=new_i;
                 v_length[vessel]=v_length[vessel]+length;
                 l=v_length[vessel];
                 vessels_x[vessel][l]=visual_j;
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
            case 2:         // down right
                 Direction_growth_2(cells_vessels,j,i,new_j,new_i,length,visual_i,visual_j);
                 previous_direction[vessel]=2;
                 tip_x[vessel]=new_j;
                 tip_y[vessel]=new_i;
                 v_length[vessel]=v_length[vessel]+length; //add branching down or right
                 l=v_length[vessel];
                 vessels_x[vessel][l]=visual_j;
    			 vessels_y[vessel][l]=visual_i;
                 if (branching==1 && length_branch!=0) // se cresce già in diagonale ? o faccio andare fuori dal piano i vasi
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
            case 4:      // down left   
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
            case 8:   //up right      
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
            case 6:     //up left    
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
            }//end switch
         }//end if  
  	}//end for 
  	
  	
  
//   cout<<"number of vessels coming in w/o density check= "<<count_in2<<endl;

//  	//count ECs and density
//	int n_ec=0;
//	float tot_dens_ec=0.0f;
//	for (j=0;j<n-1;j++)
//	{
//		for (i=0;i<n-1;i++)
//		{
//			if (cells_vessels[j][i]==2)
//			{
//				n_ec=n_ec+1;
//			}	
//		}
//	}
//	tot_dens_ec=float(n_ec)/((float(n)*float(n))-(21.0f*101.0f*4.0f));
//	cout<<"total number of endo cells"<<n_ec<<endl;
//	cout<<"total density of endo cells"<<tot_dens_ec<<endl;
//	
	int matrix_prob[n][n]; //in each lattice point I assign a probability for an EC to appear from the out of plane direction
    int p_in;
//    float density;
//    float dens_max;
//    dens_max=10.0f/7.0f*float(iter/72.0f);
//
//    int count_in=0;
    p_in=5;
//    p_in=round(10.0f*dens_max*0.3f*(1.0f/18.0f)*100.0f);
//    
//    if (iter>15 && (iter)%16==0)
//	{
//		cout<<"probability of appearing from out of plane"<<p_in<<endl;
//	}
//	
//	

	//WHAT I STILL NEED:
		
	float circ;
	float radius;
	int r0=125;
	r=0;
	int min,max,range;
	int count_in=0;
	
	
	
	if (iter>15 && (iter)%16==0) //new vessels appear only under these conditions
	{
    	for (j=1;j<n-1;j++)
    	{
    		for (i=1;i<n-1;i++)
    		{
    			//define the circle
    			circ=pow((i-125),2)+((j-125),2);
    			radius=(float(r0)/(400.0f-150.0f))*(400.0f-float(iter));
    			if (radius<0)
    			{
    				radius=0;
				}
    			node = i + (j * n) + 1;
    			if (circ>pow(radius,2) && cells_vessels[j][i]==0 && abs(EP1[node])<=0.3) //if point is empty and allow new vessels growth
    			{
    				matrix_prob[j][i]=rand()%1000;
    				if (matrix_prob[j][i]<p_in) //probability of 0.5%
    				{
						density=count_ec(j,i,cells_vessels,iter);
    					cout<<"the density is"<<density<<endl;
    					if (density<dens_max)
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
            				cout<<"entered"<<endl;
    					}
					}
    			}
    		}
		}// end for 
	}// end if
	
	if (iter<150 && iter>15 && (iter)%16==0)
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
  						max=n-1;
  						break;
  				}
  				range = max - min;
				j = (rand() % range) + min;
				node = i + (j * n) + 1;
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


	cout<<"vessel disappearing from the plane= "<<count_out<<endl;
	cout<<"new vessels appeared from out of plane= "<<count_in<<endl;
    num_vessel=new_number_vessel;
    cout<<"N vessels "<<num_vessel<<endl;
//    cout<<"updated number of vessels"<<num_vessel<<endl;
}
