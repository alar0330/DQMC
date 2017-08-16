// DQMC project: main file 

/**
 *  DQMC main source.
 *  This is DQMC Project to practice in C++ programming.
 *  C++11 compliant.
 *
 *  @version  v1.1 alpha (15.08.2017)
 *  @author   Alexander Arzhanov
 *
 *  UPDATE HISTORY.
 *  v0.4a: 
 *     - DEBUG macro added to be #defined at compile time with -DDEBUG option.
 *  v0.5a:
 *     - dynamic 2D-array implemented. SYS macro is used for its reference.
 *     - user prompt to choose simulation parameters added.
 *     - clocking of sim time added.
 *  v0.6a:
 *     - ( temporary build with draft of Walker class )
 *  v1.0a:
 *     - completely re-written from the scratch and built on classes: Walker and SystemDQMC
 *  v1.1a:
 *     - v1.0a clean-up
 *     - progress bar added
 *     - tuple<> added
 */

#include <iostream>
#include <chrono>
#include <tuple>
#include "../include/walker.hpp"
#include "../include/systemdqmc.hpp"
#include "../include/potentials.hpp"

using std::cout;
using std::cin;
using std::endl;

typedef double (*ptrV)(const Walker&);

struct MultReturn {
  unsigned d;
  ptrV pV;
  double* iX0;
};

std::tuple<unsigned, ptrV, double*> processUserPrompt(const int);
double runSim(int, int, double, double, unsigned, ptrV, double*, unsigned);


/*
 -------------------------------------------------------------------
*/

// -- main --
int main() {
  
  int pot, maxN, initN;
  double t, dt;
  unsigned dim = 0;
  ptrV potF = nullptr;
  double* X0 = nullptr;
  
  // -- user prompt start --
  cout << "/// This is DQMC -- v1.0 alpha (15.08.2017). \n\n";
  
  cout << "Type of potentials available: \n";
  cout << "  <1> - Harmonic oscillator (1D) \n";
  cout << "  <2> - Morse potential (1D) \n";
  cout << "  <3> - Harmonic oscillator (3D) \n";
  cout << "  <4> - Hydrogen Atom (3D) \n";
  cout << "  <5> - Hydrogen-2+ Ion (3D) \n\n";
  cout << ">> Select potential (1-5): ";
  cin >> pot;
  
  cout << ">> Enter maximum psips capacity: ";
  cin >> maxN;
  
  cout << ">> Enter starting number of alive psips: ";
  cin >> initN;
  
  if(initN > maxN) {
    cout << "\nCan't exceed max capacity!\n"; 
    exit(1);
  }
  
  cout << ">> Enter step size: ";
  cin >> dt;
  
  cout << ">> Enter total evolution time: ";
  cin >> t;
  cout << endl;
  
  std::tie(dim,potF,X0) = processUserPrompt(pot);
  
  cout << "Launching simulation.. \n";
  
  auto tStart = std::chrono::high_resolution_clock::now();
  
  runSim(initN, maxN, t, dt, dim, potF, X0, pot);
  
  auto tEnd = std::chrono::high_resolution_clock::now();
  
  cout << "-- Simulation run took " 
       <<  std::chrono::duration<double, std::milli>(tEnd - tStart).count()
       << " ms to complete. \n\n";
  
  cout << "/// DQMC finished running. \n\n";
  
  delete[] X0;
  return 0;
}


/*
 -------------------------------------------------------------------
*/

// -(updated v1.1a)-
std::tuple<unsigned, ptrV, double*> processUserPrompt(const int pot) {
  
  unsigned dim;
  ptrV potF;
  double* X0;
  
  // functions of potentials are defined in potential.hpp  
  switch(pot) {
    
    case 1:
      dim = 1;
      X0 = new double[dim];
      X0[0] = 0;
      potF = potentialSHO1D;
      break;
      
    case 2:
      dim = 1;
      X0 = new double[dim];
      X0[0] = 0;
      potF = potentialMorse;
      break;
    
    case 3:
      dim = 3;
      X0 = new double[dim];
      X0[0] = 0;
      X0[1] = 0;
      X0[2] = 0;
      potF = potentialSHO3D;
      break;
      
    case 4:
      dim = 3;
      X0 = new double[dim];
      X0[0] = 0;
      X0[1] = 0;
      X0[2] = 1;
      potF = potentialH;
      break;
      
    case 5 :
      dim = 3;
      X0 = new double[dim];
      X0[0] = 0;
      X0[1] = 0;
      X0[2] = 0;
      potF = potentialH2p;
      break;
      
    default:
      cout << "Error while creating system: unknown potential. Exiting!\n";
      exit(1);
      break;
  }
  
  return std::make_tuple(dim, potF, X0);
}

double runSim(int iN_, int mN_, double t_, double dt_, unsigned dim_, 
              ptrV potF_, double* X0_, unsigned pot_) {
  
  int kRep = static_cast<int>( t_ / dt_ );
  const double eVconvert = 27.21;
  double accuE = 0;
  double sqDiffE = 0;
  double aveMeanE = 0;
  double stdE = 0;
  double * aveEHist = new double[kRep];

  SystemDQMC theSys(iN_, mN_, dim_, X0_);
  
  theSys.setPotential(potF_);
  
  cout << "Progress:\n\n";
  cout << "  0%          25%         50%         75%         100% \n";
  cout << "  |-----------|-----------|-----------|-----------|||  \n";
  cout << "  ";
  
  int progressScale = (int) kRep/50;
       
  for(int k = 0; k < kRep; k++) {
    //theSys.printSystem();
    //cout << k % progressScale << endl;
    if(!(k%progressScale)) cout << "|" << std::flush;
    theSys.walk(dt_);
    aveEHist[k] = theSys.getSysEnergy();
    theSys.branch(dt_);
    if(theSys.getAlive() < 1) {
      cout << "All walkers are dead. Exiting!\n\n"; exit(1);
    }
  }

  cout << "\n  |-----------|-----------|-----------|-----------|||  \n\n";
  
  #ifdef DEBUG
    cout << "--> History of average system energy:";
    for(int k = 0; k < kRep; ++k) {
      cout << ( (k % 10) ? ", " : "  \n" ) << aveEHist[k];
    }
    cout << endl << endl;
  #endif
  
  
  int kStart = static_cast<int>(kRep/2+1);
  for(int k = kStart; k < kRep; k++) {
    accuE += aveEHist[k];
  }
  aveMeanE = accuE / (kRep-kStart);
  
  for(int k = kStart; k < kRep; k++) {
    sqDiffE += (aveEHist[k] - aveMeanE)*(aveEHist[k] - aveMeanE);
  }
  stdE = sqrt(1.0/(kStart-2.0) * sqDiffE);
  
  cout << "--> Average E during second part of Simulation is <E> = " << aveMeanE << "\n";
  cout << "    .. which in eV units is <E> = " << aveMeanE*eVconvert << " [eV]\n";
  cout << "    The standard deviation is std(<E>) = " << stdE << "\n";
  cout << "    Parameters for this potential <" << pot_ << "> in " << dim_ << "D:\n";
  cout << "      initN = " << iN_ << endl;
  cout << "       maxN = " << mN_ << endl;
  cout << "          t = " << t_ << endl; 
  cout << "         dt = " << dt_ << endl << endl;
  
  delete[] aveEHist;
  return aveMeanE;
}