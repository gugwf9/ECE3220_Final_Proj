//this is gonna be rough.. lets begin

//current parse works with:
//	all ece courses
//	all cmp sc courses
//	all other required courses except gen eds
//I need gen eds, but I am not sure how to handle them, what to look for to parse, etc

#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <stdlib.h>

using namespace std;

class parse{
	private:
		int check;
		string fname;
		string buffer;
//		string dlim1;
//		string dlim2;
		string dlim3;
		fstream inFile;
		fstream outFile;

	public:
		//single constructor
		parse();
		//destructor
		~parse();
		//finds start of info we need
//		void findStart();
		//handles/skips classes taken before mizzou
		void mizStart();
		void storeSemester();
		void checkAgainst();
		
};

parse::parse(){
	cout << "Enter file to parse: ";
	cin >> fname;
	check=0;
	buffer="";
//	dlim1="FALL";
//	dlim2="SPNG";
	dlim3="Local Campus Credits";
	
	//open files
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

//void parse::findStart(){
//	while(getline(inFile, buffer)){
//		if (buffer.find(dlim1) != string::npos || buffer.find(dlim2) != string::npos) {
//			break;		
//		}
//	}
//}

void parse::mizStart(){
	while(getline(inFile, buffer)){
		if (buffer.find(dlim3) != string::npos){
			outFile << "-----------------------------------------------" << endl;
			break;			
		}	
	}
}

void parse::storeSemester(){
	outFile << buffer << endl;
	while(getline(inFile, buffer)){
		if (buffer.find(dlim3) != string::npos){
			break;
		}
		checkAgainst();
		if(check==1) outFile << buffer << endl << endl;
	}
	outFile << "-----------------------------------------------" << endl;	
}

void parse::checkAgainst(){
	if(buffer=="ECE 1210"){
		buffer="ECE 1210\nIntro to Logic Systems\n3";
		check=1;
	}
	else if(buffer=="ECE 2100"){
		buffer="ECE 2100\nCircuit Theory 1\n4";
		check=1;	
	}
	else if(buffer=="ECE 3110"){
		buffer="ECE 3110\nProjects Lab\n3";
		check=1;	
	}		
	else if(buffer=="ECE 3210"){
		buffer="ECE 3210\nMicroprocessor Engineering\n4";
		check=1;		
	}
	else if(buffer=="ECE 3220"){
		buffer="ECE 3220\nSoftware Design\n3";
		check=1;		
	}
	else if(buffer=="ECE 3410"){
		buffer="ECE 3410\nElec Circuits & Signls 1\n4";
		check=1;		
	}
	else if(buffer=="ECE 3810"){
		buffer="ECE 3810\nCircuit Theory 2\n4";
		check=1;		
	}
	else if(buffer=="ECE 3830"){
		buffer="ECE 3830\nSignals and Linear Systems\n3";
		check=1;		
	}
	else if(buffer=="ECE 4220"){
		buffer="ECE 4220\nRt Embedded Computing\n3";
		check=1;		
	}
	else if(buffer=="ECE 4250"){
		buffer="ECE 4250\nVhdl & Prg Logic Device\n4";
		check=1;		
	}
	else if(buffer=="ECE 4270"){
		buffer="ECE 4270\nMicrocomputer Architecture and Interfacing\n4";
		check=1;		
	}
	else if(buffer=="ECE 4970"){
		buffer="ECE 4970\nCapstone Design\n3";
		check=1;		
	}
	//ece extra courses taken
	else if(buffer.find("ECE") != string::npos){
		string course=buffer;
		string creditsLine="";
		char c;
		getline(inFile, buffer);
		getline(inFile, buffer);
		getline(inFile, creditsLine);
		getline(inFile, creditsLine);
		outFile << course << endl << buffer << endl;				
//		won't work unless c++11
//		for(char& c : creditsLine){
//		    if(isdigit(c)) {credits=atoi(&c); break;}
//		}
		for(int i=0; i<creditsLine.length(); i++) {
		    c=creditsLine[i];
		    if(isdigit(c)) {outFile << c << endl << endl; break;}
		}		
		check=0;
	}
	//cs extra courses taken
	else if(buffer.find("Cmp Sc") != string::npos){
		string course=buffer;
		string creditsLine="";
		char c;
		getline(inFile, buffer);
		getline(inFile, buffer);
		getline(inFile, creditsLine);
		getline(inFile, creditsLine);		
		outFile << course << endl << buffer << endl;				
//		won't work unless c++11
//		for(char& c : creditsLine){
//		    if(isdigit(c)) {credits=atoi(&c); break;}
//		}
		for(int i=0; i<creditsLine.length(); i++) {
		    c=creditsLine[i];
		    if(isdigit(c)) {outFile << c << endl << endl; break;}
		}
		check=0;
	}	
	else if(buffer=="Cmp Sc 1050"){
		buffer="Cmp Sc 1050\nAlgorithm Design/Prog 1\n3";
		check=1;
	}
	else if(buffer=="Cmp Sc 2050"){
		buffer="Cmp Sc 2050\nAlgrthm Dsgn & Program 2\n3";
		check=1;
	}
	else if(buffer=="Physcs 2750"){
		buffer="Physcs 2750\nUniversity Physics 1\n5";
		check=1;
	}
	else if(buffer=="Physcs 2760"){
		buffer="Physcs 2760\nUniversity Physics 2\n5";
		check=1;
	}
	else if(buffer=="Calculus 1"){
		buffer="Math 1500\nCalculus 1\n5";
		check=1;
	}
	else if(buffer=="Calculus 2"){
		buffer="Math 1700\nCalculus 2\n5";
		check=1;
	}
	else if(buffer=="Calculus 3"){
		buffer="Math 2300\nCalculus 3\n3";
		check=1;
	}
	else if(buffer=="Differential Equations"){
		buffer="Math 4100\nDifferential Equations\n3";
		check=1;
	}	
	else if(buffer=="Intro to Math Statistics"){
		buffer="Stat 4710\nIntro to Math Statistics\n3";
		check=1;
	}
	else if(buffer=="2320"){
		buffer="Math 2320\nDiscrete Mathematical Structures\n3";
		check=1;
	}
	else if(buffer=="College Chemistry 1"){
		buffer="Chem 1320\nCollege Chemistry 1\n4";
		check=1;
	}
	else if(buffer=="Englsh 1000"){
		buffer="Englsh 1000\nExposition and Argumentation\n3";
		check=1;
	}
	else if(buffer=="Enginr 1000"){
		buffer="Enginr 1000\nIntro to Engineering\n1";
		check=1;
	}
	else if(buffer=="Enginr 1200"){
		buffer="Enginr 1200\nStats & Elem Strngh Matr\n3";
		check=1;
	}
	else if(buffer=="Enginr 2300"){
		buffer="Enginr 2300\nThermodynamics\n3";
		check=1;
	}
	else if(buffer=="IMSE 2710"){
		buffer="IMSE 2710\nEconomic Analysis\n3";
		check=1;
	}
	else if(buffer=="Hist 1100"){
		buffer="Hist 1100\nSurvey of American History to 1865\n3";
		check=1;
	}
	else if(buffer=="Hist 1200"){
		buffer="Hist 1200\nSurvey of American History Since 1865\n3";
		check=1;
	}
	else if(buffer=="Hist 1400"){
		buffer="Hist 1400\nAmerican History\n5";
		check=1;
	}
	else if(buffer=="Hist 2210"){
		buffer="Hist 2210\nTwentieth Century America\n3";
		check=1;
	}
	else if(buffer=="Hist 2440"){
		buffer="Hist 2440\nHistory of Missouri\n3";
		check=1;
	}
	else if(buffer=="Hist 4000"){
		buffer="Hist 4000\nAge of Jefferson\n3";
		check=1;
	}
	else if(buffer=="Hist 4220"){
		buffer="Hist 4220\nU.S. Society Between the Wars 1918-1945\n3";
		check=1;
	}
	else if(buffer=="Hist 4230"){
		buffer="Hist 4230\nOur Times: United States Since 1945\n3";
		check=1;
	}
	else if(buffer=="Pol Sc 2100"){
		buffer="Pol Sc 2100\nState Government\n3";
		check=1;
	}
	else if(buffer=="Pol Sc 1100"){
		buffer="Pol Sc 1100\nAmerican Government\n3";
		check=1;
	}																																								
//	......
//how to handle gen eds?
	else check=0;
}

//for future reference, READ first courses taken, assume all pre reqs have already been taken

int main(void){
	parse obj1;
	for (int i=0; i<7; i++) obj1.storeSemester();
	return 0;
}
