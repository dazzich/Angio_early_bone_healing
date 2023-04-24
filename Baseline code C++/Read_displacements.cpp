#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <iomanip>
#include "header.h"

using namespace std;
void Read_displacements(int cells[N][N], int dip_dir[N][N], double deformation[N][N], int t,int model_type)
{
    double U1[N][N], U2[N][N];
	int i,j;
    ostringstream s;

    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {
            deformation[j][i]=0; // initialize deformation matrix
        }
    }
       
    if (model_type==1) // choose the dat. file to open
    {
       s<<"FEM_"<<t<<".dat";
    }
    else
    {
        s<<"Temporal_FEM_"<<t<<".dat";
    }   

    string filename=s.str();
    
    ifstream infile_re;
    infile_re.open(filename.c_str());
      
 	string temp;
 	string line;
 	float value; 
    
//*********************************************************************
//               Reading node displacement   
//*********************************************************************
    cout<<"Reading nodes displacement"<<endl;
        
    infile_re>>temp;
    
    while (temp!="U1")  //find the word U1 in the file
    {
          infile_re>>temp;
    }
    getline (infile_re,line);
    
    getline (infile_re,line);
    
    getline (infile_re,line); // skip 3 lines
    
	int node;
    
    for (j=0;j<N;j++)
    {   
        for (i=0;i<N;i++)
        {
            infile_re>>node;         //Node
    
            infile_re>>U1[j][i];     //Ux
        
            infile_re>>U2[j][i];     //Uy
        }
    }
    cout<<"Node displacements read successfully "<<endl;
   
//*******************************************************************
//                Calculate deformations caused by fibroblast dipoles
//*******************************************************************
    cout<<"Calculating deformations by fibroblasts "<<endl;
    
    for (j=0;j<N;j++)
    {   
        for (i=0;i<N;i++)
        {
            if (cells[j][i]>2) // if fibroblasts
            {
                if (dip_dir[j][i]==1) // Horizontal dipole 90°
                {
					deformation[j][i]= abs(U1[j][i-1] - U1[j][i+1]);
                } 
				if (dip_dir[j][i]==2) // Diagonal right dipole 45°
                {
                    deformation[j][i]= sqrt(pow(U1[j-1][i-1] - U1[j+1][i+1], 2) + pow(U2[j-1][i-1] - U2[j+1][i+1], 2));
                }        
                if (dip_dir[j][i]==3) // Vertical dipole 0°
                {
					deformation[j][i]= abs(U2[j-1][i]- U2[j+1][i]);
                }
                if (dip_dir[j][i]==4) // Diagonal left dipole 45°
                {
					deformation[j][i]= sqrt(pow((U1[j-1][i+1] - U1[j+1][i-1]), 2) + pow((U2[j-1][i+1] - U2[j+1][i-1]), 2)); 
                } 
            }
        }
    }
    
   cout<<"Deformations by fibroblasts calculated successfully "<<endl;    
}

	
