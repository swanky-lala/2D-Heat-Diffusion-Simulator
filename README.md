# 2D-Heat-Diffusion-Simulator
The program simulates the diffusion of temperature over a 2D surface using the finite difference method, with options for serial and parallel execution.

Description

This repository contains two versions of a program that simulates the diffusion of temperature over a 2D surface using the finite difference method. The first version (program_serial.c) is a serial implementation, while the second version (program_mpi.c) is parallelized using MPI (Message Passing Interface).

Features

Calculates the temperature distribution over a 2D surface.
Implements the 4-point stencil method to update temperature values.
Allows customization of boundary conditions and initial temperature values.
Prints the initial and final temperature distributions.
Prints intermediate temperature distributions for the serial version.
Utilizes MPI for parallel execution in the MPI version.
Requirements

C compiler (e.g., GCC)
Standard C libraries (stdio.h, stdlib.h)
MPI library (for the MPI version)
Usage

Clone the repository or download the source code files.
Compile the desired version of the program using a C compiler. For example, for the serial version:
```
gcc program_serial.c -o program_serial
```
And for the MPI version:
```
mpicc program_mpi.c -o program_mpi
```
n the compiled executable. For example:
For the serial version:
```
./program_serial
```
For the MPI version:
```
mpirun -np <num_processes> ./program_mpi
```
For Openmp version compile with the following command:
```
gcc openmp_program_name.c -o openmp_program_name -fopenmp
```
To run the compiled code 

```
./openmp_program_name
```

Customization

You can customize the simulation parameters and initial conditions by modifying the macros defined at the beginning of each source code file.

Output

Both versions of the program output the initial temperature distribution and the final temperature distribution. Additionally, the serial version prints intermediate temperature distributions at regular intervals.

Memory Management

The programs dynamically allocate memory for the 2D temperature array using malloc(). Memory is properly deallocated using free() after the simulation is complete to avoid memory leaks.

MPI Version

The MPI version (program_mpi.c) parallelizes the simulation using MPI. It divides the 2D surface into subregions, with each MPI process responsible for updating its local subregion. Ghost cell values are communicated between neighboring processes to ensure proper boundary conditions.

Example

An example usage of the MPI version is as follows:
```
Final V_Temperature:
1.00    1.00    1.00    1.00    1.00    1.00    1.00    1.00    1.00    1.00    
1.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    1.00    
1.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    1.00    
1.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    1.00    
1.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    1.00    
1.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    1.00    
1.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    1.00    
1.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    1.00    
1.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    1.00    
1.00    1.00    1.00    1.00    1.00    1.00    1.00    1.00    1.00    1.00    

```
