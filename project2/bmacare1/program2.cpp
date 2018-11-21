#include <iostream>
#include <string>
#include <ctime>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <stdio.h>
#include <cmath>

using namespace std;

int directMap(vector<unsigned long> br, vector<string> flag, int size, int total){
	int totalHits = 0;
	int cacheSets = size*32;
	int index = -1;
	vector<unsigned long> mem;
	unsigned long push = 0;
	for(int i = 0; i < cacheSets; i++){
		mem.push_back(push);
	}
	unsigned long cB; //cB = currentBranch
	for(int x = 0; x < total; x++){
		cB = br[x];
		cB = cB >> 5;
		index = cB%cacheSets;
		if(mem[index] == 0){
			mem[index] = cB;
		} else if(mem[index] != cB){
			mem[index] = cB;
		} else{
			totalHits++;
		}
	}
	return totalHits;
}

int setAssociative(vector<unsigned long> br, vector<string> flag, int ways, int total){
	int totalHits = 0;
	int cacheSets = 16*32/ways;
	int index = -1;
	unsigned long mem[cacheSets][ways];
	unsigned long push = 0;
	for(int i = 0; i < cacheSets; i++){
		for(int j = 0; j < ways; j++){
			mem[i][j] = 0;
		}
	}
	unsigned long cB; //currentBranch
	for(int x = 0; x < total; x++){
		cB = br[x];
		cB = cB >> 5;
		index = cB%cacheSets;

		//LRU; 0 = Most Recent ---> Ways-1 = Least Recent
		unsigned long MRU; //Most recently used branch
		int MRUi; //Most recently used index
		int g;
		for(g = 0; g < ways; g++){
			if(mem[index][g] == cB){
				totalHits++;
				MRU = cB;
				MRUi = g;
				break;
			}
			if(g == ways-1){
				mem[index][g] = cB;
				MRU = cB;
				MRUi = g;
			}
		}
		for(int f = MRUi; f > 0; f--){
			mem[index][f] = mem[index][f-1];
		}
		mem[index][0] = MRU;
	}
	return totalHits;
}

int fullAssociativeLRU(vector<unsigned long> br, vector<string> flag, int total){
	int totalHits = 0;
	int cacheSets = 16*32;
	int index = -1;
	unsigned long mem[cacheSets];
	for(int i = 0; i < cacheSets; i++){
		mem[i] = 0;
	}
	unsigned long cB; //currentBranch
	for(int x = 0; x < total; x++){
		cB = br[x];
		cB = cB >> 5;
		unsigned long MRU;
		int MRUi;
		int g;
		for(g = 0; g < cacheSets; g++){
			if(mem[g] == cB){
				totalHits++;
				MRU = cB;
				MRUi = g;
				break; 
			}
			if(g == cacheSets-1){
				MRU = cB;
				MRUi = g;
			} 
		}
		for(int f = MRUi; f > 0; f--){
			mem[f] = mem[f-1];
		}
		mem[0] = MRU;
	}
	return totalHits;
}

int fullAssociativeHCLRU(vector<unsigned long> br, vector<string> flag, int total){
	int totalHits = 0;
	int cacheSets = 16*32;
	int index = -1;
	unsigned long mem[cacheSets];
	unsigned long hotCold[cacheSets-1];
	for(int i = 0; i < cacheSets; i++){
		mem[i] = 0;
	}
	for(int i = 0; i < cacheSets-1; i++){
		hotCold[i] = 1;
	}
	unsigned long cB; //currentBranch
	unsigned long MRU;
	int findLRU[] = {128, 64, 32, 16, 8, 4, 2, 1};
	//0 ---> LEFT IS HOT, GO RIGHT
	//1 ---> RIGHT IS HOT, GO LEFT
	for(int x = 0; x < total; x++){
		cB = br[x];
		cB = cB >> 5;
		for(int i = 0; i < cacheSets; i++){
			if(mem[i] == cB){
				totalHits++;
				int value = 255; //Checking where the hot value is
				for(int j = 0; j < 8; j++){
					if(i > value){
						hotCold[value] = 1;
						value+=findLRU[j];
					} else{
						hotCold[value] = 0;
						value-=findLRU[j];
					}
				}
				if(i > value){
					hotCold[value] = 1;
				} else{
					hotCold[value] = 0;
				}
				break;
			}
			if(i == cacheSets-1){
				MRU = cB;
				index = 255;
				for(int j = 0; j < 8; j++){
					if(hotCold[index] == 0){
						hotCold[index] = 1;
						index+=findLRU[j];
					} else{
						hotCold[index] = 0;
						index-=findLRU[j];
					}
				}
				if(hotCold[index] == 1){
					hotCold[index] = 0;
				} else{
					hotCold[index] = 1;
					index++;
				}
				mem[index] = MRU;
			}
		}
	}
	return totalHits;
}

