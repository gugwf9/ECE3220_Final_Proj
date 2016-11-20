//ECE 3220
//compare

//issuese with this code:
//cant tell the difference between gen eds, simply outputs "gen ed"
//only math, cs and ece were considered for tech electives


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class compare{
	private:
	fstream tak;
	int countGen;
	int techElect;
	int ECE_4000;
	vector<string> ECEtak;
	vector<string> CStak;
	vector<string> Mathtak;	
	vector<string> file;	
	string MathReq[6];
	string ECEReq[12];
	string HisReq[10];
	string EngReq[3];
	string OtherReq[7];
	
	public:
	fstream totak;	
	compare();
	~compare();
	bool find(const char*, const char*);
	void fill();
	void store();
	void findNeed();
	bool isElementOf(string, string[], int);
	void checkElect();
	vector<bool> checkMath();//done
	vector<bool> checkECE();//done
	vector<bool> checkOther();//done	
	bool checkHist(); //done
	bool checkEng();//done
//	void verify();
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

void compare::store(){
	
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

bool compare::find(const char *one, const char *two){
	bool ret=false;
	int size=sizeof(two)/sizeof(two[0]), j, temp;
	j=0;
	for(int i=0; one[i]!='\0'; i++){
		if(one[i]==two[0]){
			temp=i;
			while(j<=size){
				if(one[i]!=two[j]) {i=temp; break;}
				if(j==size) ret=true; 
				++j;
				++i;
			}
				
		}
		if(j-1==size) break;
	}
	return ret;
}

int main(void){
	compare obj1;
	return 0;
}
