//run all

#include <ctype.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class parse{
	private:
		int check;
		int countSemCr;
		int sem;
		int allCr;
		string buffer;
		string dlim3;
		fstream inFile;
		fstream outFile;

	public:
		//single constructor
		parse(string);
		//destructor
		~parse();
		//finds start of info we need
//		void findStart();
		//handles/skips classes taken before mizzou
		void mizStart();
		int mizSemCount;		
		void storeSemester();
		void checkAgainst();
		void numSem();
		bool find(const char*, const char*);		
		
};

parse::parse(string fname){
//	cout << "Running..." << endl << endl;// << "Note... I gave you the transccript file named transc.txt, you can use that here." << endl << endl;
	//initialize values
//	string fname="transc.txt";	
//	string fname;
//	cout << "Enter file to parse: ";
//	cin >> fname;
	check=0;
	mizSemCount=0;
	sem=0;
	countSemCr=0;
	allCr=0;
	buffer="";
	dlim3="Local Campus Credits";
	for(int i=0; i<3; i++) fname.erase(fname.size()-1);
	fname=fname+"txt";
	//open files
	inFile.open(fname.c_str(), ios::in);
	if(inFile==NULL) {perror("Error opening file"); exit(0);}
	
	outFile.open("taken.txt", ios::in | ios::out);
	if(outFile==NULL) {perror("Error opening file"); exit(0);}
	
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
	cout << "Destructing parse object..." << endl << endl;
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
	++sem;
	if(sem!=mizSemCount) outFile << buffer << endl;
	//inc sem every time a semester is stored
	int totCr=0;
	char c;
	//loop through
	while(getline(inFile, buffer)){
		//if find the next dlim3, break out of while
		if (buffer.find(dlim3) != string::npos) break;
		if(sem==mizSemCount) break;
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
//this issue has not been resolved. Some ghost characters in the text file are screwing up the output. For this iteration, I simply
//did not consider the courses for a semester you are enrolled in but have not taken. In this case, all classes I am enrolled in for 
//spring of 2017 semester will be ignored
		
		//THIS IS THE ISSUE
		//I tried both the built in "string.find()" as well as my own written find, neither working as I expect
//		if(buffer.find("PR") != string::npos && sem==mizSemCount){
//		bool tf=false;
//		if(sem==mizSemCount) tf=find(buffer.c_str(), "PR");
//		if(tf==true){
////			cout << "Semester: " << sem << endl;
//			cout << endl << "DEBUG found PR in string: " << buffer << endl << endl;
//			//if found, loop through string and if digit, add up credits
//			for(int i=0; i<buffer.length(); i++){
//			    c=buffer[i];
//			    if(isdigit(c)) totCr+=c-'0';
//			}			
//		}
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
	if(sem!=mizSemCount) outFile << "Semester " << sem << endl << "Total Semester Credits: " << totCr << endl;
	if(sem!=mizSemCount) outFile << "-----------------------------------------------" << endl;	
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

//compare class
class compare{
	protected:
		fstream tak;
		int countGen;
		int techElect;
		int ECE_4000;
		vector<string> ECEtak;
		vector<string> CStak;
		vector<string> Mathtak;	
		string MathReq[6];
		string ECEReq[12];
		string HisReq[10];
		string EngReq[3];
		string OtherReq[7];
		bool find(const char*, const char*);
		void fill();
		void findNeed();
		bool isElementOf(string, string[], int);
		void checkElect();
		vector<bool> checkMath();
		vector<bool> checkECE();
		vector<bool> checkOther();	
		bool checkHist();
		bool checkEng();
		bool ver(string);
	//	void verify();	
	
	public:
		//vector of strings, contains course info
		vector<string> file;		
		compare();
		~compare();
};

//simply initializes all array values
void compare::fill(){
	//need all
	MathReq[0]="Math 1500";
	MathReq[1]="Math 1700";
	MathReq[2]="Math 2320";
	MathReq[3]="Math 2300";
	MathReq[4]="Math 4100";
	MathReq[5]="Stat 4710";
	
	//need all
	ECEReq[0]="ECE 1210";
	ECEReq[1]="ECE 2100";
	ECEReq[2]="ECE 3210";
	ECEReq[3]="ECE 3220";
	ECEReq[4]="ECE 3410";
	ECEReq[5]="ECE 3810";
	ECEReq[6]="ECE 3830";
	ECEReq[7]="ECE 4220";
	ECEReq[8]="ECE 4250";
	ECEReq[9]="ECE 4270";
	ECEReq[10]="ECE 3110";
	ECEReq[11]="ECE 4970";
	
	//need 1 of
	HisReq[0]="Hist 1100";
	HisReq[1]="Hist 1200";
	HisReq[2]="Hist 1400";
	HisReq[3]="Hist 2210";
	HisReq[4]="Hist 2440";
	HisReq[5]="Hist 4000";
	HisReq[6]="Hist 4220";
	HisReq[7]="Hist 4230";
	HisReq[8]="Pol Sc 1100";
	HisReq[9]="Pol Sc 2100";
	
	//need 1 of
	EngReq[0]="Enginr 1200";
	EngReq[1]="Enginr 2300";
	EngReq[2]="IMSE 2710";
	
	//need all
	OtherReq[0]="Chem 1320";
	OtherReq[1]="Englsh 1000";
	OtherReq[2]="Enginr 1000";
	OtherReq[3]="Physcs 2750";
	OtherReq[4]="Physcs 2760";
	OtherReq[5]="Cmp Sc 1050";
	OtherReq[6]="Cmp Sc 2050";
}

//compare constructor
compare::compare(){
	fill();
	countGen=0;
	
	//open files
	tak.open("taken.txt", ios::in);
	if(tak==NULL) perror("Error opening file");	

	string buf="";
	bool tf=false;
	//check for specific values, put in vectors
	while(!tak.eof()){
   		getline(tak, buf);
  		if(buf.find("ECE")!=string::npos) ECEtak.push_back(buf); 		
		if(buf.find("GenEd")!=string::npos) ++countGen;
		if(buf.find("Math")!=string::npos || buf.find("Stat")!=string::npos) Mathtak.push_back(buf);
		if(buf.find("Cmp Sc")!=string::npos) CStak.push_back(buf);				
	}

//  	rewind
	tak.clear();
	tak.seekg(0);

//decides what courses you still need to take
	findNeed();
}

//basic destructor
compare::~compare(){
	cout << endl << "Destructing compare object..." << endl;
	tak.close();
}

bool compare::ver(string name){
	bool test=false;
	for(int i=0; i<file.size(); i++){
//		cout << endl << "file[i]= " << file[i] << endl << "name: " << name << endl << endl;
		if(file[i]==name) {test=true; break;}
	}	
	return test;
}

//most important method for this class
void compare::findNeed(){
	bool hist, Eng=false;
	vector<bool> math, Other, ECE;
	//these fill bool variables and vectors 
	Eng=checkEng();
	hist=checkHist();
	math=checkMath();		
	Other=checkOther();
	ECE=checkECE();
	
	//find all needed electives, push to vector
	checkElect();
	for(int i=0; i<techElect; i++) file.push_back("Technical Elective");
	for(int i=0; i<ECE_4000; i++) file.push_back("ECE 4000+");
	
	cout << "From this data, you still need to take the following courses: " << endl;

	if(countGen<6) for(int i=0; i<(6-countGen); i++) file.push_back("General Education Course");
	if(hist==false) file.push_back(HisReq[8]);
	if(Eng==false) file.push_back(EngReq[0]);
	for(int i=0; i<6; i++) if(math[i]==false) file.push_back(MathReq[i]);
	for(int i=0; i<7; i++) if(Other[i]==false) file.push_back(OtherReq[i]);
	for(int i=0; i<12; i++) if(ECE[i]==false) file.push_back(ECEReq[i]);
	
	//print
	for(int i=0; i<file.size(); i++) cout << file[i] << endl;
	tak.clear();
	tak.seekg(0);			
	
	int choice=0, checkCh=0, num;
	
	//because I cannot handle previous school courses without looking up their mizzou equivalent, I left this open to the user
//	do{ cout << endl << "Did you take any of these courses from a different school?(1-Yes, 2-No) "; cin >> choice; } while(choice!=1 && choice!=2);
//	if(choice==1){cout << "How many of these courses have you previously taken? "; cin >> num;}
//	
//	string already="";
//	if(choice==1){
//		for(int i=0; i<num; i++){
//			cout << endl << "Enter the course you took previously, EXACTLY as it is displayed above: "; 
//			cin.ignore();
//			getline(cin, already);
//			
//			do{cout << "You entered \"" << already << "\". Was this entered as you intended? (1-Yes, 2-No) "; cin >> checkCh;} while(checkCh!=1 && checkCh!=2);
//	
//			if(checkCh==1) for(int j=0; j<(int)file.size(); j++) if(file[j]==already) file.erase(remove(file.begin(), file.end(), already), file.end());
//			if(checkCh==2) --i;		
//		}
//		//rewind
//		tak.clear();
//		tak.seekg(0);		
//		cout << endl << "After updates, you still need to take the following courses: " << endl;
//	}
	do{cout << endl << "Did you take any of these courses from a different school?(1-Yes, 2-No) "; cin >> choice; } while(choice!=1 && choice!=2);
	if(choice==1){
		do{cout << "How many of these courses have you previously taken? "; cin >> num;} while(num>file.size() || num<=0);

		string already="";
		for(int i=0; i<num; i++){
			if(i==0) cin.ignore();		
	back:	
			cout << endl << "Enter the course you took previously, EXACTLY as it is displayed above: "; 		
			getline(cin, already);
			bool is=ver(already);
			if(is==false){
				cout << endl << "The given course did not match any listed. Try again." << endl;
				goto back;
			}
			for(int j=0; j<(int)file.size(); j++) if(file[j]==already) file.erase(remove(file.begin(), file.end(), already), file.end());
		}
		//rewind
		tak.clear();
		tak.seekg(0);		
		cout << endl << "After updates, you still need to take the following courses: " << endl;	
	}
	//print out to standard output and print to file need.txt
	for(int i=0; i<(int)file.size(); i++) cout << file[i] << endl;		
}		

//fills boolean vector with true/false if course was taken
vector<bool> compare::checkMath(){
	vector<bool> done;
	string buf="";
	bool temp[6]={false};
	for(int i=0; i<6; i++){
		while(!tak.eof()){
			getline(tak, buf);
			if(buf==MathReq[i]) temp[i]=true;				
		}
//	  	rewind
		tak.clear();
		tak.seekg(0);		
	}
  	//rewind
	tak.clear();
	tak.seekg(0);
	for(int i=0; i<6; i++) done.push_back(temp[i]);		
	return done;
}

//fills boolean vector with true/false if course was taken
vector<bool> compare::checkOther(){
	vector<bool> done;
	bool temp[7]={false};
	string buf="";
	for(int i=0; i<7; i++){	
		while(!tak.eof()){
			getline(tak, buf);
			if(buf==OtherReq[i]) temp[i]=true;
		}
//	  	rewind
		tak.clear();
		tak.seekg(0);			
	}	
  	//rewind
	tak.clear();
	tak.seekg(0);
	for(int i=0; i<7; i++) done.push_back(temp[i]);		
	return done;
}

//fills boolean vector with true/false if course was taken
vector<bool> compare::checkECE(){
	vector<bool> done;
	bool temp[12]={false};
	string buf="";
	for(int i=0; i<12; i++){		
		while(!tak.eof()){
			getline(tak, buf);
			if(buf==ECEReq[i]) temp[i]=true;		
		}
//	  	rewind
		tak.clear();
		tak.seekg(0);			
	}	
  	//rewind
	tak.clear();
	tak.seekg(0);
	for(int i=0; i<12; i++) done.push_back(temp[i]);
	return done;
} 

//check for history elective
bool compare::checkHist(){
	bool done=false;
	string buf="";
	while(!tak.eof()){
		getline(tak, buf);
		for(int i=0; i<10; i++){
			if(buf==HisReq[i]) done=true;
		}
	}
  	//rewind
	tak.clear();
	tak.seekg(0);
	if(done==true) ++countGen;	
	return done;
}

//check for eng elective
bool compare::checkEng(){
	bool done=false;
	string buf="";
	while(!tak.eof()){
		getline(tak, buf);
		for(int i=0; i<3; i++){
			if(buf==EngReq[i]) done=true;
		}
	}
  	//rewind
	tak.clear();
	tak.seekg(0);		
	return done;
}

//checks if electives are technical electives or ECE 3000+ and count each 
void compare::checkElect(){
	techElect=3;
	ECE_4000=2;
	bool tf;
	for(int i=0; i<ECEtak.size(); i++){
		tf=isElementOf(ECEtak[i], ECEReq, 12);
		if(tf==false){
			string temp=ECEtak[i];
			temp=temp.substr(4, temp.size()-1);
			int test=atoi(temp.c_str());
			if(test>=4000) --ECE_4000;
			if(test>=3000 && test<4000) --techElect; 
		}	
	}
	
	for(int i=0; i<CStak.size(); i++){
		if(CStak[i]!=OtherReq[5] && CStak[i]!=OtherReq[6]){
			string temp=CStak[i];
			temp=temp.substr(6, temp.size()-1);			
			int test=atoi(temp.c_str());
			if(test>=2000) --techElect;
		}	
	}		
	for(int i=0; i<Mathtak.size(); i++){
		tf=isElementOf(Mathtak[i], MathReq, 6);
		if(tf==false){
			string temp=Mathtak[i];
			temp=temp.substr(5, temp.size()-1);
			int test=atoi(temp.c_str());
			if(test>=2000) --techElect;
		}	
	}
	
	if(techElect<0) techElect=0;
	if(ECE_4000<0) ECE_4000=0;	
}

//wrote this simply for saving lines of code, checks if array contains specific value
bool compare::isElementOf(string element, string of[], int size){
	for(int i=0; i<size; i++) if(element==of[i]) return true;
	return false;
}

//my find function
bool compare::find(const char *one, const char *two){
	int size=(sizeof(two)/sizeof(two[0])), j, temp;
	//for some reason, the above sizeof(array)/sizeof(char) isnt working as I thought it would, so hardcode size
	size=2;
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

//schedule class, inherits from compare
class schedule: public compare{
	protected:
		int emph;
		int remainSem;
		int fallOnly;
		int springOnly;
		void showReq(int);
		void checkInterest();
		void setRemainingElect();
		bool checkElect(string);
//		vector<semester> list;
	
	public:
		int get_Sem() {return remainSem;}
		vector<string> elect;	
		schedule();
		~schedule();
};

//basic constructor
schedule::schedule(){
	emph=0;
	springOnly=0;
	fallOnly=0;
	//16 as highest credit size allowed per sem
	remainSem=this->file.size()/16;
	checkInterest();
//	for(int i=0; i<remainSem; i++) list.push_back(semester());
}

bool schedule::checkElect(string test){
	for(int i=0; i<ECEtak.size(); i++) if(ECEtak[i]==test) return true;
	for(int i=0; i<CStak.size(); i++) if(CStak[i]==test) return true;
	return false;
}

//checks what kind of emphasis user wants to take
//I plan to add to this, adding more than just emphasis capabilities,
//but as of yet, I have not had the chance to do so
void schedule::checkInterest(){
	int intent, loop=0;
	bool seeReq;
	string choice="";
	char c;
	do{
		choice="";
		intent=0;
		seeReq=false;
		cout << endl << "Which of the following emphasis are you interested in?" << endl;
		cout << "\t1. Communications and Signal Processing" << endl;
		cout << "\t2. Intelligent Robots" << endl;
		cout << "\t3. Robot Vision" << endl;
		cout << "\t4. Computer Vision" << endl;
		cout << "\t5. Pattern Recognition" << endl;
		cout << "\t6. Control Systems" << endl << endl;
		cout << "To see requirements for any of these, use the command \"see req number\". To choose an emphasis, simply enter the number itself. For help, enter \"help\"";
		cout << endl << endl << "Input: ";
//		if(loop==0) cin.ignore();	
		getline(cin, choice);
//		cout << endl << "INPUT WAS: " << choice << endl << endl;
		//help
		if(choice.find("help")!=string::npos){
			cout << endl << "Example Inputs" << endl << "\tInput: see req 3\n\t(This shows requirements for Robot vision emphasis)\n\tInput: 6\n\t(This selects Control system as emphasis)" << endl << endl;
		}
		//if user wanted to see requirements
		//i have no idea why, but this breaks if user wants to see requirements for 1
		else if(choice.find("see req")!=string::npos){
			seeReq=true;
			for(int i=0; i<choice.length(); i++){
				c=choice[i];							
				if(isdigit(c)) intent=atoi(&c);				
				else if(i+1==choice.length() && intent==0) cout << "Error! No numeric value was given.\n" << endl;
//				cout << "char: " << c << endl << "intent: " << intent << endl << "i: " << i << endl << endl;				
			}
		}	
		else for(int i=0; i<choice.length(); i++){
			c=choice[i];
			if(isdigit(c)) intent=atoi(&c);
			else if(i+1==choice.length() && intent==0) cout << "Error! No numeric value was given.\n" << endl;
		}
		
		if(intent==1 && seeReq==false) emph=1;
		if(intent==2 && seeReq==false) emph=2;
		if(intent==3 && seeReq==false) emph=3;
		if(intent==4 && seeReq==false) emph=4;
		if(intent==5 && seeReq==false) emph=5;
		if(intent==6 && seeReq==false) emph=6;
		
		if(intent==1 && seeReq==true) {showReq(1); intent=0;}
		if(intent==2 && seeReq==true)	{showReq(2); intent=0;}									
		if(intent==3 && seeReq==true) {showReq(3); intent=0;}
		if(intent==4 && seeReq==true)	{showReq(4); intent=0;}																						
		if(intent==5 && seeReq==true)	{showReq(5); intent=0;}									
		if(intent==6 && seeReq==true)	{showReq(6); intent=0;}									
				
		++loop;		
	}while(intent<1 || intent>6);
	
	setRemainingElect();
}

//shows each individual emphasis requirements
void schedule::showReq(int num){
	cout << endl << "-------------------------------------------------------------------";
	switch (num){
		case 1:{
			//need 3830 for 4710 and 4830
			cout << endl << "Communications and Signal Processing Course Requirements:" << endl;
			cout << "\t1. Intro to Communication Systems, ECE 4710\n\t2. Intro to Wireless Communications, ECE 4730\n\t3. Intro to Digital Signal Processing, ECE 4830";
			cout << "\n\t4. Electromagnetic Fields, ECE 3510\n\t5. Antenna Theory and Design, ECE 4940" << endl;
			break;	
		}
		case 2:{
			//4330 only offered in fall
			cout << endl << "Intelligent Robots Course Requirements:" << endl;
			cout << "\t1. Intro to Computational Intelligence, ECE 4870\n\t2. Advanced Algorithm Design, CS 3050\n\t3. Artificial Intelligence I, CS 4750\n\t4. Building Intelligent Robots, ECE 4340\n\t5. Mechatronics and Robot Vision, ECE 4330" << endl;
			break;
		}
		case 3:{
			//4330 offered only in fall
			//need stat 4710 for 4655 and 4720
			cout << endl << "Robot Visiion Course Requiremens:" << endl;
			cout << "\t1. Digital Image Processing, ECE 4655\n\t2. Mechatronics and Robot Vision, ECE 4330\n\t3. Pattern Recognition and Machine Learning, ECE 4720" << endl;
			break;
		}
		case 4:{
			//need stat 4710 for 4655 and 4720
			cout << endl << "Computer Vision Course Requirements: " << endl;
			cout << "\t1. Image Processing, ECE 4655\n\t2. Image Compression, ECE 4675\n\t3. Pattern Recognition and Machine Learning, ECE 4720" << endl;
			break;
		}
		case 5:{
			//need stat4710 for 4720
			cout << endl << "Pattern Recognition Course Requirements:" << endl;
			cout << "\t1. Introduction to Computational Intelligence, ECE 4870\n\t2. Advanced Algorithm Design, CS 3050\n\t3. Artificial Intelligence I, CS 4750\n\t4. Pattern Recognition and Machine Learning, ECE 4720" << endl;
			break;
		}
		case 6:{
			//need dif equ for 4310 and matrix theory, check mae, need 3830 for 4830
			cout << endl << "Control Systems Course Requirements:" << endl;
			cout << "\t1. Feedback Control Systems, ECE 4310\n\t2. Introduction to Digital Signal Processing, ECE 4830\n\t3. Matrix Theory, Math 4140\n\t4. Modern Control, MAE 4720" << endl;
			break;
		}										
	}
	cout << "-------------------------------------------------------------------" << endl;	
}

//sets the electives you still need to take depending on what the user chooses
void schedule::setRemainingElect(){
	cout << endl << "You still have " << ECE_4000 << " ECE 4000+ electives to take and " << techElect << " technical elective courses to take." << endl;
	int numForEmph=0, array[5];
	switch(emph){
		case 1:{
			numForEmph=5;
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 5 courses." << endl;
			showReq(1);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take? Notice, ECE 3510 is a pre-requisite for ECE 4940.";
			for(int i=0; i<5; i++){
				if((ECE_4000+techElect)==(i)) break;
				cout << endl << "Input: ";
				cin >> array[i];
		again:					
				if(array[i]==1){
					bool test=checkElect("ECE 4710");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4710. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again;
					}
					else{
						elect.push_back("ECE 4710");
						elect.push_back("Fall");
						++fallOnly;
					}
				}
				else if(array[i]==2){
					bool test=checkElect("ECE 4730");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4730. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again;						
					}
					else{
						elect.push_back("ECE 4730");
						elect.push_back("Spring");
						++springOnly;
					}
				}
				else if(array[i]==3){
					bool test=checkElect("ECE 4830");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4830. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again;
					}
					else{
						elect.push_back("ECE 4830");
						elect.push_back("Fall");
						++fallOnly;
					}
				}
				else if(array[i]==4){
					bool test=checkElect("ECE 3510");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 3510. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again;
					}
					else{
						elect.push_back("ECE 3510");
						elect.push_back("Fall/Spring");
					}
				}
				else if(array[i]==5){
					bool test=checkElect("ECE 4940");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4940. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again;					
					}
					else{	
						elect.push_back("ECE 4940");
						elect.push_back("Spring");
						++springOnly;
					}
				}					
			}
			break;	
		}
		case 2:{
			numForEmph=4;
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 4 courses.";
			showReq(2);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?";
			for(int i=0; i<5; i++){
				if((ECE_4000+techElect)==(i)) break;				
				cout << endl << "Input: ";
				cin >> array[i];
		again2:
				if(array[i]==1){
					bool test=checkElect("ECE 4870");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4870. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again2;
					}
					else{
						elect.push_back("ECE 4870");
						elect.push_back("Fall");
						++fallOnly;
					}	
				}
				else if(array[i]==2){
					bool test=checkElect("CS 3050");
					if(test==true){
						cout << endl << "Error! You have already taken CS 3050. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again2;	
					}
					else{
						elect.push_back("CS 3050");
						elect.push_back("Fall/Spring");
					}	
				}								
				else if(array[i]==3){
					bool test=checkElect("CS 4750");
					if(test==true){
						cout << endl << "Error! You have already taken CS 4750. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again2;
					}
					else{
						elect.push_back("CS 4750");
						elect.push_back("Fall");
						++fallOnly;
					}	
				}
				else if(array[i]==4){
					bool test=checkElect("ECE 4340");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4340. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again2;
					}
					else{
						elect.push_back("ECE 4340");
						elect.push_back("Spring");
						++springOnly;
					}
				}
				else if(array[i]==5){
					bool test=checkElect("ECE 4330");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4330. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again2;						
					}
					else{
						elect.push_back("ECE 4330");
						elect.push_back("Fall");
						++fallOnly;
					}	
				}
			}
			break;					
		}
		case 3:{
			numForEmph=3;		
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 3 courses.";
			showReq(3);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?" << endl;
			for(int i=0; i<3; i++){
				if((ECE_4000+techElect)==(i)) break;			
				cout << endl << "Input: ";
				cin >> array[i];
		again3:		
				if(array[i]==1){
					bool test=checkElect("ECE 4655");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4655. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again3;						
					}				
					else{
						elect.push_back("ECE 4655");
						elect.push_back("Fall");
						++fallOnly;
					}
				}
				else if(array[i]==2){
					bool test=checkElect("ECE 4330");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4330. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again3;						
					}							
					else{	
						elect.push_back("ECE 4330");
						elect.push_back("Fall");
						++fallOnly;
					}
				}
				else if(array[i]==3){
					bool test=checkElect("ECE 4720");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4720. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again3;						
					}					
					else{
						elect.push_back("ECE 4720");
						elect.push_back("Spring");
						++springOnly;
					}
				}
			}
			break;				
		}
		case 4:{
			numForEmph=3;		
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 3 courses.";
			showReq(4);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?";
			for(int i=0; i<3; i++){
				if((ECE_4000+techElect)==(i)) break;			
				cout << endl << "Input: ";
				cin >> array[i];
		again4:		
				if(array[i]==1){
					bool test=checkElect("ECE 4655");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4655. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again4;						
					}					
					else{
						elect.push_back("ECE 4655");
						elect.push_back("Fall");
						++fallOnly;
					}	
				}
				else if(array[i]==2){
					bool test=checkElect("ECE 4675");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4675. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again4;						
					}					
					else{
						elect.push_back("ECE 4675");
						elect.push_back("Fall/Spring");
					}	
				}
				else if(array[i]==3){
					bool test=checkElect("ECE 4720");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4720. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again4;						
					}					
					else{
						elect.push_back("ECE 4720");
						elect.push_back("Spring");
						++springOnly;
					}	
				}
			}
			break;
		}
		case 5:{
			numForEmph=3;
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 3 courses.";
			showReq(5);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?" << endl;
			for(int i=0; i<4; i++){
				if((ECE_4000+techElect)==(i)) break;			
				cout << endl << "Input: ";
				cin >> array[i];
		again5:		
				if(array[i]==1){
					bool test=checkElect("ECE 4870");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4870. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again5;						
					}					
					else{
						elect.push_back("ECE 4870");
						elect.push_back("Fall");
						++fallOnly;
					}	
				}
				else if(array[i]==2){
					bool test=checkElect("CS 3050");
					if(test==true){
						cout << endl << "Error! You have already taken CS 3050. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again5;						
					}					
					else{
						elect.push_back("CS 3050");
						elect.push_back("Fall/Spring");
					}
				}												
				else if(array[i]==3){
					bool test=checkElect("CS 4750");
					if(test==true){
						cout << endl << "Error! You have already taken CS 4750. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again5;						
					}					
					else{
						elect.push_back("CS 4750");
						elect.push_back("Fall");
						++fallOnly;
					}
				}
				else if(array[i]==4){
					bool test=checkElect("ECE 4720");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4720. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again5;						
					}	
					else{
						elect.push_back("ECE 4720");
						elect.push_back("Spring");
						++springOnly;
					}	
				}
			}
			break;					
			
		}
		case 6:{
			numForEmph=4;		
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 4 courses.";
			showReq(6);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?" << endl;
			for(int i=0; i<4; i++){
				if((ECE_4000+techElect)==(i)) break;			
				cout << endl << "Input: ";
				cin >> array[i];
		again6:		
				if(array[i]==1){
					bool test=checkElect("ECE 4310");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4310. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again6;						
					}					
					else{
						elect.push_back("ECE 4310");
						elect.push_back("Fall/Spring");
					}	
				}
				else if(array[i]==2){
					bool test=checkElect("ECE 4830");
					if(test==true){
						cout << endl << "Error! You have already taken ECE 4830. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again6;						
					}				
					else{
						elect.push_back("ECE 4830");
						elect.push_back("Fall");
						++fallOnly;
					}	
				} 
				else if(array[i]==3){
					bool test=checkElect("Math 4140");
					if(test==true){
						cout << endl << "Error! You have already taken Math 4140. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again6;						
					}
					else{
						elect.push_back("Math 4140");
						elect.push_back("Fall/Spring");
					}	
				}
				else if(array[i]==4){
					bool test=checkElect("MAE 4720");
					if(test==true){
						cout << endl << "Error! You have already taken MAE 4720. Choose a different elective." << endl << "Input: ";
						cin >> array[i];
						goto again6;						
					}
					else{
						elect.push_back("MAE 4720");
						elect.push_back("Fall/Spring");
					}	
				}
			}
			break;			
		}
	}
	
	int numC=elect.size()/2;
	
	if(numC<(ECE_4000+techElect)){
		ECE_4000=0;
		techElect=(numC-techElect);
		cout << endl << endl << "You still have " << techElect << " electives to take. For the remaining: " << endl << endl;
		checkInterest();
	}
}

