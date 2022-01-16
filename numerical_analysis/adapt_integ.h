#pragma once

#include"base.h"
#include "cotes_integ.h"

class AdaptInteg : public integ_base
{
public:
	AdaptInteg(); 
	~AdaptInteg() {}

	void setTol(const double& tol);
	void setMaxIter(const size_t iter);
	double calc(const double& b, const double& a, const func& f) const override;

private:
	double recursive(const double& b, const double& a, 
							const func& f, 
							const double& tol, const double& prev, 
							const size_t iter) const;

private:
	double m_tol = 1e-4;
	size_t m_maxIter = 4;
	CotesInteg m_integ;
};