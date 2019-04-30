
#include "head.h"

void Histogrem()
{
	IplImage *src;
    
    src = cvLoadImage("C:\\84.jpg", 1);
        
        // HSV image and decompose into separate planes
        
        IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
        cvCvtColor( src, hsv, CV_BGR2HSV);
        
        
        IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
        IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
        IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
        IplImage* planes[] = { h_plane, s_plane };
        cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
        
        // Build the histogram amd compute
        int h_bins = 30, s_bins = 32;
        CvHistogram* hist;
        {
            int hist_size[] = { h_bins, s_bins };
            float h_ranges[] = { 0, 180 };
            float s_ranges[] = { 0, 255 };
            float* ranges[] = { h_ranges, s_ranges };
            hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
        }
        cvCalcHist( planes, hist, 0, 0 );
        cvNormalizeHist( hist, 1.0 );
        
        // create image to visualize
        int scale = 10;
        IplImage* hist_img = cvCreateImage( cvSize(h_bins*scale, s_bins*scale), 8, 3);
        cvZero( hist_img );
        
        float max_value = 0;
        cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
        for ( int h = 0; h < h_bins; h ++ ) {
            for ( int s = 0; s < s_bins ; s++ ) {
                float bin_val = cvQueryHistValue_2D( hist, h, s );
                int intensity = cvRound( bin_val * 255 / max_value );
                cvRectangle(hist_img,
					cvPoint( h*scale, s*scale ),
					cvPoint( (h+1)*scale-1, (s+1)*scale-1 ),
					CV_RGB(intensity, intensity, intensity),
					CV_FILLED
					);
            }
        }
        cvNamedWindow( "Source", 1 );
        cvShowImage( "Source", src );
        
        cvNamedWindow( "H-S Histogrem", 1 );
        cvShowImage( "H-S Histogrem", hist_img);
        
        cvWaitKey();
        
        cvReleaseImage( &src );
        cvReleaseImage( &hist_img );
        cvReleaseHist( &hist );
        
    
}

