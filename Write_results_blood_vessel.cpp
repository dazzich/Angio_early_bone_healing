/************************************************************

 Algorithm to wite file with cell forming the blood vessels
 
************************************************************/
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "header.h"

using namespace std;

const int n=251;

void Write_results_blood_vessel(int cells_vessels[n][n],int time)
{
	
  	ostringstream f;
    if (time<10)
    {
		f<<"Blood_v_000"<<time<<".txt"; //name
 	}
 	else if (time<100)
    {
		f<<"Blood_v_00"<<time<<".txt";
 	} 
 	else if (time<1000) 
    {
		f<<"Blood_v_0"<<time<<".txt";
 	}
	else
    {
		f<<"Blood_v_"<<time<<".txt";
 	} 
    
    string name=f.str();
    ofstream myfile;

    myfile.open(name.c_str());   

    
    int j,i,node;
    
    for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
        {		
        	if (cells_vessels[j][i]==2) //ECs
        	{
        		node=i+(j*n)+1;
            	myfile<<node<<" "<<j<<" "<<i<<" "<<endl;	//PER ORA
			}
        }
    }
    
  
    myfile.close();  

     
    cout<<"Results blood vessels wrote successfully"<<endl; 
}
