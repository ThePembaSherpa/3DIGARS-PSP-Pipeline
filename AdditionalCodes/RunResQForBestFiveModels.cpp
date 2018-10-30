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

	string itasser_pkgdir = "../../../Ab-Initio-Softs/I-TASSER5.1";
	string itasser_lib_path = "../../../Ab-Initio-Softs/ITLIB/";
	string itasser_data_dir = "../../../Ab-Initio-Softs/I-TASSER5.1/ResQ/CASP13/";
	string resq_path = "../../../Ab-Initio-Softs/I-TASSER5.1/ResQ/";
	string java_home_path = "/usr/";
	

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

			if (line.length() > 0){
				string model_name = "em" + line;
				model_names.push_back(model_name);

			}

		}

		best_models_stream.close();

	}

	if (rank == 0){

		// run ResQ for first model
		string model_name = model_names.at(rank);
		int model_num = rank + 1;
		string proteiname = pdb_id + "-m" + NumberToString(model_num);
		string runResQ = resq_path + "runResQ.pl -pkgdir " + itasser_pkgdir + " -libdir " + itasser_lib_path + " -protname " + proteiname + " -model " + model_name + " -datadir " + itasser_data_dir + proteiname + "/ -java_home " + java_home_path + " -runstyle gnuparallel";
		int sysRet = system(runResQ.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	if (rank == 1){

		// run ResQ for second model
		string model_name = model_names.at(rank);
		int model_num = rank + 1;
		string proteiname = pdb_id + "-m" + NumberToString(model_num);
		string runResQ = resq_path + "runResQ.pl -pkgdir " + itasser_pkgdir + " -libdir " + itasser_lib_path + " -protname " + proteiname + " -model " + model_name + " -datadir " + itasser_data_dir + proteiname + "/ -java_home " + java_home_path + " -runstyle gnuparallel";
		int sysRet = system(runResQ.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	if (rank == 2){

		// run ResQ for third model
		string model_name = model_names.at(rank);
		int model_num = rank + 1;
		string proteiname = pdb_id + "-m" + NumberToString(model_num);
		string runResQ = resq_path + "runResQ.pl -pkgdir " + itasser_pkgdir + " -libdir " + itasser_lib_path + " -protname " + proteiname + " -model " + model_name + " -datadir " + itasser_data_dir + proteiname + "/ -java_home " + java_home_path + " -runstyle gnuparallel";
		int sysRet = system(runResQ.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	if (rank == 3){

		// run ResQ for fourth model
		string model_name = model_names.at(rank);
		int model_num = rank + 1;
		string proteiname = pdb_id + "-m" + NumberToString(model_num);
		string runResQ = resq_path + "runResQ.pl -pkgdir " + itasser_pkgdir + " -libdir " + itasser_lib_path + " -protname " + proteiname + " -model " + model_name + " -datadir " + itasser_data_dir + proteiname + "/ -java_home " + java_home_path + " -runstyle gnuparallel";
		int sysRet = system(runResQ.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}

	if (rank == 4){

		// run ResQ for fifth model
		string model_name = model_names.at(rank);
		int model_num = rank + 1;
		string proteiname = pdb_id + "-m" + NumberToString(model_num);
		string runResQ = resq_path + "runResQ.pl -pkgdir " + itasser_pkgdir + " -libdir " + itasser_lib_path + " -protname " + proteiname + " -model " + model_name + " -datadir " + itasser_data_dir + proteiname + "/ -java_home " + java_home_path + " -runstyle gnuparallel";
		int sysRet = system(runResQ.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

	}


	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;

}
