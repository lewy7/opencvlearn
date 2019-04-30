#include "head.h"
using namespace std;
void _2DLineFitting()
{
	IplImage* img = cvCreateImage( cvSize( 500, 500 ), 8, 3 );
    CvRNG rng = cvRNG(-1);
    
    cvNamedWindow( "fitline", 1 );
    for(;;) {
        char key;
        int i, count = cvRandInt(&rng) % 100 + 1, outliers = count/5;
        float a = cvRandReal(&rng) * 200;
        float b = cvRandReal(&rng) * 40;
        float angle = cvRandReal(&rng) * CV_PI;
        float cos_a = cos(angle), sin_a = sin(angle);
        CvPoint pt1, pt2;
        CvPoint* points = (CvPoint*)malloc( count * sizeof(points[0]));
        CvMat pointMat = cvMat( 1, count, CV_32SC2, points );
        float line[4];
        float d, t;
        
        b = MIN(a*0.3, b);
        
        // generate some points that are close to the line
        for( i = 0; i < count - outliers; i++ ) {
            float x = (cvRandReal(&rng)*2-1)*a;
            float y = (cvRandReal(&rng)*2-1)*b;
            points[i].x = cvRound(x*cos_a - y*sin_a + img->width/2);
            points[i].y = cvRound(x*sin_a + y*cos_a + img->height/2);
        }
        
        // generate "completely off" points
        for( ; i < count; i++ ) {
            points[i].x = cvRandInt(&rng) % img->width;
            points[i].y = cvRandInt(&rng) % img->height;
        }
        
        // find the optimal line
        cvFitLine( &pointMat, CV_DIST_L1, 1, 0.001, 0.001, line );
        cvZero( img );
        
        // draw the points
        for( i = 0; i < count; i++ ) {
            cvCircle( img, points[i], 2, i < count - outliers ? CV_RGB(255, 0, 0) : CV_RGB(255,255,0), CV_FILLED, CV_AA, 0 );
        }
        
        d = sqrt((double)line[0]*line[0] + (double)line[1]*line[1]);
        line[0] /= d;
        line[1] /= d;
        t = (float)(img->width + img->height);
        pt1.x = cvRound(line[2] - line[0]*t);
        pt1.y = cvRound(line[3] - line[1]*t);
        pt2.x = cvRound(line[2] + line[0]*t);
        pt2.y = cvRound(line[3] + line[1]*t);
        cvLine( img, pt1, pt2, CV_RGB(0,255,0), 3, CV_AA, 0 );
        
        cvShowImage( "fitline", img );
        
        key = (char) cvWaitKey(0);
        if( key == 27 )  break;
        free( points );
    }
    
    cvDestroyWindow( "fitline" );
}

void BirdsEye()
{
	
    const char* default_args[] = {"", "12", "12", "C:\\Intrinsics.xml", "C:\\Distortion.xml", "C:\\birdseye.jpg" };


    
    int board_w = 12;
    int board_h = 12;
    int board_n  = board_w * board_h;
    CvSize board_sz = cvSize( board_w, board_h );
    CvMat* intrinsic = (CvMat*)cvLoad("C:\\Intrinsics.xml");
    CvMat* distortion = (CvMat*)cvLoad("C:\\Distortion.xml");
    IplImage *image = 0, *gray_image = 0;
    if((image = cvLoadImage("C:\\birdseye.jpg"))== 0){
        printf("Error: Couldn't load %s\n","C:\\birdseye.jpg");
        return ;
    }
    gray_image = cvCreateImage(cvGetSize(image),8,1);
    cvCvtColor(image, gray_image, CV_BGR2GRAY);
    
    //UNDISTORT OUR IMAGE
    IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
    IplImage* mapy = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
    cvInitUndistortMap( intrinsic, distortion, mapx, mapy );
    IplImage* t = cvCloneImage( image );
    cvRemap( t, image, mapx, mapy );
    
    cvNamedWindow("Checkers");
    CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
    int corner_count = 0;
    int found = cvFindChessboardCorners( image, board_sz, corners, &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );
    if(!found){
        printf("Couldn't aquire checkerboard on %s, only found %d of %d corners\n", "C:\\birdseye.jpg",corner_count,board_n);
        return ;
    }
    cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11),cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1) );
    
    //GET THE IMAGE AND OBJECT POINTS:
    CvPoint2D32f objPts[4], imgPts[4];
    objPts[0].x = 0;
    objPts[0].y = 0;
    objPts[1].x = board_w-1;
    objPts[1].y = 0;
    objPts[2].x = 0;
    objPts[2].y = board_h-1;
    objPts[3].x = board_w-1;
    objPts[3].y = board_h-1;
    imgPts[0] = corners[0];
    imgPts[1] = corners[board_w-1];
    imgPts[2] = corners[(board_h-1)*board_w];
    imgPts[3] = corners[(board_h-1)*board_w + board_w-1];
    
    //DRAW THE POINTS in order: B,G,R,YELLOW
    cvCircle(image,cvPointFrom32f(imgPts[0]),9,CV_RGB(0,0,255),3);
    cvCircle(image,cvPointFrom32f(imgPts[1]),9,CV_RGB(0,255,0),3);
    cvCircle(image,cvPointFrom32f(imgPts[2]),9,CV_RGB(255,0,0),3);
    cvCircle(image,cvPointFrom32f(imgPts[3]),9,CV_RGB(255,255,0),3);
    
    //DRAW THE FOUND CHECKERBOARD
    cvDrawChessboardCorners(image, board_sz, corners, corner_count, found);
    cvShowImage( "Checkers", image );
    
    //FIND THE HOMOGRAPHY
    CvMat *H = cvCreateMat( 3, 3, CV_32F);
    cvGetPerspectiveTransform(objPts,imgPts,H);
	
    //LET THE USER ADJUST THE Z HEIGHT OF THE VIEW
    float Z = 25;
    int key = 0;
    IplImage *birds_image = cvCloneImage(image);
    cvNamedWindow("Birds_Eye");
    while(key != 27) {//escape key stops
        CV_MAT_ELEM(*H,float,2,2) = Z;
        //USE HOMOGRAPHY TO REMAP THE VIEW
        cvWarpPerspective( image,birds_image, H, CV_INTER_LINEAR+CV_WARP_INVERSE_MAP+CV_WARP_FILL_OUTLIERS );
        cvShowImage("Birds_Eye", birds_image);
        key = cvWaitKey();
        if(key == 'u') Z += 0.5;
        if(key == 'd') Z -= 0.5;
    }
    
    cvSave("H.xml",H);
}