int setAssociativeWriteMiss(vector<unsigned long> br, vector<string> flag, int ways, int total){
	int totalHits = 0;
	int cacheSets = 16*32/ways;
	int index = -1;
	unsigned long mem[cacheSets][ways];
	unsigned long push = 0;
	for(int i = 0; i < cacheSets; i++){
		for(int j = 0; j < ways; j++){
			mem[i][j] = 0;
		}
	}
	unsigned long cB; //currentBranch
	for(int x = 0; x < total; x++){
		cB = br[x];
		cB = cB >> 5;
		index = cB%cacheSets;

		//LRU; 0 = Most Recent ---> Ways-1 = Least Recent
		unsigned long MRU; //Most recently used branch
		int MRUi; //Most recently used index
		int g;
		if(flag[x].compare("L") == 0){
			for(g = 0; g < ways; g++){
				if(mem[index][g] == cB){
					totalHits++;
					MRU = cB;
					MRUi = g;
					break;
				}
				if(g == ways-1){
					mem[index][g] = cB;
					MRU = cB;
					MRUi = g;
				}
			}
			for(int f = MRUi; f > 0; f--){
				mem[index][f] = mem[index][f-1];
			}
			mem[index][0] = MRU;
		} else{
			for(g = 0; g < ways; g++){
				if(mem[index][g] == cB){
					totalHits++;
					for(int f = g; f > 0; f--){
						mem[index][f] = mem[index][f-1];
					}
					mem[index][0] = cB;
					break;
				}
			}
		}
	}
	return totalHits;
}

int nextLinePrefetch(vector<unsigned long> br, vector<string> flag, int ways, int total){
	int totalHits = 0;
	int cacheSets = 16*32/ways;
	int index = -1;
	unsigned long mem[cacheSets][ways];
	unsigned long push = 0;
	for(int i = 0; i < cacheSets; i++){
		for(int j = 0; j < ways; j++){
			mem[i][j] = 0;
		}
	}
	unsigned long cB; //currentBranch
	for(int x = 0; x < total; x++){
		cB = br[x];
		cB = cB >> 5;
		index = cB%cacheSets;

		//LRU; 0 = Most Recent ---> Ways-1 = Least Recent
		unsigned long MRU; //Most recently used branch
		int MRUi; //Most recently used index
		int g;
		for(g = 0; g < ways; g++){
			if(mem[index][g] == cB){
				totalHits++;
				MRU = cB;
				MRUi = g;
				break;
			}
			if(g == ways-1){
				mem[index][g] = cB;
				MRU = cB;
				MRUi = g;
			}
		}
		for(int f = MRUi; f > 0; f--){
			mem[index][f] = mem[index][f-1];
		}
		mem[index][0] = MRU;

		cB = br[x]+32;
		cB = cB >> 5;
		index = cB%cacheSets;
		for(g = 0; g < ways; g++){
			if(mem[index][g] == cB){
				//totalHits++;
				MRU = cB;
				MRUi = g;
				for(int f = MRUi; f > 0; f--){
					mem[index][f] = mem[index][f-1];
				}
				mem[index][0] = MRU;
				break;
			}
			if(g == ways-1){
				mem[index][g] = cB;
				MRU = cB;
				MRUi = g;
				for(int f = MRUi; f > 0; f--){
					mem[index][f] = mem[index][f-1];
				}
				mem[index][0] = MRU;
				//mem[index][ways-1] = MRU;
			}
		}
	}
	return totalHits;
}

