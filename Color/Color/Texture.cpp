
//#include "stdafx.h"
#include "Texture.h"
#include <iostream>
#include <math.h>
#include <opencv2/imgproc/types_c.h>  
#include <boost/range/numeric.hpp>
#include <vector>

using namespace std;
using namespace boost;

// Parametrized Constructor for CBIR class
Texture :: Texture(int k_size, vector<double>& sigma, vector<double>& theta, double lambd, double psi)
{
	this->k_size = k_size;
	
	for(int i=0; i<sigma.size(); i++)
		this->sigma.push_back(sigma.at(i));
	for(int i=0; i<theta.size(); i++)
		this->theta.push_back(theta.at(i));
	
	this->lambd = lambd;
	this->psi = psi;

}

// Gabor卷积核
Mat Texture :: gabor_kernel(int ks, double sig, double th, double lm, double ps)
{
	int hks = (ks-1)/2;
    double theta = th*CV_PI/180; //方向（θ）：这个参数指定了Gabor函数并行条纹的方向，它的取值为0到360度
	double psi = ps*CV_PI/180; //相位偏移（φ）：它的取值范围为-180度到180度。其中，0he180度分别对应中心对称的center-on函数和center-off函数，而-90度和90度对应反对称函数。
    double del = 2.0/(ks-1);
	//double del = (ks-1)/2.0;
    double lmbd = lm; //波长（λ）：它的值以像素为单位指定，通常大于等于2.但不能大于输入图像尺寸的五分之一。
    double sigma = sig/ks;//高斯函数的标准差，该参数决定了Gabor滤波核可接受区域的大小
    double x_theta;
    double y_theta;
	//cout <<theta<<"---"<<sigma<<endl;

    Mat kernel(ks,ks, CV_32F);//矩阵
    
	for (int y=-hks; y<=hks; y++)
    {
        for (int x=-hks; x<=hks; x++)
        {
            x_theta = x*del*cos(theta)+y*del*sin(theta);//x'
            y_theta = -x*del*sin(theta)+y*del*cos(theta);//y'
			cout <<x_theta<<"---"<<y_theta<<endl;
            kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+0.25*pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);//实数部分
			//cout<<(float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi)<<"==";
			
        }
		cout<<endl;
    }
	
    return kernel;
}

// Color features calculating function 颜色特征值

// Texture feature calculating function  纹理特征
vector<double> Texture :: texture_feature(Mat& src)
{
	// Converting src image to gray.转化为灰度矩阵
	Mat gray, gray_image,mid;
	cvtColor(src,gray,CV_BGR2GRAY); //RGB转化为灰度矩阵
	gray.convertTo(gray_image,CV_32F, 1.0/255, 0);//Mat的类型转换，最后两个参数为缩放因子和增量，CV_32F是目标矩阵的类型（通道数）

	// Vector for the texture feature definition
	vector<double> tex_feat;
	
	for(int i = 0; i < theta.size(); i++)
	{
		for(int j = 0; j < sigma.size(); j++)
		{
			//cv::Mat cv::getGaborKernel( Size ksize, double sigma, double theta,double lambd, double gamma, double psi, int ktype )
			Mat kernel = getGaborKernel(Size(k_size, k_size), sigma.at(j), theta.at(i), lambd, 0.5, psi, CV_32F);
			//Mat kernel = getGaborKernel(Size(311, 311), 10, 0, 5, 0.5, 0, CV_32F);
			//Mat kernel = gabor_kernel(k_size, sigma.at(j), theta.at(i), lambd, psi);//循环调用
			//cout<<"kernel = "<<endl<<kernel<<endl;
			//cout<<"sigma = "<<sigma.at(j)<<"theta = "<<theta.at(i)<<endl;
			
			// Convolution of gray scale image with gabor kernel 灰度图像与gabor核的卷积（过滤过程）
			filter2D(gray_image,gray,CV_32F,kernel);//参数为原始图像，目标图像，深度，卷积核
			//cout<<"gray = "<<endl<<gray;
			
			
			//测试kernel是否相等
			//int num = countNonZero(kernel-mid);
			//mid=kernel;
			//cout<<num<<endl;
			


			
			//测试gray是否相等
			//int num = countNonZero(gray-mid);
			//mid=gray;
			//cout<<num<<endl;


			//显示每个灰度图
			//cv::namedWindow("Original Image"+j);    //定义显示图像的窗口，参数为窗口名
			//cv::imshow("Original Image",gray);    //显示图像
			
			//保存每个灰度图
			string Img_Name = "C:\\Users\\Han\\Desktop\\test\\" +to_string(i)+to_string(j)+".bmp";
			//imwrite(Img_Name,gray*255);
			imwrite(Img_Name,kernel*255);

			// Calculating mean and standard deviation 计算均值和标准差
			Scalar t_mu, t_sigma;
			meanStdDev(gray,t_mu,t_sigma);//Mat的计算函数
			//cout<<" Iteration value "<<i*theta.size()+j<<endl;
			tex_feat.push_back(t_mu[0]);
			tex_feat.push_back(t_sigma[0]);
		}
	}
	return tex_feat;
}


// Euclidean distance calculation for texture feature vectors  纹理特征值的欧几里得距离
double Texture :: L2dist_texture(vector<double>& db_tex_feat, vector<double>& q_tex_feat)
{
	
	double tex_dist = 0;
	
	for(unsigned i=0; i <q_tex_feat.size(); i++)
	{
		tex_dist += pow((db_tex_feat.at(i) - q_tex_feat.at(i)), 2.0);
	}
	
	tex_dist = sqrt(tex_dist);
	
	return tex_dist;
}


