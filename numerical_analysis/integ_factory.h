#pragma once

#include "composite_integ.h"
#include "cotes_integ.h"
#include "integ_base.h"
#include "guass_integ.h"
#include "romber_integ.h"
#include "adapt_integ.h"

class IntegFactory
{
public:
	IntegFactory() {}
	~IntegFactory() {}

	enum class IntegType
	{
		COMPOSITE,
		COTES,
		GUASS,
		ROMBER,
		ADAPT,
	};

	integ_base* create(const IntegType type) const {
		integ_base* i = nullptr;
		switch (type){
		case IntegType::COMPOSITE:
			i = new CompositeInteg();
			break;
		case IntegType::COTES:
			i = new CotesInteg();
			break;
		case IntegType::GUASS:
			i = new GuassInteg();
			break;
		case IntegType::ROMBER:
			i = new RomberInteg();
			break;
		case IntegType::ADAPT:
			i = new AdaptInteg();
			break;
		default:
			break;
		}
		return i;
	}
};