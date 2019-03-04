// Lab 5 - COMP3361 Operating Systems Winter 2019
//
// Implementation of deadlock detection algorithm.
// Code is provided to read a system configuration
// from the input file (specified by the program
// parameter). You should fill in code as noted by
// the "TODO" blocks to detect deadlocks (and remove
// the corresponding "not implemented" messages)
// See the assignment for details on the output format.
//
// Template author: Mike Goss (mikegoss@cs.du.edu)
//
// Student name: Evan Derby and Jason Ghiglieri

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::cerr;
using std::ifstream;
using std::setw;
using std::vector;

namespace {  // declarations local to this file (non-global)

// Define arrays for algorithm
vector<int> available;
vector<vector<int>> allocation;
vector<vector<int>> request;

unsigned numProcesses;   // actual number of processes
unsigned numResources;   // actual number of resources

vector<bool> terminate(numProcesses);   // whether or not any process i has terminated or not

// IsRequestLessEqual - compare row of Request array to Available
//
// @param i - index of row in Request array
// @returns true if every element of Request[i] is <= corresponding element
//          of Available, false otherwise.
bool IsRequestLessEqual(int i) {
  for (int k = 0; k < request[i].size(); k++) {
      if (request[i][k] > available[k]) {
          return false;
      }
  }
  return true;
}

// AddToAvailable - add each element in Allocation[i] to corresponding
//   element in Available
//
// @param i - index of row in Allocation array
void AddToAvailable(int i) {
    for (int k = 0; k < allocation[i].size(); k++) {
        if (allocation[i][k] > 0) {
            available[k] += allocation[i][k];
            allocation[i][k] = 0;
            request[i][k] = 0;
        }
    }
    terminate[i] = true;
}

/**
 * terminated - checks if given process i has terminated
 * 
 * @param i - Process number to check
 * @return  True if all requests and allocations for i = 0. Otherwise false
 */
bool terminated(int i) {
    
    return terminate[i];
}

// PrintDeadlocks - print indices of deadlocked processes
//
void PrintDeadlocks(void) {
    cout << "Deadlocked Processes: ";
    
    for (int k = 0; k < numProcesses; k++) {
        terminate[k] = false;
    }
    
    for (int i = 0; i < numProcesses; i++) {
        if ((!terminated(i)) && IsRequestLessEqual(i)) {
            AddToAvailable(i);
            i = -1;
        }
    }
    
    for (int n = 0; n < numProcesses; n++) {
        if (!IsRequestLessEqual(n)) {
            cout << std::to_string(n) << " ";
        }
    }
    cout << std::endl;
}

// ReadSystemConfig - read the system configuration from the
//   input file, specified in argv[1].
//
// @param fileName - configuration file
void ReadSystemConfig(const char *fileName) {
  // Open the input file
  ifstream in;
  in.open(fileName);
  if (!in.is_open()) {
    cerr << "ERROR: input file not found: " << fileName << "\n";
    exit(2);
  }
  
  // Read the number of processes and resources
  in >> numProcesses >> numResources;
  if (in.fail()) {
    cerr << "ERROR: failed to read numProcesses and numResources\n";
    exit(2);
  }
  cout << numProcesses << " processes, " << numResources << " resources\n";
  // Allocate the arrays
  available.resize(numResources, 0);
  allocation.resize(numProcesses);
  request.resize(numProcesses);
  for (unsigned i = 0; i < numProcesses; ++i) {
    allocation[i].resize(numResources, 0);
    request[i].resize(numResources, 0);
  }
  
  // Read the Available array
  for (int i = 0; i < numResources; ++i) {
    in >> available[i];
    if (in.fail()) {
      cerr << "ERROR: failed to read Available[" << i << "]\n";
      exit(2);
    }
  }
  cout << "Available:\n\t";
  for (int i = 0; i < numResources; ++i) {
    cout << setw(3) << available[i];
  }
  cout << "\n";
  
  // Read the Allocation array
  for (int i = 0; i < numProcesses; ++i) {
    for (int j = 0; j < numResources; ++j) {
      in >> allocation[i][j];
      if (in.fail()) {
        cerr << "ERROR: failed to read Allocation[" << i << "][" << j << "]\n";
        exit(2);
      }
    }
  }
  cout << "Allocation:\n";
  for (int i = 0; i < numProcesses; ++i) {
    cout << "\t";
    for (int j = 0; j < numResources; ++j) {
      cout << setw(3) << allocation[i][j];
    }
    cout << "\n";
  }
  
  // Read the Request array
  for (int i = 0; i < numProcesses; ++i) {
    for (int j = 0; j < numResources; ++j) {
      in >> request[i][j];
      if (in.fail()) {
        cerr << "ERROR: failed to read Request[" << i << "][" << j << "]\n";
        exit(2);
      }
    }
  }
  cout << "Request:\n";
  for (int i = 0; i < numProcesses; ++i) {
    cout << "\t";
    for (int j = 0; j < numResources; ++j) {
      cout << setw(3) << request[i][j];
    }
    cout << "\n";
  }
}

} // namespace

int main(int argc, char** argv) {
  // Read system configuration
  if (argc != 2) {
    cerr << "usage: lab5 filename\n";
    exit(1);
  }
  ReadSystemConfig(argv[1]);
  PrintDeadlocks();

  return 0;
}
