#  EE569 Homework Assignment #Prob 2a A : Fixed Thresholding
#  Name:   Morayo Abisola Ogunsina
#  ID:     7371213793
#  Date:   02/23/2021
#  email:  ogunsina@usc.edu

OS Version - Windows 10 
IDE - Visual Studio Community 2019
Compiler- g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

1.  Open terminal.
2.  Navigate where the source code files are stored along with the input image files (.cpp and .raw files)

To Run Code :
 	1. Use terminal and navigate to code path = /prob2aA
 	2. type in g++ -o prob2aA code2aA.cpp
 	3. then prob2aA.exe <input_raw> <ouput_raw> <image width> <image height> <threshold_val>
 	4. e.g prob2aA.exe bridge.raw bridgeFixed_64.raw 332 501 64
 	
4.  Run and execute the code and the output images are generated and stored in the same folder.

To view output on ImageJ:
	1. import raw output file
	2. select image type 8-bit RGB
	3. set width to 332, height to 501
 	4. OK.
