#  EE569 Homework Assignment 3 #Prob2 : Homographic Transformation and Image Stitching
#  Name:   Morayo Abisola Ogunsina
#  ID:     7371213793
#  Date:   03/12/2021
#  email:  ogunsina@usc.edu


OS Version - Windows 10 
IDE - Visual Studio Community 2019
Compiler- g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)


To Run Code for prob1:
 	  1. Use terminal and navigate to code path = /prob2
          2. type in g++ -o prob2 code2.cpp
          3. then prob3.exe <colored_input_left> <colored_input_middle> <colored_input_right>
          4. e.g prob2.exe left.raw middle.raw right.raw
	
To view output on ImageJ:
	1. import raw output file
	2. select image type 24-bit RGB
	3. set width to 2000, height to 1300
 	4. OK.

To Run the getVertex.cpp code:
        1. install opencv for c++
        2. place the left, middle and right images in the same folder
        2. Set the environment variables for the visual studio and run the program.
        4. The SURF matched features are drawn on the images and displayed on the window
       
To Use cpSelect:
        1. Open Matlab R2019 and run the getControlPoints.m
        2. Run the program and but place the images in same folder
        3. Select the control points and export to the workstation

To Calculate the Homography Matrix,
        1. Open Matlab R2019 and run the getControlPoints.m
        2. replace the x0, yo, xd and yd values as needed and run again
        3. h values can be viewed on the output pane
