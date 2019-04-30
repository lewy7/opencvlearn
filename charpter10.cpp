#include "head.h"



#define CVX_DONT_CARE -1

#define CVX_CAMERA  0
#define CVX_AVI     1

#define CVX_RED     CV_RGB(0xff,0x00,0x00)
#define CVX_GREEN   CV_RGB(0x00,0xff,0x00)
#define CVX_BLUE    CV_RGB(0x00,0x00,0xff)

#define CVX_CYAN    CV_RGB(0x00,0xff,0xff)
#define CVX_MAGENTA CV_RGB(0xff,0x00,0xff)
#define CVX_YELLOW  CV_RGB(0xff,0xff,0x00)

#define CVX_WHITE   CV_RGB(0xff,0xff,0xff)
#define CVX_BLACK   CV_RGB(0x00,0x00,0x00)
#define CVX_GRAY50  CV_RGB(0x88,0x88,0x88)

typedef struct {
    float x;
    float y;
    float width;
    float height;
}
CvxRect32f;

CV_INLINE  CvxRect32f  cvxRect32f(float x, float y, float width, float height) {
    CvxRect32f r;
    
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;
    
    return r;
}
CvScalar cvx_hsv2rgb( CvScalar hsv ) {
    
    // H is given on [0, 180]. S and V are given on [0, 255].
    // RGB are each returned on [0, 255].
    //
    float h = hsv.val[0]/30.0f;
    float s = hsv.val[1]/255.0f;
    float v = hsv.val[2]/255.0f;
    while( h>6.0f ) h-=6.0f;
    while( h<0.0f ) h+=6.0f;
    float m, n, f;
    int i;
    
    CvScalar rgb;
    
    i = floor(h);
    f = h - i;
    if ( !(i&1) ) f = 1 - f; // if i is even
    m = v * (1 - s);
    n = v * (1 - s * f);
    switch (i) {
	case 6:
	case 0: rgb = CV_RGB(v, n, m); break;
	case 1: rgb = CV_RGB(n, v, m); break;
	case 2: rgb = CV_RGB(m, v, n); break;
	case 3: rgb = CV_RGB(m, n, v); break;
	case 4: rgb = CV_RGB(n, m, v); break;
	case 5: rgb = CV_RGB(v, m, n); break;
    }
    
    rgb.val[0] *= 255.0f;
    rgb.val[1] *= 255.0f;
    rgb.val[2] *= 255.0f;
    
    return rgb;
}

#define phi2xy(mat)                                                  \
	cvPoint( cvRound(img->width/2 + img->width/3*cos(mat->data.fl[0])),\
cvRound( img->height/2 - img->width/3*sin(mat->data.fl[0])) )

