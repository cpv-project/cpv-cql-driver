#pragma once
#include <vector>
#include <core/shared_ptr.hh>
#include "CqlNodeConfiguration.hpp"

namespace cql {
	class CqlNodeCollection {
		virtual seastar::shared_ptr<CqlNodeConfiguration> chooseOneNode() = 0;
		
		virtual void reportFailure(const seastar::shared_ptr<CqlNodeConfiguration>& node) = 0;

		virtual void reportSuccess(const seastar::shared_ptr<CqlNodeConfiguration>& node) = 0;

		static seastar::shared_ptr<CqlNodeCollection> create(std::vector<CqlNodeConfiguration>&& nodes);
	};
}

