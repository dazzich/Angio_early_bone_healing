#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <limits> // Press inter to continue
#include "header.h"

//********************************************************************
//                   Write results for matlab
//           ( Node    j   i    cell  dipole direction)
//           (19755   49   105  1112        1         )
//***********************************************************************
using namespace std;

void Write_results(int cell_matrix[N][N], int dipole[N][N],int t_write)
{ 
    int node;    
    ostringstream f1;
    if (t_write<10)
    {
	f1<<"Results_000"<<t_write<<".txt"; //name
 	}
 	else if (t_write<100)
    {
	 f1<<"Results_00"<<t_write<<".txt";
 	} 
 	else if (t_write<1000) 
    {
	 f1<<"Results_0"<<t_write<<".txt";
 	}
	else
    {
	 f1<<"Results_"<<t_write<<".txt";
 	} 
    
    string name1=f1.str();
    ofstream myfile;

    myfile.open(name1.c_str());   
    
    int j,i;
    
    for (j=0;j<N;j++)
    {
        for (i=0;i<N;i++)
        {		
			node=i+(j*N)+1;
            myfile<<node<<" "<<j<<" "<<i<<" "<<cell_matrix[j][i]<<" "<<dipole[j][i]<<endl;
        }
    }    
    cout<<"Results wrote successfully"<<endl; 
}
