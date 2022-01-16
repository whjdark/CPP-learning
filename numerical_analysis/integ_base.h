#pragma once

#include "base.h"

class integ_base
{
public:
	integ_base() {}
	virtual ~integ_base() {}

	// integration function
	using func = std::function<double(double)>;

	virtual double calc(const double& b, const double& a, const func& f) const = 0;
};