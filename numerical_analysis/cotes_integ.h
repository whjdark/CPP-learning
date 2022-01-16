#pragma once

#include "integ_base.h"

class CotesInteg : public integ_base
{
public:
	CotesInteg() {}
	~CotesInteg() {}

	void setOrder(const size_t order);
	double calc(const double& b, const double& a, const func& f) const override;

private:
	size_t m_order = 1;
	static constexpr double C[8][8]{ {0.5, 0.5, 0, 0, 0, 0, 0, 0},
													 {1./6., 2./3., 1./6., 0, 0, 0, 0, 0},
													 {1./8., 3./8., 3./8.,1./8., 0, 0, 0, 0},
													 {7./90., 16./45., 2./15., 16./45., 7./90., 0, 0, 0},
													 {19./288., 25./96., 25./144., 25./144., 25./96., 19./288., 0, 0},
													 {41./810., 9./35., 9./280., 34./105., 9./280., 9./35., 41./840., 0},
													 {751./17280., 3577./17280., 1323./17280., 2989./17280., 2989./17280., 1323./17280., 3577./17280., 751./17280.}
													};
};