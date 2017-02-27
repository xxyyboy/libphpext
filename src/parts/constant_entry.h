#pragma once

namespace php
{

class constant_entry {
public:
	// 常量须使用 持久 persistent 类型值 value（字符串、数组）
	constant_entry(const std::string& name, const value& val);
	void fill(zend_constant* entry, int module);
private:
	std::string name_;
	value       value_;
};

}
