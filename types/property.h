#pragma once

namespace php {
	class object;
	class property: public value {
	private:
		php::object& object_;
		php::string  name_;
	public:
		property(php::object& obj, const php::string& name);
		property& operator =(const php::value& val);
		property& operator =(php::value&& val);
	};
}