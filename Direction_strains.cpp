#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>       //To read and write data from files as input/output streams
#include <math.h> 
#include <cmath>
#include "header.h"

using namespace std;

const int n=251;

int Direction_strains(int tip_x, int tip_y,double E11[n*n], double E22[n*n], double E12[n*n], float k[n*n])
{
	//Calculus of max and min principal strains 
    
    double princ_strain_1, princ_strain_2;
    double theta_princ,theta_perp,alpha; // contains angles from 0 to 180 of directions of principal strains
    double pi=3.14159265;
    int node;
    int cnt=0;
    int r;
    int dir_1; int dir_2;
    int direction;
    
    node=tip_y+(tip_x*n)+1;
    
	princ_strain_1=0.5*(E11[node]+E22[node])+sqrt(pow(0.5*(E11[node]-E22[node]),2)+pow((E12[node]*0.5),2));
    princ_strain_2=0.5*(E11[node]+E22[node])-sqrt(pow(0.5*(E11[node]-E22[node]),2)+pow((E12[node]*0.5),2));
    if (princ_strain_1<princ_strain_2)
    {
    	theta_princ=atan((princ_strain_1-E11[node])/(E12[node]*0.5));
    	theta_princ=theta_princ*180/pi;
//    	theta_princ_old[k]=((atan((2*E12[k])/(E11[k]-E22[k])))/2)*180/pi;
    	if (theta_princ<0)
    	{
    		theta_princ=90-abs(theta_princ)+90;
		}
		theta_perp=theta_princ-90;
		if (theta_perp<0)
    	{
    		theta_perp=90-abs(theta_perp)+90;
		}
		
	}
	else
	{
		theta_princ=atan((princ_strain_2-E11[node])/(E12[node]*0.5));
		theta_princ=theta_princ*180/pi;
//		theta_princ_old[k]=((atan((2*E12[k])/(E11[k]-E22[k])))/2)*180/pi;
    	if (theta_princ<0)
    	{
    		theta_princ=90-abs(theta_princ)+90;
		}
		theta_perp=theta_princ-90;
		if (theta_perp<0)
    	{
    		theta_perp=90-abs(theta_perp)+90;
		}
					
	}
	
 
	alpha=k[node]*theta_princ+(1-k[node])*theta_perp;	
    
		
	// select the closer direction 
	if (alpha<=22.5 || alpha>157.5)
	{
		dir_1=1;
		dir_2=5;
		r=rand()%2;
		if (r==1)
    	{
        	direction=dir_1;
    	}
    	else
    	{
    		direction=dir_2;
		}
	}
	else if (alpha>22.5 && alpha<=67.5)
	{
		dir_1=8;
		dir_2=4;
		r=rand()%2;
		if (r==1)
    	{
        	direction=dir_1;
    	}
    	else
    	{
    		direction=dir_2;
		}
	}
	else if (alpha>67.5 && alpha<=112.5)
	{
		dir_1=7;
		dir_2=3;
		r=rand()%2;
		if (r==1)
    	{
        	direction=dir_1;
    	}
    	else
    	{
    		direction=dir_2;
		}
	}
    else if (alpha>112.5 && alpha<=157.5)
    {
    	dir_1=6;
    	dir_2=2;
    	r=rand()%2;
    	if (r==1)
    	{
        	direction=dir_1;
    	}
    	else
    	{
    		direction=dir_2;
		}
	}
	return(direction);
}
