#pragma once

#include "string.h"
#include "value_fn.h"

namespace php {
	class array_member: public value_fn {
	private:
		value&       arr_;
		zend_ulong   idx_;
		HashPosition pos_;
		string       key_;
	public:
		array_member(value& arr, const string& key);
		array_member(value& arr, zend_ulong idx);
		array_member(value& arr, HashPosition pos);
		array_member& operator =(const value& val);
		bool exists() const;
		// ---------------------------------------------------------
		virtual value ptr() const override;
		virtual zval* raw() const override;
	};
}
