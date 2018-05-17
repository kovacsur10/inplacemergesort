#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include "merge_list.h"

using namespace std;

// Ez a függvény beolvassa az adatokat egy forrásfájlból.
// Paraméterek:
//     file_name: a beolvasandó fájl neve
//     v1: az első részvektor
//     v2: a második részvektor
// Visszatérési érték: -
// Kivétel: Ha a fájl nem megnyitható, akkor egy runtime_error kiváltódik.
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
		throw std::runtime_error("File could not be opened! "+file_name);
	}
}

vector<int> trivial_merging(const vector<int>& v1, const vector<int>& v2){
	auto it1 = v1.begin();
	auto it2 = v2.begin();
	vector<int> v_ret;
	for(unsigned i = 0; i < v1.size() + v2.size(); ++i){
		if(it1 == v1.end() || (it2 != v2.end() && *it2 < *it1)){
			v_ret.push_back(*it2);
			it2++;
		} else{
			v_ret.push_back(*it1);
			it1++;
		}
	}
	return v_ret;
}

// Ez a függvény kiírja a képernyőre a használathoz szükséges segítséget.
void printHelp(){
	cout << endl;
	cout << "Usage: ./inplacemergesort <text file name>" << endl;
	cout << "Usage: ./inplacemergesort <text file name> <first len> <second len>" << endl;
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	if(argc != 2 && argc != 4){
		printHelp();
		return 1;
	}

	if(argc == 4){
		// Input generation.
		int first_len, second_len;
		istringstream ss(argv[2]);
		if(!(ss >> first_len)){
			cout << "Invalid number " << argv[2] << " as parameter(1)!" << endl;
			return 1;
		}
		istringstream ss2(argv[3]);
		if(!(ss2 >> second_len)){
			cout << "Invalid number " << argv[3] << " as parameter(2)!" << endl;
			return 1;
		}
		ofstream file(argv[1]);
		if(file.is_open()){
			file << first_len << " " << second_len << endl;
			// First line
			vector<int> vect;
			for(unsigned i = 0; i < first_len; i++){
				vect.push_back(rand() % 10000000);
			}
			sort(vect.begin(), vect.end());
			for(const int e : vect){
				file << e << " ";
			}
			file << endl;
			// Second line
			vect.clear();
			for(unsigned i = 0; i < second_len; i++){
				vect.push_back(rand() % 10000000);
			}
			sort(vect.begin(), vect.end());
			for(const int e : vect){
				file << e << " ";
			}
			file.close();
		} else{
			cout << "Cannot open the output file!" << endl;
			return 1;
		}
	}else{
		// Running the algorithm.
		std::vector<int> part1, part2;
		try{
			read_from_file(std::string(argv[1]), part1, part2);
		} catch(const std::runtime_error& err){
			cout << "Error happened: " << err.what() << endl;
			return 1;
		}

		chrono::system_clock::time_point start_time;
		chrono::system_clock::time_point end_time;
		chrono::nanoseconds inplace_dur;
		chrono::nanoseconds trivial_dur;
		chrono::nanoseconds normal_dur;

		merge_list list(part1, part2);
		start_time = chrono::system_clock::now();
		list.merge();
		end_time = chrono::system_clock::now();
		inplace_dur = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

		vector<int> test;
		start_time = chrono::system_clock::now();
		merge(part1.begin(), part1.end(), part2.begin(), part2.end(), back_inserter(test));
		end_time = chrono::system_clock::now();
		normal_dur = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

		vector<int> test_trivial;
		start_time = chrono::system_clock::now();
		test_trivial = trivial_merging(part1, part2);
		end_time = chrono::system_clock::now();
		trivial_dur = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

		if(list.verify()){
			cout << "Merging finished successfully!" << endl;
		} else{
			cout << "Merging finished with failing!" << endl;
		}

		cout << "Elapsed time (in-place): " << inplace_dur.count() << endl;
		cout << "Elapsed time (std::merge): " << normal_dur.count() << endl;
		cout << "  Ratio: ";
		if(normal_dur.count() == 0){
			cout << (inplace_dur.count() == 0 ? "1" : "infinity") << endl;
		} else{
			cout << (inplace_dur.count() / (double) normal_dur.count()) << endl;
		}
		cout << "Elapsed time (trivial): " << trivial_dur.count() << endl;
		cout << "  Ratio: ";
		if(trivial_dur.count() == 0){
			cout << (inplace_dur.count() == 0 ? "1" : "infinity") << endl;
		} else{
			cout << (inplace_dur.count() / (double) trivial_dur.count()) << endl;
		}
	}
	cout << "The program quits..." << endl;
	return 0;
}