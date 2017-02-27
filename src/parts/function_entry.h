#pragma once

#include "../vendor.h"
#include "../types/value.h"
#include "../types/parameters.h"
#include "../types/exception.h"
#include "arguments.h"

namespace php {
	template <value FUNCTION(parameters& params)>
	class function_entry {
	public:
		static void fill(zend_function_entry* entry, const char* name) {
			entry->fname   = name;
			entry->handler = function_entry<FUNCTION>::function_delegate;
			entry->arg_info = nullptr;
			entry->num_args = 0;
			entry->flags    = 0;
		}
		static void fill(zend_function_entry* entry, const char* name, const arguments& argi) {
			entry->fname   = name;
			entry->handler = function_entry<FUNCTION>::function_delegate;
			entry->arg_info = static_cast<zend_internal_arg_info*>(argi);
			entry->num_args = (std::uint32_t)argi.length();
			entry->flags    = 0;
		}
	private:
		static void function_delegate(zend_execute_data* execute_data, zval* return_value) {
			parameters params(execute_data);
			value rv(nullptr);
			try {
				rv = FUNCTION(params);
			}catch(const exception& e) {
			 	zend_throw_exception(zend_ce_exception, e.what(), e.code());
			}catch(const std::exception& e){
				zend_throw_exception(zend_ce_exception, e.what(), -1);
			}catch(...){
				zend_throw_exception(zend_ce_exception, "unknown exception", -1);
			};
			ZVAL_COPY(return_value, rv.data());
		}
	};
}
