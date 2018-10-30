#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <time.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <map>
using namespace std;


const int array_size = 5000;

struct Genome
{
	double fitness;
	double Xcor[array_size];
	double Ycor[array_size];
	double Zcor[array_size];

};

template <typename T>
string NumberToString(T pNumber)
{
	ostringstream oOStrStream;
	oOStrStream << pNumber;
	return oOStrStream.str();
}

string createPDBFromChromosomeAfterEachGeneration(Genome &geno, string id, int generations, int i, string fastaSeq, int amino_start_index, int chromo_len);
void createIndividualModelsFromCombinedStructure(string id, int generation, int chromo_index, string op_file_name, vector<string> fastas_vec);

int main(int argc, char* argv[]){

	// pass the id from command line;

	if (argc < 2){

		cout << "please provide the protein id as an argument" << endl;
		exit(0);

	}

	string id = argv[1];



	string createDirCmd = "mkdir ../" + id;
	int sysRet = system(createDirCmd.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful ... terminating " << endl;

		exit(EXIT_FAILURE);

	}

	/* read the fasta file and parse fasta line */
	string fastaFilePath = "../input/" + id + ".fasta";
	ifstream fastaFileStream(fastaFilePath.c_str());
	string fastaLine;
	string fastaSeq;
	if (fastaFileStream.is_open()){

		getline(fastaFileStream, fastaLine); // ignore the header line that starts with >
		getline(fastaFileStream, fastaLine);
		fastaSeq = fastaLine;
		fastaFileStream.close();
	}

	cout << "fasta file read successful" << endl;

	/* read the fasta file for complex and save the number of fastas in array list */
	string fastacomplexFilePath = "../input/" + id + "_comp.fasta";
	ifstream fastacomplexFileStream(fastacomplexFilePath.c_str());
	string fastacompLine;
	vector<string> fastas_vec;

	if (fastacomplexFileStream.is_open()){

		while (getline(fastacomplexFileStream, fastacompLine)){

			if (fastacompLine[0] == '>'){

				continue;

			}
			else{

				fastas_vec.push_back(fastacompLine);

			}


		}

		fastacomplexFileStream.close();

	}

	cout << "fasta complex file read successful" << endl;
	cout << "number of models in complex " << fastas_vec.size() << endl;


	int pop_size = 0;
	string outFilePath = "../input/" + id + "_out.txt";
	cout << "file Path " << outFilePath << endl;
	ifstream readFile(outFilePath.c_str());
	string outLine;
	string prot_id;
	int chromo_len = 0;
	int amino_start_index = 0;

	if (readFile.is_open()){

		string line;
		string value;
		getline(readFile, outLine); // ignore the line

		for (int i = 0; i < 4; i++){

			getline(readFile, outLine);
			line = outLine;
			istringstream str(line);
			str >> value;

			if (value == "TARGET"){

				str >> prot_id;

			}
			else if (value == "pop_size"){

				str >> pop_size;

			}
			else if (value == "chromo_len"){

				str >> chromo_len;

			}
			else if (value == "amino_start_index"){

				str >> amino_start_index;

			}

		}

		cout << "prot_id " << prot_id << endl;
		cout << "pop_size " << pop_size << endl;
		cout << "chromo_len " << chromo_len << endl;
		cout << "amino_start_index" << amino_start_index << endl;




		while (!readFile.eof()){

			int generation = 0;
			float obtained_fitness = 0;
			getline(readFile, outLine);

			if (outLine.length() == 0){

				continue;
			}

			line = outLine;
			istringstream str(line);
			str >> value;
			if (value == "Generations"){

				str >> generation;

			}

			cout << "generation " << generation << endl;


			for (int i = 0; i < pop_size; i++){

				getline(readFile, outLine);
				line = outLine;
				istringstream str(line);
				getline(str, value, ',');
				char* pEnd;
				// convert string to float
				obtained_fitness = strtof(value.c_str(), &pEnd);

				Genome geno;

				geno.fitness = obtained_fitness;

				for (int j = 0; j < chromo_len; j++){

					getline(str, value, ',');
					char *end1;
					// convert string to float
					float x_cor = strtof(value.c_str(), &end1);

					getline(str, value, ',');

					char *end2;
					// convert string to float
					float y_cor = strtof(value.c_str(), &end2);

					getline(str, value, ',');

					char *end3;
					// convert string to float
					float z_cor = strtof(value.c_str(), &end3);

					geno.Xcor[j] = x_cor;
					geno.Ycor[j] = y_cor;
					geno.Zcor[j] = z_cor;


				}

				string oscar_op_file_name = createPDBFromChromosomeAfterEachGeneration(geno, id, generation, i, fastaSeq, amino_start_index, chromo_len);

				createIndividualModelsFromCombinedStructure(id, generation, i, oscar_op_file_name, fastas_vec);


			}


		}


		readFile.close();

	}

	return 0;

}

