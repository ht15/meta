﻿#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "node.h"

namespace spiritsaway::meta::language
{
	class name_space
	{

	public:
		using name_space_set = std::unordered_set<node*>;
		const std::string& qualified_name;
		const std::string& name;
		const name_space_set& get_synonymouses() const;
		bool add_synonymous(node* _in_node);
		static const name_space_set& get_synonymous_name_spaces(const std::string& in_qualified_name);

	private:
		static std::unordered_map<std::string, name_space*> name_space_db;
		std::unordered_set<node*> synonymouses;
		name_space(node* _in_node);
		friend class node_db;
		static name_space* get_name_space_for_node(node* _in_node);
	};

}