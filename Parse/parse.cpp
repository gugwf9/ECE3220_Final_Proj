//this is gonna be rough.. lets begin

#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <stdlib.h>

using namespace std;

class parse{
	private:
		int check;
		int mizSemCount;
		int countSemCr;
		int sem;
		int allCr;
		string buffer;
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
		void numSem();
		bool find(const char*, const char*);		
		
};

parse::parse(){
	cout << "Running..." << endl;
	//initialize values
	string fname="transc.txt";	
//	cout << "Enter file to parse: ";
//	cin >> fname;
	check=0;
	mizSemCount=0;
	sem=0;
	countSemCr=0;
	allCr=0;
	buffer="";
	dlim3="Local Campus Credits";
	
	//open files
	inFile.open(fname.c_str(), ios::in);
	if(inFile==NULL) perror("Error opening file");
	
	outFile.open("taken.txt", ios::in | ios::out);
	if(outFile==NULL) perror("Error opening file");
	
	//count number of semesters in transcript
	numSem();
	
	//skip ahead to first actual semester
	mizStart();
	
	//call store while less that num of semesters
	for (int i=0; i<mizSemCount; i++) storeSemester();
	
	//print to file total credits after finished
	outFile << endl << "Total Mizzou credits considered: " << allCr << endl;	
}

parse::~parse(){
	//simple, close files and destruct
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
	//clear fstream, verify it points to begining of file
	inFile.clear();
	inFile.seekg(0);
	
	//loop through until find dlim3, which will be start of first semester	
	while(getline(inFile, buffer)){
		if (buffer.find(dlim3) != string::npos){
			outFile << "-----------------------------------------------" << endl;
			break;			
		}	
	}
}

void parse::numSem(){
	//loops, if sees dlim3, increments mizSemCount
	while(getline(inFile, buffer)) if (buffer.find(dlim3) != string::npos) ++mizSemCount;	
}

void parse::storeSemester(){
	outFile << buffer << endl;
	//inc sem every time a semester is stored
	++sem;
	int totCr=0;
	char c;
	//loop through
	while(getline(inFile, buffer)){
		//if find the next dlim3, break out of while
		if (buffer.find(dlim3) != string::npos) break;
		
		//if find "GPA" and not looking at last or second to last semester
		//credits are stored four lines ahead of this, getlinex4 and store.
		//keep credits correct
		if (buffer.find("GPA") != string::npos && sem!=mizSemCount && sem!=(mizSemCount-1)){
			for(int i=0; i<4; i++) getline(inFile, buffer);
			totCr=atoi(buffer.c_str());
			if(totCr>20) totCr-=allCr;
		}
		
		//if second to last semester and find "ip "
		//loop through buffer and if there is a digit, store it in totCr
		if(sem==(mizSemCount-1) && buffer.find("IP ") != string::npos){
			for(int i=0; i<buffer.length(); i++) {
			    c=buffer[i];
			    if(isdigit(c)){
			    		totCr+=atoi(&c);
			    }
			}			
		}
		//THIS IS THE ISSUE
		//I tried both the built in "string.find()" as well as my own written find, neither working as I expect
//		if(buffer.find("PR") != string::npos && sem==mizSemCount){
		bool tf=false;
		if(sem==mizSemCount) tf=find(buffer.c_str(), "PR");
		if(tf==true){
//			cout << "Semester: " << sem << endl;
			cout << endl << "DEBUG found PR in string: " << buffer << endl << endl;
			//if found, loop through string and if digit, add up credits
			for(int i=0; i<buffer.length(); i++){
			    c=buffer[i];
			    if(isdigit(c)) totCr+=c-'0';
			}			
		}
		//call check against					
		checkAgainst();
		//check agains sets check to 1 if value matches, and print
		if (check==1) outFile << buffer << endl << endl;
	}
	
	//this is for geneds
	//As i do not have information to store every single mizzou course, I assume anything that 
	//doesnt match my specified courses will be a gened
	if (countSemCr!=totCr){
		int genEds=(totCr-countSemCr)/3;
		for(int i=0; i<genEds; i++){
			outFile << "GenEd Course\n3" << endl << endl;
		}
	}
	
	//print out semester info
	allCr+=totCr;	
	outFile << "Semester " << sem << endl << "Total Semester Credits: " << totCr << endl;
	outFile << "-----------------------------------------------" << endl;	
	countSemCr=0;	
}