void createIndividualModelsFromCombinedStructure(string id, int generation, int chromo_index, string oscar_op_file_name, vector<string> fastas_vec){

	map<int, string> chain_map;
	chain_map.insert(pair<int, string>(1, "A"));
	chain_map.insert(pair<int, string>(2, "B"));
	chain_map.insert(pair<int, string>(3, "C"));
	chain_map.insert(pair<int, string>(4, "D"));
	chain_map.insert(pair<int, string>(5, "E"));
	chain_map.insert(pair<int, string>(6, "F"));
	chain_map.insert(pair<int, string>(7, "G"));
	chain_map.insert(pair<int, string>(8, "H"));
	chain_map.insert(pair<int, string>(9, "I"));
	chain_map.insert(pair<int, string>(10, "J"));

	// create combined structure file for CASP submission
	//string comb_stru_file = "../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + ".pdb";
	//ofstream comb_stru_file_stream(comb_stru_file.c_str());


	// from the fastas_vec read the fastas one by one to decide

	ifstream str_file_stream(oscar_op_file_name.c_str());
	string str_line;

	if (str_file_stream.is_open()){
		int model_num = 1;
		int model_start = 1;
		string deficient_line = "";
		int model_end = fastas_vec.at(0).length();
		for (int i = 0; i < fastas_vec.size(); i++){


			cout << "model_end" << model_end << endl;
			string model_path = "../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_M" + NumberToString(model_num) + ".pdb";
			ofstream op_model_stream(model_path.c_str());

			if (deficient_line.length() > 0 && model_num > 1){

				string substr_first = deficient_line.substr(0, 20);
				string substr_sec = deficient_line.substr(30, deficient_line.length());
				int res_ind = atoi(deficient_line.substr(23, 4).c_str()) - model_start + 1;
				string chain = chain_map.find(model_num)->second;
				op_model_stream << substr_first;
				op_model_stream << " " + chain;
				op_model_stream << setw(4) << fixed << res_ind;
				op_model_stream << "    ";
				op_model_stream << substr_sec << endl;
				op_model_stream.flush();

				/*
				comb_stru_file_stream << "PARENT N/A\r";
				comb_stru_file_stream << substr_first;
				comb_stru_file_stream << " " + chain;
				comb_stru_file_stream << setw(4) << fixed << res_ind;
				comb_stru_file_stream << "    ";
				comb_stru_file_stream << substr_sec << "  1.00" << endl;
				comb_stru_file_stream.flush();
				*/

			}

			while (getline(str_file_stream, str_line)){

				string str_cpy = str_line;
				string value;
				istringstream str(str_cpy);
				str >> value;

				cout << value << endl;


				if (value == "ATOM"){

					int res_ind = atoi(str_cpy.substr(23, 4).c_str());

					cout << res_ind << endl;


					if (res_ind >= model_start && res_ind <= model_end && model_num == 1){

						op_model_stream << str_cpy << endl;
						op_model_stream.flush();
						//cout << str_cpy << endl;
						/*
						comb_stru_file_stream << str_cpy << "  1.00" << "\r";
						comb_stru_file_stream.flush();
						*/

					}
					else if (res_ind >= model_start && res_ind <= model_end && model_num > 1){

						string substr_first = str_cpy.substr(0, 20);
						string substr_sec = str_cpy.substr(30, str_cpy.length());
						int res_ind = atoi(str_cpy.substr(23, 4).c_str()) - model_start + 1;
						string chain = chain_map.find(model_num)->second;
						op_model_stream << substr_first;
						op_model_stream << " " + chain;
						op_model_stream << setw(4) << fixed << res_ind;
						op_model_stream << "    ";
						op_model_stream << substr_sec << endl;
						op_model_stream.flush();

						/*
						comb_stru_file_stream << substr_first;
						comb_stru_file_stream << " " + chain;
						comb_stru_file_stream << setw(4) << fixed << res_ind;
						comb_stru_file_stream << "    ";
						comb_stru_file_stream << substr_sec << "  1.00" << endl;
						comb_stru_file_stream.flush();
						*/


					}
					else{

						if ((model_num + 1) <= fastas_vec.size()){

							int next_model_start = model_end + 20 + 1;

							if (res_ind < next_model_start){

								continue;

							}
							else{

								deficient_line = str_cpy;
								break;

							}

						}


					}

				}


			}

			op_model_stream << "TER\r";
			op_model_stream.close();
			//comb_stru_file_stream << "TER\r";
			model_start = model_end + 20 + 1;
			if ((i + 1) < fastas_vec.size()){

				model_end = model_end + 20 + fastas_vec.at(i + 1).length();

			}
			else{

				model_end = 0;

			}

			model_num++;

		}

		//comb_stru_file_stream << "END";
		str_file_stream.close();

	}

	// remove the full atomic model created by oscar-star2
	/*
	string removefullatomicmodel = "rm -f ../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_full.pdb";
	int sysRet = system(removefullatomicmodel.c_str());
	if (sysRet == -1){

	cout << "System Call Unsuccessful" << endl;
	exit(EXIT_FAILURE);

	}
	*/


}

