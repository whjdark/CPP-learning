#include "RKs.h"

void RKs::setType(const Type t)
{
	m_type = t;
}

//void RKs::setRKF45Opt(const RKF45Option opt)
//{
//	m_opt = opt;
//}

std::vector<std::vector<Point2D<double>>> RKs::rks4(const std::vector<std::function<double(double, double)>>& f,
																	const double& b, 
																	const double& a, 
																	const std::vector<double>& ya, 
																	const size_t& M) const
{
	if (f.size() != ya.size()) {
		throw "Initial value != number of equtions.";
	}
	double h = (b - a) / M;
	std::vector<std::vector<Point2D<double>>> out;
	
	return out;
}

std::vector<std::vector<Point2D<double>>> RKs::calc(const std::vector<std::function<double(double, double)>>& f,
																	const double& b, const double& a,
																	const std::vector<double>& ya,
																	const size_t& M) const
{
	if (b - a <= 0) {
		throw "Invalid interval b-a, b should be greater than a.";
	}
	switch (m_type)
	{
	//case Type::MIDPOINT: //modified Euler-cauchy method
	//	return midPoint(f, b, a, ya, M);
	//	break;
	//case Type::HEUN:
	//	return Heun(f, b, a, ya, M);
	//	break;
	case Type::RK4:
		return rks4(f, b, a, ya, M);
		break;
	//case Type::RKF45: //Runge-Kunta-Fehlbrg
	//	return rkf45(f, b, a, ya, M);
	//	break;
	default:
		return rks4(f, b, a, ya, M);
		break;
	}
}