#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <math.h>


using namespace std;
void Write_batch_file(int iter, int model_type)
{
     
    ofstream myfile;
    myfile.open("run.bat"); 
     
    if (model_type==1)
    {  
		myfile<<". $MODULESHOME/init/bash"<<endl;
	    myfile<<"module load abaqus"<<endl;
		myfile<<"abaqus job=FEM_"<<iter<<" ask_delete=OFF cpus=8 interactive";
    }
    else
    {
		myfile<<". $MODULESHOME/init/bash"<<endl;
	    myfile<<"module load abaqus"<<endl;
		myfile<<"abaqus job=Temporal_FEM_"<<iter<<" ask_delete=OFF cpus=8 interactive";
    }
     
	myfile.close();
	 
	system("chmod +x run.bat");
} 
