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
#define PI 3.14159265
using namespace std;



template <typename T>
string NumberToString(T pNumber)
{
	ostringstream oOStrStream;
	oOStrStream << pNumber;
	return oOStrStream.str();
}

int main(int argc, char* argv[]){

	// pass the id from command line;

	if (argc < 2){
		
		cout << "please provide the protein id as an argument" << endl;
		exit(0);

	}

	string id = argv[1];
	// list the number of models
	string list_models = "ls ../" + id + "/ | awk -F'/' '{print $NF}' > ../" + id + "/list.txt";
	int sysRet = system(list_models.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful .. listing models directory" << endl;
		exit(EXIT_FAILURE);

	}

	
	string model_list_path = "../" + id + "/list.txt";
	ifstream model_list_stream(model_list_path.c_str());
	string model_id;
	int model_count = 0;

	if (model_list_stream.is_open()){

		while (!model_list_stream.eof()){

			getline(model_list_stream, model_id);

			if ((model_id == "list.txt") || (model_id.length() == 0)){

				continue;

			}

			cout << model_id << endl;
			model_count++;
			

			// remove existing output model files
			string output_model_exist = "../" + id + "/" + model_id + "/" + id + "_Model" + NumberToString(model_count) + ".pdb";
			ifstream output_model_stream(output_model_exist.c_str());
			if (output_model_stream.good()){ // check if output file already exist

				string rm_model = "rm " + output_model_exist;
				sysRet = system(rm_model.c_str());
				if (sysRet == -1){

					cout << "System Call Unsuccessful .. listing models directory" << endl;
					exit(EXIT_FAILURE);

				}

			}




			string list_pdb = "ls ../" + id + "/" + model_id + "/*.pdb | awk -F'/' '{print $NF}' > ../" + id + "/" + model_id + "/list.txt";
			sysRet = system(list_pdb.c_str());
			if (sysRet == -1){

				cout << "System Call Unsuccessful .. listing pdb files inside models" << endl;
				exit(EXIT_FAILURE);

			}

			

			string pdb_list_path = "../" + id + "/" + model_id + "/list.txt";
			ifstream pdb_list_stream(pdb_list_path.c_str());
			string pdb_id;
			int counter = 1;

			if (pdb_list_stream.is_open()){

				while (!pdb_list_stream.eof()){

					getline(pdb_list_stream, pdb_id);

					if (pdb_id.length() == 0){

						continue;
					}

					cout << pdb_id << endl;
										
					if (counter > 1){

						cout << "multiple pdb files not accepted in a single model directory ... exiting ..." << endl;
						exit(EXIT_FAILURE);

					}

					
										
					// read betafactors and save in a vector
					string betafactor_path = "../" + id + "/" + model_id + "/local.txt";
					string betafactor_line;
					vector<float> betafactor_vector;
					ifstream betafactor_stream(betafactor_path.c_str());

					if (betafactor_stream.is_open()){

						while (!betafactor_stream.eof()){

							getline(betafactor_stream, betafactor_line);

							string token;
							istringstream str(betafactor_line);
							str >> token;
							int res_num;
							int ss_type;
							int asa_type;
							float cov;
							float nbfp;
							float rbfp;
							float rsq_l;
							float rsq_s;


							if (token != "#RES"){

								str >> res_num >> ss_type >> asa_type >> cov >> nbfp >> rbfp >> rsq_l >> rsq_s;
								betafactor_vector.push_back(rbfp);
								
							}
							
						}

						betafactor_stream.close();

					}

					// create a output structure file
				//	string output_pdb_id = pdb_id.substr(0, pdb_id.length()-4);
					string pdb_output = "../" + id + "/" + model_id + "/" + id + "_Model" + NumberToString(model_count) + ".pdb";
					ofstream pdb_wr_stream(pdb_output.c_str());
					pdb_wr_stream << "PFRMAT TS" << endl;
					pdb_wr_stream << "TARGET " << id << endl;
					pdb_wr_stream << "AUTHOR 0591-1752-4975" << endl;
					pdb_wr_stream << "METHOD Ab-Initio-PSP" << endl;
					pdb_wr_stream << "METHOD Ab-Initio-PSP" << endl;
					pdb_wr_stream << "MODEL  " << model_count << endl;
					pdb_wr_stream << "PARENT N/A" << endl;

					// read pdb file and insert the beta factor after conversion to angstrom ... CASP13 standard
					string pdb_path = "../" + id + "/" + model_id + "/" + pdb_id;
					ifstream pdb_stream(pdb_path.c_str());
					string pdb_line;
					
					if (pdb_stream.is_open()){

						while (!pdb_stream.eof()){

							getline(pdb_stream, pdb_line);

							if (pdb_line.length() == 0){

								continue;

							}

							string value;
							istringstream str_pdb(pdb_line);
							str_pdb >> value;
							int atom_num;
							string atom_type;
							string res_type;
							// string chain;
							int res_num;
							float x;
							float y;
							float z;
							float occupancy;
							// string elem_symbol;


							if (value == "ATOM"){

								// str_pdb >> atom_num >> atom_type >> res_type >> chain >> res_num >> x >> y >> z >> occupancy >> elem_symbol;
								//str_pdb >> atom_num >> atom_type >> res_type >> res_num >> x >> y >> z >> occupancy;
								int res_num = atoi(pdb_line.substr(23, 4).c_str());
								//cout << res_num << endl;
								string pdb_sub_first = pdb_line.substr(0, 60);
								
								float beta_factor = betafactor_vector.at(res_num - 1);
								float beta_factor_dist = sqrt((beta_factor * 3) / (8 * PI*PI));


								pdb_wr_stream << pdb_sub_first;
								pdb_wr_stream << setw(6) << fixed << setprecision(2) << beta_factor_dist << endl;
								//pdb_wr_stream << setw(12);
								//pdb_wr_stream << elem_symbol;
								//pdb_wr_stream << "  " << endl;

							}
							else if (value == "MODEL"){

								continue;

							}
							else{

								pdb_wr_stream << pdb_line << endl;
							}



						}

						pdb_wr_stream << "TER" << endl;
						pdb_wr_stream << "END" << endl;
						pdb_stream.close();

					}
					
					pdb_wr_stream.close();

					counter++;

				}
				
				pdb_list_stream.close();
			}

		}
		
		model_list_stream.close();

	}


}




