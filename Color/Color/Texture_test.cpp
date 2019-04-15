//#include "stdafx.h"
#include <tchar.h>
#include <iostream>
#include "Texture.h"
#include <vector>
#include <boost/assign/std/vector.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/types_c.h>
#include <math.h>

using namespace std;
using std::vector;
using namespace boost::assign;
int _tmain(int argc, _TCHAR* argv[])
{
	//int k_size = 21;
	int k_size = 210;
	vector<double> sigma ;
	//sigma += 0.05, 0.55, 1;
	//sigma += 0.05, 0.10, 0.15, 0.20;
	//sigma+=0.1,1,5;
	sigma +=  0.05;
	vector<double> theta;
	theta += 0, CV_PI/6, CV_PI/3, CV_PI/2, 2*CV_PI/3, 5*CV_PI/6; 
	//theta +=CV_PI/2;
	//double lambd = 50; 
	double lambd = 50; 
	//double psi = CV_PI/2;
	double psi = 0;

	// Calling constructor
	Texture cbir(k_size, sigma, theta, lambd, psi);
	
	
	Mat q_image = imread("C:\\Users\\Yiwu\\Desktop\\test\\0000976000_001_A.bmp",1);
	if(!q_image.data ) // Check for invalid input
    {
	  cout <<"Could not open or find the image" <<endl ;
	  return -1;
	}
	

	vector<double> tex_feat = cbir.texture_feature(q_image);
	
	for(int i = 0;i<tex_feat.size();i++)
		cout<<tex_feat[i]<<" ";
	system( "PAUSE "); 
	return 0;
}
