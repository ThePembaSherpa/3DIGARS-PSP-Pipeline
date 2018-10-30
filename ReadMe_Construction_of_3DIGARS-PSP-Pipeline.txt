************************************************** 3DIGARS-PSP Pipleline Construction Approach *******************************************

In the Script add following things to create the 3DIGARS-PSP Pipeline
1) Collect initial seed from I-TASSER5.1
	Add code to run I-TASSER5.1 automatically
2) Collect initial seed from UniCon3D
	a) add code to run "NNcon" to obtain contact-map
	b) add code to run "Scratch" to obtain Secondary structure
	c) add code to run "UniCon3D" to obtain decoys
	d) add code to Run "Pulchra" to obtain full atomic model for the decoys obtained from UniCon3D
	e) add code to Run "Qprob" to rank the full atomic models
	f) add code to Run "MUFold_Clustering" to cluster the models
	g) add code to Run "5_models_selection_code" to obtain 5 top models
	h) add code to Run "Mode_Refiner" to refine the top 5 models
3) Run "PSSM" to obtain the PSSM score.
4) Run 3DIGARS-PSP
	a) add code to Collect 5 top models from I-TASSER and 5 top models from UniCon3D and put these models inside input/seeds/input/ directory in 3DIGARS-PSP
	b) add code to collect PSSM and put in pssmout directory
	c) add code to place fasta file in Input/fasta directory
	d) add code to run 3DIGARS-PSP program
		-- Modify 3DIGARS-PSP program such that it can read the number of generations from the file so that the program knows when to terminate
	e) add code to run 3DIGARS-PSP output parser program
	f) add code to Run "Qprob" to rank the full atomic models
	g) add code to Run "MUFold_Clustering" to cluster the models
	h) add code to Run "5_models_selection_code" to obtain 5 top models
	i) add code to Run "Mode_Refiner" to refine the top 5 models

	