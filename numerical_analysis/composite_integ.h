#pragma once

#include "cotes_integ.h"

class CompositeInteg : public integ_base
{
public:
	CompositeInteg() {}
	~CompositeInteg() {}

	enum class Order {
		MIDPOINT,
		TRAPEZOIDAL,
		SIMPSON,
	};

	void setOrder(const Order order);
	void setSubInterval(const size_t subInterval);
	double calc(const double& b, const double& a, const func& f) const override;

private:
	Order m_order = Order::TRAPEZOIDAL;
	size_t m_subInterval = 2;
};