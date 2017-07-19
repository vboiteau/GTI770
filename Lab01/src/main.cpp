/**
* GTI770 - Systemes intelligents et apprentissage machine
* Alessandro L. Koerich
* alessandro.koerich@etsmtl.ca
* 2015
*
* EXEMPLE 1: Feature extraction from RGB images
*                       Simpsons Family
*
* USAGE:
* make;./main {HB,HBL,HBLO};./build-arff {HB, HBL, HBLO}                     
**/

// INCLUDES
#include <cv.h> 			//OpenCV lib
#include <cvaux.h>			//OpenCV lib
#include <highgui.h>		//OpenCV lib
#include <stdio.h>	
#include <math.h>


// DEFINES
#define NUM_SAMPLES 200
#define NUM_FEATURES 8

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
	return 80 <= red && red <= 100 && 130 <= green && green <= 200 && 20 <= blue && blue <= 50;
}

// blue
int matchPrim5(unsigned char red, unsigned char green, unsigned char blue) {
	return 9 <= red && red <= 90 && 60 <= green && green <= 125 && 125 <= blue && blue <= 175;
}

// red 
int matchPrim6(unsigned char red, unsigned char green, unsigned char blue) {
	return 254 <= red && red <= 255 && 0 <= green && green <= 1 && 0 <= blue && blue <= 1;
}

