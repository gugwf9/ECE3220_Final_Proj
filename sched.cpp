//Schedule

#include <iostream>
#include <string>

using namespace std;

class schedule{
	private:
		
	
	public:
		schedule();
		~schedule();
};

schedule::schedule(){
	
}

schedule::~schedule(){
	cout << "Destructing Schedule object..." << endl;
}

class semester: public schedule{
	private:
	
	public:
	semester();
	~semester();
	
};

semester::semester(){
	
}

semester::~semester(){
	
}

class course: public semester{
	private:
	
	public:
	course();
	~course();
};

course::course(){
	
}

course::~course(){
	
}




int main(void){
	schedule obj1;
	return 0;
}