int missPrefetch(vector<unsigned long> br, vector<string> flag, int ways, int total){
	int totalHits = 0;
	int cacheSets = 16*32/ways;
	int index = -1;
	unsigned long mem[cacheSets][ways];
	unsigned long push = 0;
	for(int i = 0; i < cacheSets; i++){
		for(int j = 0; j < ways; j++){
			mem[i][j] = 0;
		}
	}
	unsigned long cB; //currentBranch
	for(int x = 0; x < total; x++){
		cB = br[x];
		cB = cB >> 5;
		index = cB%cacheSets;
		bool miss = true;
		//LRU; 0 = Most Recent ---> Ways-1 = Least Recent
		unsigned long MRU; //Most recently used branch
		int MRUi; //Most recently used index
		int g;
		for(g = 0; g < ways; g++){
			if(mem[index][g] == cB){
				totalHits++;
				MRU = cB;
				MRUi = g;
				miss = false;
				break;
			}
			if(g == ways-1){
				mem[index][g] = cB;
				MRU = cB;
				MRUi = g;
			}
		}
		for(int f = MRUi; f > 0; f--){
			mem[index][f] = mem[index][f-1];
		}
		mem[index][0] = MRU;

		if(miss == true){
			cB = br[x]+32;
			cB = cB >> 5;
			index = cB%cacheSets;
			for(g = 0; g < ways; g++){
				if(mem[index][g] == cB){
					//totalHits++;
					MRU = cB;
					MRUi = g;
					for(int f = MRUi; f > 0; f--){
						mem[index][f] = mem[index][f-1];
					}
					mem[index][0] = MRU;
					break;
				}
				if(g == ways-1){
					mem[index][g] = cB;
					MRU = cB;
					MRUi = g;
					for(int f = MRUi; f > 0; f--){
						mem[index][f] = mem[index][f-1];
					}
					mem[index][0] = MRU;
					//mem[index][ways-1] = MRU;
				}
			}
		}
	}
	return totalHits;
}

int main(int argc, char *argv[]){
	/*

	READ INPUTS + STORE FILE INFORMATION
	Puts all the addresses for each branch into the vector 'branchAddress'
	Stores S/T into 'flags'
	*/
	string inputFile = argv[1];
	string outputFile = argv[2];

	string data;
	//Make from string to hex eventually
	vector<unsigned long> branchAddress;
	vector<string> flags;

	int totalBranches = 0;
	ifstream file;
	file.open(inputFile);
	if(file.is_open()){
		while(file.good()){
			if(!(getline(file, data))){
				break;
			}
			istringstream buf(data);
			istream_iterator<string> beg(buf), end;
			vector<string> splitUp(beg, end);
			unsigned long a;
			istringstream(splitUp[1]) >> hex >> a;
			branchAddress.push_back(a); //FIND OUT HOW TO CONVERT THIS PROPERLY
			flags.push_back(splitUp[0]);
			totalBranches++;
		}
	}
	file.close();

	ofstream output;
	output.open(outputFile);

	output << directMap(branchAddress, flags, 1, totalBranches) << "," << totalBranches << "; ";
	output << directMap(branchAddress, flags, 4, totalBranches) << "," << totalBranches << "; ";
	output << directMap(branchAddress, flags, 16, totalBranches) << "," << totalBranches << "; ";
	output << directMap(branchAddress, flags, 32, totalBranches) << "," << totalBranches << ";\n";

	output << setAssociative(branchAddress, flags, 2, totalBranches) << "," << totalBranches << "; ";
	output << setAssociative(branchAddress, flags, 4, totalBranches) << "," << totalBranches << "; ";
	output << setAssociative(branchAddress, flags, 8, totalBranches) << "," << totalBranches << "; ";
	output << setAssociative(branchAddress, flags, 16, totalBranches) << "," << totalBranches << ";\n";

	output << fullAssociativeLRU(branchAddress, flags, totalBranches) << "," << totalBranches << ";\n";

	output << fullAssociativeHCLRU(branchAddress, flags, totalBranches) << "," << totalBranches << ";\n";

	output << setAssociativeWriteMiss(branchAddress, flags, 2, totalBranches) << "," << totalBranches << "; ";
	output << setAssociativeWriteMiss(branchAddress, flags, 4, totalBranches) << "," << totalBranches << "; ";
	output << setAssociativeWriteMiss(branchAddress, flags, 8, totalBranches) << "," << totalBranches << "; ";
	output << setAssociativeWriteMiss(branchAddress, flags, 16, totalBranches) << "," << totalBranches << ";\n";

	output << nextLinePrefetch(branchAddress, flags, 2, totalBranches) << "," << totalBranches << "; ";
	output << nextLinePrefetch(branchAddress, flags, 4, totalBranches) << "," << totalBranches << "; ";
	output << nextLinePrefetch(branchAddress, flags, 8, totalBranches) << "," << totalBranches << "; ";
	output << nextLinePrefetch(branchAddress, flags, 16, totalBranches) << "," << totalBranches << ";\n";

	output << missPrefetch(branchAddress, flags, 2, totalBranches) << "," << totalBranches << "; ";
	output << missPrefetch(branchAddress, flags, 4, totalBranches) << "," << totalBranches << "; ";
	output << missPrefetch(branchAddress, flags, 8, totalBranches) << "," << totalBranches << "; ";
	output << missPrefetch(branchAddress, flags, 16, totalBranches) << "," << totalBranches << ";\n";

	output.close();
	return 0;
}