int matchOthers(unsigned char red, unsigned char green, unsigned char blue) {
    return (!(matchPrim1(red, green, blue) || matchPrim3(red, green, blue) || matchPrim6(red, green, blue) || matchPrim2(red, green, blue)));
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
							 IplImage *processed,
                             int fileToGenerate,
                             int offset)
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
	float fRed;
	float fOthers;

	for ( iNum = 1; iNum <= count; iNum++ )
	{
		// Build the image filename and path to read from disk
		sprintf ( cFileName, cImageName, (int)(iNum+offset) );
		//sprintf ( cFileName, "Valid/homer%d.bmp", (int)(iNum) ); 
		//printf ( " %s\n", cFileName);

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
		fOrange = 0.0;
		fWhite 	= 0.0;
		fGray   = 0.0;
		fGreen	= 0.0;
		fBlue 	= 0.0;
        fRed    = 0.0;
		fOthers = 0.0;

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

				if ( matchPrim6( red, green, blue ) )
				{
				   fRed++;
				}

				if ( matchOthers(red, green, blue) )
				{
					fOthers++;
				}
				
			}
			borderPixels += leftBorderPixels + rightBorderPixels;
		}

		// Lets make our counting somewhat independent on the image size...
		// Compute the percentage of pixels of a given colour.
		// Normalize the feature by the image size
		fWhite -= borderPixels;

		fOrange = fOrange / ( (int)img->height * (int)img->width - borderPixels);
		fWhite 	= fWhite  / ( (int)img->height * (int)img->width - borderPixels);
		fGray  	= fGray   / ( (int)img->height * (int)img->width - borderPixels);
		fGreen  = fGreen  / ( (int)img->height * (int)img->width - borderPixels);
		fBlue  	= fBlue   / ( (int)img->height * (int)img->width - borderPixels);
		fRed  	= fRed    / ( (int)img->height * (int)img->width - borderPixels);
		fOthers = fOthers / ( (int)img->height * (int)img->width - borderPixels);

		// Store the feature value in the columns of the feature (matrix) vector
		fVector[iNum][1] = fOrange;
		fVector[iNum][2] = fWhite;
		fVector[iNum][3] = fGray;
		fVector[iNum][4] = fGreen;
		fVector[iNum][5] = fBlue;
		fVector[iNum][6] = fRed;
		fVector[iNum][7] = fOthers;

		
		// Here you can add more features to your feature vector by filling the other columns: fVector[iNum][3] = ???; fVector[iNum][4] = ???;
		
		// Shows the feature vector at the screen
		//printf( "\n%d %f %f, %f, %f, %f, %f", iNum, fVector[iNum][1], fVector[iNum][2], fVector[iNum][3], fVector[iNum][4], fVector[iNum][5], fVector[iNum][6]);
		//printf( "\n%d %f %f %f %f %f", iNum, fVector[iNum][1], fVector[iNum][2], fVector[iNum][3], fVector[iNum][4], fVector[iNum][5] );

		// And finally, store your features in a file
		fprintf( fp, "%f,", fVector[iNum][1]);
		fprintf( fp, "%f,", fVector[iNum][2]);
		fprintf( fp, "%f,", fVector[iNum][3]);
		fprintf( fp, "%f,", fVector[iNum][4]);
		fprintf( fp, "%f,", fVector[iNum][5]);
        if (fileToGenerate > 0) {
            fprintf( fp, "%f,", fVector[iNum][6]);

			if (fileToGenerate > 1)
				fprintf(fp, "%f,", fVector[iNum][7]);
        }
		
		// IMPORTANT
		// Do not forget the label.... 	
		fprintf( fp, "%s\n", cCharacterName);


		// Finally, give a look at the original image and the image with the pixels of interest in green
		// OpenCV create an output window
		cvShowImage( "Original", img );
		cvShowImage( "Processed", processed );
		
		// Wait until a key is pressed to continue... 	
	
	}
	//tecla = cvWaitKey(0);
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
    int fileToGenerate = 0;

    const char* HBL = "HBL";
    const char* HBLO = "HBLO";

    if (strcmp(argv[1], HBL) == 0) {
       fileToGenerate = 1;
    } else if(strcmp(argv[1], HBLO) == 0) {
       fileToGenerate = 2;
    }

	// Variable filename
	static char cFileName[ 50 ] = {'\0'};
    static char fTrainName[50] = {'\0'};
    static char errorMsg[100] = {'\0'};
    
    sprintf(fTrainName,"Output/EquipeI-Apprentissage-%s.txt",argv[1]);

    FILE *fTrain;
    fTrain = fopen (fTrainName,"w");
	
	if(fTrain == NULL) {
        sprintf(errorMsg,"failed to open %s", fTrainName);
		perror(errorMsg);
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
	processCharacter(fVector, cFileName, "Train/homer%d.bmp", "Homer", 62, fTrain, img, processed, fileToGenerate, 0);
	
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
	processCharacter(fVector, cFileName, "Train/bart%d.bmp", "Bart", 80, fTrain, img, processed, fileToGenerate, 0);

	// *****************************************************************************************************************************************
	// *****************************************************************************************************************************************
	// *****************************************************************************************************************************************
	// TRAINING SAMPLES 
	// LISA 
	// Lisa Train: 33 items: lisa1.bmp - lisa33.bmp
	// The code below is exactly the same for HOMER, except that we have changed the values of iNum and Homer -> Lisa
	// along the file
	// *****************************************************************************************************************************************

	// Take all the image files at the range
    if (fileToGenerate > 0) {
        processCharacter(fVector, cFileName, "Train/lisa%d.bmp", "Lisa", 33, fTrain, img, processed, fileToGenerate, 0);
    }
    // *****************************************************************************************************************************************
    // *****************************************************************************************************************************************
    // *****************************************************************************************************************************************
    // TRAINING SAMPLES 
    // OTHERS 
    // Others Train: 121 items: other1.bmp - other121.bmp
    // The code below is exactly the same for HOMER, except that we have changed the values of iNum and Homer -> Others
    // along the file
    // *****************************************************************************************************************************************

    // Take all the image files at the range
    if (fileToGenerate == 2) {
        processCharacter(fVector, cFileName, "Train/other%d.bmp", "Others", 121, fTrain, img, processed, fileToGenerate, 0);
    }

    cvReleaseImage(&img);
    cvDestroyWindow("Original");

    cvReleaseImage(&processed);
    cvDestroyWindow("Processed");

	fclose(fTrain);
    static char fValidationName[50] = {'\0'};
    
    sprintf(fValidationName,"Output/EquipeI-Validation-%s.txt",argv[1]);

    FILE *fValidation;

    fValidation = fopen (fValidationName, "w");

	if(fValidation == NULL) {
        sprintf(errorMsg,"failed to open %s", fValidationName);
		perror(errorMsg);
		// perror("failed to open validation-homer-bart.txt");
		return EXIT_FAILURE;
	}

    
	// *****************************************************************************************************************************************
	// VALIDATION SAMPLES 
	// HOMER
	// Homer Validate 37 items: homer1.bmp - homer37.bmp
	// *****************************************************************************************************************************************

	// Take all the image files at the range
	processCharacter(fVector, cFileName, "Valid/homer%d.bmp", "Homer", 37, fValidation, img, processed, fileToGenerate, 87);
	
	// *****************************************************************************************************************************************
	// *****************************************************************************************************************************************
	// *****************************************************************************************************************************************
	// VALIDATION SAMPLES 
	// BART
	// Bart Validate: 54 items: bart1.bmp - bart54.bmp
	// The code below is exactly the same for HOMER, except that we have changed the values of iNum and Homer -> Bart
	// along the file
	// *****************************************************************************************************************************************

	// Take all the image files at the range
	processCharacter(fVector, cFileName, "Valid/bart%d.bmp", "Bart", 54, fValidation, img, processed, fileToGenerate, 115);

	// *****************************************************************************************************************************************
	// *****************************************************************************************************************************************
	// *****************************************************************************************************************************************
	// VALIDATION SAMPLES 
	// LISA 
	// Lisa Validate: 13 items: lisa1.bmp - lisa13.bmp
	// The code below is exactly the same for HOMER, except that we have changed the values of iNum and Homer -> Lisa
	// along the file
	// *****************************************************************************************************************************************

	// Take all the image files at the range
    if (fileToGenerate > 0) {
        processCharacter(fVector, cFileName, "Valid/lisa%d.bmp", "Lisa", 13, fValidation, img, processed, fileToGenerate, 33);
    }
    // *****************************************************************************************************************************************
    // VALIDATION SAMPLES 
    // Other 
    // Other Validate: 49 items: other1.bmp - other49.bmp
    // The code below is exactly the same for HOMER, except that we have changed the values of iNum and Homer -> Other
    // along the file
    // *****************************************************************************************************************************************

    // Take all the image files at the range
    if (fileToGenerate == 2) {
        processCharacter(fVector, cFileName, "Valid/other%d.bmp", "Others", 49, fValidation, img, processed, fileToGenerate, 121);
    }

    cvReleaseImage(&img);
    cvDestroyWindow("Original");

    cvReleaseImage(&processed);
    cvDestroyWindow("Processed");
    
	fclose(fValidation);


	return 0;
} 
