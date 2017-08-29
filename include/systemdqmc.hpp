// DQMC project: systemdqmc.hpp

/**
 *  DQMC project: systemdqmc.hpp header.
 *  This is DQMC Project to practice in C++ programming.
 *
 *  @version  v0.7 alpha (14.08.2017)
 *  @author   Alexander Arzhanov
 */

#include <iostream>
#include <random>
#include "walker.hpp"

#ifndef _DQMC_SYSTEMDQMC_HPP_
#define _DQMC_SYSTEMDQMC_HPP_

class SystemDQMC {

  private:
    unsigned dim;
    Walker* arrW;
    int maxN, initN, curN, prevN;
    double (*potV)(const Walker&);
    double evalV(const Walker& w) const {
      return potV(w);  // C++11 syntax
    }

    // -- random gens --
    unsigned seed;
    std::mt19937 gen;
    std::normal_distribution<> nd;
    std::uniform_real_distribution<> urd;

  public:
    // -- disable auto ctors --
    SystemDQMC & operator=(const SystemDQMC&) = delete;
    SystemDQMC(const SystemDQMC&) = delete;

    // -- only one ctor --
    SystemDQMC(int initN_, int maxN_, unsigned dim_, double* initR_);
    ~SystemDQMC();

    // -- other --
    void setPotential(double (*potV_)(const Walker&)) {
      potV = potV_;
#ifdef DEBUG
      std::cout << "[DEBUG] SystemDQMC::setPotential: setPotential called with arg potV_ = "
                << (void*) potV_ << ", system potential is now set to potV = " << (void*) potV
                << "\n";
#endif
    }

    int getAlive() const {
      return curN;
    }
    void walk(double);
    void branch(double);
    double getSysEnergy() const;
    void printSystem() const;
};

#endif
