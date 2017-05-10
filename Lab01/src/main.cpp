/**
* GTI770 - Systemes intelligents et apprentissage machine
* Alessandro L. Koerich
* alessandro.koerich@etsmtl.ca
* 2015
*
* EXEMPLE 1: Feature extraction from RGB images
*                       Simpsons Family
**/

// INCLUDES
#include <cv.h> 			//OpenCV lib
#include <cvaux.h>			//OpenCV lib
#include <highgui.h>		//OpenCV lib
#include <stdio.h>	
#include <math.h>


// DEFINES
#define NUM_SAMPLES 100
#define NUM_FEATURES 6

// Bart Train: 80 items: bart1.bmp - bart80.bmp
// Homer Train 62 items: homer1.bmp - homer62.bmp
// Bart Valid: 54 items: bart116.bmp - bart169.bmp
// Homer Valid: 37 items: homer88.bmp - homer124.bmp

// orange
int matchPrim1(unsigned char red, unsigned char green, unsigned char blue) {
	return 11 <= blue && blue <= 22 && 85 <= green && green <= 105 && 240 <= red && red <= 255;
}

// white
int matchPrim2(unsigned char red, unsigned char green, unsigned char blue) {
	return red > 240 && green > 240 && blue > 240;
}

// gray
int matchPrim3(unsigned char red, unsigned char green, unsigned char blue) {
	return 180 <= red && red <= 212 && 150 <= green && green <= 185 && 96 <= blue && blue <= 150;
}

// green
int matchPrim4(unsigned char red, unsigned char green, unsigned char blue) {
	return 85 <= red && red <= 100 && 130 <= green && green <= 200 && 40 <= blue && blue <= 70;
}

// blue
int matchPrim5(unsigned char red, unsigned char green, unsigned char blue) {
	return 9 <= red && red <= 90 && 60 <= green && green <= 125 && 125 <= blue && blue <= 175;
}

int matchBorder(unsigned char red, unsigned char green, unsigned char blue) {
	return matchPrim2(red, green, blue);
}

void processCharacter(float fVector[ NUM_SAMPLES ][ NUM_FEATURES ], 
							 char *cFileName, 
							 const char *cImageName, 
							 const char *cCharacterName, 
							 int count, 
							 FILE *fp,
							 IplImage *img,
							 IplImage *processed)
{
	// OpenCV variables related to the image structure.
	// IplImage structure contains several information of the image (See OpenCV manual).	
	IplImage *threshold 	= NULL;
	
	// OpenCV variable that stores the image width and height
	CvSize tam;

	// OpenCV variable that stores a pixel value
	CvScalar element;

	// Variable store pressed key
	int tecla;

	int iNum;
	int h;
	int w;
	
	// Variables to store the RGB values of a pixel
	unsigned char red;
	unsigned char blue;
	unsigned char green;

	// Feature variables
	float fOrange;
	float fWhite;
	float fGray;
	float fGreen;
	float fBlue;

	for ( iNum = 1; iNum <= count; iNum++ )
	{
		// Build the image filename and path to read from disk
		sprintf ( cFileName, cImageName, (int)(iNum) );
		//sprintf ( cFileName, "Valid/homer%d.bmp", (int)(iNum) ); 
		printf ( " %s\n", cFileName);

		// Load the image from disk to the structure img.
		// 1  - Load a 3-channel image (color)
		// 0  - Load a 1-channel image (gray level)
		// -1 - Load the image as it is  (depends on the file)
		
		img = cvLoadImage( cFileName, -1 );

		// Gets the image size (width, height) 'img' 
		tam = cvGetSize( img );

		// Creates a header and allocates memory (tam) to store a copy of the original image.
		// 1 - gray level image
		// 3 - color image	
		// processed = cvCreateImage( tam, IPL_DEPTH_8U, 3);

		// Make a image clone and store it at processed and threshold
		processed 	= cvCloneImage( img );
		threshold  	= cvCloneImage( img );

		// Initialize variables with zero 
		fOrange 	= 0.0;
		fWhite 	= 0.0;
		fGray    = 0.0;
		fGreen	= 0.0;
		fBlue 	= 0.0;

		int borderPixels = 0;

		// Loop that reads each image pixel
		for( h = 0; h < img->height; h++ ) // rows
		{
			int leftBorderPixels = 0;
			int rightBorderPixels = 0;
			bool leftBorderDone = false;

			for( w = 0; w < img->width; w++ ) // columns
			{
				// Read each channel and writes it into the blue, green and red variables. Notice that OpenCV considers BGR
				blue  	= ( (uchar *)(img->imageData + h*img->widthStep) )[ w*img->nChannels + 0 ];
				green 	= ( (uchar *)(img->imageData + h*img->widthStep) )[ w*img->nChannels + 1 ];
				red   	= ( (uchar *)(img->imageData + h*img->widthStep) )[ w*img->nChannels + 2 ];

				// Shows the pixel value at the screenl
				//printf( "pixel[%d][%d]= %d %d %d \n", h, w, (int)blue, (int)green, (int)red );

				if ( matchBorder(red, green, blue) )
				{
					if (!leftBorderDone)
					{
						leftBorderPixels++;
						( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 0 ] = 0; 
						( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 1 ] = 0; 
						( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 2 ] = 255; 
					}
					else
					{
						rightBorderPixels++;
						( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 0 ] = 0;
						( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 1 ] = 255; 
						( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 2 ] = 0; 
					}
				}
				else
				{
					leftBorderDone = true;
					rightBorderPixels = 0;
				}

				// Here starts the feature extraction....
				
				// Detect and count the number of orange pixels
				// Verify if the pixels have a given value ( Orange, defined as R[240-255], G[85-105], B[11-22] ). If so, count it...
				if ( matchPrim1( red, green, blue ) )
				{
					fOrange++;
				
					// Just to be sure we are doing the right thing, we change the color of the orange pixels to green [R=0, G=255, B=0] and show them into a cloned image (processed)
				
					( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 0 ] = 0; 
					( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 1 ] = 255; 
					( (uchar *)(processed->imageData + h*processed->widthStep) )[ w*processed->nChannels + 2 ] = 0; 
				}
				
				// Detect and count the number of white pixels (just a dummy feature...)
				// Verify if the pixels have a given value ( White, defined as R[253-255], G[253-255], B[253-255] ). If so, count it...

				if ( matchPrim2( red, green, blue ) )
            {
               fWhite++;
            }
				// Here you can add your own features....... Good luck
            if ( matchPrim3( red, green, blue ) )
            {
            	fGray++;
            }

            if ( matchPrim4 (red, green, blue ) )
            {
            	fGreen++;
            }

            if ( matchPrim5( red, green, blue ) )
            {
               fBlue++;
            }
				
			}
			borderPixels += leftBorderPixels + rightBorderPixels;
		}

		// Lets make our counting somewhat independent on the image size...
		// Compute the percentage of pixels of a given colour.
		// Normalize the feature by the image size
		fWhite -= borderPixels;

		fOrange 	= fOrange / ( (int)img->height * (int)img->width - borderPixels);
		fWhite 	= fWhite  / ( (int)img->height * (int)img->width - borderPixels);
		fGray  	= fGray   / ( (int)img->height * (int)img->width - borderPixels);
		fGreen  	= fGreen  / ( (int)img->height * (int)img->width - borderPixels);
		fBlue  	= fBlue   / ( (int)img->height * (int)img->width - borderPixels);

		// Store the feature value in the columns of the feature (matrix) vector
		fVector[iNum][1] = fOrange;
		fVector[iNum][2] = fWhite;
		fVector[iNum][3] = fGray;
		fVector[iNum][4] = fGreen;
		fVector[iNum][5] = fBlue;
		
		// Here you can add more features to your feature vector by filling the other columns: fVector[iNum][3] = ???; fVector[iNum][4] = ???;
		
		// Shows the feature vector at the screen
		printf( "\n%d %f %f, %f, %f, %f", iNum, fVector[iNum][1], fVector[iNum][2], fVector[iNum][3], fVector[iNum][4], fVector[iNum][5]);
		//printf( "\n%d %f %f %f %f %f", iNum, fVector[iNum][1], fVector[iNum][2], fVector[iNum][3], fVector[iNum][4], fVector[iNum][5] );

		// And finally, store your features in a file
		fprintf( fp, "%f,", fVector[iNum][1]);
		fprintf( fp, "%f,", fVector[iNum][2]);
		fprintf( fp, "%f,", fVector[iNum][3]);
		fprintf( fp, "%f,", fVector[iNum][4]);
		fprintf( fp, "%f,", fVector[iNum][5]);
		
		// IMPORTANT
		// Do not forget the label.... 	
		fprintf( fp, "%s\n", cCharacterName);


		// Finally, give a look at the original image and the image with the pixels of interest in green
		// OpenCV create an output window
		cvShowImage( "Original", img );
		cvShowImage( "Processed", processed );
		
		// Wait until a key is pressed to continue... 	
	
	}
	tecla = cvWaitKey(0);
}