void Histogrem_EMD()
{
	IplImage *src1, *src2;
    src1 = cvLoadImage("C:\\84.jpg", 1);
	src2 = cvLoadImage("C:\\86.jpg", 1);
    
        // HSV image and decompose into separate planes
        
        IplImage* hsv1 = cvCreateImage( cvGetSize(src1), 8, 3 );
        cvCvtColor( src1, hsv1, CV_BGR2HSV);
        
        
        IplImage* h_plane1 = cvCreateImage( cvGetSize(src1), 8, 1 );
        IplImage* s_plane1 = cvCreateImage( cvGetSize(src1), 8, 1 );
        IplImage* v_plane1 = cvCreateImage( cvGetSize(src1), 8, 1 );
        IplImage* planes1[] = { h_plane1, s_plane1 };
        cvCvtPixToPlane( hsv1, h_plane1, s_plane1, v_plane1, 0 );
        
        IplImage* hsv2 = cvCreateImage( cvGetSize(src2), 8, 3 );
        cvCvtColor( src2, hsv2, CV_BGR2HSV);
        
        
        IplImage* h_plane2 = cvCreateImage( cvGetSize(src2), 8, 1 );
        IplImage* s_plane2 = cvCreateImage( cvGetSize(src2), 8, 1 );
        IplImage* v_plane2 = cvCreateImage( cvGetSize(src2), 8, 1 );
        IplImage* planes2[] = { h_plane2, s_plane2 };
        cvCvtPixToPlane( hsv2, h_plane2, s_plane2, v_plane2, 0 );
        
        // Build the histogram amd compute
        int h_bins = 30, s_bins = 32;
        CvHistogram *hist1, *hist2;
        {
            int hist_size[] = { h_bins, s_bins };
            float h_ranges[] = { 0, 180 };
            float s_ranges[] = { 0, 255 };
            float* ranges[] = { h_ranges, s_ranges };
            hist1 = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
            hist2 = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
        }
        cvCalcHist( planes1, hist1, 0, 0 );
        cvNormalizeHist( hist1, 1.0 );
        
        cvCalcHist( planes2, hist2, 0, 0 );
        cvNormalizeHist( hist2, 1.0 );
        
        // Get signature using EMD
        CvMat *sig1,*sig2;
        int numrows = h_bins * s_bins;
        
        sig1 = cvCreateMat(numrows, 3, CV_32FC1);
        sig2 = cvCreateMat(numrows, 3, CV_32FC1);
        
        // Create image to visualize
        int scale = 10;
        IplImage* hist_img1 = cvCreateImage( cvSize(h_bins*scale, s_bins*scale), 8, 3);
        cvZero( hist_img1 );
        IplImage* hist_img2 = cvCreateImage( cvSize(h_bins*scale, s_bins*scale), 8, 3);
        cvZero( hist_img2 );
        
        float max_value1 = 0;
        cvGetMinMaxHistValue( hist1, 0, &max_value1, 0, 0 );
        float max_value2 = 0;
        cvGetMinMaxHistValue( hist2, 0, &max_value2, 0, 0 );
        
        // Fill
        for ( int h = 0; h < h_bins; h ++ ) {
            for ( int s = 0; s < s_bins ; s++ ) {
                float bin_val1 = cvQueryHistValue_2D( hist1, h, s );
                float bin_val2 = cvQueryHistValue_2D( hist2, h, s );
                // Image
                int intensity1 = cvRound( bin_val1 * 255 / max_value1 );
                cvRectangle(hist_img1,
                            cvPoint( h*scale, s*scale ),
                            cvPoint( (h+1)*scale-1, (s+1)*scale-1 ),
                            CV_RGB(intensity1, intensity1, intensity1),
                            CV_FILLED
                            );
                int intensity2 = cvRound( bin_val2 * 255 / max_value2 );
                cvRectangle(hist_img2,
                            cvPoint( h*scale, s*scale ),
                            cvPoint( (h+1)*scale-1, (s+1)*scale-1 ),
                            CV_RGB(intensity2, intensity2, intensity2),
                            CV_FILLED
                            );
                
                // Signature
                cvSet2D(sig1, h*s_bins+s, 0, cvScalar(bin_val1)); // bin value
                cvSet2D(sig1, h*s_bins+s, 1, cvScalar(h)); // Coord 1
                cvSet2D(sig1, h*s_bins+s, 2, cvScalar(s)); // Coord 2
                cvSet2D(sig2, h*s_bins+s, 0, cvScalar(bin_val2)); // bin value
                cvSet2D(sig2, h*s_bins+s, 1, cvScalar(h)); // Coord 1
                cvSet2D(sig2, h*s_bins+s, 2, cvScalar(s)); // Coord 2
            }
        }
        cvNamedWindow( "Source - 1", 1 );
        cvShowImage( "Source - 1", src1 );
        cvNamedWindow( "H-S Histogrem - 1", 1 );
        cvShowImage( "H-S Histogrem - 1", hist_img1 );
        
        cvNamedWindow( "Source - 2", 1 );
        cvShowImage( "Source - 2", src2 );
        cvNamedWindow( "H-S Histogrem - 2", 1 );
        cvShowImage( "H-S Histogrem - 2", hist_img2 );
        
        float emd = cvCalcEMD2( sig1, sig2, CV_DIST_L2);
        printf("EMD : %f ;", emd);
        
        cvWaitKey();
        
        cvReleaseImage( &src1 );
        cvReleaseImage( &hist_img1 );
        cvReleaseHist( &hist1 );
        cvReleaseMat( &sig1 );
        cvReleaseImage( &src2 );
        cvReleaseImage( &hist_img2 );
        cvReleaseHist( &hist2 );
        cvReleaseMat( &sig2 );
        
    
}

void MatchTemplate()
{
	IplImage *src, *templ, *ftmp[6];
    int i;
    src = cvLoadImage("C:\\84.jpg", 1);
	templ = cvLoadImage("C:\\86.jpg", 1);
        // Allocate output images
        int i_width = src->width - templ->width + 1;
        int i_height = src->height - templ->height + 1;
        for ( i = 0; i < 6; i++ ) {
            ftmp[i] = cvCreateImage( cvSize(i_width, i_height), 32, 1 );
        }
        // Do template matching
        for ( i = 0; i < 6; i++) {
            cvMatchTemplate( src, templ, ftmp[i], i );
            cvNormalize( ftmp[i], ftmp[i], 1, 0, CV_MINMAX );
        }
        
        //DISPLAY
        cvNamedWindow( "Template", 0 );
        cvShowImage(   "Template", templ );
        cvNamedWindow( "Image", 0 );
        cvShowImage(   "Image", src );
        
        cvNamedWindow( "SQDIFF", 0 );
        cvShowImage(   "SQDIFF", ftmp[0] );
        
        cvNamedWindow( "SQDIFF_NORMED", 0 );
        cvShowImage(   "SQDIFF_NORMED", ftmp[1] );
        
        cvNamedWindow( "CCORR", 0 );
        cvShowImage(   "CCORR", ftmp[2] );
        
        cvNamedWindow( "CCORR_NORMED", 0 );
        cvShowImage(   "CCORR_NORMED", ftmp[3] );
        
        cvNamedWindow( "CCOEFF", 0 );
        cvShowImage(   "CCOEFF", ftmp[4] );
        
        cvNamedWindow( "CCOEFF_NORMED", 0 );
        cvShowImage(   "CCOEFF_NORMED", ftmp[5] );
        
        cvWaitKey();
		
    
}