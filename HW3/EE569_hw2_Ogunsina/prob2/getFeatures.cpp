//  EE569 Homework Assignment #Prob 2b 
//  Date:   03/11/2021
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


int main(int argc, char* argv[])

{

	//--------------------------------------------- SURF  FEATURE DETECTION -------------------------------------------//

	// Read image files
	Mat imgL = imread("left.png");
	Mat imgM = imread("middle.png");
	Mat imgR = imread("right.png");

	// Convert to Grayscale
	Mat grayImageL;
	Mat grayImageM;
	Mat grayImageR;

	cvtColor(imgL, grayImageL, COLOR_BGR2GRAY);
	cvtColor(imgM, grayImageM, COLOR_BGR2GRAY);
	cvtColor(imgR, grayImageR, COLOR_BGR2GRAY);

	if (grayImageL.empty() || grayImageM.empty() || grayImageR.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		return -1;
	}

	//------------------------Step 2-------------------------------------------------//
	//         Detect the keypoints using SURF Detector, compute the descriptors

	int minHessian = 400;

	Ptr<SURF> detector = SURF::create(minHessian);                       // initialize the SURF detector
	vector<KeyPoint> keypointsLeft, keypointsMiddle, keypointsRight;     // create keypoints for each image to stitch
	Mat descriptorsLeft, descriptorsMiddle, descriptorsRight;
	detector->detectAndCompute(grayImageL, noArray(), keypointsLeft, descriptorsLeft);
	detector->detectAndCompute(grayImageM, noArray(), keypointsMiddle, descriptorsMiddle);
	detector->detectAndCompute(grayImageR, noArray(), keypointsRight, descriptorsRight);


	//------------------------Step 3------------------------------------------------------//
	//              Matching descriptor vectors with a FLANN based matcher                //
	//              Since SURF is a floating-point descriptor NORM_L2 is used             //

	Ptr<DescriptorMatcher> matcherLeftMid = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	Ptr<DescriptorMatcher> matcherRightMid = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);

	vector< vector<DMatch> > knn_matches1;  // matches between left and middle
	vector< vector<DMatch> > knn_matches2;  // mathes between middle and right

	// Order matters here! 
	matcherLeftMid->knnMatch(descriptorsLeft, descriptorsMiddle, knn_matches1, 2);
	matcherRightMid->knnMatch(descriptorsMiddle, descriptorsRight, knn_matches2, 2);


	//------------------------Step 4------------------------------------------------------//
	//                  Filter matches using the Lowe's ratio test                        //
	const float ratio_thresh = 0.7f;
	vector<DMatch> good_matches1;
	vector<DMatch> good_matches2;

	// for left and middle
	for (size_t i = 0; i < knn_matches1.size(); i++) // 326 KNN matches!
	{
		if (knn_matches1[i][0].distance < ratio_thresh * knn_matches1[i][1].distance)
		{
			good_matches1.push_back(knn_matches1[i][0]);  // good matches are saved here!, 74 good matches
		}
	}

	// for right and middle
	for (size_t i = 0; i < knn_matches2.size(); i++)  // 245 KNN matches!
	{
		if (knn_matches2[i][0].distance < ratio_thresh * knn_matches2[i][1].distance)
		{
			good_matches2.push_back(knn_matches2[i][0]);  // good matches are saved here!, 25 good matches
		}
	}


	//----------------------------- Optional Step: for Reporting------------------------------//
	//                                Draw Matches                                            //
	Mat img_matches1;
	Mat img_matches2;

	// Order must match the knnMatch
	drawMatches(imgL, keypointsLeft, imgM, keypointsMiddle, good_matches1, img_matches1, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	drawMatches(imgM, keypointsMiddle, imgR, keypointsRight, good_matches2, img_matches2, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	// Four corners of the book in destination image.
	vector<Point2f> obj;
	vector<Point2f> scene;

	for (size_t i = 0; i < good_matches1.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypointsMiddle[good_matches1[i].queryIdx] .pt);
		scene.push_back(keypointsLeft[good_matches1[i].trainIdx].pt);
	}

	// Calculate Homography
	
	//Mat H = findHomography(obj, scene, RANSAC);
	//print(H);

	// Output image
	//Mat im_out;
	//dst = cv2.warpPerspective(img1, M1, (1000 + img1.shape[1] + img2.shape[1], img1.shape[0] + 800))
	//Size m(imgM.size().height , 1500);

	// Warp source image to destination based on homography
	//warpPerspective(imgL, imgM, H, imgM.size());

	//imshow("Warped Image", im_out);
	//imshow("left image",imgL);
	//imshow("Middle image", imgM);
	//waitKey(0);
	vector<Point2f> pts_src;
	vector<Point2f> pts_dst;

	pts_src.push_back(Point2f(310.25, 307.75));
	pts_src.push_back(Point2f(293.75, 344.75));
	pts_src.push_back(Point2f(223.75, 358.75));
	pts_src.push_back(Point2f(236.75, 301.25));

	pts_dst.push_back(Point2f(961.25, 598.25));
	pts_dst.push_back(Point2f(949.25, 632.75));
	pts_dst.push_back(Point2f(886.75, 665.75));
	pts_dst.push_back(Point2f(898.25, 604.25));

	Mat H = findHomography(pts_src, pts_dst, RANSAC);
	print(H);

	//imshow("Warped Image", im_out);
	//imshow("left image", imgL);
	//imshow("Middle image", imgM);
	//waitKey(0);
	
	
	//cout << H << endl;

	//String windowNameLM = "Good Matches for Left and Middle Image"; //Name of the window
	//String windowNameRM = "Good Matches for Middle and Right Image"; //Name of the window

	//namedWindow(windowNameLM); // Create a window
	//namedWindow(windowNameRM); // Create a window
    
	//-- Show detected matches
	//imshow(windowNameLM, img_matches1);
	//imshow(windowNameRM, img_matches2);

	//imshow("Warped", im_out);
	//waitKey(0); // Wait for any keystroke in the window

	//destroyWindow(windowNameLM); //destroy the created window
	//destroyWindow(windowNameRM); //destroy the created window

	return 0;
}