schedule::~schedule(){
	cout << endl << "Destructing Schedule object...";
}

class divider{
	private:
		int mathMin[6];
		int eceMin[12];
		int engMin;
		int othMin;
		int semLeft;
		string MathReq[6];
		string ECEReq[12];
		string EngReq[3];
		string OtherReq[7];
		vector<string> classes;
		vector<string> electives;
		void fill();
		void check_PreReq();
		void set_sched();
		void set_elect_sched();
	
	public:
		vector< vector<string> > sched;
		divider(vector<string>, vector<string>);
		~divider();
		void print(int);		
};

void divider::fill(){
	//need all
	MathReq[0]="Math 1500";
	MathReq[1]="Math 1700";
	MathReq[2]="Math 2320";
	MathReq[3]="Math 2300";
	MathReq[4]="Math 4100";
	MathReq[5]="Stat 4710";
	
	//need all
	ECEReq[0]="ECE 1210";
	ECEReq[1]="ECE 2100";
	ECEReq[2]="ECE 3210";
	ECEReq[3]="ECE 4250";	
	ECEReq[4]="ECE 3220";
	ECEReq[5]="ECE 3410";
	ECEReq[6]="ECE 3810";	
	ECEReq[7]="ECE 3830";	
	ECEReq[8]="ECE 4220";
	ECEReq[9]="ECE 4270";
	ECEReq[10]="ECE 3110";
	ECEReq[11]="ECE 4970";
	
	//need 1 of
	EngReq[0]="Enginr 1200";
	EngReq[1]="Enginr 2300";
	EngReq[2]="IMSE 2710";
	
	//need all
	OtherReq[0]="Chem 1320";
	OtherReq[1]="Englsh 1000";
	OtherReq[2]="Enginr 1000";
	OtherReq[3]="Physcs 2750";
	OtherReq[4]="Physcs 2760";
	OtherReq[5]="Cmp Sc 1050";
	OtherReq[6]="Cmp Sc 2050";	
}

