
//  EE569 Homework Assignment #Prob1b part a
//  Date:   02/07/2021
//  Name:   Morayo Abisola Ogunsina
//  ID:     7371213793
//  email:  ogunsina@usc.edu

//1. Use terminal and navigate to code path = / prob1bB
//2. type in g++ - o prob1bA code1b_B.cpp
//3. then prob1bB.exe <input_raw> <output_raw> < color mode>
//4. e.g prob1bB.exe Toy.raw Toy_output.raw 1

//1. import raw output file
//2. select image type 8 - bit
//3. set width to 400, height to 560
//4. OK.



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
	int BytesPerPixel = 1;
	const int ImgWidth = 400, ImgHeight = 560; // number of columns in the image, // number of rows in the image
	const int SIZE = ImgWidth * ImgHeight;

	//---------------------------------Get aruguments------------------------------------------

	BytesPerPixel = atoi(argv[4]); // else it is RGB

	//------------------------------------------------------------------------------------------------------
			// Allocate image data array
	unsigned char Imagedata[SIZE];   // Use a 2D array
			// for the output array, in greyscale
	unsigned char Outputdata[SIZE]; // use a 2d


	//------------------------------------------------------------------------------------------------------
	// read the Toy.raw and store in 1d array
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), SIZE, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////


	//------------------------------METHOD B: CUMMULATIVE-PROBABILITY-ASED METHOD-----------------------------------------

	double normalized[256] = { 0 };           // array for normalized pixels
	double temp1[256] = { 0 };                 // array for Cumulative Density Funct  
	double temp2[256] = { 0 };                 // array for Cumulative Density Funct 
	double pixelVal[256];                     // array for new mapped pixel values
	unsigned long pixelCount1[256] = { 0 };    // array for enhanced pixels count
	unsigned long pixelCount2[256] = { 0 };    // array for enhanced pixels count

   //----------BEFORE ENHANCEMENT----------------------------------------------

	for (int i = 0; i < SIZE; i++) {
		pixelCount1[(int)Imagedata[i]]++;
	}

	//----------cummulative distribution here----------------------------------------------
	// 
	for (int i = 0; i <= 255; i++)
	{
		if (i == 0)
			// initialize first CDF to first normalized
			temp1[0] = pixelCount1[0];
		else
			// Now get the Cummulative prob
			temp1[i] = temp1[i - 1] + pixelCount1[i];
	}


	//---------------------------Use Bucket filling method here.------------------------------------------- 
	int sizeOfBucket = SIZE / (255 + 1) + (SIZE % (255 + 1) > 0 ? 1 : 0);
	unsigned char output[SIZE];
	int gray = 0;
	int count = 0;
	for (int i = 0; i <= 255; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (Imagedata[j] == i) {
				Outputdata[j] = gray;
				count++;
				if (count >= sizeOfBucket) {
					gray++;
					count = 0;
				}
			}
		}
	}



	//----------AFTER ENHANCEMENT----------------------------------------------


	//------------------Find the frequency of 0 to 255 pixel values for enhanced image-----------------------
	for (int i = 0; i < SIZE; i++) {
				pixelCount2[(int)Outputdata[i]]++;
		}
		

	//----------cummulative distribution here----------------------------------------------
	// 
	for (int i = 0; i <= 255; i++)
		{

			if (i == 0)
				// initialize first CDF to first normalized
				temp2[0] = pixelCount2[0];
			else
				// Now get the Cummulative Density using CDF
				temp2[i] = temp2[i - 1] + pixelCount2[i];
		}


		//--------------------------Get histogram of equalized/enhance image from method B-------------------
		ofstream file5;
		file5.open("histogramB.txt", ios::out);
		for (int i = 0; i < 256; i++)
		{
			file5 << i << " \t  " << pixelCount2[i] << endl;
		}
		file5.close();

		//--------------------------get CDF distribution before-------------------
		ofstream file6;
		file6.open("histogramBTransfer.txt", ios::out);
		for (int i = 0; i < 256; i++)
		{
			file6 << i << " \t  " << temp1[i] << endl;
		}
		file6.close();


		//--------------------------get CDF distribution after-------------------
		ofstream file7;
		file7.open("histogramBPixel.txt", ios::out);
		for (int i = 0; i < 256; i++)
		{
			file7 << i << " \t  " << temp2[i] << endl;
		}
		file7.close();

		//------------------------------------------------------------------------------------------------------
			// Write image data (filename specified by second argument) from image data matrix
		if (!(file = fopen(argv[2], "wb"))) {
			cout << "Cannot open file: " << argv[2] << endl;
			exit(1);
		}
		fwrite(Outputdata, sizeof(unsigned char), SIZE, file); // method A output

		return 0;
	}

