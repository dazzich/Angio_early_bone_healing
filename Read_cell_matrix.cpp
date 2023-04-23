#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iomanip>
#include "header.h"

using namespace std;

const int n=251;

void Read_cell_matrix(int cell_matrix[n][n], int dipole_matrix[n][n], int iteration)
{
    ostringstream s;
     if (iteration-1<10)
     {
	 s<<"Results_000"<<iteration-1<<".txt"; 
 	 }
 	 else if (iteration-1<100)
     {
	 s<<"Results_00"<<iteration-1<<".txt";
 	 } 
 	 else if (iteration-1<1000)
     {
	 s<<"Results_0"<<iteration-1<<".txt";
 	 } 
 	 else 
     {
	 s<<"Results_"<<iteration-1<<".txt";
 	 } 
      
    string filename=s.str();
    
    ifstream infile;
    infile.open(filename.c_str());
    
    int j,i,t;
    int node,dipole,cell;
    
     for (t=1;t<n*n;t++)
     {
	    infile>>node;
		infile>>j;
		infile>>i;
		infile>>cell;
		cell_matrix[j][i]=cell;
		infile>>dipole;
		dipole_matrix[j][i]=dipole;      
     }
 }
