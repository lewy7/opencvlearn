#include "head.h"

IplImage* g_image = NULL;
IplImage* g_gray = NULL;
int g_thresh = 100;
CvMemStorage* g_storage = NULL;

void on_trackbar(int) {
    if ( g_storage == NULL ) {
        g_gray = cvCreateImage( cvGetSize(g_image), 8, 1);
        g_storage = cvCreateMemStorage( 0 );
    } else {
        cvClearMemStorage( g_storage );
    }
    CvSeq* contours = 0;
    cvCvtColor( g_image, g_gray, CV_BGR2GRAY );
    cvThreshold( g_gray, g_gray, g_thresh, 255, CV_THRESH_BINARY );
    cvFindContours( g_gray, g_storage, &contours );
    cvSetZero( g_gray );
    if ( contours ) {
        cvDrawContours( g_gray, contours, cvScalarAll(255), cvScalarAll(255), 100);
        cvShowImage( "Contours", g_gray );
    }
}

void Contours1()
{
	g_image = cvLoadImage("C:\\84.jpg", 1);
	cvNamedWindow( "Contours" );
	cvCreateTrackbar( "Threshold", "Contours", &g_thresh, 100,on_trackbar );
	on_trackbar(0);
	
	cvWaitKey();
	
	cvReleaseImage( &g_image );
	cvReleaseImage( &g_gray );
	cvReleaseMemStorage( &g_storage );
     cvDestroyWindow( "Contours" );
}

#define CVX_RED CV_RGB( 0xff, 0x00, 0x00 )
#define CVX_BLUE CV_RGB( 0x00, 0x00, 0xff )

void Contours2()
{
	cvNamedWindow( "Contours2" );
    
    IplImage* img_8uc1 = cvLoadImage("C:\\84.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    IplImage* img_edge = cvCreateImage( cvGetSize(img_8uc1), 8, 1 );
    IplImage* img_8uc3 = cvCreateImage( cvGetSize(img_8uc1), 8, 3 );
    cvThreshold( img_8uc1, img_edge, 128, 255, CV_THRESH_BINARY );
    
    CvMemStorage* storage = cvCreateMemStorage();
    CvSeq* first_contour = NULL;
    int Nc = cvFindContours( img_edge, storage, &first_contour, sizeof(CvContour), CV_RETR_LIST );
    int n = 0;
    printf( "Total Contours Detected : %d\n", Nc );
    
    for ( CvSeq* c = first_contour; c != NULL; c = c->h_next ) {
        cvCvtColor( img_8uc1, img_8uc3, CV_GRAY2BGR );
        cvDrawContours( img_8uc3, c, CVX_RED, CVX_BLUE, 0, 2, 8 );
        printf( "Contour #%d\n", n );
        cvShowImage( "Contours2", img_8uc3);
        printf( " %d elements:\n", c->total );
        for( int i = 0; i < c -> total; ++i )
        {
            CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, c, i );
            printf( "    (%d,%d)\n", p -> x, p -> y );
        }
        cvWaitKey(0);
        n++;
    }
    
    printf( "Finished all contours.\n" );
    cvCvtColor( img_8uc1, img_8uc3, CV_GRAY2BGR );
    cvShowImage( "Contours2", img_8uc3 );
    cvWaitKey(0);
    cvDestroyWindow( "Contours2" );
    cvReleaseImage( &img_8uc1 );
    cvReleaseImage( &img_8uc3 );
    cvReleaseImage( &img_edge );
}