// MAIN
int main( int argc, char** argv )   
{
	// General variables (loop)
	int ii;
	int jj;

	// Feature vector [rows] [columns]
	// In fact it is a "matrix of features"
	float fVector[ NUM_SAMPLES ][ NUM_FEATURES ];

	// Variable filename
	static char cFileName[ 50 ] = {'\0'};
	FILE *fp;
	
	// Open a text file to store the feature vectors
	fp = fopen ("apprentissage-homer-bart.txt","w");
	// fp = fopen ("validation-homer-bart.txt","w");

	if(fp == NULL) {
		perror("failed to open apprentissage-homer-bart.txt");
		// perror("failed to open validation-homer-bart.txt");
		return EXIT_FAILURE;
	}
	
	// Fill fVector with zeros
	for ( ii = 0 ; ii < NUM_SAMPLES ; ii++ )
	{
		for ( jj = 0; jj < NUM_FEATURES; jj++ )
		{
			fVector[ii][jj] = 0.0;
		}
	}

	// Fill cFileName with zeros
	for ( ii = 0 ; ii < 50 ; ii++ )
	{
		cFileName[ ii ] = '\0';
	}

	IplImage *img 			= NULL;
	IplImage *processed 	= NULL;


	// *****************************************************************************************************************************************
	// TRAINING SAMPLES 
	// HOMER
	// Homer Train 62 items: homer1.bmp - homer62.bmp
	// *****************************************************************************************************************************************

	// Take all the image files at the range
	processCharacter(fVector, cFileName, "Train/homer%d.bmp", "Homer", 62, fp, img, processed);
	
	// *****************************************************************************************************************************************
	// *****************************************************************************************************************************************
	// *****************************************************************************************************************************************
	// TRAINING SAMPLES 
	// BART
	// Bart Train: 80 items: bart1.bmp - bart80.bmp
	// The code below is exactly the same for HOMER, except that we have changed the values of iNum and Homer -> Bart
	// along the file
	// *****************************************************************************************************************************************

	// Take all the image files at the range
	processCharacter(fVector, cFileName, "Train/bart%d.bmp", "Bart", 80, fp, img, processed);

	cvReleaseImage(&img);
	cvDestroyWindow("Original");

	cvReleaseImage(&processed);
	cvDestroyWindow("Processed");

	fclose(fp);

	return 0;
} 
