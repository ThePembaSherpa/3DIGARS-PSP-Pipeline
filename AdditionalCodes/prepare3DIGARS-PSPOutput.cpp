#include <mpi.h>
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
using namespace std;

template <typename T>
string NumberToString(T pNumber)
{
	ostringstream oOStrStream;
	oOStrStream << pNumber;
	return oOStrStream.str();
}

// run this program with np = 1 ... mpirun
int main(int argc, char *argv[]){

	int rank = -1;
	int size = -1;
	// MPI_Status status;

	//  Initialize MPI.
	MPI_Init(&argc, &argv);

	string pdb_id = string(argv[1]);
	cout << "pdb_id: " << pdb_id << endl;

	//  Get the number of processes.
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//  Determine the rank of this process.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	string three_DIGARS_OpParser_Path = "./createStructureFromAbInitioOutput/codes/";
	string three_DIGARS_Op_Model_Path = "../output/" + pdb_id+"/";
	string qprob_path = "../../../Ab-Initio-CASP13/structure_ranking/qprob_package/";
	string mufold_clustering_tool_path = "../../../Ab-Initio-CASP13/clustering_tools/";
	string modrefiner_path = "../../../Ab-Initio-CASP13/refinement_tool/ModRefiner-l/";
	string resq_path = "../../../Ab-Initio-Softs/I-TASSER5.1/ResQ/";
	string resq_input_path = "../../../Ab-Initio-Softs/I-TASSER5.0/ResQ/CASP13/";
	string insert_bfactor_path = "./Insert-BFactors-InPredStructure/";
	string insert_bfactor_code_path = "./Insert-BFactors-InPredStructure/code/";
	
	int sysRet = 0;

	/*
	// run 3DIGARS-PSP output parser;
	cout << "Start Running 3DIGARS-PSP Output Parser" << endl;
	string run_three_DIGARS_OpParser = "cd "+three_DIGARS_OpParser_Path +"&&"+ "./abInitioOuputParser.exe " + pdb_id;
	sysRet = system(run_three_DIGARS_OpParser.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful ... terminating " << endl;

		exit(EXIT_FAILURE);

	}
	cout << "End Running 3DIGARS-PSP Output Parser" << endl;
	
	
	
	// copy 3DIGARS-PSP output files to Qprob
	string rm_exist_target_dir_from_qprob = "rm " + qprob_path + "test/" + pdb_id+"/*";
	sysRet = system(rm_exist_target_dir_from_qprob.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful ... terminating " << endl;

		exit(EXIT_FAILURE);

	}

	string rm_exist_target_test_dir_from_qprob = "rm -r " + qprob_path + "test/test_" + pdb_id;
	sysRet = system(rm_exist_target_test_dir_from_qprob.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful ... terminating " << endl;

		exit(EXIT_FAILURE);

	}

	string cmd_cp_three_DIGARS_decoys = "scp " + three_DIGARS_Op_Model_Path + "* " + qprob_path + "test/" + pdb_id + "/";
	sysRet = system(cmd_cp_three_DIGARS_decoys.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}
	*/
	// Run Qprob
	cout << "Start Running Qprob" << endl;
	string cmd_run_qprob = "cd " + qprob_path + "test/ && " + "../bin/Qprob.sh " + pdb_id + ".fasta " + pdb_id + " " + "test_" + pdb_id;
	// cout << "cmd_run_qprob " << cmd_run_qprob << endl;
	sysRet = system(cmd_run_qprob.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}
	cout << "End Running Qprob" << endl;
	
	// Copy 3DIGARS-PSP output files to clustering program
	string cmd_rm_exist_models_mufold_cluster = "rm " + mufold_clustering_tool_path + "Decoys/" + pdb_id+"/*";
	// cout << "cmd_make_dir_mufold_cluster " << cmd_make_dir_mufold_cluster << endl;
	sysRet = system(cmd_rm_exist_models_mufold_cluster.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}


	// Run clustering program
	string cmd_cp_three_DIGARS_decoys_to_mufold = "scp " + qprob_path + "test/" + pdb_id + "/* " + mufold_clustering_tool_path + "Decoys/" + pdb_id + "/";
	// cout << "cmd_cp_pulchra_decoys_to_mufold " << cmd_cp_pulchra_decoys_to_mufold << endl;
	sysRet = system(cmd_cp_three_DIGARS_decoys_to_mufold.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}

	// mufold clustering program generates output in the same directory from which it was called.
	cout << "Start Running MUFOLD_CL" << endl;
	string run_mufold_clustering = mufold_clustering_tool_path + "MUFOLD_CL_SourceCode/Clustering -P " + mufold_clustering_tool_path + "Decoys/" + pdb_id + "/";
	// cout << "run_mufold_clustering " << run_mufold_clustering << endl;
	sysRet = system(run_mufold_clustering.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}
	cout << "End Running MUFOLD_CL" << endl;

	// copy the qprob result to current working directory "/AdditionalCodes/"
	string cp_qprob_output = "cp " + qprob_path + "test/" + "test_" + pdb_id + "/" + pdb_id + ".Qprob_score ./";
	sysRet = system(cp_qprob_output.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}
	

	// run java code to get top 5 models
	cout << "Start Running GetFiveModels" << endl;
	string run_get_five_models = "java GetFiveModelsByParsingQprobAndMUFOLD " + pdb_id + " 3DIGARS_top_five";
	sysRet = system(run_get_five_models.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}
	cout << "End Running GetFiveModels" << endl;
	
	
	/*
	// copy top 5 models to refinement program
	string cmd_rm_models_modrefiner = "rm " + modrefiner_path + pdb_id+"/*";
	int sysRet = system(cmd_rm_models_modrefiner.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful" << endl;
		exit(EXIT_FAILURE);

	}

	// copy the best five models inside ModRefiner directory
	string path_five_best_models_file = "./" + pdb_id + ".3DIGARS_top_five";
	ifstream best_models_stream(path_five_best_models_file.c_str());
	string line;
	vector<string> model_names;

	if (best_models_stream.fail()){

		cout << "Could not open file " << path_five_best_models_file << "\n";
		exit(EXIT_FAILURE);

	}

	if (best_models_stream.is_open()){

		while (!best_models_stream.eof()){

			getline(best_models_stream, line);
			if (line.length() > 0){

				model_names.push_back(line);

			}


		}

		best_models_stream.close();

	}



	for (int i = 0; i < model_names.size(); i++){

		string model_name = model_names.at(i);
		string cp_best_model_to_modrefiner = "cp ../output/" + pdb_id + "/" + model_name + " " + modrefiner_path + pdb_id + "/";
		sysRet = system(cp_best_model_to_modrefiner.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	
	
	// Run refinement program
	int np = model_names.size();
	int errcodes[np];
	string name_modrefiner_exe = "./runModRefiner.exe";
	char * path = new char[path_five_best_models_file.length() + 1];
	strcpy(path, path_five_best_models_file.c_str());
	char * id = new char[pdb_id.length() + 1];
	strcpy(id, pdb_id.c_str());
	char ** argu = (char **)malloc(sizeof(char *)* 3);
	argu[0] = path;
	argu[1] = id;
	argu[2] = NULL;
	cout << "np " << np << endl;
	//strncpy(argu[0], path_five_best_models_file, 100);
	//strncpy(argu[1], pdb_id, 100);
	//char *argu[] = { path_five_best_models_file.c_str(), pdb_id.c_str(), NULL};
	MPI_Comm spawnedcomm;
	cout << "started modrefiner" << endl;
	// MPI_Comm_spawn is a collective function so it can be only callect for all processes
	MPI_Comm_spawn(name_modrefiner_exe.c_str(), argu, np, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &spawnedcomm, errcodes);
	MPI_Comm_disconnect(&spawnedcomm);
	cout << "end modrefiner" << endl;

//	MPI_Barrier(MPI_COMM_WORLD);

	
	// Copy top 5 models to ResQ program
	for (int i = 0; i < model_names.size(); i++){ // create a separate directory inside ResQ program to Run ResQ in parallel for top five models

		int model_num = i + 1;
		string mkdir_model_resq = "mkdir " + resq_input_path + pdb_id + "-m" + NumberToString(model_num);
		sysRet = system(mkdir_model_resq.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		string model_name = "em"+model_names.at(i);
		string cp_best_model_to_resq = "cp " + modrefiner_path + pdb_id +"/"+ model_name + " " + resq_input_path + pdb_id + "-m" + NumberToString(model_num)+"/";
		sysRet = system(cp_best_model_to_resq.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}
		
	

	// Run ResQ program
	string name_resq_exe = "./runResQ.exe";
	MPI_Comm spawnedcomm_resq;
	cout << "started ResQ" << endl;
	// MPI_Comm_spawn is a collective function so it can be only callect for all processes
	MPI_Comm_spawn(name_resq_exe.c_str(), argu, np, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &spawnedcomm_resq, errcodes);
	MPI_Comm_disconnect(&spawnedcomm_resq);
	cout << "end ResQ" << endl;

//	MPI_Barrier(MPI_COMM_WORLD);

	
	// copy the refined models and B-factor prediction "local.txt" file inside "Insert-Bfactors-InPredStructure" directory and run insert-betafactor code
	string createDirCmd = "mkdir " + insert_bfactor_path + pdb_id; // create a dir name pdb_id
	sysRet = system(createDirCmd.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful ... terminating " << endl;

		exit(EXIT_FAILURE);

	}

	for (int i = 0; i < model_names.size(); i++){ // create directory inside insert_bfactor_path

		int model_num = i + 1;
		string mkdir_insert_beta = "mkdir " + insert_bfactor_path + pdb_id + "/" + pdb_id + "-m" + NumberToString(model_num);
		sysRet = system(mkdir_insert_beta.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		string model_name = "em" + model_names.at(i); // copy the refined model file into insert_bfactor_path
		string cp_best_model_to_resq = "cp " + modrefiner_path +pdb_id+ "/" + model_name + " " + insert_bfactor_path + pdb_id + "/" + pdb_id + "-m" + NumberToString(model_num) + "/";
		sysRet = system(cp_best_model_to_resq.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		// copy ResQ output
		string proteiname = pdb_id + "-m" + NumberToString(model_num);
		string cp_resq_output = "cp " + resq_input_path + proteiname + "/local.txt " + insert_bfactor_path + pdb_id + "/" + pdb_id + "-m" + NumberToString(model_num) + "/";
		sysRet = system(cp_resq_output.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	// run insert-betafactor program
	cout << "Start Running InsertBetaFactor" << endl;
	string run_insert_betafactor_code = "cd " + insert_bfactor_code_path + "&&" + "./main.exe " + pdb_id;
	sysRet = system(run_insert_betafactor_code.c_str());
	if (sysRet == -1){

		cout << "System Call Unsuccessful ... terminating " << endl;

		exit(EXIT_FAILURE);

	}
	cout << "End Running InsertBetaFactor" << endl;
	
	for (int i = 0; i < model_names.size(); i++){ // copy final prediction models to the output directory

		int model_num = i + 1;
		
		string cp_final_models = "cp " + insert_bfactor_path + pdb_id + "/" + pdb_id + "-m" + NumberToString(model_num) + "/" + pdb_id + "_Model" + NumberToString(model_num) + ".pdb ../output/";
		sysRet = system(cp_final_models.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}
	
	*/

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;


}