//simply checks for specific classes
void parse::checkAgainst(){
	if(buffer=="ECE 1210"){
		buffer="ECE 1210\nIntro to Logic Systems\n3";
		check=1;
		countSemCr+=3;
	}
	else if(buffer=="ECE 2100"){
		buffer="ECE 2100\nCircuit Theory 1\n4";
		check=1;
		countSemCr+=4;			
	}
	else if(buffer=="ECE 3110"){
		buffer="ECE 3110\nProjects Lab\n3";
		check=1;	
		countSemCr+=3;	
	}		
	else if(buffer=="ECE 3210"){
		buffer="ECE 3210\nMicroprocessor Engineering\n4";
		check=1;
		countSemCr+=4;				
	}
	else if(buffer=="ECE 3220"){
		buffer="ECE 3220\nSoftware Design\n3";
		check=1;
		countSemCr+=3;			
	}
	else if(buffer=="ECE 3410"){
		buffer="ECE 3410\nElec Circuits & Signls 1\n4";
		check=1;
		countSemCr+=4;			
	}
	else if(buffer=="Circuit Theory 2"){
		buffer="ECE 3810\nCircuit Theory 2\n4";
		check=1;
		countSemCr+=4;			
	}
	else if(buffer=="ECE 3830"){
		buffer="ECE 3830\nSignals and Linear Systems\n3";
		check=1;
		countSemCr+=4;			
	}
	else if(buffer=="ECE 4220"){
		buffer="ECE 4220\nRt Embedded Computing\n3";
		check=1;
		countSemCr+=3;			
	}
	else if(buffer=="ECE 4250"){
		buffer="ECE 4250\nVhdl & Prg Logic Device\n4";
		check=1;
		countSemCr+=4;		
	}
	else if(buffer=="ECE 4270"){
		buffer="ECE 4270\nMicrocomputer Architecture and Interfacing\n4";
		check=1;
		countSemCr+=4;			
	}
	else if(buffer=="ECE 4970"){
		buffer="ECE 4970\nCapstone Design\n3";
		check=1;
		countSemCr+=3;		
	}
	else if(buffer=="Cmp Sc 1050"){
		buffer="Cmp Sc 1050\nAlgorithm Design/Prog 1\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Cmp Sc 2050"){
		buffer="Cmp Sc 2050\nAlgrthm Dsgn & Program 2\n3";
		check=1;
		countSemCr+=3;	
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
		int credits=atoi(&c);
		countSemCr+=credits;	
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
		int credits=atoi(&c);		
		countSemCr+=credits;	
	}	
	else if(buffer=="Physcs 2750"){
		buffer="Physcs 2750\nUniversity Physics 1\n5";
		check=1;
		countSemCr+=5;	
	}
	else if(buffer=="Physcs 2760"){
		buffer="Physcs 2760\nUniversity Physics 2\n5";
		check=1;
		countSemCr+=5;	
	}
	else if(buffer=="Calculus 1"){
		buffer="Math 1500\nCalculus 1\n5";
		check=1;
		countSemCr+=5;	
	}
	else if(buffer=="Calculus 2"){
		buffer="Math 1700\nCalculus 2\n5";
		check=1;
		countSemCr+=5;	
	}
	else if(buffer=="Calculus 3"){
		buffer="Math 2300\nCalculus 3\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Differential Equations"){
		buffer="Math 4100\nDifferential Equations\n3";
		check=1;
		countSemCr+=3;	
	}	
	else if(buffer=="Intro to Math Statistics"){
		buffer="Stat 4710\nIntro to Math Statistics\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="2320"){
		buffer="Math 2320\nDiscrete Mathematical Structures\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="College Chemistry 1"){
		buffer="Chem 1320\nCollege Chemistry 1\n4";
		check=1;
		countSemCr+=4;	
	}
	else if(buffer=="Englsh 1000"){
		buffer="Englsh 1000\nExposition and Argumentation\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Enginr 1000"){
		buffer="Enginr 1000\nIntro to Engineering\n1";
		check=1;
		countSemCr+=1;	
	}
	else if(buffer=="Enginr 1200"){
		buffer="Enginr 1200\nStats & Elem Strngh Matr\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Enginr 2300"){
		buffer="Enginr 2300\nThermodynamics\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="IMSE 2710"){
		buffer="IMSE 2710\nEconomic Analysis\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Hist 1100"){
		buffer="Hist 1100\nSurvey of American History to 1865\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Hist 1200"){
		buffer="Hist 1200\nSurvey of American History Since 1865\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Hist 1400"){
		buffer="Hist 1400\nAmerican History\n5";
		check=1;
		countSemCr+=5;	
	}
	else if(buffer=="Hist 2210"){
		buffer="Hist 2210\nTwentieth Century America\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Hist 2440"){
		buffer="Hist 2440\nHistory of Missouri\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Hist 4000"){
		buffer="Hist 4000\nAge of Jefferson\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Hist 4220"){
		buffer="Hist 4220\nU.S. Society Between the Wars 1918-1945\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Hist 4230"){
		buffer="Hist 4230\nOur Times: United States Since 1945\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Pol Sc 2100"){
		buffer="Pol Sc 2100\nState Government\n3";
		check=1;
		countSemCr+=3;	
	}
	else if(buffer=="Pol Sc 1100"){
		buffer="Pol Sc 1100\nAmerican Government\n3";
		check=1;
		countSemCr+=3;	
	}																																								
	else check=0;
}

//my find function
bool parse::find(const char *one, const char *two){
	int size=(sizeof(two)/sizeof(two[0])), j, temp;
	//for some reason, the above sizeof(array)/sizeof(char) isnt working as I thought it would, so hardcode size
	size=2;
//	cout << endl << "DEBUG size: " << size << endl;
	//loop through until char* one hits null character 
	for(int i=0; one[i]!='\0'; i++){
		//if you find letter that is the same as char* two first letter
		if(one[i]==two[0]){
			//put i in temp, in case of situations like "ababc" when looking for "abc"
			temp=i;
			j=0; // reset j to 0
			while(j<size){
				//if characters do not match, set i back to temp+1 character and loop again
				if(one[i]!=two[j]) {i=++temp; break;}
				//if you loop through all of char* two and everything matches, string contains value. return true
				if(j+1==size) return true;
				++j;
				++i;
			}
				
		}
	}
	return false;
}

int main(void){
	parse obj1;
	return 0;
}
