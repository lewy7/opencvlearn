
#include "head.h"

void smooth()
{
	IplImage* img = cvLoadImage("C:\\84.jpg");
	cvNamedWindow("smooth_in");
	cvNamedWindow("smooth_out");
	cvShowImage("smooth_in",img);

	IplImage *out = cvCreateImage(
		cvGetSize(img),
		IPL_DEPTH_8U,
		3);
	cvSmooth(img,out,CV_GAUSSIAN,3,3);
	cvShowImage("smooth_out",out);
	

	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&out);
	cvDestroyWindow("smooth_in");
	cvDestroyWindow("smooth_out");
	
	
}

void struct_show()
{
	CvPoint cvp; //x,y
	CvSize cvz; // w,h
	CvRect cvr; //x,y  w,h
	CvScalar cvs; // rgba
	

	CvMat cvMat;
	float vals[] = {0.001,0.02,0.232,0.58};

	cvInitMatHeader(&cvMat,2,2,CV_32FC1,vals);

	printf("cvGetElemType %d \n",cvGetElemType(&cvMat));
	printf("cvGetDims %d \n",cvGetDims(&cvMat,NULL));
	printf("cvGetDimSize %d \n",cvGetDimSize(&cvMat,0));

	//CvMat *mat = cvCreateMat(5,5,CV_32FC1);
	//float element = CV_MAT_ELEM(*mat,float,3,2)
	IplImage* img = cvLoadImage("C:\\84.jpg");
	for (int y=0;y<img->height;y++)
	{
		uchar* ptr = (uchar *)(img->imageData + y*img->widthStep);
		for(int x =0 ; x<img->width;x++){
			ptr[3*x+1] = 255;
			ptr[3*x+2] = 255;
		}
	}
	cvNamedWindow("aaa",CV_WINDOW_AUTOSIZE);
	cvShowImage("aaa",img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("aaa");

	img = cvLoadImage("C:\\84.jpg");
	cvSetImageROI(img,cvRect(620,300,200,200));
	cvAddS(img,cvScalar(150),img);
	cvResetImageROI(img);
	cvNamedWindow("aaa",CV_WINDOW_AUTOSIZE);
	cvShowImage("aaa",img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("aaa");

	
	img = cvLoadImage("C:\\84.jpg");
	IplImage* img2 = cvLoadImage("C:\\88.jpg");

	cvSetImageROI(img,cvRect(620,300,200,200));
	cvSetImageROI(img2,cvRect(620,530,200,200));
	cvAddWeighted(img,0.5,img2,0.5,0.0,img);  //alpha + beta = 1   gama=0

	cvResetImageROI(img);
	cvNamedWindow("aaa",CV_WINDOW_AUTOSIZE);
	cvShowImage("aaa",img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("aaa");
}

void demo1()
{
	IplImage* img = cvLoadImage("C:\\84.jpg");
	cvNamedWindow("aaa",CV_WINDOW_AUTOSIZE);
	cvShowImage("aaa",img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("aaa");
}

int main()
{
	//smooth();
	//demo1();
	//struct_show();
	///6  Í¼Ïñ±ä»»
	//AffineTransform(); //Í¸ÊÓ
	//cvHoughCircles(); //»ô·òÔ²±ä»» ±ßÔµ¼ì²â
	//drawBoxs();
	//PerspectiveTransform();//Í¸ÊÓ
	

	//7 Ö±·½Í¼ÓëÆ¥Åä
	//Histogrem();
	//Histogrem_EMD();
	//MatchTemplate();


	//8  ÂÖÀª
	//Contours1();
	//Contours2();


	//10 ÔË¶¯¸ú×Ù  lkdemo ¹âÁ÷  camshiftdemo ÑÕÉ«¸ú×Ù  motempl ÔË¶¯°æ±¾ kaman ¿¨ÂüÂË²¨Æ÷
	//Kalman();
	//OpticalFlow();


	//11 Ïà»ú½ÃÕý
	//calib();
	
	//12  3Î¬Í¼Ïñ
	//_2DLineFitting();
	//BirdsEye();
		return 0;
}