void Kalman()
{
	cvNamedWindow( "Kalman", 1 );
    CvRandState rng;
    cvRandInit( &rng, 0, 1, -1, CV_RAND_UNI );
    
    IplImage* img = cvCreateImage( cvSize(500,500), 8, 3 );
    CvKalman* kalman = cvCreateKalman( 2, 1, 0 );
	
    CvMat* x_k = cvCreateMat( 2, 1, CV_32FC1 );
    cvRandSetRange( &rng, 0, 0.1, 0 );
    rng.disttype = CV_RAND_NORMAL;
    cvRand( &rng, x_k );
    
    // process noise
    CvMat* w_k = cvCreateMat( 2, 1, CV_32FC1 );
    
    // measurements, only one parameter for angle
    CvMat* z_k = cvCreateMat( 1, 1, CV_32FC1 );
    cvZero( z_k );
    
    const float F[] = { 1, 1, 0, 1 };
    memcpy( kalman->transition_matrix->data.fl, F, sizeof(F));
    // Initialize other Kalman filter parameters.
    cvSetIdentity( kalman->measurement_matrix,    cvRealScalar(1) );
    cvSetIdentity( kalman->process_noise_cov,     cvRealScalar(1e-5) );
    cvSetIdentity( kalman->measurement_noise_cov, cvRealScalar(1e-1) );
    cvSetIdentity( kalman->error_cov_post,        cvRealScalar(1));
    
    // choose random initial state
    cvRand( &rng, kalman->state_post );
    
    while( 1 ) {
        // predict point position
        const CvMat* y_k = cvKalmanPredict( kalman, 0 );
        
        // generate measurement (z_k)
        cvRandSetRange( &rng, 0, sqrt(kalman->measurement_noise_cov->data.fl[0]), 0 );
        cvRand( &rng, z_k );
        cvMatMulAdd( kalman->measurement_matrix, x_k, z_k, z_k );
        // plot points (eg convert to planar co-ordinates and draw)
        cvZero( img );
        cvCircle( img, phi2xy(z_k), 4, CVX_YELLOW );   // observed state
        cvCircle( img, phi2xy(y_k), 4, CVX_WHITE, 2 ); // "predicted" state
        cvCircle( img, phi2xy(x_k), 4, CVX_RED );      // real state
        cvShowImage( "Kalman", img );
        // adjust Kalman filter state
        cvKalmanCorrect( kalman, z_k );
        
        cvRandSetRange( &rng, 0, sqrt(kalman->process_noise_cov->data.fl[0]), 0 );
        cvRand( &rng, w_k );
        cvMatMulAdd( kalman->transition_matrix, x_k, w_k, x_k );
        
        // exit if user hits 'Esc'
        if( cvWaitKey( 100 ) == 27 ) break;
    }

}


const int MAX_CORNERS = 500;

void OpticalFlow()
{
	IplImage* imgA = cvLoadImage( "C:\\91.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    IplImage* imgB = cvLoadImage( "C:\\92.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    
    CvSize img_sz = cvGetSize( imgA );
    int win_size = 10;
    
    IplImage* imgC = cvLoadImage( "C:\\92.jpg", CV_LOAD_IMAGE_UNCHANGED );
    
    IplImage* image_eig = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
    IplImage* image_tmp = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
    
    int corner_count = MAX_CORNERS;
    CvPoint2D32f* cornersA = new CvPoint2D32f[ MAX_CORNERS ];
    CvPoint2D32f* cornersB = new CvPoint2D32f[ MAX_CORNERS ];
    
    cvGoodFeaturesToTrack( imgA, image_eig, image_tmp, cornersA, &corner_count, 0.01, 5.0, 0, 3, 0, 0.04 );
    
    cvFindCornerSubPix(
		imgA,
		cornersA,
		corner_count,
		cvSize(win_size, win_size),
		cvSize(-1, -1),
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20.0, 0.03)
		);
    
    char features_found[ MAX_CORNERS ];
    float feature_errors[ MAX_CORNERS ];
    
    CvSize pyr_sz = cvSize( imgA->width+8, imgB->height/3 );
    
    IplImage* pyrA = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1 );
    IplImage* pyrB = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1 );
    
    cvCalcOpticalFlowPyrLK(imgA,
		imgB,
		pyrA,
		pyrB,
		cornersA,
		cornersB,
		corner_count,
		cvSize(win_size, win_size),
		5,
		features_found,
		feature_errors,
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20.0, 0.3),
		0
		);
    
    for( int i=0; i<corner_count; i++ ) {
        if( features_found[i]==0|| feature_errors[i]>550 ) {
            printf("Error is %f\n",feature_errors[i]);
            continue;
        }
        CvPoint p0 = cvPoint(
			cvRound( cornersA[i].x ),
			cvRound( cornersA[i].y )
			);
        CvPoint p1 = cvPoint(
			cvRound( cornersB[i].x ),
			cvRound( cornersB[i].y )
			);
        
        cvLine( imgC, p0, p1, CV_RGB(255,0,0),2 );
    }
    cvNamedWindow("ImageA",0);
    cvNamedWindow("ImageB",0);
    cvNamedWindow("LKpyr_OpticalFlow",0);
    cvShowImage("ImageA",imgA);
    cvShowImage("ImageB",imgB);
    cvShowImage("LKpyr_OpticalFlow",imgC);
    cvWaitKey(0);
}