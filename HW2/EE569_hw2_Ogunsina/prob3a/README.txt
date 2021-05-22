#  EE569 Homework Assignment Prob 3a : Separable Error Diff for Color Images
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
 	   1. Use terminal and navigate to code path = /prob3a
           2. type in g++ -o prob3a code3a.cpp
           3. then prob3a.exe <input_raw> <image width> <image height> <color mode> 
           e.g prob3a.exe fish.raw 640 426 3
           4.  Run and execute the code and the output images are generated and stored in the same folder.

To view output on ImageJ:
	1. import raw output file
	2. select image type 8-bit RGB
	3. set width to 640, height to 426
 	4. OK.
