
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

// Gabor�����
Mat Texture :: gabor_kernel(int ks, double sig, double th, double lm, double ps)
{
	int hks = (ks-1)/2;
    double theta = th*CV_PI/180; //���򣨦ȣ����������ָ����Gabor�����������Ƶķ�������ȡֵΪ0��360��
	double psi = ps*CV_PI/180; //��λƫ�ƣ��գ�������ȡֵ��ΧΪ-180�ȵ�180�ȡ����У�0he180�ȷֱ��Ӧ���ĶԳƵ�center-on������center-off��������-90�Ⱥ�90�ȶ�Ӧ���Գƺ�����
    double del = 2.0/(ks-1);
	//double del = (ks-1)/2.0;
    double lmbd = lm; //�������ˣ�������ֵ������Ϊ��λָ����ͨ�����ڵ���2.�����ܴ�������ͼ��ߴ�����֮һ��
    double sigma = sig/ks;//��˹�����ı�׼��ò���������Gabor�˲��˿ɽ�������Ĵ�С
    double x_theta;
    double y_theta;
	//cout <<theta<<"---"<<sigma<<endl;

    Mat kernel(ks,ks, CV_32F);//����
    
	for (int y=-hks; y<=hks; y++)
    {
        for (int x=-hks; x<=hks; x++)
        {
            x_theta = x*del*cos(theta)+y*del*sin(theta);//x'
            y_theta = -x*del*sin(theta)+y*del*cos(theta);//y'
			cout <<x_theta<<"---"<<y_theta<<endl;
            kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+0.25*pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);//ʵ������
			//cout<<(float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi)<<"==";
			
        }
		cout<<endl;
    }
	
    return kernel;
}

// Color features calculating function ��ɫ����ֵ

// Texture feature calculating function  ��������
vector<double> Texture :: texture_feature(Mat& src)
{
	// Converting src image to gray.ת��Ϊ�ҶȾ���
	Mat gray, gray_image,mid;
	cvtColor(src,gray,CV_BGR2GRAY); //RGBת��Ϊ�ҶȾ���
	gray.convertTo(gray_image,CV_32F, 1.0/255, 0);//Mat������ת���������������Ϊ�������Ӻ�������CV_32F��Ŀ���������ͣ�ͨ������

	// Vector for the texture feature definition
	vector<double> tex_feat;
	
	for(int i = 0; i < theta.size(); i++)
	{
		for(int j = 0; j < sigma.size(); j++)
		{
			//cv::Mat cv::getGaborKernel( Size ksize, double sigma, double theta,double lambd, double gamma, double psi, int ktype )
			Mat kernel = getGaborKernel(Size(k_size, k_size), sigma.at(j), theta.at(i), lambd, 0.5, psi, CV_32F);
			//Mat kernel = getGaborKernel(Size(311, 311), 10, 0, 5, 0.5, 0, CV_32F);
			//Mat kernel = gabor_kernel(k_size, sigma.at(j), theta.at(i), lambd, psi);//ѭ������
			//cout<<"kernel = "<<endl<<kernel<<endl;
			//cout<<"sigma = "<<sigma.at(j)<<"theta = "<<theta.at(i)<<endl;
			
			// Convolution of gray scale image with gabor kernel �Ҷ�ͼ����gabor�˵ľ�������˹��̣�
			filter2D(gray_image,gray,CV_32F,kernel);//����Ϊԭʼͼ��Ŀ��ͼ����ȣ������
			//cout<<"gray = "<<endl<<gray;
			
			
			//����kernel�Ƿ����
			//int num = countNonZero(kernel-mid);
			//mid=kernel;
			//cout<<num<<endl;
			


			
			//����gray�Ƿ����
			//int num = countNonZero(gray-mid);
			//mid=gray;
			//cout<<num<<endl;


			//��ʾÿ���Ҷ�ͼ
			//cv::namedWindow("Original Image"+j);    //������ʾͼ��Ĵ��ڣ�����Ϊ������
			//cv::imshow("Original Image",gray);    //��ʾͼ��
			
			//����ÿ���Ҷ�ͼ
			string Img_Name = "C:\\Users\\Han\\Desktop\\test\\" +to_string(i)+to_string(j)+".bmp";
			//imwrite(Img_Name,gray*255);
			imwrite(Img_Name,kernel*255);

			// Calculating mean and standard deviation �����ֵ�ͱ�׼��
			Scalar t_mu, t_sigma;
			meanStdDev(gray,t_mu,t_sigma);//Mat�ļ��㺯��
			//cout<<" Iteration value "<<i*theta.size()+j<<endl;
			tex_feat.push_back(t_mu[0]);
			tex_feat.push_back(t_sigma[0]);
		}
	}
	return tex_feat;
}


// Euclidean distance calculation for texture feature vectors  ��������ֵ��ŷ����þ���
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


