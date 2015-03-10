all: bricks3 basederived foo virtualinheritancetest

bricks3: bricks3.o
	 g++ -std=c++11 -o bricks3.exe bricks3.o

bricks3.o: bricks3.cpp
#	 g++ -std=c++11 -g -c bricks3.cpp
	 g++ -std=c++11 -c bricks3.cpp
     
cleanbricks:
	 rm bricks3.o bricks3.exe

### Base Derived	 
basederived: basederived.o
	 g++ -std=c++11 -o basederived.exe basederived.o

basederived.o: basederived.cpp
	 g++ -std=c++11 -g -c basederived.cpp
     
cleanbasederived:
	 rm basederived.o basederived.exe
	
## Foo
foo: foo.o
	 g++ -std=c++11 -o foo.exe foo.o

foo.o: foo.cpp
	 g++ -std=c++11 -g -c foo.cpp
     
cleanfoo:
	 rm foo.o foo.exe


### Virtual Inheritance Test	 
virtualinheritancetest: virtualinheritancetest.o
	 g++ -std=c++11 -o virtualinheritancetest.exe virtualinheritancetest.o

virtualinheritancetest.o: virtualinheritancetest.cpp
	 g++ -std=c++11 -g -c virtualinheritancetest.cpp
     
cleanvirtualinheritancetest:
	 rm virtualinheritancetest.o virtualinheritancetest.exe
	 	 
###	 	 
clean:
	cleanbricks
	cleanbasederived	 
