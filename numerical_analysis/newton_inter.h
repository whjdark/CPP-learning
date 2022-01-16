#pragma once

#include "inter_base.h"

class NewtonInterp : public inter_base
{
public:
	NewtonInterp(const std::vector<Point2D<double>>& _pts);
	~NewtonInterp();

	double calc(const double& target_x) const override;
	void setPoints(const std::vector<Point2D<double>>& _pts) override;

private:
	void constructDiffTable(const std::vector<Point2D<double>>& _pts);

private:
	double* m_diffTable;
	size_t m_tableSize;
};

