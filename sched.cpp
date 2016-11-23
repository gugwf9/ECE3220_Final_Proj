//Schedule

#include <ctype.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

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
//	void verify();	
	
	public:
	vector<string> file;		
	fstream totak;	
	compare();
	~compare();
};

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

compare::compare(){
	fill();
	countGen=0;
	
	tak.open("taken.txt", ios::in);
	if(tak==NULL) perror("Error opening file");	
	totak.open("need.txt", ios::in | ios::out);
	if(totak==NULL) perror("Error opening file");

	string buf="";
	bool tf=false;
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

	findNeed();
}

compare::~compare(){
	cout << endl << "Destructing compare object..." << endl;
	tak.close();
	totak.close();
}

void compare::findNeed(){
	bool hist, Eng=false;
	vector<bool> math, Other, ECE;
	Eng=checkEng();
	hist=checkHist();
	math=checkMath();		
	Other=checkOther();
	ECE=checkECE();
	
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
	
	for(int i=0; i<file.size(); i++) cout << file[i] << endl;
	tak.clear();
	tak.seekg(0);			
	
	int choice=0, checkCh=0, num;
	do{ cout << endl << "Did you take any of these courses from a different school?(1-Yes, 2-No) "; cin >> choice; } while(choice!=1 && choice!=2);
	if(choice==1){cout << "How many of these courses have you previously taken? "; cin >> num;}
	
	string already="";
	if(choice==1){
		for(int i=0; i<num; i++){
			cout << endl << "Enter the course you took previously, EXACTLY as it is displayed above: "; 
			cin.ignore();
			getline(cin, already);
			
			do{cout << "You entered \"" << already << "\". Was this entered as you intended? (1-Yes, 2-No) "; cin >> checkCh;} while(checkCh!=1 && checkCh!=2);
	
			if(checkCh==1) for(int j=0; j<(int)file.size(); j++) if(file[j]==already) file.erase(remove(file.begin(), file.end(), already), file.end());
			if(checkCh==2) --i;		
		}
		tak.clear();
		tak.seekg(0);		
		cout << endl << "After updates, you still need to take the following courses: " << endl;
	}
	for(int i=0; i<(int)file.size(); i++) {cout << file[i] << endl; totak << file[i] << endl;}		
}		

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

class schedule: public compare{
	protected:
		int emph;
		int remainSem;
		int fallOnly;
		vector<string> elect;
		void showReq(int);
		void checkDates();
		void checkInterest();
		void setRemainingElect();
	
	public:
		schedule();
		~schedule();
};

schedule::schedule(){
	emph=0;
	fallOnly=0;
	//16 as highest credit size allowed per sem
	remainSem=this->file.size()/16;
	checkInterest();
}

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
//		cin.ignore();	
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
			cout << "\t1. Intro to Computational Intelligence, ECE 4870\n\t2. Artificial Intelligence I, CS 4750\n\t3. Building Intelligent Robots, ECE 4340\n\t4. Mechatronics and Robot Vision, ECE 4330"<< endl;
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
			cout << "\t1. Introduction to Computational Intelligence, ECE 4870\n\t2. Artificial Intelligence I, CS 4750\n\t3. Pattern Recognition and Machine Learning, ECE 4720" << endl;
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

void schedule::setRemainingElect(){
	cout << endl << "You still have " << ECE_4000 << " ECE 4000+ electives to take and " << techElect << " technical elective courses to take." << endl;
	int numForEmph=0, array[5];
	switch(emph){
		case 1:{
			numForEmph=5;
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 5 courses." << endl;
			showReq(1);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take? Notice, ECE 3510 is a pre-requisite for ECE 4940.";
			for(int i=0; i<ECE_4000+techElect; i++){
				cout << endl << "Input: ";
				cin >> array[i];
				if(array[i]==1) elect.push_back("ECE 4710");
				else if(array[i]==2) elect.push_back("ECE 4730");
				else if(array[i]==3) elect.push_back("ECE 4830");
				else if(array[i]==4) elect.push_back("ECE 3510");
				else if(array[i]==5) elect.push_back("ECE 4940");																		
			}
			break;	
		}
		case 2:{
			numForEmph=4;
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 4 courses.";
			showReq(2);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?";
			for(int i=0; i<ECE_4000+techElect; i++){
				cout << endl << "Input: ";
				cin >> array[i];
				if(array[i]==1) elect.push_back("ECE 4870");
				else if(array[i]==2) elect.push_back("CS 4750");
				else if(array[i]==3) elect.push_back("ECE 4340");
				else if(array[i]==4) elect.push_back("ECE 4330");
			}
			break;					
		}
		case 3:{
			numForEmph=3;		
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 3 courses.";
			showReq(3);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?" << endl;
			for(int i=0; i<ECE_4000+techElect; i++){
				cout << endl << "Input: ";
				cin >> array[i];
				if(array[i]==1) elect.push_back("ECE 4655");
				else if(array[i]==2) elect.push_back("ECE 4330");
				else if(array[i]==3) elect.push_back("ECE 4720");
			}
			break;				
		}
		case 4:{
			numForEmph=3;		
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 3 courses.";
			showReq(4);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?";
			for(int i=0; i<ECE_4000+techElect; i++){
				cout << endl << "Input: ";
				cin >> array[i];
				if(array[i]==1) elect.push_back("ECE 4655");
				else if(array[i]==2) elect.push_back("ECE 4675");
				else if(array[i]==3) elect.push_back("ECE 4720");
			}
			break;			
		}
		case 5:{
			numForEmph=3;
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 3 courses.";
			showReq(5);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?" << endl;
			for(int i=0; i<ECE_4000+techElect; i++){
				cout << endl << "Input: ";
				cin >> array[i];
				if(array[i]==1) elect.push_back("ECE 4870");
				else if(array[i]==2) elect.push_back("CS 4750");
				else if(array[i]==3) elect.push_back("ECE 4720");
			}
			break;					
			
		}
		case 6:{
			numForEmph=4;		
			if(ECE_4000+techElect<numForEmph) cout << "To graduate, it is only necessary that you take " << ECE_4000+techElect << " more electives, while this emphasis includes 4 courses.";
			showReq(6);
			cout << "Of these, which " << ECE_4000+techElect << " would you like to take?" << endl;
			for(int i=0; i<ECE_4000+techElect; i++){
				cout << endl << "Input: ";
				cin >> array[i];
				if(array[i]==1) elect.push_back("ECE 4310");
				else if(array[i]==2) elect.push_back("CS 4830");
				else if(array[i]==3) elect.push_back("Math 4140");
				else if(array[i]==4) elect.push_back("MAE 4720");
			}
			break;			
		}
	}
}

void schedule::checkDates(){
	for(int i=0; i<this->file.size(); i++){
		if(this->file[i]=="ECE 4270") ++fallOnly;
	}
	for(int i=0; i<elect.size(); i++){
		///////need to find courses only offred in fall and spring////////
		if(this->file[i]=="") ++fallOnly;
		if(this->file[i]=="") ++fallOnly;
		if(this->file[i]=="") ++fallOnly;				
	}
}

schedule::~schedule(){
	cout << endl << "Destructing Schedule object..." << endl;
}

//class semester: public schedule{
//	private:
//	
//	public:
//	semester();
//	~semester();
//	
//};

////semester::semester(){
////	
////}

//semester::~semester(){
//	
//}

//class course: public semester{
//	private:
//	
//	public:
//	course();
//	~course();
//};

//course::course(){
//	
//}

//course::~course(){
//	
//}




int main(void){
	schedule *Sobj1=new schedule();
	delete Sobj1;
	return 0;
}
