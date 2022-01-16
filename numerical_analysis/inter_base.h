#pragma once

#include "base.h"

class inter_base
{
public:
	inter_base(const std::vector<Point2D<double>>& _pts) : m_pts(_pts){}
	virtual ~inter_base() {}

	virtual double calc(const double& target_x) const = 0;
	virtual void setPoints(const std::vector<Point2D<double>>& _pts) { m_pts = _pts; }

protected:
	std::vector<Point2D<double>> m_pts;
};
