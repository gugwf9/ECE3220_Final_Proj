//this is gonna be rough.. lets begin

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class parse{
	private:
		string fname;
		string buffer;
		string dlim1;
		string dlim2;
		string dlim3;
		fstream inFile;
		fstream outFile;

	public:
		//single constructor
		parse();
		//destructor
		~parse();
		//finds start of info we need
		void findStart();
		//handles/skips classes taken before mizzou
		void mizStart();
		void storeSemester();
		
};

parse::parse(){
	cout << "Enter file to parse: ";
	cin >> fname;
	buffer="";
	dlim1="FALL";
	dlim2="SPNG";
	dlim3="Local Campus Credits";
	
	inFile.open(fname.c_str(), ios::in);
	if(inFile==NULL) perror("Error opening file");
	
	outFile.open("taken.txt", ios::in | ios::out);
	if(outFile==NULL) perror("Error opening file");
	mizStart();	
}

parse::~parse(){
	inFile.close();
	outFile.close();
	cout << "Destructing parse object." << endl << endl;
}

void parse::findStart(){
	while(getline(inFile, buffer)){
		if (buffer.find(dlim1) != string::npos || buffer.find(dlim2) != string::npos) {
			break;		
		}
	}
}

void parse::mizStart(){
	while(getline(inFile, buffer)){
		if (buffer.find(dlim3) != string::npos){
			outFile << buffer << endl;
			break;			
		}	
	}
}

void parse::storeSemester(){
	while(getline(inFile, buffer)){
		//check for classes, write them to file by semester
		
		if (buffer.find(dlim3) != string::npos) break;
	}
}

//for future reference, READ first courses taken, assume all pre reqs have already been taken

int main(void){
	parse obj1;
	
	return 0;
}
