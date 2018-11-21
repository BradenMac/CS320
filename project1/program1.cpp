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

int singleBit(vector<unsigned long> br, vector<string> out, int size, int total){
	vector<string> predictions;
	for(int i = 0; i < size; i++){
		predictions.push_back("T"); //Sets all predictions initally to Taken
	}
	int totalCorrect = 0;
	string prediction;
	int index;
	for(int x = 0; x < total; x++){
		index = (br[x])%size;
		prediction = predictions[index];
		if(prediction == out[x]){
			totalCorrect++;
		} else if(prediction == "T"){
			predictions[index] = "NT";
		} else{
			predictions[index] = "T";
		}
	}
	return totalCorrect;
}

int doubleBit(vector<unsigned long> br, vector<string> out, int size, int total){
	vector<int> predictions;
	for(int i = 0; i < size; i++){
		predictions.push_back(3); //Sets all preditions initially to Strongly Taken
	}
	int totalCorrect = 0;
	int prediction;
	int index;
	for(int x = 0; x < total; x++){
		index = (br[x])%size;
		prediction = predictions[index];
		if(prediction == 3 && out[x] == "T"){
			totalCorrect++;
		} else if(prediction == 2 && out[x] == "T"){
			totalCorrect++;
			predictions[index]++;
		} else if(prediction == 1 && out[x] == "NT"){
			totalCorrect++;
			predictions[index]--;
		} else if(prediction == 0 && out[x] == "NT"){
			totalCorrect++;
		} else if(prediction == 0 || prediction == 1){
			predictions[index]++;
		} else{
			predictions[index]--;
		}
	}
	return totalCorrect;
}

int gShare(vector<unsigned long> br, vector<string> out, int size, int total){
	vector<int> predictions;
	for(int i = 0; i < 2048; i++){
		predictions.push_back(3); //Sets all preditions initially to Strongly Taken
	}
	vector<int> gHist; //Global History Register
	for(int j = 0; j < size; j++){
		gHist.push_back(0);
	}
	int globalReg = 0;
	int mod;
	int index;
	int prediction;
	int totalCorrect = 0;
	for(int x = 0; x < total; x++){
		globalReg = 0;
		for(int z = 0; z < size; z++){
			if(gHist[z] == 1){
				globalReg += pow(2,z);
			}
		}
		mod = (br[x])%2048;
		index = mod ^ globalReg;
		prediction = predictions[index];
		if(prediction == 3 && out[x] == "T"){
			totalCorrect++;
		} else if(prediction == 2 && out[x] == "T"){
			totalCorrect++;
			predictions[index]++;
		} else if(prediction == 1 && out[x] == "NT"){
			totalCorrect++;
			predictions[index]--;
		} else if(prediction == 0 && out[x] == "NT"){
			totalCorrect++;
		} else if(prediction == 0 || prediction == 1){
			predictions[index]++;
		} else{
			predictions[index]--;
		}

		int newHistory = 0;
		if(out[x] == "T"){
			newHistory = 1;
		}
		for(int c = (size-1); c > 0; c--){
			gHist[c] = gHist[c-1];
		}
		gHist[0] = newHistory;
	}
	return totalCorrect;
}

