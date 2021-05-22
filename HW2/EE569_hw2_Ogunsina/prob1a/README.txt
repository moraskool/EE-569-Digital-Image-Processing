#  EE569 Homework Assignment 2 #Prob1a
#  Name:   Morayo Abisola Ogunsina
#  ID:     7371213793
#  Date:   02/23/2021
#  email:  ogunsina@usc.edu

For some reason, My version of Visual Studio does not give me an output, 
and after much struggle, I decided to use an online c++ code editor and IDE called repl.it
to use, go to https://repl.it/languages/cpp

IDE - repl.it
Compiler- clang++-7 , std=c++17
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

1.  Upload prob1a.cpp, elephant.raw, ski_person.raw to the IDE 

/*
   First, default width and height is set to 481 and 321 for elephant.raw
   to run code for ski_person.raw, uncomment lines 69 - 67
   and comment lines 65 - 66
*/

To Run Code prob1a:
 	 1. Use terminal and navigate to code path = /prob1a
	 2. type in clang++-7 -pthread -std=c++17 -o prob1a prob1a.cpp
	 3. then ./prob1a <input_raw> <color mode> <threshold> // set color mode to 3
	 e.g ./prob1a elephant.raw 3 0.90
	 all relevant output files are atomatically stored in the current folder
 	
4. create a new folder in the directory and move all the output files into that folder
5. then download the whole directory as a zip. 
6. Navigate to the folder the output .raw files are stored

To view output on ImageJ:
	1. import raw output file
	2. select image type 8-bit RGB
	3a. For elephant.raw, set width to 481, height to 321
        3b. For ski_perseon.raw, set width to 321, height to 481
 	4. OK.
