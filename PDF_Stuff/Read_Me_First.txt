I was able to find some software that converted .pdf to .txt  plus some other things and with some time I was able to extract a single binary file to only convert .pdfs. I also created version of the code to run on linux system, windows and mac. I know for a fact windows and linux work, not sure about the mac versions though. I am not experienced with Mac, so I simply made these as .dmg files and I am not sure how it will actually act upon execution.

To call one of these, here is the syntax from a terminal/cmd:
Linux-
	./convert_linux64.out example.pdf
	this will take in example.pdf, rip contents and print it to a newly created example.txt file
	
Windows-
	convert_win64.exe example.pdf
	this will take in example.pdf, rip contents and print it to a newly created example.txt file
	
Mac-
	You'll have to let me know, I havent tried it yet.
	
I know how to call system commands from with c/c++ files, so I can code in calls to these files to make sure the correct version will be executed when the project is actually finished and running.

Included in this folder is also my unofficial transcript pdf, I have been using for testing. For parsing, I might need more examples, though, for best error handling. 		
