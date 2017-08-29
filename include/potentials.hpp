// DQMC project: potentials

/**
 *  DQMC project: potentials.hpp header.
 *  This is DQMC Project to practice in C++ programming.
 *
 *  @version  v1.1 alpha (15.08.2017)
 *  @author   Alexander Arzhanov
 */

inline double potentialSHO1D(const Walker& w) {
  return w.m_r[0] * w.m_r[0] / 2;
}

inline double potentialSHO3D(const Walker& w) {
  return ( w.m_r[0] * w.m_r[0] + w.m_r[1] * w.m_r[1] + w.m_r[2] * w.m_r[2] ) / 2;
}

inline double potentialMorse(const Walker& w) {
  return 0.5 * ( exp(-2.0*w.m_r[0]) - 2.0 * exp(-w.m_r[0]) );
}

inline double potentialH(const Walker& w) {
  return -1.0 / sqrt ( w.m_r[0] * w.m_r[0] + w.m_r[1] * w.m_r[1] + w.m_r[2] * w.m_r[2] );
}

inline double potentialH2p(const Walker& w) {
  static const double R1[3] = {0,0,1};
  double v(0);
  double v2(0);

  for(unsigned d = 0; d < 3; ++d) {
    v  += (w.m_r[d]-R1[d])*(w.m_r[d]-R1[d]);
    v2 += (w.m_r[d]+R1[d])*(w.m_r[d]+R1[d]);
  }

  return -1.0 / sqrt(v) - 1.0 / sqrt(v2) + 0.5;
}
