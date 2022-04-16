# SAT - Solver

## Requirements
The `docker.sh` bash script allows to build the docker image and run the container. The `.env` file is used for the purpose mentioned before.

For building the docker image and run the container:

```sh
chmod +x ./docker.sh
./docker.sh build
```
```sh
./docker.sh run
root@3d7b38678779:/code# 
```

## Build the binary files
Once the container is running, the app can be compiled with the following command:
```sh
root@3d7b38678779:/code# make all
Solve successfully compiled
Slave successfully compiled
View successfully compiled
```
The binaries will be available inside the `src` folder.

## Run the app from the container
The app can be run in two different ways.

### Using pipes
```sh
root@3d7b38678779:/code/src# ./solve <path> | ./view 
```
> Note: `path` represents all the relative paths to the `.cnf` files.


### In background
First, open two terminals:
```sh
# Terminal 1
./docker.sh run
root@3d7b38678779:/code#
```
```sh
# Terminal 2
docker ps
# Copy the CONTAINER_ID whose NAME matches the CONTAINER_NAME defined in the .env file
docker exec -ti CONTAINER_ID bash
root@513e76963393:/code#
```
Now run the following commands:
```sh
# Terminal 1
root@3d7b38678779:/code/src# ./solve <path> &
<output>

# Terminal 2
root@513e76963393:/code#/src# ./view <output> 
```
> Note: `path` represents all the relative paths to the `.cnf` files.

## Results
In any case, when the execution of the application process finishes, a file called `results.txt` will be created with the results of the processed `.cnf` files. If the view process manages to connect before the application process does a *shmClose()* the same results will be displayed in standard output.

## Final comments
The app employs `minisat` application to speed up the evaluation of a propositional logic formula.
SAT solving problems are very complex. Given a formula in propositional logic, a SAT solver automatically determines whether the formula has a variable assignment that makes it true (i.e. is satisfiable). 

The code was tested using the `PVS-Studio` and `cppcheck` tools. To see these results the user can run the following command:
```sh
root@3d7b38678779:/code/src# make check 
```
The results will be available inside the `check` folder in `src`.

To test the absence memory leaks and open files, tests were made with `Valgrind` and `lsof`

```sh
root@3d7b38678779:/code/src# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./solve <path> ./view 2> ./check/pipe.valgrind 
```
> Note: `path` represents all the relative paths to the `.cnf` files.