#pragma once

#include "value.h"
#include "parameters.h"
#include "exception.h"
#include "class_wrapper.h"

namespace php {
	// 普通函数
	template <value FUNCTION(parameters& params)>
	static void function_delegate(zend_execute_data* execute_data, zval* return_value) {
		parameters params(execute_data);
		// TODO class return_type
		php::value rv;
		try {
			if(execute_data->func->common.required_num_args > ZEND_NUM_ARGS()) {
				throw exception(zend_ce_type_error, "expects at least " + std::to_string(execute_data->func->common.required_num_args) + " parameters, " + std::to_string(ZEND_NUM_ARGS()) + " given");
			}
			rv = FUNCTION(params);
		} catch (const exception& e) {
			exception::rethrow(e);
		 	return;
		}/*catch (const std::exception& e) {
			zend_throw_exception(zend_ce_error, e.what(), 0);
			return;
		} catch(...) {
			// 非可控范围的异常继续抛出
		}*/
		ZVAL_COPY(return_value, rv);
	}
	// 对象方法
	template <class T, value (T::*FUNCTION)(parameters& params)>
	static void method_delegate(zend_execute_data* execute_data, zval* return_value) {
		parameters params(execute_data);
		php::value rv;
		try {
			if(execute_data->func->common.required_num_args > params.size()) {
				throw exception(zend_ce_type_error, "expects at least " + std::to_string(execute_data->func->common.required_num_args) + " parameters, " + std::to_string(ZEND_NUM_ARGS()) + " given");
			}
			rv = (static_cast<T*>(native( Z_OBJ_P(getThis()) ))->*FUNCTION)(params);
		}catch (const exception& e) {
			exception::rethrow(e);
		 	return;
		}/*catch (const std::exception& e) {
			zend_throw_exception(zend_ce_error, e.what(), 0);
			return;
		} catch(...) {
			// 非可控范围的异常继续抛出
		}*/
		ZVAL_COPY(return_value, rv);
	}
}
