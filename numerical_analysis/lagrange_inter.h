#pragma once

#include "inter_base.h"

class LagrangeInterp : public inter_base
{
public:
	LagrangeInterp(const std::vector<Point2D<double>>& _pts) : inter_base(_pts) {}
	~LagrangeInterp() {};

	double calc(const double& target_x) const override;
};
