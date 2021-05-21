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
 	1. Use terminal and navigate to code path = /prob3
 	2. type in g++ -o prob2b code2b.cpp
 	3. then prob3.exe <colored_input_raw> <colored_output_quantized> <colored_output_oileffect> <image width> <image height> <color mode> <number of colors to quantize to> <N>
 	4. e.g prob3.exe Fruits.raw Fruits_quant.raw Fruits_oil.raw 64 7
 	
4.  Run and execute the code and the output images are generated and stored in the same folder.

To view output on ImageJ:
	1. import raw output file
	2. select image type 24-bit RGB
	3. set width to 500, height to 400
 	4. OK.