divider::divider(vector<string> c, vector<string> e){
	othMin=0;
	classes=c;
	electives=e;
	if(classes.size()%5!=0) semLeft=classes.size()/4+1;
	else semLeft=classes.size()/4;
	fill();
//	cout << endl << "DB1" << endl;
	//initialize 2d vector
//	vector<string> temp(5, "test");
//	for(int i=0; i<10; i++) sched.push_back(temp);	
//	sched.resize(semLeft);
//	for(int i=0; i<sched.size(); i++) sched[i].resize(5);
//	cout << endl << "DB2" << endl;	
	check_PreReq();
	set_sched();
	set_elect_sched();
//	print();
}

void divider::check_PreReq(){
	int place1=0, place2=0, verify=0, verify2=0;
	vector<string> temp=classes;
//	cout << endl << "DB3" << endl;	
	for(int i=0; i<classes.size(); i++){
		//math
		for(int j=0; j<6; j++) if(classes[i]==MathReq[j]) {mathMin[place1]=j; ++place1;}
		//ECE
		for(int j=0; j<12; j++) if(classes[i]==ECEReq[j]) {eceMin[place2]=j; ++place2;}
	}
	
//	cout << "DB place1=" << place1 << endl;
//	for(int i=0; i<place1; i++){
//		cout << MathReq[mathMin[i]] << endl;
//	}
	
	//initialize 2d vector	
	if(place2==5) 	semLeft=3;
	sched.resize(semLeft, vector<string>(0, ""));	
		
//	cout << "DB place2=" << place2 << endl;
//	for(int i=0; i<place2; i++){
//		cout << "j:" << i << " " << ECEReq[eceMin[i]] << endl;
//	}	
	
	for(int i=0; i<classes.size(); i++){
		//eng 1200
		if(classes[i]==EngReq[0]){
			for(int j=0; j<6; j++){
				if(mathMin[j]==0) {sched[0].push_back(MathReq[0]); ++verify; sched[1].push_back(EngReq[0]);} //calc 1 need to be taken first
				else sched[0].push_back(EngReq[0]);
			}
		}
		//eng 2300
		if(classes[i]==EngReq[1]){
			for(int j=0; j<temp.size(); j++){
				if(temp[j]==OtherReq[3]) {sched[0].push_back(OtherReq[3]); sched[1].push_back(EngReq[1]);} //physics 1 needs to be taken 
				else sched[0].push_back(EngReq[1]);
			}
		}
		//physics 2750
		if(classes[i]==OtherReq[3]){
			for(int j=0; j<6; j++){
				if(mathMin[j]==0) {sched[0].push_back(MathReq[0]); ++verify; sched[1].push_back(OtherReq[3]);} //calc 1 need to be taken first
				else sched[0].push_back(OtherReq[3]);
			}					
		}
		//physics 2760
		if(classes[i]==OtherReq[4]){
			for(int j=0; j<6; j++){
				if(mathMin[j]==0) {sched[0].push_back(MathReq[0]); ++verify; sched[1].push_back(MathReq[1]); sched[2].push_back(OtherReq[4]);} //calc 1 need to be taken first
				else if(mathMin[j]==1) {sched[0].push_back(MathReq[1]); ++verify2; sched[1].push_back(OtherReq[4]);} //calc 2 need to be taken first
				else sched[0].push_back(OtherReq[4]);
			}				
		}
		//cs2050
		if(classes[i]==OtherReq[6]){
			for(int j=0; j<temp.size(); j++){
				if(temp[i]==OtherReq[5]) {sched[0].push_back(OtherReq[5]); sched[1].push_back(OtherReq[6]);} //cs 1050 needs to be taken first
			}
		}
	}
	
//	cout << endl << "DB4" << endl;
	int count=0;	
	//set math courses into planned semesters
	for(int i=0; i<place1; i++){
		if(verify!=0) ++i;
		if(verify2!=0) ++i;
		if(!MathReq[mathMin[i]].empty()) {sched[count].push_back(MathReq[mathMin[i]]); /*cout << "DB: " << MathReq[mathMin[i]] << endl;*/}
		++count;
	}
	
//	cout << endl << endl <<"-----------TEST---------------" << endl;
//   sched[0].push_back(ECEReq[eceMin[0]]);	
//	cout << endl << "NUM SEMESTERS LEFT: " << semLeft << endl;
//	cout << endl <<"-----------TEST---------------" << endl;	
	
	
	count=0;
//	cout << endl << "DB5" << endl;
	//set ece courses into planned semester
	for(int i=0; i<place2; i++){
		if(place2<=4) sched[count].push_back(ECEReq[eceMin[i]]);
		else if(place2==5){
//			cout << endl << "i: " << i << " j: " << j << " count: " << count << endl;
			if(i==0 || i==2) {sched[count].push_back(ECEReq[eceMin[i]]); /*cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";*/ ++i;}
			sched[count].push_back(ECEReq[eceMin[i]]);
//			cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";
		}
		else if(place2==6){
			if(i==1 || i==3) {sched[count].push_back(ECEReq[eceMin[i]]); /*cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";*/ ++i;}
			sched[count].push_back(ECEReq[eceMin[i]]);
		}
		else if(place2==7){
			if(i==0 || i==2 || i==4) {sched[count].push_back(ECEReq[eceMin[i]]); /*cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";*/ ++i;}
			sched[count].push_back(ECEReq[eceMin[i]]);			
		}
		else if(place2==8){
			if(i==0) {sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i;}
			if(i==3 || i==5) {sched[count].push_back(ECEReq[eceMin[i]]); /*cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";*/ ++i;}
			sched[count].push_back(ECEReq[eceMin[i]]);			
		}
		else if(place2==9){
			if(i==0) {sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i;}
			if(i==4 || i==6) {sched[count].push_back(ECEReq[eceMin[i]]); /*cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";*/ ++i;}	
			sched[count].push_back(ECEReq[eceMin[i]]);						
		}
		else if(place2==10){
			if(i==1) {sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i;}
			if(i==5 || i==7) {sched[count].push_back(ECEReq[eceMin[i]]); /*cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";*/ ++i;}	
			sched[count].push_back(ECEReq[eceMin[i]]);			
		}
		else if(place2==11){
			if(i==0) {sched[count].push_back(ECEReq[eceMin[i]]); ++i;}	
			if(i==2) {sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i;}
			if(i==6 || i==8) {sched[count].push_back(ECEReq[eceMin[i]]); /*cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";*/ ++i;}
			sched[count].push_back(ECEReq[eceMin[i]]);				
		}
		else if(place2==12){
			if(i==1) {sched[count].push_back(ECEReq[eceMin[i]]); ++i;}	
			if(i==3) {sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i; sched[count].push_back(ECEReq[eceMin[i]]); ++i;}
			if(i==7 || i==9) {sched[count].push_back(ECEReq[eceMin[i]]); /*cout << endl << "Addition was " << ECEReq[eceMin[i]] << " at sched[" << count << "]";*/ ++i;}
			sched[count].push_back(ECEReq[eceMin[i]]);					
		}
		++count;
	}
	
////	cout << endl << "DB TEST PRINT:" << endl;
//	for(int i=0; i<sched.size(); i++){
//		cout << endl << "Semester " << i+1 << ": " << endl;
//		for(int j=0; j<sched[i].size(); j++){
//			cout << sched[i][j] << endl;
//		}
//	}
//	cout << endl << endl;
}