string createPDBFromChromosomeAfterEachGeneration(Genome &geno, string id, int generation, int chromo_index, string fastaSeq, int amino_start_index, int chromo_len){

	string aaMapper[20][2];

	aaMapper[0][0] = "ALA";
	aaMapper[0][1] = "A";
	aaMapper[1][0] = "ARG";
	aaMapper[1][1] = "R";
	aaMapper[2][0] = "ASP";
	aaMapper[2][1] = "D";
	aaMapper[3][0] = "ASN";
	aaMapper[3][1] = "N";
	aaMapper[4][0] = "CYS";
	aaMapper[4][1] = "C";
	aaMapper[5][0] = "GLU";
	aaMapper[5][1] = "E";
	aaMapper[6][0] = "GLN";
	aaMapper[6][1] = "Q";
	aaMapper[7][0] = "GLY";
	aaMapper[7][1] = "G";
	aaMapper[8][0] = "HIS";
	aaMapper[8][1] = "H";
	aaMapper[9][0] = "ILE";
	aaMapper[9][1] = "I";
	aaMapper[10][0] = "LEU";
	aaMapper[10][1] = "L";
	aaMapper[11][0] = "LYS";
	aaMapper[11][1] = "K";
	aaMapper[12][0] = "MET";
	aaMapper[12][1] = "M";
	aaMapper[13][0] = "PHE";
	aaMapper[13][1] = "F";
	aaMapper[14][0] = "PRO";
	aaMapper[14][1] = "P";
	aaMapper[15][0] = "SER";
	aaMapper[15][1] = "S";
	aaMapper[16][0] = "THR";
	aaMapper[16][1] = "T";
	aaMapper[17][0] = "TRP";
	aaMapper[17][1] = "W";
	aaMapper[18][0] = "TYR";
	aaMapper[18][1] = "Y";
	aaMapper[19][0] = "VAL";
	aaMapper[19][1] = "V";

	// create a pdb file with backbone atoms
	string opBackbonePdbFile = "../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_bb.pdb";
	ofstream bbWriter(opBackbonePdbFile.c_str());
	int atomSerial = 1;
	if (bbWriter.is_open()){

		bbWriter << "FITNESS = " << geno.fitness << endl;
		bbWriter << "PFRMAT TS" << endl;
		bbWriter << "TARGET 1" << endl;
		bbWriter << "AUTHOR 0591-1752-4975" << endl;
		bbWriter << "METHOD Ab-Initio-PSP" << endl;
		bbWriter << "METHOD Ab-Initio-PSP" << endl;
		bbWriter << "MODEL  1" << endl;
		bbWriter << "PARENT N/A" << endl;
		bbWriter.flush();
		int chromo_pos = 0;

		while (chromo_pos < chromo_len){

			bbWriter << "ATOM  ";
			bbWriter.flush();
			// write atom serial number
			int number = atomSerial;
			int countDigits = 0;
			while (number != 0){

				number /= 10;
				++countDigits;

			}

			int totalAtomSerialWhiteSpaces = 5;

			string atomSerialWhiteSpaces;
			int requiredAtomSerialWhiteSpaces = totalAtomSerialWhiteSpaces - countDigits;

			for (int i = 0; i < requiredAtomSerialWhiteSpaces; i++){

				atomSerialWhiteSpaces += " ";

			}

			bbWriter << atomSerialWhiteSpaces;
			bbWriter << atomSerial;
			bbWriter << "  ";

			// write atom name
			int bbAtomIndex = chromo_pos % 4;
			if (bbAtomIndex == 0){

				bbWriter << "N   ";

			}
			else if (bbAtomIndex == 1){

				bbWriter << "CA  ";

			}
			else if (bbAtomIndex == 2){

				bbWriter << "C   ";

			}
			else if (bbAtomIndex == 3){

				bbWriter << "O   ";

			}

			// write amino acid name
			int aaIndex = chromo_pos / 4;
			char aaOneLetter = fastaSeq[aaIndex];
			//			cout << aaOneLetter << " ";
			stringstream ss;
			string oneLetterAA;
			ss << aaOneLetter;
			ss >> oneLetterAA;

			//			cout << oneLetterAA << " ";

			string threeLetterAA;

			for (int j = 0; j < 20; j++){

				if (oneLetterAA == aaMapper[j][1]){

					threeLetterAA = aaMapper[j][0];
				}

			}
			//			cout << threeLetterAA << "\n";
			bbWriter << threeLetterAA + " " + "A";

			// write residue serial number
			//	int resNumber = aaIndex + 1;
			int resNumber = amino_start_index + aaIndex;
			int countResDigits = 0;
			while (resNumber != 0){

				resNumber /= 10;
				++countResDigits;

			}

			if (countResDigits == 1){

				bbWriter << "   " + NumberToString(amino_start_index + aaIndex) + "    ";

			}
			else if (countResDigits == 2){

				bbWriter << "  " + NumberToString(amino_start_index + aaIndex) + "    ";
			}
			else if (countResDigits == 3){

				bbWriter << " " + NumberToString(amino_start_index + aaIndex) + "    ";
			}
			else if (countResDigits == 4){

				bbWriter << NumberToString(amino_start_index + aaIndex) + "    ";
			}

			// write x-cor			
			double x_cor = geno.Xcor[chromo_pos];
			bbWriter << setw(8) << fixed << setprecision(3) << x_cor;

			// write y-cor
			double y_cor = geno.Ycor[chromo_pos];
			bbWriter << setw(8) << fixed << setprecision(3) << y_cor;

			// write z-cor
			double z_cor = geno.Zcor[chromo_pos];
			bbWriter << setw(8) << fixed << setprecision(3) << z_cor;

			bbWriter << setw(6) << "1.00";
			bbWriter << setw(6) << "0.00";
			bbWriter << setw(13);

			if (bbAtomIndex == 0){

				bbWriter << "N\r\n";

			}
			else if (bbAtomIndex == 1){

				bbWriter << "C\r\n";

			}
			else if (bbAtomIndex == 2){

				bbWriter << "C\r\n";

			}
			else if (bbAtomIndex == 3){

				bbWriter << "O\r\n";

			}

			chromo_pos++;
			atomSerial++;
			bbWriter.flush();



		}

		bbWriter << "TER";
		bbWriter.flush();
		bbWriter.close();


	}

	// generate complete structure using the Scwrl Program
	string scwrlInputPdb = "../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_bb.pdb";
	string scwrlOutputPdb = "../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_full.pdb";

	string scwrlCmd = "Scwrl4 -i " + scwrlInputPdb + " -o " + scwrlOutputPdb;
	int sysRet = system(scwrlCmd.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}

	/*
	string scwrlCmd = "/usr/local/bin/scwrl4/Scwrl4 -i " + scwrlInputPdb + " -o " + scwrlOutputPdb;
	int sysRet = system(scwrlCmd.c_str());
	if (sysRet == -1){

	cout << "System Call Unsuccessful" << endl;
	exit(EXIT_FAILURE);

	}

	*/

	/*
	string oscarScriptFile = "../oscar-star2/runOscarStar2_comp";
	ofstream oscarScript(oscarScriptFile.c_str());
	if (oscarScript.is_open()){

		oscarScript << "#!/bin/sh\n";
		oscarScript << "#purpose: run oscar-star2\n";
		oscarScript << "#author: Avdesh\n";
		oscarScript << "\n";
		oscarScript << "cd ../oscar-star2/\n";
		//		oscarScript << "javac *.java\n";
		oscarScript << "./oscar-star " + scwrlInputPdb;

		oscarScript.close();
	}

	// make script file executable
	string makeOscarScriptExecutable = "chmod +x " + oscarScriptFile;
	int sysRet = system(makeOscarScriptExecutable.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}


	// run the script
	sysRet = system(oscarScriptFile.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}



	// copy the oscar output file inside PDBID directory
	string oscarOutputPdb = "../oscar-star2/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_bb_model.pdb";
	string final_pdb = "../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_full.pdb";
	//string copyOscarOp = "mv -f ../oscar-star2/" + oscarOutputPdb + " ../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_full.pdb";
	string copyOscarOp = "mv -f ../oscar-star2/" + oscarOutputPdb + " " + final_pdb;
	sysRet = system(copyOscarOp.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}

	*/

	/*
	// read the pdb file created and insert the header information
	string pdbFilePath = "../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_full.pdb";
	ifstream pdbFileStream(pdbFilePath.c_str());
	string linePdbFile;

	string opPdbFilePath = "../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + ".pdb";

	if (pdbFileStream.is_open()){

	ofstream pdbWriter(opPdbFilePath.c_str());
	if (pdbWriter.is_open()){

	pdbWriter << "PFRMAT TS" << endl;
	pdbWriter << "TARGET 1" << endl;
	pdbWriter << "AUTHOR 0591-1752-4975" << endl;
	pdbWriter << "METHOD Ab-Initio-PSP" << endl;
	pdbWriter << "METHOD Ab-Initio-PSP" << endl;
	pdbWriter << "MODEL  1" << endl;
	pdbWriter << "PARENT N/A" << endl;
	pdbWriter.flush();

	}

	while (getline(pdbFileStream, linePdbFile)){

	pdbWriter << linePdbFile << endl;

	}

	pdbWriter << "TER" << endl;
	pdbWriter << "END" << endl;
	pdbWriter.close();

	}

	pdbFileStream.close();

	// remove the full atomic model created by oscar-star2
	string removefullatomicmodel = "rm -f ../" + id + "/" + id + "_" + NumberToString(generation) + "_" + NumberToString(chromo_index) + "_full.pdb";
	sysRet = system(removefullatomicmodel.c_str());
	if (sysRet == -1){

	cout << "System Call Unsuccessful" << endl;
	exit(EXIT_FAILURE);

	}

	*/

	/*
	string oscarCmd = "../oscar-star2/oscar-star";
	int sysRet = system(oscarCmd.c_str());
	if (sysRet == -1){

	cout << "System Call Unsuccessful" << endl;
	exit(EXIT_FAILURE);


	}
	*/

	return scwrlOutputPdb;

}



