#ifndef _VIRTUALINHERITANCETEST_CPP_
#define _VIRTUALINHERITANCETEST_CPP_

#include <stdio.h>
#include <iostream>

//class professor {};
//class teacher : public virtual professor {};
//class researcher : public virtual professor {};
//class myprofessor : public teacher, public researcher {};

using std::cout;
using std::endl;


class Person {
   int personId;
   protected:
   		void setPersonId(int personId) { this->personId = personId;}
   public:
    Person() {
    cout << "Building person" <<endl;
    }
    int getPersonId() {return personId;}
    ~Person() {
		cout << "Destroying person" <<endl; };
        
};

class Researcher: public virtual  Person{
	int papers;
	public:
		Researcher(int p) { 
		cout << "Building researcher" <<endl;
		papers = p; setPersonId(100);
		}
		int getProductivity() { return papers; }
		~Researcher() {
		cout << "Destroying researcher" <<endl; }
};

class Teacher: public virtual Person{
	int courses;
	public:
		Teacher(int c) { 
		cout << "Building teacher" <<endl;
		courses = c; setPersonId(200);}
		int getProductivity() { return courses; };
		~Teacher() {
		cout << "Destroying teacher" <<endl; }
};

class Professor: public Researcher, public  Teacher {
	public:
		Professor(int p, int c) : Researcher(p), Teacher(c) {
		cout << "Building professor" <<endl; };
		~Professor() {
		cout << "Destroying professor" <<endl; }
};


int main() {
  Professor *professor = new Professor(1,2);
  int personId = professor->getPersonId(); // order is person, researcher, teacher, professor with person id value of 200 - 
 					// with Researcher and Teacher derives fromvitual person
  cout <<"Person Id " << personId <<endl;
  int productivity = ((Teacher*)professor)->getProductivity(); 
  cout <<"Productivity " << productivity <<endl; //reruens 2
  delete professor;
  cout << "Return" <<endl;
}

#endif
