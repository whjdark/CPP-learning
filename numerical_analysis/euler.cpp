#include "euler.h"

void Euler::setType(const Type t)
{
	m_type = t;
}

auto Euler::calc(const ODEqu& f,
						const double& b, const double& a, 
						const double& ya, const size_t& M) const -> std::vector<Point2D<double>>
{
	double h = (b - a) / M;
	if (h <= 0) {
		throw "Invalid interval b-a, b should be greater than a.";
	}
	std::vector<Point2D<double>> pts;
	pts.reserve(M);
	pts.emplace_back(a, ya);
	switch (m_type)
	{
	case Type::EXPLICIT:
		for (size_t n = 0; n < M; n++) {
			double x_next = a + h * (n + 1);
			double y_next = pts[n].y + h * f(pts[n].x, pts[n].y);
			pts.emplace_back(x_next, y_next);
		}
		break;
	case Type::IMPLICIT:
		for (size_t n = 0; n < M; n++) {
			// y[n+1] is predicted using Euler method
			// Euler method can be replaced by others
			// such as Newton method
			// which involve symbolic computation
			double x_next = a + h * (n + 1);
			double yp = pts[n].y + h * f(pts[n].x, pts[n].y);
			double y_next = pts[n].y + h * f(x_next, yp);
			pts.emplace_back(x_next, y_next);
		}
		break;
	case Type::IMPROVED:  //Heun method
		for (size_t n = 0; n < M; n++) {
			double x_next = a + h * (n + 1);
			double k1 = f(pts[n].x, pts[n].y);
			double k2 = f(x_next, pts[n].y + h * k1);
			double y_next = pts[n].y + 0.5 * h * (k1 + k2);
			pts.emplace_back(x_next, y_next);
		}
		break;
	default:
		break;
	}

	return pts;
}
