#include "cotes_integ.h"

void CotesInteg::setOrder(const size_t order)
{
	if (order < 1 || order > 7) {
		printf("Invalid order: %lld, should be 1~7, default is 2.", order);
		m_order = 2;
		return;
	}
	m_order = order;
}

auto CotesInteg::calc(const double& b, const double& a, const func& f) const ->double
{
	double interval = b - a;
	if (interval <= 0) {
		throw "Invalid interval b-a, b should be greater than a.";
	}
	double s = 0.0;
	double h = interval / m_order;
	double xk = a;

	const double* p_C = C[m_order - 1];
	for (size_t k = 0; k <= m_order; k++) {
		s += p_C[k] * f(xk);
		xk += h;
	}
	return s * interval;
}