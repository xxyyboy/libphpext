#pragma once

#include "value.h"

namespace php {
	class parameter;
	class array_member;
	class property;
	
	class callable: public value {
	private:
		static value __call(zval* cb);
		static value __call(zval* cb, std::vector<value> argv);
	public:
		callable(); // undefined
		callable(std::nullptr_t n);
		callable(zval* v, bool ref = false);
		callable(const char* fname);
		callable(const std::string& fname);
		callable(zend_object* cb);
		callable(std::function<php::value (php::parameters& params)> v);
		callable(const value& v);
		callable(value&& v);
		callable(const parameter& v);
		callable(const array_member& v);
		callable(const property& v);
		// ---------------------------------------------------------------------
		value call() const;
		value call(std::vector<value> argv) const;
		value operator()() const;
		value operator()(std::vector<value> argv) const;
		// -------------------------------------------------------------------
		using value::operator =;
	};
}
