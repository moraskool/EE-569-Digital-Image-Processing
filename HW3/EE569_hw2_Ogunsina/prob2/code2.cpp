//  EE569 Homework Assignment  3 #Prob2 : Homographic Transformation and Image Stitching
//  Date:   03/12/2021
//  Name:   Morayo Abisola Ogunsina
//  ID:     7371213793
//  email:  ogunsina@usc.edu

 // To run the code :
 // 1. Use terminal and navigate to code path = /prob2
 // 2. type in g++ -o prob2 code2.cpp
 // 3. then prob3.exe <colored_input_left> <colored_input_middle> <colored_input_right>
 //  e.g prob2.exe left.raw middle.raw right.raw


 // to view output on ImageJ:
 // 1. import raw output file
 // 2. select image type 24-bit RGB
 // 3. set width to 2000, height to 1300
 // 4. OK.



#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

/*
 Write Raw funtion:
 writes the processed image data, saves into a .raw file for viewing
*/
void writeRaw(void* outputdata, const int length, const char* filename) {

	FILE* file;
	// Write image data (filename specified by second argument) from image data matrix
	if (!(file = fopen(filename, "wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	fwrite(outputdata, sizeof(unsigned char), length, file); // mult by 3 for rgb color
	fclose(file);
}

// here place the middle image in the panorama of larger size
// this is the image we stitch left and right warped images to
void sticthLeftMiddleImg(unsigned char* leftStitchedImg, unsigned char* leftImage, long double Hleft[3][3], int height, int width, int panoramaH, int panoramaW)
{
	

	float W;
	long double u[3];
	for (int i = 0; i < panoramaH; i++)
	{
		for (int j = 0; j < panoramaW; j++)
		{
			// hmatrix element
			int a[3] = { i, j, 1 };
			W = 1 / ((Hleft[2][0] * i) + (Hleft[2][1] * j) + Hleft[2][2]);
	
			// Multiply Left image coord with left Hmography Matrix
			for (int i = 0; i < 3; i++)
			{
				u[i] = 0;
				for (int k = 0; k < 3; k++)
				{
					u[i] = u[i] + Hleft[i][k] * a[k];
				}
			}

			// do conversion here
			// Homogeneous coordinates to cartesian co-ordinates
			// u * W, v * W
			float x = (float)u[0] * (float)W;  
			float y = (float)u[1] * (float)W;

			int xCartesian = floor(x);
			int yCartesian = floor(y);

			float Xdiff = x - xCartesian;
			float Ydiff = y - yCartesian;

			// calculate the intensity values for matching left and right points
			// do bilinear interpolation
			if (xCartesian > 0 && xCartesian < height && yCartesian > 0 && yCartesian < width)
			{

				// from this indexing method
				// OutputdataRecon[(i * (ImgWidth ) + j) * 3 + k] = imageRecon3D[i][j][k];

				// get corresponding pixel value and location of top left
				int edgeLeft1 = (xCartesian * width) + yCartesian;
				int edgeLeftIdx = edgeLeft1 * 3;

				// get corresponding pixel value and location of bottom left
				int edgeLeft2 = (xCartesian * width) + yCartesian + width;
				int edgeLeft2Idx = edgeLeft2 * 3;

				// get corresponding pixel value and location of top right
				int edgeRight1 = (xCartesian * width) + yCartesian + 1;
				int edgeRightIdx = edgeRight1 * 3;

				// get corresponding pixel value and location of bottom right
				int edgeRight2 = (xCartesian * width) + yCartesian + width + 1;
				int edgeRight2Idx = edgeRight2 * 3;

				// get the new location stitched image
				int stitchedLocation = ((i * panoramaW) + j) * 3;

				// bilinear interpol here
				leftStitchedImg[stitchedLocation + 0] = (unsigned char)(leftImage[edgeLeftIdx + 0] * (1 - Xdiff) * (1 - Ydiff)
					+ leftImage[edgeLeft2Idx + 0] * (Xdiff) * (1 - Ydiff) 
					+ leftImage[edgeRightIdx + 0] * (1 - Xdiff) * (Ydiff)
					+ leftImage[edgeRight2Idx + 0] * (Xdiff) * (Ydiff));

				leftStitchedImg[stitchedLocation + 1] = (unsigned char)(leftImage[edgeLeftIdx + 1] * (1 - Xdiff) * (1 - Ydiff)
					+ leftImage[edgeLeft2Idx + 1] * (Xdiff) * (1 - Ydiff)
					+  leftImage[edgeRightIdx + 1] * (1 - Xdiff) * (Ydiff)
					+ leftImage[edgeRight2Idx + 1] * (Xdiff) * (Ydiff));

				leftStitchedImg[stitchedLocation + 2] = (unsigned char)(leftImage[edgeLeftIdx + 2] * (1 - Xdiff) * (1 - Ydiff)
					+ leftImage[edgeLeft2Idx + 2] * (Xdiff) * (1 - Ydiff)
					+  leftImage[edgeRightIdx + 2] * (1 - Xdiff) * (Ydiff)
					+ leftImage[edgeRight2Idx + 2] * (Xdiff) * (Ydiff));
				//cout << (int)leftStitchedImg[stitchedLocation + 0] << ' ' << (int)leftStitchedImg[stitchedLocation + 1] << ' ' << leftStitchedImg[stitchedLocation + 2] << endl;
			
			}

		}
	}
}

// here place the middle image in the panorama of larger size
// this is the image we stitch left and right warped images to
void sticthMiddleImg(unsigned char* middleStitched, unsigned char* middleImage, int height, int width, int panoramaH, int panoramaW, int rowOffset, int colOffset)
{
	int stitchedPixel;
	int panoramaPixelLocation;
	int middlePixelLocation = 0;
	
	for (int i = rowOffset; i < rowOffset + height; i++)
	{
		for (int j = colOffset; j < colOffset + width; j++)
		{

			stitchedPixel = (i * panoramaW) + j;
			panoramaPixelLocation = stitchedPixel * 3;
			middleStitched[panoramaPixelLocation + 0] = middleImage[middlePixelLocation + 0];
			middleStitched[panoramaPixelLocation + 1] = middleImage[middlePixelLocation + 1];
			middleStitched[panoramaPixelLocation + 2] = middleImage[middlePixelLocation + 2];
			middlePixelLocation = middlePixelLocation + 3;
		}
	}
}


// here stitch teh right image to the middle image in the panorama
// and account for overlaps by using homography matrix
void sticthMiddleRightImg(unsigned char* rightStitchedImg, unsigned char* rightImage, long double HRight[3][3],int height, int width, int panoramaH, int panoramaW)
{

	long double W;
	long double v[3];
	for (int i = 0; i <= panoramaH; i++)
	{
		for (int j = 0; j <= panoramaW; j++)
		{
			// array of index for Mat multiply
			int a[3] = { i,j,1 };

			W = 1 / ((HRight[2][0] * i) + (HRight[2][1] * j) + HRight[2][2]);

			// Multiply Left image coord with right Hmography Matrix
			for (int i = 0; i < 3; i++)
			{
				v[i] = 0; // image coordinate
				for (int k = 0; k < 3; k++)
				{
					v[i] = v[i] + HRight[i][k] * a[k];
				}
			}

			// do conversion here
			// Homogeneous coordinates to image coordinates/cartesian
			// u * W, v * W
			long double x = v[0] * W; 
			long double y = v[1] * W;

			int xCartesian = floor(x); // very important!
			int yCartesian = floor(y);

			// our delta
			long double Xdiff = x - xCartesian;
			long double Ydiff = y - yCartesian;


			// calculate the intensity values for matching left and right points
			// do bilinear interpolation
			if (xCartesian > 0 && xCartesian < height && yCartesian > 0 && yCartesian < width)
			{

				// from this indexing method
				// OutputdataRecon[(i * (ImgWidth ) + j) * 3 + k] = imageRecon3D[i][j][k];

				// get corresponding pixel value and location of top left
				int edgeLeft1 = (xCartesian * width) + yCartesian;
				int edgeLeftIdx = edgeLeft1 * 3;

				// get corresponding pixel value and location of bottom left
				int edgeLeft2 = (xCartesian * width) + yCartesian + width;
				int edgeLeft2Idx = edgeLeft2 * 3;

				// get corresponding pixel value and location of top right
				int edgeRight1 = (xCartesian * width) + yCartesian + 1;
				int edgeRightIdx = edgeRight1 * 3;

				// get corresponding pixel value and location of bottom right
				int edgeRight2 = (xCartesian * width) + yCartesian + width + 1;
				int edgeRight2Idx = edgeRight2 * 3;

				// get the new location and pixel for stitched image
				int stitchedLocation = ((i * panoramaW) + j) * 3;

				// can use a for loop here for the bytesperpixel, for later

				rightStitchedImg[stitchedLocation + 0] = (unsigned char)(rightImage[edgeLeftIdx + 0] * (1 - Xdiff) * (1 - Ydiff) 
					+ rightImage[edgeLeft2Idx + 0] * (Xdiff) * (1 - Ydiff) 
					+ rightImage[edgeRightIdx + 0] * (1 - Xdiff) * (Ydiff)
					+ rightImage[edgeRight2Idx + 0] * (Xdiff) * (Ydiff));

				rightStitchedImg[stitchedLocation + 1] = (unsigned char)(rightImage[edgeLeftIdx + 1] * (1 - Xdiff) * (1 - Ydiff) 
					+ rightImage[edgeLeft2Idx + 1] * (Xdiff) * (1 - Ydiff) 
					+ rightImage[edgeRightIdx + 1] * (1 - Xdiff) * (Ydiff)
					+ rightImage[edgeRight2Idx + 1] * (Xdiff) * (Ydiff));

				rightStitchedImg[stitchedLocation + 2] = (unsigned char)(rightImage[edgeLeftIdx + 2] * (1 - Xdiff) * (1 - Ydiff) 
					+ rightImage[edgeLeft2Idx + 2] * (Xdiff) * (1 - Ydiff) 
					+  rightImage[edgeRightIdx + 2] * (1 - Xdiff) * (Ydiff)
					+ rightImage[edgeRight2Idx + 2] * (Xdiff) * (Ydiff));
				
				//cout << (int)rightStitchedImg[stitchedLocation + 0] << ' ' <<(int)rightStitchedImg[stitchedLocation + 1] << ' ' << rightStitchedImg[stitchedLocation + 2] << endl;
			}
		}
	}
}

int main(int argc, char* argv[])

{
	///////////////////////// BEGINNING CODE PROVIDED BY TA, ADDED MODIFICATIONS /////////////////////////

	// Define file pointer and variables

	FILE* file;

	//--------------------------------- SIZES  ------------------------------------------   
	int ImgWidthL = 322;       // number of columns in the image,
	int ImgHeightL = 483;      // number of rows in the image

	int ImgWidthM = 325;       // number of columns in the image,
	int ImgHeightM = 487;      // number of rows in the image

	int ImgWidthR = 325;       // number of columns in the image,
	int ImgHeightR = 489;      // number of rows in the image
	int BytesPerPixel = 3;     // color type 1, grayscale, 3 RGB 

	// size of the images
	const int SIZEL = ImgHeightL * ImgWidthL * BytesPerPixel;
	const int SIZEM = ImgHeightM * ImgWidthM * BytesPerPixel;
	const int SIZER = ImgHeightR * ImgWidthR * BytesPerPixel;

	// Dimensions for final image (middle image), stitching into a bigger image 
	const int panoramaRowOffset = 300;
	const int panoramaColOffset = 800;
	const int panoramaHeight = 1300;  // final image height
	const int panoramaWidth = 2000;   // final image width
	const int pSIZE = panoramaWidth * panoramaHeight * BytesPerPixel; // size of the final image output

	//--------------------input variables storing the imagefile names------------------//
	unsigned char* LeftImagedata = new unsigned char[SIZEL]();      //  left 
	unsigned char* MiddleImagedata = new unsigned char[SIZEM]();    //  middle
	unsigned char* RightImagedata = new unsigned char[SIZER]();     //  right
    
	//--------------------for storing the imagefile names----------------------------------//
	const char* MiddleStitchedfile = "MiddleStitched.raw";
	const char* LeftStitchedfile = "LeftStitched.raw";
	const char* RightStitchedfile = "RightStitched.raw";
	const char* panoramafile = "Panorama.raw";

	//--------------------------read Left image----------------------------//
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(LeftImagedata, sizeof(unsigned char), SIZEL, file);
	fclose(file);

	////--------------------------read Middle image----------------------------//
	if (!(file = fopen(argv[2], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(MiddleImagedata, sizeof(unsigned char), SIZEM, file);
	fclose(file);

	////--------------------------read Right image----------------------------//
	if (!(file = fopen(argv[3], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(RightImagedata, sizeof(unsigned char), SIZER, file);
	fclose(file);


	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	///////////////////////// MEMORY ALLOCATION FOR VARIABLES /////////////////////////

	unsigned char* panorama1DOutput = new unsigned char[pSIZE];
	memset(panorama1DOutput, 0, pSIZE * sizeof(unsigned char));

	//------------------------------------------------------------------------------------------------------------------------------------//
	// ------------------------------Get the Homography Matrix for the LeftMiddle and MiddleRight matching features ----//
	// 4 control points were used to create the Matrices
	// from Matlab, get the Homography Matrix, this is the biggest problem I had. I was getting NO image at all. the struggle!!
	// I had to go back to matlab and get good control points that were further apart from each other
	// update : still unable to get good image. 
	// I had to manually tweak the values to get the result I curently have now

	long double HLeft[3][3] = 
	{ 
		{0.70881,  -0.1418118,  -78.70942},   // controls height up and down
		{-0.0179564,   0.52471 , -254.1839},  // [][stretching][inside moving to right]controls the moving in horiz to right
		{-0.00003162,  -0.000504036,   1.205476}     // scaling big or small 

		// 2nd
		//{20.58369564203936, 3.156090334186795, -586.8962310303887},
		//{8.694853375563991, 7.952754467750342, -931.3157511397977},
		//{0.01621385906693558, 0.003291842978936028, 1}

		// 3rd
		//{1.110981575213,0.320334477785,-702.220261254909},
		//{ 0.001011231121,1.247497630304,-1260.823570050726},
		//{-0.000041246898,0.000950951874,0.053206924333} ;

		// 4th
		//{ 2.9751, 0.0111, -466.8329},
		//{1.3802, 2.4018, -316.8628},
		//{ 0.0059, 0.0002637,1}

	};

	long double HRight[3][3] =
	{
		// first
		//{0.1856794441002821, -0.5655575806211089, 168.7571559832684},
		//{0.1206015981771817, 1.336588364219407, -54.79569669500227},
		//{-0.0007796651008410991, 0.0006888778060265697, 1}

		// 2nd
		// after multiple times of tweeking
		{1.210981575213, 0.320334477785,-676.220261254909}, 
		{ 0.001011231121, 1.137497630304,-1038.823570050726}, //controls distance left and right, how further away from the fixed image
		{-0.000041246898,0.000950951874,0.365906924333} // controls scaling up and down
	};

	// Place the middle image in the final panorama output
	sticthMiddleImg(panorama1DOutput, MiddleImagedata, ImgHeightM, ImgWidthM, panoramaHeight, panoramaWidth, panoramaRowOffset, panoramaColOffset);
	writeRaw(panorama1DOutput, pSIZE, MiddleStitchedfile); // write to file for report purpose

	//stitch the left image into the current panorama image
	sticthLeftMiddleImg(panorama1DOutput, LeftImagedata, HLeft, ImgHeightL, ImgWidthL, panoramaHeight, panoramaWidth);
	writeRaw(panorama1DOutput, pSIZE, LeftStitchedfile); // write to file for report purpose

	// stitch the left image into the current panorama image
	sticthMiddleRightImg(panorama1DOutput, RightImagedata, HRight, ImgHeightR, ImgWidthR, panoramaHeight, panoramaWidth);
	writeRaw(panorama1DOutput, pSIZE, RightStitchedfile); // write to file for report purpose

	//Embed the middle image back into the embedded image containing the left,right images 
	// a fix i decided to use
	sticthMiddleImg(panorama1DOutput, MiddleImagedata, ImgHeightM, ImgWidthM, panoramaHeight, panoramaWidth, panoramaRowOffset, panoramaColOffset);
	writeRaw(panorama1DOutput, pSIZE, panoramafile); // write to file for report purpose

	delete[] LeftImagedata;
	delete[] RightImagedata;
	delete[] MiddleImagedata;

	return 0;
}
