#pragma once

#include "base.h"

class ODE_base
{
public:
	ODE_base() {}
	virtual ~ODE_base() {}

	using ODEqu = std::function<double(double, double)>;

	virtual std::vector<Point2D<double>> calc(const ODEqu& f,
																		const double& b, const double& a, 
																		const double& ya, const size_t& M) const = 0;
};