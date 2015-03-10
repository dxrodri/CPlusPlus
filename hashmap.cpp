
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::ostream;
using std::istream;
using std::ifstream;
using std::string;

//Copied from http://www.math.ucla.edu/~wittman/10b.1.10w/Lectures/Lec23.pdf
//http://javarevisited.blogspot.com/2011/02/how-hashmap-works-in-java.html

class Record {
	public:
		Record();
		string getKey();
		void setKey(string key);
		int getHash(int M);
		friend istream& operator>> (istream& in, Record& right);
		friend ostream& operator<< (ostream& out, const Record& right);
	private:
		string name; string id_number;
		int rank; double GPA; string major;
};

string Record::getKey() {
	return name;
}
void Record::setKey(string key) {
	name = key;
}

int Record::getHash(int M) {
	string key = getKey();
	int index = 0;
	for (int i=0; i<key.length(); i++)
		index += (int) key[i];
	index = index%M;
	return index;
}

template <typename T,typename K>
class HashTable {
	public:
		HashTable(int tableSize);
		void insert(T newRecord);
		T* find(K key);
		void erase(T* pos);
		template <typename T,typename K>
		friend ostream& operator<< (ostream& out, const HashTable& right);
	private:
		vector< vector<T> > table;
};

template <typename T,typename K>
T* HashTable<T,K>::find(K key) {
	T tempRecord;
	tempRecord.setKey(key);
	int index = tempRecord.getHash(table.size());
	for (int i=0; i<table[index].size(); i++)
		if (table[index][i].getKey() == key)
			return &table[index][i];
	return NULL;
}
//cout << *(myHashTable.find("Luke Skywalker"));


template <typename T,typename K>
void HashTable<T,K>::erase(T* pos) {
	if (pos == NULL) return;
	int index = pos->getHash(table.size());
	int i=0;
	while (&table[index][i] != pos && i < table[index].size())
		i++;
	for (int j=i; j<table[index].size()-1; j++)
		table[index][j] = table[index][j+1];
	table[index].pop_back();
}
//myHashTable.erase( myHashTable.find("Luke Skywalker") );

template <typename T,typename K>
void HashTable<T,K>::insert(T newRecord) {
	int index = newRecord.getHash(table.size());
	table[index].push_back(newRecord);
}

template <typename T,typename K>
ostream& operator<< (ostream& out, const HashTable<T,K>& right) {
	for (int i=0; i < right.table.size(); i++)
		for (int j=0; j < right.table[i].size(); j++)
			out << "Bucket " << i << ", Record " << j
			<< "\n" << right.table[i][j] << "\n\n";
	return out;
}


//cout << myHashTable

int main() {
	HashTable<Record,string> myHash(50);
	ifstream fin;
	fin.open("students.txt");
	Record newStudent;
	string blank_line;
	while (fin >> newStudent) {
		myHash.insert(newStudent);
		getline(fin,blank_line);
	}
	fin.close();
	myHash.erase(myHash.find("Luke Skywalker"));
	cout << myHash;
}
