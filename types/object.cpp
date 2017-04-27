#include "../phpext.h"

namespace php {
	value object::_call(zend_object* obj, const char* name, std::size_t len, int argc, zval argv[]) {
		value rv, fn(name, len, false);
		zval ov;
		ZVAL_OBJ(&ov, obj);
		if(FAILURE == call_user_function_ex(&obj->ce->function_table, &ov, (zval*)&fn, (zval*)&rv, argc, argv, 1, nullptr)) {
			zend_error(E_USER_ERROR, "failed to call method '%s::%s'", ZSTR_VAL(obj->ce->name), Z_STRVAL_P((zval*)&fn));
		}
		return std::move(rv);
	}
	object::~object() {
		if(obj_ != nullptr && --GC_REFCOUNT(obj_) == 0) {
			zend_objects_destroy_object(obj_);
		}
	}
	object::object(zend_object* obj):obj_(obj) {
		++GC_REFCOUNT(obj_);
	}
	object::object(zend_class_entry* ce):obj_(zend_objects_new(ce)) {
	}
	object object::clone(const object& o1) {
		object o2(o1.obj_->ce);
		zend_objects_clone_members(o2.obj_, o1.obj_);
		return std::move(o1);
	}
	object::object(const object& obj):obj_(obj.obj_) {
		++GC_REFCOUNT(obj_);
	}
	object::object(object&& obj):obj_(obj.obj_) {
		obj.obj_ = nullptr;
	}
	object::object(const std::string& name) {
		zend_string* name_ = zend_string_init(name.c_str(), name.length(), false);
		zend_class_entry* ce = zend_lookup_class(name_);
		obj_ = zend_objects_new(ce);
	}
	value& object::prop(const char* name, std::size_t len) {
		zval objv, defv;
		value* rv;
		ZVAL_OBJ(&objv, obj_);
		rv = (value*)zend_read_property(obj_->ce, &objv, name, len, false, &defv);
		return *rv;
	}
}
