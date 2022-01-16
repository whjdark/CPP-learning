#include "adapt_integ.h"

AdaptInteg::AdaptInteg() 
{ //simpson version, can be replaced by something else.
	m_integ.setOrder(2); 
}

void AdaptInteg::setTol(const double& tol)
{
	m_tol = tol;
}

void AdaptInteg::setMaxIter(const size_t iter)
{
	if (iter < 1) {
		printf("Invalid iter: %lld, should > 1, default is 4.", iter);
		m_maxIter = 4;
		return;
	}
	m_maxIter = iter;
}

double AdaptInteg::calc(const double& b, const double& a, const func& f) const
{
	if (b - a <= 0.) {
		throw "Invalid interval b-a, b should be greater than a.";
	}
	return recursive(b, a, f, m_tol, m_integ.calc(b, a, f), m_maxIter);
}

double AdaptInteg::recursive(const double& b, const double& a, 
											const func& f,
											const double& tol, const double& prev, 
											const size_t iter) const
{
	double mid = 0.5 * (b + a);
	double aa = m_integ.calc(mid, a, f);
	double bb = m_integ.calc(b, mid, f);
	if (abs(aa + bb - prev) <= 15. * tol ||  iter < 1) // recursive terminated by maxIter or less than tolerance
		return aa + bb + (1./15.) * (aa + bb - prev); // S1 + S2 + eps 
	// divided into 2 subsection
	return recursive(mid, a, f, 0.5 * tol, aa, iter - 1) + recursive(b, mid, f, 0.5 * tol, bb, iter - 1);
}