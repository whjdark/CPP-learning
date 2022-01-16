#pragma once

#include "ODE.h"

class Euler : public ODE_base
{
public:
	Euler() {}
	~Euler() {}

	enum class Type {
		EXPLICIT,
		IMPLICIT,
		IMPROVED,
	};

	void setType(const Type t);
	std::vector<Point2D<double>> calc(const	ODEqu& f,
															const double& b, const double& a,
															const double& ya, const size_t& M) const override;

private:
	Type m_type = Type::EXPLICIT;
};