#include <stdio.h>
#include <iostream>

#define MESSAGE "Hello"

#ifdef MESSAGE
#include <algorithm>
#endif

using std::cout;
using std::endl;

class basep{
	int x;
public:
	void setx(int y) {
		x = y;
		cout << x << endl;
}
};

class basex{
	int x;
public:
	void setx(int y) { x = y; cout << x << endl; }
};

class derived : basex{};



class derived1 : public basex{};


class SomeClass {
public:
	int data;
public:
	class Nest{
	public:
		int nested;
	};
public:
	static Nest* createNest(){ return new Nest; }
};

void use_someclass() {
	SomeClass::Nest* nst = SomeClass::createNest();
	nst->nested = 5;
}

class ConstRef
{
public:
	ConstRef(int ii);
private:
	const int I;
};

//ConstRef::ConstRef(int ii)
//{
//	I = ii;
//}




int main() {
	cout << "The Size of \"ulysses\"\n";
	class ConstRef
	{
	public:
		ConstRef(int ii);
	private:
		const int I;
	};

	//ConstRef::ConstRef(int ii)
	//{
	//	I = ii;
	//}

	printf("Hello, world!\n");

	//char test[] = "abcdef\r\n";
	//derived d = new derived();
	//d.setx(12);
	//derived d1 = new derived1();
	//d1.setx(13);
	basep b;
	b.setx(12);
	basex x;
	x.setx(13);
	derived d;
	//d.setx(14);
	derived1 d1;
	d1.setx(15);

	return 0;
}
