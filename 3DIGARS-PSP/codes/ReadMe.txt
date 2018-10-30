this version -- uses randomzone steric clash removal process

fixes: the GenoA[pop_size] array were changed to the pointers ... Genome *genoA = new Genome[pop_size] ...

pop_size: 200
no of Gen: 500

screen -S T0953s2-PSP
source activate python2 ############# Spiders2 Program Needs Python2.7 #############
mpiCC *.cpp -o main.exe -lm
mpirun -np 4 ./main.exe > console_T0953s2.txt

Compile: 
	mpiCC *.cpp -o main.exe -lm
Run: 
	mpirun -np 2 ./main.exe > console.txt
	mpirun -np 6 ./main.exe > console_1pohA.txt
	
	