int main(int argc, char *argv[]){
	/*

	READ INPUTS + STORE FILE INFORMATION
	Puts all the addresses for each branch into the vector 'branchAddress'
	In the correspdoning index, the outcome of the branch (T/NT) is stored in the vector 'outcome'
	*/
	string inputFile = argv[1];
	string outputFile = argv[2];

	string data;
	//Make from string to hex eventually
	vector<unsigned long> branchAddress;
	vector<string> outcomes;

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
			istringstream(splitUp[0]) >> hex >> a;
			branchAddress.push_back(a); //FIND OUT HOW TO CONVERT THIS PROPERLY
			outcomes.push_back(splitUp[1]);
			totalBranches++;
		}
	}
	file.close();

	ofstream output;
	output.open(outputFile);
	/*

	ALWAYS TAKEN

	*/
	int correct = 0;
	for(int x = 0; x < outcomes.size(); x++){
		if(outcomes[x] == "T"){
			correct++;
		}
	}
	output << correct << "," << totalBranches << ";\n";

	/*

	ALWAYS NOT TAKEN


	*/
	correct = 0;
	for(int x = 0; x < outcomes.size(); x++){
		if(outcomes[x] == "NT"){
			correct++;
		}
	}
	output << correct << "," << totalBranches << ";\n";

	/*

	SINGLE BIT BIMODAL

	*/
	output << singleBit(branchAddress, outcomes, 16, totalBranches) << "," << totalBranches << "; ";
	output << singleBit(branchAddress, outcomes, 32, totalBranches) << "," << totalBranches << "; ";
	output << singleBit(branchAddress, outcomes, 128, totalBranches) << "," << totalBranches << "; ";
	output << singleBit(branchAddress, outcomes, 256, totalBranches) << "," << totalBranches << "; ";
	output << singleBit(branchAddress, outcomes, 512, totalBranches) << "," << totalBranches << "; ";
	output << singleBit(branchAddress, outcomes, 1024, totalBranches) << "," << totalBranches << "; ";
	output << singleBit(branchAddress, outcomes, 2048, totalBranches) << "," << totalBranches << ";\n";

	/*

	DOUBLE BIT BIMODAL

	*/
	output << doubleBit(branchAddress, outcomes, 16, totalBranches) << "," << totalBranches << "; ";
	output << doubleBit(branchAddress, outcomes, 32, totalBranches) << "," << totalBranches << "; ";
	output << doubleBit(branchAddress, outcomes, 128, totalBranches) << "," << totalBranches << "; ";
	output << doubleBit(branchAddress, outcomes, 256, totalBranches) << "," << totalBranches << "; ";
	output << doubleBit(branchAddress, outcomes, 512, totalBranches) << "," << totalBranches << "; ";
	output << doubleBit(branchAddress, outcomes, 1024, totalBranches) << "," << totalBranches << "; ";
	output << doubleBit(branchAddress, outcomes, 2048, totalBranches) << "," << totalBranches << ";\n";
	
	/*

	GSHARE

	*/
	output << gShare(branchAddress, outcomes, 3, totalBranches) << "," << totalBranches << "; ";
	output << gShare(branchAddress, outcomes, 4, totalBranches) << "," << totalBranches << "; ";
	output << gShare(branchAddress, outcomes, 5, totalBranches) << "," << totalBranches << "; ";
	output << gShare(branchAddress, outcomes, 6, totalBranches) << "," << totalBranches << "; ";
	output << gShare(branchAddress, outcomes, 7, totalBranches) << "," << totalBranches << "; ";
	output << gShare(branchAddress, outcomes, 8, totalBranches) << "," << totalBranches << "; ";
	output << gShare(branchAddress, outcomes, 9, totalBranches) << "," << totalBranches << "; ";
	output << gShare(branchAddress, outcomes, 10, totalBranches) << "," << totalBranches << "; ";
	output << gShare(branchAddress, outcomes, 11, totalBranches) << "," << totalBranches << ";\n";

	/*

	TOURNAMENT

	*/
	correct = 0;
	vector<int> predictionsDB;
	vector<int> predictionsG;
	vector<int> predictionsT;
	int totalCorrect = 0;
	int predictionDB;
	int predictionG;
	int predictionT;
	int index;
	int globalReg = 0;
	int mod;
	for(int i = 0; i < 2048; i++){
		predictionsT.push_back(0); //Sets all predictions initially to Strongly Prefer GShare
	}
	for(int i = 0; i < 2048; i++){
		predictionsDB.push_back(3); //Sets all preditions initially to Strongly Taken
	}
	for(int i = 0; i < 2048; i++){
		predictionsG.push_back(3); //Sets all preditions initially to Strongly Taken
	}
	vector<int> gHist; //Global History Register
	for(int j = 0; j < 11; j++){
		gHist.push_back(0);
	}
	
	bool DBCorrect = false;
	bool GCorrect = false;
	for(int x = 0; x < totalBranches; x++){
		DBCorrect = false;
		GCorrect = false;
		//Gshare portion
		globalReg = 0;
		for(int z = 0; z < 11; z++){
			if(gHist[z] == 1){
				globalReg += pow(2,z);
			}
		}
		mod = (branchAddress[x])%2048;
		index = mod ^ globalReg;
		predictionG = predictionsG[index];
		if(predictionG == 3 && outcomes[x] == "T"){
			GCorrect = true;
		} else if(predictionG == 2 && outcomes[x] == "T"){
			GCorrect = true;
			predictionsG[index]++;
		} else if(predictionG == 1 && outcomes[x] == "NT"){
			GCorrect = true;
			predictionsG[index]--;
		} else if(predictionG == 0 && outcomes[x] == "NT"){
			GCorrect = true;
		} else if(predictionG == 0 || predictionG == 1){
			predictionsG[index]++;
		} else{
			predictionsG[index]--;
		}

		int newHistory = 0;
		if(outcomes[x] == "T"){
			newHistory = 1;
		}
		for(int c = 10; c > 0; c--){
			gHist[c] = gHist[c-1];
		}
		gHist[0] = newHistory;

		//Double bit
		index = (branchAddress[x])%2048;
		predictionDB = predictionsDB[index];
		if(predictionDB == 3 && outcomes[x] == "T"){
			DBCorrect = true;
		} else if(predictionDB == 2 && outcomes[x] == "T"){
			DBCorrect = true;
			predictionsDB[index]++;
		} else if(predictionDB == 1 && outcomes[x] == "NT"){
			DBCorrect = true;
			predictionsDB[index]--;
		} else if(predictionDB == 0 && outcomes[x] == "NT"){
			DBCorrect = true;
		} else if(predictionDB == 0 || predictionDB == 1){
			predictionsDB[index]++;
		} else{
			predictionsDB[index]--;
		}

		//Tournament Prediction
		predictionT = predictionsT[index];
		if(predictionT == 0){ //Strong GShare
			if(GCorrect == true && DBCorrect == true){
				totalCorrect++;
			} else if(GCorrect == true && DBCorrect == false){
				totalCorrect++;
			} else if(GCorrect == false && DBCorrect == true){
				predictionsT[index]++;
			}
		} else if(predictionT == 1){ //GShare
			if(GCorrect == true && DBCorrect == true){
				totalCorrect++;
			} else if(GCorrect == true && DBCorrect == false){
				totalCorrect++;
				predictionsT[index]--;
			} else if(GCorrect == false && DBCorrect == true){
				predictionsT[index]++;
			}
		} else if(predictionT == 2){ //Double Bit
			if(GCorrect == true && DBCorrect == true){
				totalCorrect++;
			} else if(GCorrect == true && DBCorrect == false){
				predictionsT[index]--;
			} else if(GCorrect == false && DBCorrect == true){
				totalCorrect++;
				predictionsT[index]++;
			}
		} else{ //Strong Double Bit
			if(GCorrect == true && DBCorrect == true){
				totalCorrect++;
			} else if(GCorrect == true && DBCorrect == false){
				predictionsT[index]--;
			} else if(GCorrect == false && DBCorrect == true){
				totalCorrect++;
			}
		}	
	}

	output << totalCorrect << "," << totalBranches << ";\n";

	/*
	
	Close Output

	*/
	output.close();
	return 0;
}
