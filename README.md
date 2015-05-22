####share memory

1) pthread
g++ -o gauss gauss.c -lpthread

./gauss 1024 4 5555


2) parallel programming directives: 

% g++ -o gauss gauss.c -fopenmp


####distributed memory

mpicc -o cpi cpi.c -llam

condor_submit submit.cpi.mpi

condor_q

condor_rm id


Compile the program (with MPICH2, mpicc does not require an additional -llam flag):
% mpicc -o cpi cpi.c

% mpirun -n 4 path
