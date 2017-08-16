// DQMC-project: macro test

#include <iostream>
#include <random>
#include <chrono>

using std::cout;
using std::cin;
using std::endl;

// -- macro alternative
#define MTX(n,j) mtx[n*dim+j]

unsigned dim, nmax;
double* mtx;

// -- inline alternative
inline double & matrix(unsigned n_, unsigned j_) {
  return mtx[n_*dim+j_];
}

// -- main --
int main() {
  
  //unsigned dim, nmax;
  //double* mtx;
  
  // Random misc
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
	std::uniform_real_distribution<> rndreal(0,100);
	
	cout << ">>> This is Macro Test. \n\n";
  
  cout << "Enter dim = "; 
  cin >> dim;
  cout << endl;
	
  cout << "Enter nmax = "; 
  cin >> nmax;
  cout << endl << endl;
  
  mtx = new double[dim*nmax];
  
  for(unsigned n = 0; n < nmax; n++) {
    for(unsigned j = 0; j < dim; j++) {
      //MTX(n,j) = rndreal(gen);
      matrix(n,j) = rndreal(gen);
    }
  }
  

  cout << "--- Created random matrix: \n\n";
  cout << "| n | Coors \n";
	cout << "|---|---------------\n";
		
  for(unsigned n = 0; n < nmax; n++) {
    cout << "| " << n << " | ( ";
    
    for(unsigned j = 0; j < dim; j++) {
      //cout << MTX(n,j)  << " ";
      cout << matrix(n,j)  << " ";
    }
    
    cout << ")\n";
  }
	
	cout << "\n>>> Macro Test finished running. \n\n";
  
  delete[] mtx;
	
	return 0;
}

