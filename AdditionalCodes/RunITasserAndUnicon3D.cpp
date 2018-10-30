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

// the library has to be set from the terminal before running the pipeline ... it can not be done from program
// set environment variable for "libgfortran.so.1"
// it is required that this program is run using 2 processes
int main(int argc, char *argv[]){

	int rank = -1;
	int size = -1;
	// MPI_Status status;

	//  Initialize MPI.
	MPI_Init(&argc, &argv);

	string pdb_id = string(argv[1]);
	cout << "pdb_id: " << pdb_id << endl;
	string num_unicon_decoys = string(argv[2]);

	//  Get the number of processes.
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//  Determine the rank of this process.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	string input_path = "../" + pdb_id + "/";
//	string itasser_pkgdir = "../../Ab-Initio/I-TASSER5.0";
//	string itasser_path = "../../Ab-Initio/I-TASSER5.0/I-TASSERmod/";
//	string itasser_lib_path = "../../Ab-Initio/ITLIB/";
//	string itasser_data_dir = "../../Ab-Initio/I-TASSER5.0/CASP13/";
//	string java_home_path = "/usr/";

	string nncon_path = "../../../Contact-Prediction/nncon1.0/bin/predict_ss_sa_cm.sh";
	string scratch_path = "../../../SSpro5/SCRATCH-1D_1.1/bin/run_SCRATCH-1D_predictors.sh";

	string libgfortran_lib_path = "../";
	string unicon_path = "../UniCon3D-1.0/bin/linux/";
	string unicon_lib_path = "../UniCon3D-1.0/lib/";
	

	string scripts_path = "../scripts/";

	string qprob_path = "../../../Ab-Initio-CASP13/structure_ranking/qprob_package/";
	string mufold_clustering_tool_path = "../../../Ab-Initio-CASP13/clustering_tools/";
	string modrefiner_path = "../../../Ab-Initio-CASP13/refinement_tool/ModRefiner-l/";
	string three_DIGARS_PSP_path = "../3DIGARS-PSP/";
	int sysRet;
	
	if (rank == 0){ // run I-Tasser5.0

		/*
		cout << "started running I-Tasser5.0" << endl;
		// make a directory for specific protein
		string cmd_make_dir = "mkdir " + itasser_data_dir + pdb_id;
		sysRet = system(cmd_make_dir.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		// copy the .fasta file to the specific protein id inside I-Tasser program
		string cmd_cp_fasta = "scp " + input_path + pdb_id + ".fasta " + itasser_data_dir + pdb_id + "/seq.fasta";
		sysRet = system(cmd_cp_fasta.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		// run I-TASSER ... in server#2 we have I-TASSER5.0 and in server#10 we have I-TASSER5.1 ... commands to run these versions are slightly different		
		// ./runI-TASSER.pl -libdir /home/amishra2/Ab-Initio-Softs/ITLIB/ -seqname T0982 -datadir /home/amishra2/Ab-Initio-Softs/I-TASSER5.1/CASP13/T0982/ -runstyle gnuparallel
		// ./runI-TASSER.pl -pkgdir /home/amishra2/Ab-Initio/I-TASSER5.0 -libdir /home/amishra2/Ab-Initio/ITLIB/ -seqname T0949 -datadir /home/amishra2/Ab-Initio/I-TASSER5.0/CASP13/T0949/ -java_home /usr/ -outdir /home/amishra2/Ab-Initio/results/CASP13-Results/T0949

		string itasser_run_cmd = itasser_path + "runI-TASSER.pl -pkgdir " + itasser_pkgdir + " -libdir " + itasser_lib_path + " -seqname " + pdb_id + " -datadir " + itasser_data_dir + pdb_id + "/ -java_home " + java_home_path+" -runstyle gnuparallel";
		// cout << "itasser_run_cmd: " << itasser_run_cmd << endl;
		sysRet = system(itasser_run_cmd.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		

		// copy I-tasser models files from I-Tassers directory to 3DIGARS-PSP directory
		string cmd_cp_models = "cp " + itasser_data_dir + pdb_id + "/model*.pdb " + three_DIGARS_PSP_path + "input/seeds/input/";
		sysRet = system(cmd_cp_models.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		*/


	}

	if (rank == 1){ // run all the preliminary programs such as NNcon and Scratch then run UniCon3D ... next run Qprob, MUFOLD_Clustering, "5_model_selections_code" and "Mode_Refiner"

		/*
		cout << "started running NNcon" << endl;
		// make a directory called "ConMap" inside input target directory
		string cmd_make_dir = "mkdir ../" + pdb_id + "/ConMap/";
		int sysRet = system(cmd_make_dir.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		// run NNcon
		string nncon_run_cmd = "perl " + nncon_path + " " + input_path+ pdb_id + ".fasta" + " ../" + pdb_id + "/ConMap/";
		sysRet = system(nncon_run_cmd.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		// remove first four lines from a contact-map file "*_fasta.cm12a"
		string ip_cmap_file = "../" + pdb_id + "/ConMap/" + pdb_id + "_fasta.cm12a";
		ifstream conStream(ip_cmap_file.c_str());
		string conLine;

		string op_cmap_file = "../" + pdb_id + "/ConMap/" + pdb_id + "_fasta_updated.cm12a";
		ofstream conOutStream(op_cmap_file.c_str());
		
		if (conStream.fail()){

			cout << "Could not open file " << ip_cmap_file << "\n";
			exit(EXIT_FAILURE);

		}

		if (conStream.is_open()){

			getline(conStream, conLine); // ignore four lines from the top of the file
			getline(conStream, conLine);
			getline(conStream, conLine);
			getline(conStream, conLine);

			while (!conStream.eof()){

				getline(conStream, conLine);
				conOutStream << conLine << "\n";

			}

			conStream.close();

		}


		cout << "started running Scratch" << endl;
		// run Scratch
		string scratch_run_cmd = scratch_path + " " + input_path + pdb_id + ".fasta" + " " + input_path + pdb_id + ".out 4";
		sysRet = system(scratch_run_cmd.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		
		
		cout << "started running UniCon3D" << endl;
		// output of NNcon and Scratch is saved in the input target directory "../pdb_id/"
		// run UniCon3D
		string unicon_run_cmd = unicon_path + "UniCon3D -i " + pdb_id + " -f " + input_path + pdb_id + ".fasta -s " + input_path + pdb_id + ".out.ss8 -c " + input_path + "ConMap/" + pdb_id + "_fasta_updated.cm12a -m " + unicon_lib_path + "UniCon.iohmm -d " + num_unicon_decoys;
		cout << "unicon_run_cmd: " << unicon_run_cmd << endl;
		sysRet = system(unicon_run_cmd.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		

		
		// the Unicon3D decoys are generated under "/AdditionalCodes/" directory
		// Run pulchra software
		string pulchra_run_cmd = scripts_path + "runPulchra" + " ./";
		sysRet = system(pulchra_run_cmd.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		
		*/
		// copy the full atomic decoys generated by Pulchra to the Qprob directory to rank the models
		string cmd_make_dir_qprob = "mkdir " + qprob_path + "test/" + pdb_id;
		sysRet = system(cmd_make_dir_qprob.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		string cmd_rm_exist_model_qprob = "rm " + qprob_path + "test/" + pdb_id+"/*";
		sysRet = system(cmd_rm_exist_model_qprob.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		
		string cmd_cp_pulchra_decoys = "scp ./*.rebuilt.pdb " + qprob_path + "test/" + pdb_id + "/";
		sysRet = system(cmd_cp_pulchra_decoys.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		

		// copy fasta file to qprob test directory
		string cmd_cp_fasta = "cp " + input_path + pdb_id + ".fasta " + qprob_path + "test/";
		sysRet = system(cmd_cp_fasta.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
				
		//string cmd_run_qprob = qprob_path + "bin/Qprob.sh " + qprob_path + "test/" + pdb_id + ".fasta " + qprob_path + "test/" + pdb_id + " " + qprob_path + "test/" + "test_" + pdb_id;
		string cmd_run_qprob = "cd " + qprob_path + "test/ && " + "../bin/Qprob.sh " + pdb_id + ".fasta " + pdb_id + " " + "test_" + pdb_id;
		cout << "cmd_run_qprob " << cmd_run_qprob << endl;
		sysRet = system(cmd_run_qprob.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}		
		
		
		// run clustering program
		string cmd_make_dir_mufold_cluster = "mkdir " + mufold_clustering_tool_path + "Decoys/" + pdb_id;
		// cout << "cmd_make_dir_mufold_cluster " << cmd_make_dir_mufold_cluster << endl;
		sysRet = system(cmd_make_dir_mufold_cluster.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		string cmd_rm_exist_models_mufold_cluster = "rm " + mufold_clustering_tool_path + "Decoys/" + pdb_id+"/*";
		sysRet = system(cmd_rm_exist_models_mufold_cluster.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		string cmd_cp_pulchra_decoys_to_mufold = "scp " + qprob_path + "test/" + pdb_id + "/* " + mufold_clustering_tool_path+"Decoys/"+pdb_id+"/";
		// cout << "cmd_cp_pulchra_decoys_to_mufold " << cmd_cp_pulchra_decoys_to_mufold << endl;
		sysRet = system(cmd_cp_pulchra_decoys_to_mufold.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}

		string run_mufold_clustering = mufold_clustering_tool_path + "MUFOLD_CL_SourceCode/Clustering -P " + mufold_clustering_tool_path + "Decoys/" + pdb_id + "/";
		// cout << "run_mufold_clustering " << run_mufold_clustering << endl;
		sysRet = system(run_mufold_clustering.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		

		// copy the qprob result to current working directory "/AdditionalCodes/"
		string cp_qprob_output = "cp "+qprob_path + "test/" + "test_" + pdb_id + "/" + pdb_id + ".Qprob_score ./";
		sysRet = system(cp_qprob_output.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
		
		
		// run java code to get top 5 models
		string run_get_five_models = "java GetFiveModelsByParsingQprobAndMUFOLD " + pdb_id + " unicon_top_five";
		sysRet = system(run_get_five_models.c_str());
		if (sysRet == -1){

			cout << "System Call Unsuccessful" << endl;
			exit(EXIT_FAILURE);

		}
							

	}

		
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;

}