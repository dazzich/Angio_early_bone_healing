#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>

using namespace std;
const int n=251;

float count_ec(int j, int i, int cells[n][n],int iter)
{
	

	int i_min,j_min;
    int i_max,j_max;
    int i1,j1;
    float area;
    int all_av=0; // total number of available nodes
	int ec=0;
	float density;
	
	i_min=0;
	j_min=0;
	i_max=n-1;
	j_max=n-1;
	
	
	// define the region of interest
    if (i-10>i_min)
	{
	  	i_min=i-10;
	}
	else
	{
		i_min=0;
	}
	if (j-10>j_min)
	{
		j_min=j-10;
	}
	else
	{
		j_min=0;
	}
	if (i+10<i_max)
	{
	  	i_max=i+10;
	}
	else
	{
		i_max=n-1;
	}

	if (j+10<j_max)
	{
		j_max=j+10;
	}
	else 
	{
		j_max=n-1;
	}
	
	for (j1=j_min;j1<j_max;j1++)
	{
		for (i1=i_min;i1<i_max;i1++)
		{
			if (cells[j1][i1]==2)
			{
				ec=ec+1;
			}
			if (cells[j1][i1]!=1)
			{
				all_av=all_av+1;
			}
		} // end for j
	}//end for i
	
	
	//calculate area around tip
	
	density=(float(ec)*100)/float(all_av);
    return (density);
}
