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

int main(int argc, char *argv[]){

	int rank = -1;
	int size = -1;
	// MPI_Status status;

	//  Initialize MPI.
	MPI_Init(&argc, &argv);
	string path_five_best_models_file = string(argv[1]);
	cout << "path_five_best_models_file: " << path_five_best_models_file << endl;
	string pdb_id = string(argv[2]);
	cout << "pdb_id: " << pdb_id << endl;
	//  Get the number of processes.
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//  Determine the rank of this process.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm comm;

	
	string modrefiner_path = "../../../Ab-Initio-CASP13/refinement_tool/ModRefiner-l/";

	vector<string> model_names;
	ifstream best_models_stream(path_five_best_models_file.c_str());
	string line;

	if (best_models_stream.fail()){

		cout << "Could not open file " << path_five_best_models_file << "\n";
		exit(EXIT_FAILURE);

	}

	if (best_models_stream.is_open()){

		while (!best_models_stream.eof()){

			getline(best_models_stream, line);
			model_names.push_back(line);

		}

		best_models_stream.close();

	}

	if (rank == 0){

		// run modrefiner on first model
		string model_name = model_names.at(rank);
		string runModRefiner = modrefiner_path + "emrefinement " + modrefiner_path + pdb_id + " " + modrefiner_path + " " + model_name + " NA 100 102131";
		int sysRet = system(runModRefiner.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	if (rank == 1){

		// run modrefiner on second model
		string model_name = model_names.at(rank);
		string runModRefiner = modrefiner_path + "emrefinement " + modrefiner_path + pdb_id + " " + modrefiner_path + " " + model_name + " NA 100 102131";
		int sysRet = system(runModRefiner.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	if (rank == 2){

		// run modrefiner on third model
		string model_name = model_names.at(rank);
		string runModRefiner = modrefiner_path + "emrefinement " + modrefiner_path + pdb_id + " " + modrefiner_path + " " + model_name + " NA 100 102131";
		int sysRet = system(runModRefiner.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	if (rank == 3){

		// run modrefiner on fourth model
		string model_name = model_names.at(rank);
		string runModRefiner = modrefiner_path + "emrefinement " + modrefiner_path + pdb_id + " " + modrefiner_path + " " + model_name + " NA 100 102131";
		int sysRet = system(runModRefiner.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	if (rank == 4){

		// run modrefiner on fifth model
		string model_name = model_names.at(rank);
		string runModRefiner = modrefiner_path + "emrefinement " + modrefiner_path + pdb_id + " " + modrefiner_path + " " + model_name + " NA 100 102131";
		int sysRet = system(runModRefiner.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	MPI_Comm_disconnect(&comm);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;

}