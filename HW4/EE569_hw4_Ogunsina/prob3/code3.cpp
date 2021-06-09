//  EE569 Homework Assignment 4 #Prob3: SIFT and Image Matching
//  Date:   03/28/2021
//  Name:   Morayo Abisola Ogunsina
//  ID:     7371213793
//  email:  ogunsina@usc.edu

// Original Source code from https://docs.opencv.org/3.4/d5/d6f/tutorial_feature_flann_matcher.html


#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

// get the SIFT features for individual images in grayscale
Mat detectSIFTKeyPoints(Mat img, int minHessian) {
	
	std::vector<KeyPoint> keypoints;
	Mat imgWithKeyPoints;
	Ptr<SIFT> detector = SIFT::create(minHessian);

	// Detect and Draw Key points
	detector->detect(img, keypoints);

	drawKeypoints(img, keypoints, imgWithKeyPoints, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	return imgWithKeyPoints;
}

// write histogram values to a txt file for viewing
void WriteHistToText(string filepath, int* hist, int size) {

	ofstream file; // create an output filestream

	file.open(filepath);

	if (file.is_open()) {

		for (int i = 0; i < size; i++) {

			file << i << " \t  " << hist[i] << endl;;
		}
		file.close();
	}
	else
	{
		cout << "unable to open file" << endl;
	}

}

// Get the error
double FindError(int* histCodebook, int* histCodebookCompare) {
	double error = 0;

	for (int j = 0; j < 8; j++) {

		error += abs(histCodebook[j] - histCodebookCompare[j]); // additive difference
	}
	error = error / 8.0; // normalize
	return error;
}

// get the frequency of the features
int* getCodebookhHistogram(Mat descriptor, Mat codebook) {
	
	double* CentroidDistances = new double[codebook.rows];
	double tempD = 0.0;
	double temp = 0; 
	int index = 0;
	int* histCount = new int[codebook.rows];
	
	// initialize histogram array to 0
	for (int i = 0; i < codebook.rows; i++) {

		histCount[i] = 0;
	}

	// then get the frequency of each feature
	// use Euclidean Distance to find the closest Centroid to every feature
	for (int i = 0; i < descriptor.rows; i++) {
		
		// loop through each centroid
		// find the distance from all centroids
		for (int k = 0; k < codebook.rows; k++) {

			for (int j = 0; j < descriptor.cols; j++) {

				tempD += ( (descriptor.at<float>(i, j) - codebook.at<float>(k, j) ) * (descriptor.at<float>(i, j) - codebook.at<float>(k, j)));
				//long double tempDiff =  descriptor.at<float>(i, j) - codebook.at<float>(k, j);
				//tempD = tempD + pow(tempDiff, 2);
			}

			CentroidDistances[k] = (sqrt(tempD));
			tempD = 0.0;

		}

		temp = CentroidDistances[0]; index = 0;

		// find the centroid with the minimum distance
		for (int k = 1; k < codebook.rows; k++) {

			if (temp > CentroidDistances[k]) {

				temp = CentroidDistances[k];
				index = k;
			
			}
		}	

		// Add this frequency count to the histogram of minimum centroids
		histCount[index] += 1;

	}

	return histCount;
}


int main(int argc, char* argv[])

{

	//--------------------------------------------- SIFT  Keypoints DETECTION -------------------------------------------//

	// Read image files
	Mat Dog_1Img = imread("Dog_1.png");
	Mat Dog_2Img = imread("Dog_2.png");
	Mat Dog_3Img = imread("Dog_3.png");
	Mat CatImg = imread("Cat.png");

	// Convert to Grayscale
	Mat grayDog_1;
	Mat grayDog_2;
	Mat grayDog_3;
	Mat grayCat;

	cvtColor(Dog_1Img, grayDog_1, COLOR_BGR2GRAY);
	cvtColor(Dog_2Img, grayDog_2, COLOR_BGR2GRAY);
	cvtColor(Dog_3Img, grayDog_3, COLOR_BGR2GRAY);
	cvtColor(CatImg, grayCat, COLOR_BGR2GRAY);

	if (grayDog_1.empty() || grayDog_2.empty() || grayDog_3.empty() || grayCat.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		return -1;
	}

	// ------------------ Get the individual SIFT features/keypoints ---------------------------//
	Mat Dog1, Dog2, Dog3, Cat;
	int minHessian = 400;
	Dog1 = detectSIFTKeyPoints(grayDog_1, minHessian);
	Dog2 = detectSIFTKeyPoints(grayDog_2, minHessian);
	Dog3 = detectSIFTKeyPoints(grayDog_3, minHessian);
	Cat = detectSIFTKeyPoints(grayCat, minHessian);


	//------------------------Step 2-------------------------------------------------//
	//         Detect the keypoints using SIFT Detector, compute the descriptors

    // initialize the SURF detector
	Ptr<SIFT> detector = SIFT::create(minHessian);

	// create keypoints for each image //
	std::vector<cv::KeyPoint> Dog_1keypoints, Dog_2keypoints, Dog_3keypoints, Cat_keypoints;  
	Mat descriptorsDog1, descriptorsDog2, descriptorsDog3, descriptorsCat;

	detector->detectAndCompute(grayDog_1, Mat(), Dog_1keypoints, descriptorsDog1);
	detector->detectAndCompute(grayDog_2, Mat(), Dog_2keypoints, descriptorsDog2);
	detector->detectAndCompute(grayDog_3, Mat(), Dog_3keypoints, descriptorsDog3);
	detector->detectAndCompute(grayCat, Mat(), Cat_keypoints, descriptorsCat);

	//------------------------Step 3------------------------------------------------------//
	//              Matching descriptor vectors with a FLANN based matcher                //
	

	Ptr<DescriptorMatcher> matcherDog1Dog2 = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	Ptr<DescriptorMatcher> matcherDog1Dog3 = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	Ptr<DescriptorMatcher> matcherDog3Dog2 = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	Ptr<DescriptorMatcher> matcherDog1Cat = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	Ptr<DescriptorMatcher> matcherDog3Cat = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);

	vector< vector<DMatch> > knn_matches1;  // matches between Dog1 and Dog2
	vector< vector<DMatch> > knn_matches2;  // mathes between Dog1 and Dog3
	vector< vector<DMatch> > knn_matches3;  // mathes between Dog1 and Cat
	vector< vector<DMatch> > knn_matches4;  // mathes between Dog3 and Dog2
	vector< vector<DMatch> > knn_matches5;  // mathes between Dog3 and Cat


	// Order matters here! 
	matcherDog1Dog2->knnMatch(descriptorsDog1, descriptorsDog2, knn_matches1, 2);
	matcherDog1Dog3->knnMatch(descriptorsDog1, descriptorsDog3, knn_matches2, 2);
	matcherDog1Cat->knnMatch(descriptorsDog1, descriptorsCat, knn_matches3, 2);
	matcherDog3Dog2->knnMatch(descriptorsDog3, descriptorsDog2, knn_matches4, 2);
	matcherDog3Cat->knnMatch(descriptorsDog3, descriptorsCat, knn_matches5, 2);


	//------------------------Step 4------------------------------------------------------//
	//                  Filter matches using the Lowe's ratio test                        //
	const float ratio_thresh = 0.7f;

	vector<DMatch> good_matches1;
	vector<DMatch> good_matches2;
	vector<DMatch> good_matches3;
	vector<DMatch> good_matches4;
	vector<DMatch> good_matches5;

	// matches between Dog1 and Dog2
	for (size_t i = 0; i < knn_matches1.size(); i++) // 326 KNN matches!
	{
		if (knn_matches1[i][0].distance < ratio_thresh * knn_matches1[i][1].distance)
		{
			good_matches1.push_back(knn_matches1[i][0]);  // good matches are saved here!
		}
	}

	// mathes between Dog1 and Dog3
	for (size_t i = 0; i < knn_matches2.size(); i++)  // 245 KNN matches!
	{
		if (knn_matches2[i][0].distance < ratio_thresh * knn_matches2[i][1].distance)
		{
			good_matches2.push_back(knn_matches2[i][0]);  // good matches are saved here!
		}
	}

	// mathes between Dog1 and Cat
	for (size_t i = 0; i < knn_matches3.size(); i++)  
	{
		if (knn_matches3[i][0].distance < ratio_thresh * knn_matches3[i][1].distance)
		{
			good_matches3.push_back(knn_matches3[i][0]);  // good matches are saved here!
		}
	}

	// mathes between Dog3 and Dog2
	for (size_t i = 0; i < knn_matches4.size(); i++)  // 245 KNN matches!
	{
		if (knn_matches4[i][0].distance < ratio_thresh * knn_matches4[i][1].distance)
		{
			good_matches4.push_back(knn_matches4[i][0]);  // good matches are saved here!
		}
	}

	//  mathes between Dog3 and Cat
	for (size_t i = 0; i < knn_matches5.size(); i++)  
	{
		if (knn_matches5[i][0].distance < ratio_thresh * knn_matches5[i][1].distance)
		{
			good_matches5.push_back(knn_matches5[i][0]);  // good matches are saved here!
		}
	}


	//----Draw Matches---------//
	Mat img_matches1;
	Mat img_matches2;
	Mat img_matches3;
	Mat img_matches4;
	Mat img_matches5;

	// Draw matches between Dog1 and Dog2
	drawMatches(Dog_1Img, Dog_1keypoints, Dog_2Img, Dog_2keypoints, good_matches1, img_matches1, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	// Draw matches between Dog1 and Dog3
	drawMatches(Dog_1Img, Dog_1keypoints, Dog_3Img, Dog_3keypoints, good_matches2, img_matches2, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	// Draw matches between Dog1 and Cat
	drawMatches(Dog_1Img, Dog_1keypoints, CatImg, Cat_keypoints, good_matches3, img_matches3, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	// Draw matches between Dog3 and Dog2
	drawMatches(Dog_3Img, Dog_3keypoints, Dog_2Img, Dog_2keypoints, good_matches4, img_matches4, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	// Draw matches between Dog3 and Cat
	drawMatches(Dog_3Img, Dog_3keypoints, CatImg, Cat_keypoints, good_matches5, img_matches5, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	

//-------------------------------BAG OF WORDS CODE HERE------------------------------------------//
	// find cluster centroids based on Dog1, Dog2 and Cat3
	BOWKMeansTrainer BGOWORDS(8);   // Create codebook for SIFT features of 8 bins
	BGOWORDS.add(descriptorsDog1);  // for dog 1
	BGOWORDS.add(descriptorsDog2);  // for dog 2
	BGOWORDS.add(descriptorsCat);   // for cat
	
    // get the centroid of the cluster of the codebook
	Mat codebook = BGOWORDS.cluster();
	cout << "-----------------" << endl;
	cout << "Codebook Number Of Rows: " << codebook.rows << endl;
	cout << "Codebook Number Of Cols: " << codebook.cols << endl;

	// create an array to save the histogram count of the features
	int* histCount1 = new int[codebook.rows];
	int* histCount2 = new int[codebook.rows];
	int* histCount3 = new int[codebook.rows];
	int* histCount4 = new int[codebook.rows];

	// get the histogram values from the cluster
	histCount1 = getCodebookhHistogram(descriptorsDog1, codebook);
	histCount2 = getCodebookhHistogram(descriptorsDog2, codebook);
	histCount3 = getCodebookhHistogram(descriptorsDog3, codebook);
	histCount4 = getCodebookhHistogram(descriptorsCat, codebook);

	// write histogram values to a txt file for viewing
	WriteHistToText("histCountDog1Dog3.txt", histCount1, 8);
	WriteHistToText("histCountDog2Dog3.txt", histCount2, 8);
	WriteHistToText("histCountDog3Dog3.txt", histCount3, 8);
	WriteHistToText("histCountCatDog3.txt", histCount4, 8);

	// DO THIS FOR REPORTING!
	// find how the histogram compares to with Dog3 feature codebooK
	double error1 = FindError(histCount1, histCount3);
	double error2 = FindError(histCount2, histCount3);
	double error3 = FindError(histCount4, histCount3);

	// print out
	cout << "Error for Dog1 to Dog3 : " << error1 << endl;
	cout << "Error for Dog2 to Dog3 : " << error2 << endl;
	cout << "Error for Dog3 to Cat : " << error3 << endl;

	// console output for the above
	// Codebook No Of Rows: 8
	// Codebook No Of Cols : 128
	//	15.25
	//	14.25
	//	21.5

//--------------------ends here------------------------------------------------//

	String windowNameDg12 = "Image Matching between Dog 1 and Dog 2";  //Name of the window
	String windowNameDg13 = "Image Matching between Dog 1 and Dog 3";  //Name of the window
	String windowNameDg1C = "Image Matching between Dog 1 and Cat";    //Name of the window
	String windowNameDg32 = "Image Matching between Dog 3 and Dog 2";  //Name of the window
	String windowNameDg3C = "Image Matching between Dog 3 and Cat";    //Name of the window

	String windowNameDg1 = "Image Keypoints for Dog1";    // Name of the window
	String windowNameDg2 = "Image Keypoints for Dog 2";   // Name of the window
	String windowNameDg3 = "Image Keypoints for Dog3";    // Name of the window
	String windowNameC = "Image Keypoints for Cat";       // Name of the window

	namedWindow(windowNameDg12); // Create a window
	namedWindow(windowNameDg13); // Create a window
	namedWindow(windowNameDg1C); // Create a window
	namedWindow(windowNameDg32); // Create a window
	namedWindow(windowNameDg3C); // Create a window

	// window name for each image keypoints
	namedWindow(windowNameDg1); // Create a window
	namedWindow(windowNameDg2); // Create a window
	namedWindow(windowNameDg3); // Create a window
	namedWindow(windowNameC);   // Create a window

	//-- Show detected matches in keypoints
	imshow(windowNameDg12, img_matches1);
	imshow(windowNameDg13, img_matches2);
	imshow(windowNameDg1C, img_matches3);
	imshow(windowNameDg32, img_matches4);
	imshow(windowNameDg3C, img_matches5);
	

	// show each image keypoints
	imshow(windowNameDg1, Dog1);
	imshow(windowNameDg2, Dog2);
	imshow(windowNameDg3, Dog3);
	imshow(windowNameC, Cat);
	waitKey(0);


	// Destroy windows
	destroyWindow(windowNameDg12); //destroy the created window
	destroyWindow(windowNameDg13); //destroy the created window
	destroyWindow(windowNameDg1C); //destroy the created window
	destroyWindow(windowNameDg32); //destroy the created window
	destroyWindow(windowNameDg3C); //destroy the created window

	destroyWindow(windowNameDg1); //destroy the created window
	destroyWindow(windowNameDg2); //destroy the created window
	destroyWindow(windowNameDg3); //destroy the created window
	destroyWindow(windowNameC);   //destroy the created window

	return 0;
}
