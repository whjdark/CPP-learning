#include "guass_integ.h"

void GuassInteg::setType(const Type t)
{
	m_type = t;
}

void GuassInteg::setPoint(const size_t point)
{
	if (point <= 1 || point > 5) {
		printf("Invalid point: %lld, should be 2~5, default is 2.", point);
		m_point = 2;
		return;
	}
	m_point = point;
}

auto GuassInteg::calc(const double& b, const double& a, const func& f) const ->double
{
	double ba = 0.5 * (b + a);
	double b_a = 0.5 * (b - a);
	if (b_a <= 0) {
		throw "Invalid interval b-a, b should be greater than a.";
	}

	double s = 0.;
	if (m_type == Type::Legendre){
		const double* p_Ak = l_Ak[m_point - 2];
		const double* p_Xk = l_Xk[m_point - 2];
		for (size_t k = 0; k < m_point; k++) {
			s += p_Ak[k] * f(ba + b_a * p_Xk[k]);
		}
	}
	else if (m_type == Type::Chebyshev) {
		const double* p_Xk = c_Xk[m_point - 2];
		for (size_t k = 0; k < m_point; k++) {
			s += f(ba + b_a * p_Xk[k]);
		}
		s *= ht::constants::pi / m_point;
	}

	return b_a * s;
}