void divider::set_sched(){
	for(int i=0; i<classes.size(); i++){
		if(classes[i]==OtherReq[5]) sched[0].push_back(OtherReq[5]);
		if(classes[i]==OtherReq[2]) sched[0].push_back(OtherReq[2]);
		if(classes[i]==OtherReq[1]) sched[0].push_back(OtherReq[1]);
		if(classes[i]==OtherReq[0]) sched[0].push_back(OtherReq[0]);										
	}
	int gen=0, imse=0;
	for(int i=0; i<classes.size(); i++){
		if(classes[i]=="General Education Course") ++gen;
		if(classes[i]==EngReq[2]) ++imse;
	}
	for(int i=sched.size(); i>0; i--){
		if(sched[i].size()<3 && gen>0) {sched[i].push_back("General Education Course"); --gen;}
		if(gen<0 && sched[i].size()<3 && imse==1) sched[i].push_back(EngReq[2]); 
	}
}

void divider::set_elect_sched(){
	bool flag;
	int temp;
	for(int i=0; i<electives.size(); i++){
		flag=false;
//		cout << endl << "i: " << i << endl;
		//odd
		if(electives[i]=="ECE 4710"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==ECEReq[7]){
//						cout << endl << "j: " << j << " sched.size()-1: " << sched.size()-1 << endl;
//						cout << endl << "k: "<< k << " sched[j].size()-1: " << sched[j].size()-1 << endl;					
//						cout << endl << "In if k=" << k << endl;
//						is j odd
						temp=j%2;
						if(temp==1){
							sched[j+2].push_back("ECE 4710");
							flag=true;
						}
						else if(temp==0){
							sched[j+1].push_back("ECE 4710");
							flag=true;						
						}
						break;
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
//						cout << endl << "j: " << j << " sched.size()-1: " << sched.size()-1 << endl;
//						cout << endl << "k: "<< k << " sched[j].size()-1: " << sched[j].size()-1 << endl;						
//						cout << endl << "in else if k=" << k << endl;
						for(int x=0; x<sched.size(); x++){
							temp=x%2;
							if(sched[x].size()<5 && temp==1){
								sched[x].push_back("ECE 4710");
								flag=true;
								break;
							}
						}
					}
				}
			}	
		}
		//even
		else if(electives[i]=="ECE 4730"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==ECEReq[4]){
						temp=j%2;
						if(temp==0){
							sched[j+2].push_back("ECE 4730");
							flag=true;
						}
						else if(temp==1){
							sched[j+1].push_back("ECE 4730");
							flag=true;							
						}
						break;					
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=x%2;						
							if(sched[x].size()<5 && temp==0){
								sched[x].push_back("ECE 4730");
								flag=true;
								break;
							}
						}
					}					
				}
			}			
		}
		//odd
		else if(electives[i]=="ECE 4830"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==ECEReq[7]){
						temp=j%2;
						if(temp==1){
							sched[j+2].push_back("ECE 4830");
							flag=true;
						}
						else if(temp==0){
							sched[j+1].push_back("ECE 4830");
							flag=true;						
						}
						break;						
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=j%2;						
							if(sched[x].size()<5 && temp==1){
								sched[x].push_back("ECE 4830");
								flag=true;
								break;
							}
						}
					}										
				}
			}			
		}
		else if(electives[i]=="ECE 3510"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					int phy=0;
					if(sched[j][k]==OtherReq[4]){
						phy=1;
					}
					if(sched[j][k]==MathReq[4] && phy==1){
						sched[j+1].push_back("ECE 3510");
						flag=true;
						break;																	
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							if(sched[x].size()<5){
								sched[x].push_back("ECE 3510");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		//even
		else if(electives[i]=="ECE 4940"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]=="ECE 3510"){
						temp=j%2;
						if(temp==0){
							sched[j+2].push_back("ECE 4940");
							flag=true;
						}
						else if(temp==1){
							sched[j+1].push_back("ECE 4940");
							flag=true;							
						}
						break;											
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=x%2;						
							if(sched[x].size()<5 && temp==0){
								sched[x].push_back("ECE 4940");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		//odd
		else if(electives[i]=="ECE 4870"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==OtherReq[6]){
						temp=j%2;
						if(temp==1){
							sched[j+2].push_back("ECE 4870");
							flag=true;
						}
						else if(temp==0){
							sched[j+1].push_back("ECE 4870");
							flag=true;						
						}
						break;											
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=x%2;
							if(sched[x].size()<5 && temp==1){
								sched[x].push_back("ECE 4870");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		else if(electives[i]=="CS 3050"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==OtherReq[6]){
						sched[j+1].push_back("ECE 3050");
						flag=true;	
						break;																	
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							if(sched[x].size()<5){
								sched[x].push_back("CS 3050");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		//odd		
		else if(electives[i]=="CS 4750"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]=="CS 3050"){
						temp=j%2;
						if(temp==1){
							sched[j+2].push_back("CS 4750");
							flag=true;
						}
						else if(temp==0){
							sched[j+1].push_back("CS 4750");
							flag=true;						
						}	
						break;											
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=x%2;						
							if(sched[x].size()<5 && temp==1){
								sched[x].push_back("CS 4750");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		//even
		else if(electives[i]=="ECE 4340"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==OtherReq[6]){
						temp=j%2;
						if(temp==0){
							sched[j+2].push_back("ECE 4340");
							flag=true;
						}
						else if(temp==1){
							sched[j+1].push_back("ECE 4340");
							flag=true;							
						}
						break;																	
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=x%2;						
							if(sched[x].size()<5 && temp==0){
								sched[x].push_back("ECE 4340");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		//odd
		else if(electives[i]=="ECE 4330"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==ECEReq[6]){
						temp=j%2;
						if(temp==1){
							sched[j+2].push_back("ECE 4330");
							flag=true;
						}
						else if(temp==0){
							sched[j+1].push_back("ECE 4330");
							flag=true;						
						}	
						break;																
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=x%2;						
							if(sched[x].size()<5 && temp==1){
								sched[x].push_back("ECE 4330");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		//odd
		else if(electives[i]=="ECE 4655"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					int cs2050=0;
					if(sched[j][k]==OtherReq[6]){
						cs2050=1;
					}
					if(sched[j][k]==MathReq[5] && cs2050==1){
						temp=j%2;
						if(temp==1){
							sched[j+2].push_back("ECE 4655");
							flag=true;
						}
						else if(temp==0){
							sched[j+1].push_back("ECE 4655");
							flag=true;						
						}
						break;																		
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=x%2;						
							if(sched[x].size()<5 && temp==1){
								sched[x].push_back("ECE 4655");
								flag=true;
								break;
							}
						}
					}											
				}
			}		
		}
		//even
		else if(electives[i]=="ECE 4720"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					int cs2050=0;
					if(sched[j][k]==OtherReq[6]){
						cs2050=1;
					}
					if(sched[j][k]==MathReq[5] && cs2050==1){
						temp=j%2;
						if(temp==0){
							sched[j+2].push_back("ECE 4720");
							flag=true;
						}
						else if(temp==1){
							sched[j+1].push_back("ECE 4720");
							flag=true;							
						}
						break;																	
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							temp=x%2;						
							if(sched[x].size()<5 && temp==0){
								sched[x].push_back("ECE 4720");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		else if(electives[i]=="ECE 4675"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					int cs2050=0;
					if(sched[j][k]==OtherReq[6]){
						cs2050=1;
					}
					if(sched[j][k]==MathReq[5] && cs2050==1){
						sched[j+1].push_back("ECE 4675");
						flag=true;
						break;																		
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							if(sched[x].size()<5){
								sched[x].push_back("ECE 4675");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		else if(electives[i]=="ECE 4310"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==MathReq[4]){
						sched[j+1].push_back("ECE 4310");
						flag=true;	
						break;					
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							if(sched[x].size()<5){
								sched[x].push_back("ECE 4310");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		else if(electives[i]=="MAE 4720"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==MathReq[4]){
						sched[j+1].push_back("MAE 4720");
						flag=true;
						break;						
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							if(sched[x].size()<5){
								sched[x].push_back("MAE 4720");
								flag=true;
								break;
							}
						}
					}										
				}
			}		
		}
		else if(electives[i]=="Math 4140"){
			for(int j=0; j<sched.size(); j++){
				if(flag==true) break;			
				for(int k=0; k<sched[j].size(); k++){
					if(sched[j][k]==MathReq[4]){
						sched[j+1].push_back("Math 4140");	
						flag=true;
						break;					
					}
					else if(j==sched.size()-1 && k==sched[j].size()-1){
						for(int x=0; x<sched.size(); x++){
							if(sched[x].size()<5){
								sched[x].push_back("Math 4140");
								flag=true;
								break;
							}
						}
					}										
				}
			}			
		}	
	}		
	
}

void divider::print(int alr){
	fstream done;
	done.open("schedule.txt", ios::out);
	if(done==NULL) {perror("Error opening file"); exit(0);}	

	done << "-----------------------------------------------" << endl;
	int temp, year=2017;
	for(int i=0; i<sched.size(); i++){
		temp=(i+alr)%2;
		if(temp==0) done << "Spring " << year << " Local Campus Credits" << endl << endl;
		else done << "Fall " << year << " Local Campus Credits" << endl << endl;
		if(i%2==1) ++year;
		for(int j=0; j<sched[i].size(); j++){
			done << sched[i][j] << endl << endl;
		}
		done << "-----------------------------------------------" << endl;			
	}

//	cout << endl << endl << "-----------------------------DERIVED SCHEDULE-----------------------------" << endl;
//	for(int i=0; i<sched.size(); i++){
//		cout << endl << "Semester " << i+alr << ": " << endl;
//		for(int j=0; j<sched[i].size(); j++){
//			cout << sched[i][j] << endl;
//		}
//	}
//	cout << endl << "--------------------------------------------------------------------------" << endl;
//	cout << endl << endl;	
}

divider::~divider(){
	cout << "Destructing divider object..." << endl;
}

int main(void){
//	int compType=0;
	//decide which executable file to call to translate .pdf to text
//	do{
//		cout << "What kind of the operating system are you executing this on? (1, 2, 3)" << endl << "\t1- Win32 or Win64\n\t2- Linux32 or Linux64\n\t3- Mac32 or Mac64\n\nInput: ";
//		cin >> compType;
//		cout << endl; 
//	}while(compType!=1 && compType!=2 & compType!=3);
	
//	if(compType==1){
//		system("convert_win32.exe transc.pdf");
//		sleep(1);
//	}
//	if(compType==2){
//		system("./convert_linux32.out transc.pdf");
//		sleep(1);	
//	}
//	if(compType==3){
//		system("./convert_Mac32.dmg transc.pdf");
//		sleep(1);
//	}
	
	string pause;
	cout << endl << "Place the unofficial transcript file you wish to operate on within the same directory as this executing file." << endl << endl;
	cout << "Press \"Enter\" to continue once the action has been completed..." << endl << endl;
	getline(cin, pause);
	
#ifdef OS_WINDOWS
	cout << endl << "-------------------------------------------------WINDOWS OPERATING SYSTEM DETECTED-------------------------------------------------" << endl;
label1:	
	string fname;
	cout << "Enter unofficial transcript file name: ";
	cin >> fname;
	if(access(fname.c_str(), F_OK)!=-1){
		string temp="convert_win32.exe "+fname;	
		system(temp.c_str());
		cout << "Running..." << endl << endl;		
		sleep(1);
	}
	else{
		cout << endl << "File did not exist..." << endl << endl; 
		goto label1;
	}
#elif __linux__
	cout << endl << "-------------------------------------------------LINUX OPERATING SYSTEM DETECTED-------------------------------------------------" << endl;
label2:	
	string fname;
	cout << "Enter unofficial transcript file name: ";
	cin >> fname;
	if(access(fname.c_str(), F_OK)!=-1){	
		string temp="./convert_linux32.out "+fname;		
		system(temp.c_str());
		cout << "Running..." << endl << endl;		
		sleep(1);
	}
	else{
		cout << endl << "File did not exist..." << endl << endl; 
		goto label2;
	}	
#elif __APPLE__
	cout << endl << "-------------------------------------------------MAC OPERATING SYSTEM DETECTED-------------------------------------------------" << endl;
label3:	
	string fname;
	cout << "Enter unofficial transcript file name: ";
	cin >> fname;
	if(access(fname.c_str(), F_OK)!=-1){
		string temp="./convert_Mac32.dmg "+fname;	
		system(temp.c_str());
		cout << "Running..." << endl << endl;
		sleep(1);
	}
	else{
		cout << endl << "File did not exist..." << endl << endl; 
		goto label3;
	}			
#else 
	cout << endl << "-------------------------------------------------ERROR-------------------------------------------------" << endl;
	cout << endl << "Operating System unidetifiable. Terminating..." << endl;
	cout << endl << "-------------------------------------------------------------------------------------------------------" << endl;	
	return 0;
#endif 		
	
	//create objects
	parse obj1(fname);
	schedule *Sobj1=new schedule();
	int sem=obj1.mizSemCount;
	--sem;
//	int semCount=Sobj1->get_Sem();
	
//	cout << endl << endl << "----DB: sem: " << semCount << "-----" << endl << endl;
	
	
	vector<string> class_left=Sobj1->file;
	vector<string> elect_left=Sobj1->elect;
	
	divider obj(class_left, elect_left);
	
	
//	//display file contents
//	cout << endl << endl << "Mizzou Course History:" << endl;	
//	system("cat taken.txt"); 
	obj.print(sem);
	cout << endl << endl;
	
	system("cat schedule.txt");
//	
//	//display what courses were decided you still need to take
//	cout << endl << "Courses to take: " << endl;
//	for(int i=0; i<Sobj1->file.size(); i++) cout << Sobj1->file[i] << endl;
//	
	//display what electives you chose
//	cout << endl << "Electives will be: " << endl;
//	for(int i=0; i<Sobj1->elect.size(); i++) cout << Sobj1->elect[i] << endl;
//	
//	cout << endl << endl << "Terminating..." << endl;
	delete Sobj1;	
	return 0;
}
