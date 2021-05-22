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

1.  Upload prob3b.cpp and fish.raw,to the IDE 

To Run Code prob1a:
	 1. type in clang++-7 -pthread -std=c++17 -o prob3b prob3b.cpp
	 2. then ./prob1a <input_raw> <output_raw> 
	 e.g ./prob3b fish.raw fishMBVQ.raw
	 all relevant output files are atomatically stored in the current folder
 	
4. create a new folder in the directory and move all the output files into that folder
5. then download the whole directory as a zip. 
6. Navigate to the folder the output .raw files are stored

To view output on ImageJ:
	1. import raw output file
	2. select image type 24-bit RGB
	3a. For elephant.raw, set width to 640, height to 426
        3b. For ski_perseon.raw, set width to 321, height to 481
 	4. OK.
