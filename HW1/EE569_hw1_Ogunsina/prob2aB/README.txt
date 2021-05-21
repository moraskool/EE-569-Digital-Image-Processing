#  EE569 Homework Assignment #Prob 2b 
#  Name:   Morayo Abisola Ogunsina
#  ID:     7371213793
#  Date:   02/07/2021
#  email:  ogunsina@usc.edu

OS Version - Windows 10 
IDE - Visual Studio Community 2019
Compiler- g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

1.  Open terminal.
2.  Navigate where the source code files are stored along with the input image files (.cpp and .raw files)

To Run Code :
 	1. Use terminal and navigate to code path = /prob2aB
 	2. type in g++ -o prob2aB code2a_gaussian.cpp
	3. then type in prob2aB.exe <noisy_input_raw> <original_grey_input_raw> <output_raw> < window size> <SigmaC>
 	4. e.g prob2aB.exe Fruit_grey_noisy.raw Fruits_grey.raw output.raw 3
 	
4.  Run and execute the code and the output images are generated and stored in the same folder.

To view output on ImageJ:
	1. import raw output file
	2. select image type 8-bit
	3. set width to 500, height to 400
 	4. OK.
