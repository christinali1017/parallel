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


####Recursive bisection : find quadrants

Time of 256 quadrants, 524288 points:

256 quadrants 128 processors : elapsed time = 1.905887

256 quadrants 64 processors : elapsed time = 1.332699

256 quadrants 32 processors: elapsed time = 1.558071

256 quadrants 16 processors : elapsed time = 1.479581

256 quadrants 8 processors : elapsed time = 1.888402

256 quadrants 4 processors : elapsed time = 2.576493

256 quadrants 2 processors : elapsed time = 4.355613

256 quadrants 1 processors : elapsed time = 6.514836


