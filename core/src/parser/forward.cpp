#include <core/parser/nodes/forward.h>
#include <core/parser/nodes/callable.h>
#include <core/parser/nodes/variable.h>
#include <core/parser/nodes/class.h>
#include <core/parser/nodes/enum.h>

namespace spiritsaway::meta::language
{
	void to_json(json& j, const class_node& cur_node)
	{
		j = cur_node.to_json();
	}
	void to_json(json& j, const variable_node& cur_node)
	{
		j = cur_node.to_json();
	}
	void to_json(json& j, const callable_node& cur_node)
	{
		j = cur_node.to_json();
	}
	void to_json(json& j, const enum_node& cur_node)
	{
		j = cur_node.to_json();
	}
}