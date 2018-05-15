#include <iostream>
#include <string>
#include <fstream>
#include "merge_list.h"

using namespace std;

// Ez a függvény beolvassa az adatokat egy forrásfájlból.
void read_from_file(const std::string& file_name, std::vector<int>& v1, std::vector<int>& v2){
	unsigned count1, count2;
	ifstream file;
	file.open(file_name);
	if(file.is_open()){
		file >> count1 >> count2;
		for(unsigned i = 0; i < count1; i++){
			int x;
			file >> x;
			v1.push_back(x);
		}
		for(unsigned i = 0; i < count2; i++){
			int x;
			file >> x;
			v2.push_back(x);
		}
		file.close();
	} else{
		throw new std::runtime_error("File could not be opened!");
	}
}

// Ez a függvény kiírja a képernyőre a használathoz szükséges segítséget.
void printHelp(){
	cout << endl;
	cout << "Usage: ./inplacemergesort <text file name>" << endl;
}

int main(int argc, char *argv[]){
	if(argc != 2){
		printHelp();
		return 1;
	}
	std::vector<int> part1, part2;
	try{
		read_from_file(std::string(argv[1]), part1, part2);
	} catch(const std::runtime_error& err){
		cout << err.what() << endl;
		return 1;
	}


	return 0;
}