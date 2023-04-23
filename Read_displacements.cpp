#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <iomanip>

const int n=251;

using namespace std;
void Read_displacements(int cells[n][n], int dip_dir[n][n], double deformation[n][n], int t,int model_type, int& number_vessels_displ, int tip_x_displ[(n-1)*20], int tip_y_displ[(n-1)*20],  double X_read[n*n],double Y_read[n*n])
{
    double U1[n][n], U2[n][n], R1[n][n], R2[n][n];
	int i,j;
    ostringstream s;

    for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
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
    
    for (j=0;j<n;j++)
    {   
        for (i=0;i<n;i++)
        {
            infile_re>>node;         //Node
    
            infile_re>>U1[j][i];     //Ux
        
            infile_re>>U2[j][i];     //Uy
            
            infile_re>>R1[j][i];     //RFX
            
            infile_re>>R2[j][i];     //RFY
        }
    }
    cout<<"Node displacements read successfully "<<endl;
   
//*******************************************************************
//                Calculate deformations caused by fibroblast dipoles
//*******************************************************************
    cout<<"Calculating deformations by fibroblasts "<<endl;
    
    for (j=0; j<n;j++)
    {   
        for (i=0;i<n;i++)
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
   
//**************************************************************************
//                       Writing reaction forces  
//**************************************************************************

//    int cnt;
//    cnt=0;
//    j=j+1<120;
//    double Reaction_Top;
//    
//    if (model_type==1)
//    {
//       Reaction_Top=0;      
//                
//       for (i=0;i<121;i++)
//       {
//           cnt=cnt+1;
//           Reaction_Top=R2[j][i]+Reaction_Top;
//       }
//       
//	   Reaction_Top= abs(Reaction_Top)/cnt;
//       
//       ofstream myfile;
//       myfile.open("Reaction_Force_Top.txt", ios::app);
//     
//       myfile<<t<<" "<<Reaction_Top<<endl;
//       myfile.close(); 
//    }
    
//    cnt=0;
//    j=0;
//    double Reaction_Bottom;
//    
//    if (model_type==1)
//    {
//       Reaction_Bottom=0;      
//                
//       for (i=0;i<121;i++)
//       {
//           cnt=cnt+1;
//           Reaction_Bottom=R2[j][i]+Reaction_Bottom;
//       }
//       
//	   Reaction_Bottom= abs(Reaction_Bottom)/cnt;
//       
//       ofstream myfile;
//       myfile.open("Reaction_Force_Bottom.txt", ios::app);
//   	   if (t==0)
//	    {
//	    	myfile<<t<<" "<<"Reaction_Bottom"<<endl;
//	    	myfile<<t<<" "<<Reaction_Bottom<<endl;
//	    }
//	    else
//	    {
//	    	myfile<<t<<" "<<Reaction_Bottom<<endl;
//	    }
//       
//        myfile.close(); 
//    }
    
    //cout<<"Reaction forces wrote successfully "<<endl;
//*******************************************************************************************************
//                 						Creae new position for the mesh
//******************************************************************************************************
//    if (model_type==1) // no temporary file
//    {
//       for (j=0;j<121;j++)
//       {   
//           for (i=0;i<121;i++)
//           {
//               node=i+(j*121)+1;
//               
//			   X_read[node]= X_read[node]+U1[j][i];
//			   Y_read[node]= Y_read[node]+U2[j][i];   
//           }
//       }
//    }    
}

	
