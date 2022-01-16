#include "romber_integ.h"

RomberInteg::RomberInteg() :
	m_tol(1e-6)
{
	setMaxIter(4);
}

RomberInteg::~RomberInteg()
{
	if (m_R) {
		for (size_t i = 0; i < m_maxIter; i++) {
			delete[] m_R[i];
		}
		delete[] m_R;
	}
}

void RomberInteg::setTol(const double& tol)
{
	m_tol = tol;
}

void RomberInteg::setMaxIter(const size_t iter)
{
	if (iter == m_maxIter) {
		return;
	}
	//delete old mat
	if (m_R) {
		for (size_t i = 0; i < m_maxIter; i++) {
			delete[] m_R[i];
		}
		delete[] m_R;
	}
	//set new MaxIter
	if (iter < 1) {
		printf("Invalid iter: %lld, should > 1, default is 4.", iter);
		m_maxIter = 4;

	}
	else{ 
		m_maxIter = iter; 
	}
	// create new lower trianglar mat
	m_R = new double* [m_maxIter];
	for (size_t i = 0; i < m_maxIter; i++) {
		m_R[i] = new double[i + 1];
	}
	return;
}

double RomberInteg::calc(const double& b, const double& a, const func& f) const
{
	double h = (b - a) / 2.;
	if (h <= 0.) {
		throw "Invalid interval b-a, b should be greater than a.";
	}
	size_t M = 1;
	size_t J = 0;
	double err = 114514191810.;
	if (m_R == nullptr) {
		throw "lower trianglar matrix does not exist.";
	}

	m_R[0][0] = h * (f(a) + f(b));
	while((err > m_tol) && (J < m_maxIter - 1)){
		double s = 0;
		//composite trapezoid
		for (size_t p = 1; p <= M; p++) {
			s += f(a + h * (2 * p - 1));
		}
		m_R[J + 1][0] = 0.5 * m_R[J][0] + h * s;
		//romber table
		for (size_t K = 0; K <= J; K++) {
			m_R[J + 1][K + 1] = m_R[J + 1][K] + (m_R[J + 1][K] - m_R[J][K]) / (pow(4, K + 1) - 1);
		}
		err = abs(m_R[J][J] - m_R[J + 1][J + 1]);
		J++;
		h = 0.5 * h;
		M *= 2;
	}

	return m_R[J][J];
}
