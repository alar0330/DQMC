#include <iostream>
#include "../include/walker.hpp"

using std::ostream;

unsigned Walker::s_dim = 0;

Walker::Walker() {

  m_r = new double[s_dim];

  for(unsigned d = 0; d < s_dim; ++d) {
    m_r[d] = 0;
  }

  m_state = false;
  m_dim = s_dim;
}

Walker::Walker(const bool s_, const double* r_) {

  m_r = new double[s_dim];

  for(unsigned d = 0; d < s_dim; ++d) {
    m_r[d] = r_[d];
  }

  m_state = s_;
  m_dim = s_dim;
}

Walker::Walker(const Walker& w_) {

  m_r = new double[s_dim];

  for(unsigned d = 0; d < s_dim; ++d) {
    m_r[d] = w_.m_r[d];
  }

  m_state = w_.getState();
  m_dim = s_dim;
}

Walker::~Walker() {

  delete[] m_r;
}

Walker& Walker::operator=(const Walker& w_) {

  if (this == &w_) return *this;

  delete[] m_r;

  m_dim = w_.getDim();
  m_r = new double[m_dim];

  for(unsigned d = 0; d < m_dim; ++d) {
    m_r[d] = w_.m_r[d];
  }

  m_state = w_.getState();

  return *this;
}

ostream& operator<<(ostream& os, const Walker& w_) {

  os << "Walker at (";
  for(unsigned d = 0; d < w_.getDim(); ++d) {
    os << w_.m_r[d] << ",";
  }
  os << ") is " << (w_.getState() ? "ALIVE" : "DEAD") << ". \n";

  return os;
}

void Walker::freeze() {

  m_state = false;
}

void Walker::wake(const double* r_) {

  m_state = true;

  for(unsigned d = 0; d < m_dim; ++d) {
    m_r[d] = r_[d];
  }
}

void Walker::wake(const Walker& w_) {

  m_state = true;

  for(unsigned d = 0; d < w_.getDim(); ++d) {
    m_r[d] = w_.m_r[d];
  }
}
