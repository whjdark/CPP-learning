#pragma once

#include "base.h"

class ODEs_base
{
public:
	ODEs_base() {}
	virtual ~ODEs_base() {}

	virtual std::vector<std::vector<Point2D<double>>> calc(const std::vector<std::function<double(double, double)>>& f,
																			const double& b, const double& a,
																			const std::vector<double>& ya, 
																			const size_t& M) const = 0;
};