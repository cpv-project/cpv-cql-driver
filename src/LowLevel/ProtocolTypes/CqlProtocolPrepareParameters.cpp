#include "CqlProtocolPrepareParameters.hpp"

namespace cql {
	void CqlProtocolPrepareParameters::reset() {
		flags_.set(enumValue(CqlPrepareParametersFlags::None));
	}

	CqlPrepareParametersFlags CqlProtocolPrepareParameters::getFlags() const {
		return static_cast<CqlPrepareParametersFlags>(flags_.get());
	}

	const seastar::sstring& CqlProtocolPrepareParameters::getKeySpace() const& {
		return keySpace_.get();
	}

	void CqlProtocolPrepareParameters::setKeySpace(const seastar::sstring& keySpace) {
		keySpace_.set(keySpace.data(), keySpace.size());
		flags_.set(enumValue(getFlags() | CqlPrepareParametersFlags::WithKeySpace));
	}

	void CqlProtocolPrepareParameters::setKeySpace(seastar::sstring&& keySpace) {
		keySpace_.set(CqlProtocolStringState::Normal);
		keySpace_.get() = std::move(keySpace);
		flags_.set(enumValue(getFlags() | CqlPrepareParametersFlags::WithKeySpace));
	}

	void CqlProtocolPrepareParameters::encode(seastar::sstring& data) const {
		auto flags = getFlags();
		flags_.encode(data);
		if (enumTrue(flags & CqlPrepareParametersFlags::WithKeySpace)) {
			keySpace_.encode(data);
		}
	}

	void CqlProtocolPrepareParameters::decode(const char*& ptr, const char* end) {
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & CqlPrepareParametersFlags::WithKeySpace)) {
			keySpace_.decode(ptr, end);
		}
	}

	CqlProtocolPrepareParameters::CqlProtocolPrepareParameters() :
		flags_(),
		keySpace_() { }
}

