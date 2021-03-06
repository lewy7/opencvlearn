

#include "head.h"

void AffineTransform()
{
	CvPoint2D32f srcTri[3], dstTri[3];
    CvMat* rot_mat = cvCreateMat(2, 3, CV_32FC1);
    CvMat* warp_mat = cvCreateMat(2, 3, CV_32FC1);
    IplImage *src, *dst;
    
		src = cvLoadImage("C:\\84.jpg", 1);
        dst = cvCloneImage( src );
        dst->origin = src->origin;
        cvZero( dst );
        
        // warp matrix
        srcTri[0].x = 0;
        srcTri[0].y = 0;
        srcTri[1].x = src->width -1;
        srcTri[1].y = 0;
        srcTri[2].x = 0;
        srcTri[2].y = src->height -1;
        dstTri[0].x = src->width * 0.0;
        dstTri[0].y = src->height * 0.33;
        dstTri[1].x = src->width * 0.85;
        dstTri[1].y = src->height * 0.25;
        dstTri[2].x = src->width * 0.15;
        dstTri[2].y = src->height * 0.7;
        
        cvGetAffineTransform( srcTri, dstTri, warp_mat );
        cvWarpAffine( src, dst, warp_mat );
        cvCopy( dst, src );
        
        // rotation matrix
        CvPoint2D32f center = cvPoint2D32f( src->width/2, src->height/2 );
        double angle = -50.0;
        double scale = 0.6;
        
        cv2DRotationMatrix( center, angle, scale, rot_mat );
        cvWarpAffine( src, dst, rot_mat );
        
        cvNamedWindow( "Affine_Transform", 1 );
        cvShowImage( "Affine_Transform" , dst );
        
        cvWaitKey();
		
        cvReleaseImage( &dst );
		
    
    cvReleaseMat( &rot_mat );
    cvReleaseMat( &warp_mat );
}

void cvHoughCircles()
{
	IplImage* image = cvLoadImage("C:\\88.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    
    CvMemStorage* storage = cvCreateMemStorage(0);
    cvSmooth(image, image, CV_GAUSSIAN, 5, 5);
    
    CvSeq* results = cvHoughCircles( image, storage, CV_HOUGH_GRADIENT, 2, image->width/10 );
    
    for ( int i = 0; i < results->total ; i++ ) {
        float* p = (float *) cvGetSeqElem( results, i);
        CvPoint pt = cvPoint( cvRound(p[0]), cvRound(p[1]) );
        cvCircle(image, pt, cvRound(p[2]), CV_RGB(0xff, 0xff, 0xff));
    }
    cvNamedWindow( "cvHoughCircles", 1 );
    cvShowImage( "cvHoughCircles", image );
    cvWaitKey(0);
    
}

/////////////////////////////////////////////////
CvRect box;
bool drawing_box = false;

void draw_box( IplImage* img, CvRect rect ) {
    cvRectangle(
        img ,
        cvPoint(rect.x, rect.y),
        cvPoint(rect.x+rect.width, rect.y+rect.height),
        cvScalar(0x00, 0x00, 0xff) // Red
		);
}

void my_mous_callback( int evetnt, int x, int y, int flag, void* param ) {
    
    IplImage* image = (IplImage *) param;
    
    switch (evetnt) {
	case CV_EVENT_MOUSEMOVE:
		if ( drawing_box ) {
			box.width = x-box.x;
			box.height = y-box.y;
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		drawing_box = true;
		box = cvRect(x, y, 0, 0);
		break;
	case CV_EVENT_LBUTTONUP:
		drawing_box = false;
		if (box.width<0) {
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height<0) {
			box.y += box.height;
			box.height *= -1;
		}
		draw_box(image, box);
		break;
	default:
		break;
    }
    
}

void drawBoxs()
{
	box = cvRect(-1, -1, 0, 0);
    IplImage* image = cvCreateImage( cvSize(200, 200), IPL_DEPTH_8U, 3 );
    cvZero( image );
    IplImage* temp = cvCloneImage( image );
    
    cvNamedWindow( "Box Example" );
    
    cvSetMouseCallback( "Box Example", my_mous_callback, (void *) image );
    
    while (1) {
        cvCopyImage( image, temp );
        
        if (drawing_box) draw_box( temp, box );
        cvShowImage( "Box Example", temp );
        
        if ( cvWaitKey(15) == 27) break;
        
    }
    
    cvReleaseImage( &image );
    cvReleaseImage( &temp );
    cvDestroyWindow( "Box Example" );
}





////////////////////////////

void PerspectiveTransform()
{
	CvPoint2D32f srcQuad[4], dstQuad[4];
    CvMat* warp_matrix = cvCreateMat(3, 3, CV_32FC1);
    IplImage *src, *dst;
    
    src = cvLoadImage("C:\\84.jpg", 1);
        dst = cvCloneImage( src );
        dst->origin = src->origin;
        cvZero( dst );
        
        // warp matrix
        srcQuad[0].x = 0;
        srcQuad[0].y = 0;
        srcQuad[1].x = src->width - 1;
        srcQuad[1].y = 0;
        srcQuad[2].x = 0;
        srcQuad[2].y = src->height - 1;
        srcQuad[3].x = src->width - 1;
        srcQuad[3].y = src->height - 1;
        
        dstQuad[0].x = src->width * 0.005;
        dstQuad[0].y = src->height * 0.33;
        dstQuad[1].x = src->width * 0.9;
        dstQuad[1].y = src->height * 0.25;
        dstQuad[2].x = src->width * 0.2;
        dstQuad[2].y = src->height * 0.7;
        dstQuad[3].x = src->width * 0.8;
        dstQuad[3].y = src->height * 0.9;
        
        cvGetPerspectiveTransform( srcQuad, dstQuad, warp_matrix );
        cvWarpPerspective( src, dst, warp_matrix );
        
        
        cvNamedWindow( "Perspective_Transform", 1 );
        cvShowImage( "Perspective_Transform" , dst );
        
        cvWaitKey();
        
        cvReleaseImage( &dst );
		
    
    
    cvReleaseMat( &warp_matrix );
}