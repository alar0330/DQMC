#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include "../include/walker.hpp"
#include "../include/systemdqmc.hpp"

using std::ostream;
using std::cout;
using std::endl;

SystemDQMC::SystemDQMC(int initN_, int maxN_, unsigned dim_, double* initR_) :
    seed( std::chrono::system_clock::now().time_since_epoch().count() ),
    gen( seed ),
    nd( 0,1 ),
    urd( 0,1 ) {
  
  initN = initN_;
  maxN = maxN_;
  dim = dim_;
  prevN = initN_;
  curN = 0;
  
  Walker::setStaticDim(dim);
  
  arrW = new Walker[maxN];
  
  
  for(int i = 0; i < initN; ++i) {
    arrW[i].wake(initR_);
    ++curN;
  }
  
  #ifdef DEBUG
    cout << "[DEBUG] SystemDQMC(...) ctor finished.\n";
  #endif  
}

SystemDQMC::~SystemDQMC() {
  
  delete[] arrW;
  
  #ifdef DEBUG
    cout << "[DEBUG] ~SystemDQMC() dtor finished.\n";
  #endif 
}

void SystemDQMC::walk(double dt_) {
  
  double sig = sqrt(dt_);
  
  for(int n = 0; n < maxN; ++n) {
    if(arrW[n].getState()) {
      //cout << "arrW[n].getState() = " << arrW[n].getState() << "! \n";
      for(unsigned d = 0; d < dim; ++d) {
        arrW[n].m_r[d] += sig * nd(gen);
      }
    }
  }
  
  #ifdef DEBUG
    cout << "[DEBUG] SystemDQMC::walk(...) finished.\n";
  #endif
}

void SystemDQMC::branch(double dt_) {
  
  double W = 0;
  int m = 0;
  
  #ifdef DEBUG
    cout << "[DEBUG] SystemDQMC::branch(...) started with " 
         << "prevN = " << prevN << ", curN = " << curN 
         << ", sysEnergy = " << getSysEnergy() << ".\n";
  #endif
  
  double refE = getSysEnergy() - (curN - prevN) / (4 * prevN * dt_);
  
  prevN = curN;
  
  for(int n = 0; n < maxN; ++n) {
    
    if(arrW[n].getState()) {
      W = 1 - dt_ * (evalV(arrW[n]) - refE);
      m = static_cast<int>(W + urd(gen));
      
      if(m < 1) {
				arrW[n].freeze();                   // <-- freeze walker
        --curN;
			} else if(m == 1) {
				;                                   // <-- do nothing
			} else if(m == 2) {
          for(int i = 0; i < maxN; ++i) {   // <-- seed one walker
            if(!arrW[i].getState()) {
              arrW[i].wake(arrW[n]);
              ++curN;
              break;
            }
          }
			} else if(m > 2) {
          int ii;
          for(ii = 0; ii < maxN; ++ii) {     // <-- seed one walker
            if(!arrW[ii].getState()) {
              arrW[ii].wake(arrW[n]);
              ++curN;
              break;
            }
          }
          
          for(int j = ii; j < maxN; ++j) {  // <-- seed another walkers (start search from ii)
            if(!arrW[j].getState()) {
              arrW[j].wake(arrW[n]);
              ++curN;
              break;
            }
          }
			} else {
				cout << "SystemDQMC::Branching error. \n\n";
			}     
    }
  }
  
  #ifdef DEBUG
    cout << "[DEBUG] SystemDQMC::branch(...) finished with curN = " << curN
         << " refE = " << refE << ".\n";
  #endif
}

double SystemDQMC::getSysEnergy() const {
  
  double acc = 0;
  
  for(int n = 0; n < maxN; ++n) {
    if(arrW[n].getState()) {
      acc += evalV(arrW[n]);
    }
  }  
  return acc / curN;  
}

void SystemDQMC::printSystem() const {
  
  cout << "| S | D | Coors \n";
  cout << "|---|---|-----------\n";
  
  for(int n = 0; n < maxN; n++) {
    cout << "| " << arrW[n].getState() << " | " << arrW[n].getDim() << " | ( ";
    
    for(unsigned d = 0; d < dim; d++) {
      cout << arrW[n].m_r[d]  << " ";
    }
    
    cout << ")\n";
  } 
}












