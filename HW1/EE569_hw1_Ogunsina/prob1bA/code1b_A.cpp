
//  EE569 Homework Assignment #Prob1b part a
//  Date:   02/07/2021
//  Name:   Morayo Abisola Ogunsina
//  ID:     7371213793
//  email:  ogunsina@usc.edu

 // To run the code :
 // 1. Use terminal and navigate to code path = /prob1bA
 // 2. type in g++ -o prob1bA code1b_A.cpp
 // 3. then prob1bA.exe <input_raw> <output_raw> <color mode> use 1 since image is in grayscale
 // e.g prob1bA.exe Toy.raw Toy_output.raw 1

 // to view output on ImageJ:
 // 1. import raw output file
 // 2. select image type 8-bit 
 // 3. set width to 400, height to 560
 // 4. OK.



#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <math.h>

using namespace std;

int main(int argc, char* argv[])

{
	///////////////////////// BEGINNING CODE PROVIDED BY TA, ADDED MODIFICATIONS /////////////////////////

	// Define file pointer and variables
	FILE* file;
	int BytesPerPixel;
	const int ImgWidth = 400, ImgHeight = 560; // number of columns in the image, // number of rows in the image

	// Check for proper syntax
	if (argc < 4) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw [HistogramA.raw] [HistgramB.raw] [BytesPerPixel = 1]" << endl;
		return 0;
	}

	//---------------------------------Get aruguments------------------------------------------

	BytesPerPixel = atoi(argv[3]); // else it is RGB

			// get size of image length
	int length = BytesPerPixel * ImgHeight * ImgWidth;

	//------------------------------------------------------------------------------------------------------
			// Allocate image data array
	unsigned char Imagedata[ImgHeight][ImgWidth][1];   // Use a 2D array
			// for the output array, in greyscale
	unsigned char Outputdata[ImgHeight][ImgWidth][1]; // use a 2d
	//------------------------------------------------------------------------------------------------------
			// Read image (filename specified by first argument) into image data matrix
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}

	//------------------------------------------------------------------------------------------------------
			// read the House.raw and store in 1d array
	fread(Imagedata, sizeof(unsigned char), length, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	//-----------------------------------METHOD A : TRANSFER FUNCTION-BASED METHOD--------------------

	double normalized[256] = { 0 };           // array for normalized pixels
	double temp[256] = { 0 };                 // array for Cumulative Density Funct  
	double pixelVal[256];                     // array for new mapped pixel values
	unsigned long pixelCountO[256] = { 0 };   // array for pixels count
	unsigned long pixelCount1[256] = { 0 };   // array for enhanced pixels count


	//------------------Find the frequency of 0 to 255 pixel values for original image----------
	for (int i = 0; i < ImgHeight; i++) {
		for (int j = 0; j < ImgWidth; j++) {
			pixelCountO[(int)Imagedata[i][j][0]]++;
		}
	}

	//------------------------------------------------------------------------------------------------------
	// do histogram equalization using cummulative probabilty
	for (int i = 0; i <= 255; i++)
	{
		// Normalized
		normalized[i] += ((double)pixelCountO[i]) / (double)(ImgHeight * ImgWidth);
		if (i == 0)
			// initialize first Cprob to first normalized
			temp[0] = normalized[0];
		else
			// Now get the Cummulative probaility
			temp[i] = temp[i - 1] + normalized[i];

		// Multiply 255.0, then round off to lower
		pixelVal[i] = temp[i] * 255.0;
		pixelVal[i] = floor(pixelVal[i]);
	}

	//------------------------------------------------------------------------------------------------------
	// Now map pixel val to new values from histogram equalization
	// basically get the enhanced image
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			Outputdata[i][j][0] = pixelVal[(int)Imagedata[i][j][0]]; // it's all about mapping the right indices. 
		}
	}

	//------------------Find the frequency of 0 to 255 pixel values for enhanced image-----------------------
	for (int i = 0; i < ImgHeight; i++) {
		for (int j = 0; j < ImgWidth; j++) {
			pixelCount1[(int)Outputdata[i][j][0]]++;
		}
	}


	//--------------------------Write histogram of original image-----------------------------
	ofstream file1;
	file1.open("histogramOri.txt", ios::out);
	for (int i = 0; i < 256; i++)
	{
		file1 << i << " \t  " << pixelCountO[i] << endl;
	}
	file1.close();

	//--------------------------Write histogram of enhanced image from method A-------------------
	ofstream file2;
	file2.open("histogramA.txt", ios::out);
	for (int i = 0; i < 256; i++)
	{
		file2 << i << " \t  " << pixelCount1[i] << endl;
	}
	file2.close();

	//--------------------------data to plot transfer function for mapped pixel values-------------------
	ofstream file3;
	file3.open("histogramATransfer.txt", ios::out);
	for (int i = 0; i < 256; i++)
	{
		file3 << i << " \t  " << pixelVal[i] << endl;
	}
	file3.close();

	//--------------------------get data to show distribution of pixels-------------------
	ofstream file4;
	file4.open("histogramAPixel.txt", ios::out);
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			file4 << i << " \t  " << (int)Imagedata[i][j][0] << "\t" << (int)Outputdata[i][j][0] << endl; // check here again
		}
	}
	file4.close();

	//------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------
		// Write image data (filename specified by second argument) from image data matrix
	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Outputdata, sizeof(unsigned char), ImgHeight * ImgWidth * BytesPerPixel, file); // method A output

	
	return 0;
}

