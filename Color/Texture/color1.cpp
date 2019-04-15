// Texture.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <tchar.h>
#include <iostream>
#include "Color1.h"
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
	Mat q_image = imread("C:\\Users\\Yiwu\\Desktop\\test\\0000976000_001_A.bmp",1);
	if(!q_image.data ) // Check for invalid input
    {
	  cout <<"Could not open or find the image" <<endl ;
	  return -1;
	}
	Mat hsv_image;
	cvtColor(q_image,hsv_image,CV_BGR2HSV); //转换到HSV颜色空间
	
	// Image for the quantization
	Mat quant_image = Mat::zeros(hsv_image.rows,hsv_image.cols,CV_8UC1);//初始化为零矩阵
	
	// vector for color features with 72x1 bins
	vector<int> col_feat(32,0);
	
	// Color Quantization based on "IMAGE RETRIEVAL USING BOTH COLOR AND TEXTURE FEATURES" FAN-HUI KONG 
	//同时使用颜色和纹理特征进行检索
	for(int i=0; i < hsv_image.rows; ++i)
	{
		for(int j=0; j < hsv_image.cols; ++j)
		{
			
			int h = (int) hsv_image.at<Vec3b>(i,j)[0];//取出彩色图像中i行j列第1通道的颜色点
			int s = (int) hsv_image.at<Vec3b>(i,j)[1];//取出彩色图像中i行j列第2通道的颜色点
			int v = (int) hsv_image.at<Vec3b>(i,j)[2];//取出彩色图像中i行j列第3通道的颜色点
			
			int n;
			if(v<=0.1*255)
			{
				n = 0;
			}
			else if((s<=0.1*255)&&(v>0.1*255)&&(v<=0.4*255))
			{
				n = 1;
			}
			else if((s<=0.1*255)&&(v>0.4*255)&&(v<=0.7*255))
			{
				n = 2;
			}
			else if((s<=0.1*255)&&(v>0.7*255)&&(v<=1*255))
			{
				n = 3;
			}
			else if((h>=0.0/360.0*180 && h<=20.0/360.0*180) || (h>330.0/360.0*180 && h<360.0/360.0*180))
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					n = 4;
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					n = 5;
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					n = 6;
				}
				else
				{
					n = 7;
				}
			}
			else if(h>20.0/360.0*180&&h<=45.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					n = 8;
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					n = 9;
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					n = 10;
				}
				else
				{
					n = 11;
				}
			}
			else if(h>45.0/360.0*180&&h<=75.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					n = 12;
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					n = 13;
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					n = 14;
				}
				else
				{
					n = 15;
				}
			}
			else if(h>75.0/360.0*180&&h<=155.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					n = 16;
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					n = 17;
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					n = 18;
				}
				else
				{
					n = 19;
				}
			}
			else if(h>155.0/360.0*180&&h<=210.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					n = 20;
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					n = 21;
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					n = 22;
				}
				else
				{
					n = 23;
				}
			}
			else if(h>210.0/360.0*180&&h<=270.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					n = 24;
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					n = 25;
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					n = 26;
				}
				else
				{
					n =27;
				}
			}
			else
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					n = 28;
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					n = 29;
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					n = 30;
				}
				else
				{
					n = 31;
				}
			}
			col_feat.at(n) = col_feat.at(n) + 1; 
			quant_image.at<uchar>(i,j) = (uchar)n;
		} 
		
	}
	for(int i=0; i<	col_feat.size(); i++)
		cout<<col_feat[i]<<" " ;
		cout<<endl;
	system( "PAUSE "); 
	return 0;
}

