/************************************************************

 Algorithm to write information on vessels
 
************************************************************/
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "header.h"

using namespace std;

const int n=251;

void Write_data_vessels(int time, int vessels_x[(n-1)*20][n-1],int vessels_y[(n-1)*20][n-1], int& num_vess)
{
	
  	ostringstream f;
    if (time<10)
    {
		f<<"Info_x_000"<<time<<".txt"; //name
 	}
 	else if (time<100)
    {
		f<<"Info_x_00"<<time<<".txt";
 	} 
 	else if (time<1000) 
    {
		f<<"Info_x_0"<<time<<".txt";
 	}
	else
    {
		f<<"Info_x_"<<time<<".txt";
 	} 
    
    string name=f.str();
    ofstream myfile;

    myfile.open(name.c_str());   

    int i,j;
    int vessel,l;
    int length_vessel;
    for (vessel=0;vessel<num_vess;vessel++) 
    {
    	for (l=0;l<n-1;l++)
    	{
    	    myfile<<vessels_x[vessel][l]<<" "; //number of vessel, tip cell coordinates, length of vessel	
		}
		myfile<<";"<<endl;
	}
	
    
  
    myfile.close();  
    
    ostringstream s;
    if (time<10)
    {
		s<<"Info_y_000"<<time<<".txt"; //name
 	}
 	else if (time<100)
    {
		s<<"Info_y_00"<<time<<".txt";
 	} 
 	else if (time<1000) 
    {
		s<<"Info_y_0"<<time<<".txt";
 	}
	else
    {
		s<<"Info_y_"<<time<<".txt";
 	} 
    
    string name2=s.str();
    ofstream myfile2;

    myfile2.open(name2.c_str());   
    
   
    for (vessel=0;vessel<num_vess;vessel++)
    {
    	for (l=0;l<n-1;l++)
    	{
    	    myfile2<<vessels_y[vessel][l]<<" "; //number of vessel, tip cell coordinates, length of vessel	
		}
		myfile2<<";"<<endl;
	}
    
  
    myfile2.close();  

     
    cout<<"Info on blood vessels wrote successfully"<<endl; 
}
