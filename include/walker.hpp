// DQMC project: walker.hpp

/**
 *  DQMC project: walker.hpp header.
 *  This is DQMC Project to practice in C++ programming.
 *
 *  @version  v0.7 alpha (14.08.2017)
 *  @author   Alexander Arzhanov
 */

#include <iostream>

#ifndef _DQMC_WALKER_HPP_
#define _DQMC_WALKER_HPP_

class Walker {

  private:
    bool m_state;
    unsigned m_dim;
    static unsigned s_dim;

  public:
    double* m_r;

    // -- ctors --
    Walker();
    Walker(const bool, const double *);
    Walker(const Walker &);
    ~Walker();

    // -- overloads --
    Walker& operator=(const Walker &);
    friend std::ostream & operator<<(std::ostream &, const Walker &);

    // -- fun --
    static void setStaticDim(unsigned dim_) {
      s_dim = dim_;
    }
    bool getState() const {
      return m_state;
    }
    unsigned getDim() const {
      return m_dim;
    }
    void freeze();
    void wake(const double *);
    void wake(const Walker &);
};

#endif
