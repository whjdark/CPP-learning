#pragma once

#include"integ_base.h"

class RomberInteg : public integ_base
{
public:
	RomberInteg();
	~RomberInteg();

	void setTol(const double& tol);
	void setMaxIter(const size_t iter);
	double calc(const double& b, const double& a, const func& f) const override;

private:
	double m_tol;
	size_t m_maxIter;
	double** m_R;
};