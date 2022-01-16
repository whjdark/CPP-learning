#include "composite_integ.h"

void CompositeInteg::setOrder(const Order order)
{
    m_order = order;
}

void CompositeInteg::setSubInterval(const size_t subInterval)
{
	if (subInterval < 1) {
		printf("Invalid order: %lld, should be > 0, default is 2.", subInterval);
		m_subInterval = 2;
		return;
	}
    m_subInterval = subInterval;
}

auto CompositeInteg::calc(const double& b, const double& a, const func& f) const ->double
{
	double h = (b - a) / m_subInterval;
	double xk = a;
	if (h <= 0) {
		throw "Invalid interval b-a, b should be greater than a.";
	}
	double s = 0.0;
	switch (m_order){
	case Order::MIDPOINT:
		xk += 0.5 * h;
		for (size_t i = 0; i < m_subInterval; i++) {
			s += f(xk);
			xk += h;
		}
		s = h * s;
		break;
	case Order::TRAPEZOIDAL:
		for (size_t i = 0; i < m_subInterval - 1; i++) {
			xk += h;
			s += f(xk);
		}
		s = (f(a) + s * 2. + f(b)) * h * 0.5;
		break;
	case Order::SIMPSON:
		s = 4. * f(xk + 0.5 * h); // first term
		for (size_t k = 0; k < m_subInterval - 1; k++) {
			xk += h;
			s += 4. * f(xk + 0.5 * h); // f(xk+1/2) n sum
			s += 2. * f(xk); // f(xk) n-1 sum
		}
		s = (f(a) + s + f(b)) * h / 6.;
		break;
	default:
		throw "unknown method";
		break;
	}

	return s;
}