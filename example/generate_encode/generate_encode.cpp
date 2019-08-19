﻿
#include <iostream>
#include <iomanip>

#include <cstdint>
#include <fstream>
#include <iomanip>

#include <iostream>
#include <sstream>
#include <queue>
#include <filesystem>

#include <mustache.hpp>

#include "nodes/type_info.h"
#include "name_space.h"
#include "utils.h"
#include "nodes/class.h"
#include "nodes/enum.h"
#include "nodes/variable.h"
#include <serialize/encode.h>
#include <utility/generate_utils.h>

using namespace std;
using namespace meta;
namespace mustache = kainjow::mustache;


std::unordered_map<std::string, std::string> generate_encode()
{
	// 遍历所有的class 对于里面表明了需要生成encode的类进行处理
	auto& the_logger = utils::get_logger();
	auto _class_with_encode_prop = [&the_logger](const language::class_node& _cur_node)
	{
		auto& _cur_annotations = _cur_node.annotations();
		auto cur_iter = _cur_annotations.find("encode");
		if (cur_iter == _cur_annotations.end())
		{
			return false;
		}
		else
		{
			switch (cur_iter->second.size())
			{
			case 0:
				the_logger.error("class {} has encode annotation but without args", _cur_node.name());
				return false;
				break;
			case 1:
				if (cur_iter->second[0] == "auto")
				{
					return true;
				}
				else
				{
					return false;
				}
			default:
				return false;
				break;
			}
		}
	};
	auto _field_with_encode_prop = [&the_logger](const language::variable_node& _cur_node)
	{
		auto& _cur_annotations = _cur_node.annotations();
		auto cur_iter = _cur_annotations.find("encode");
		if (cur_iter == _cur_annotations.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	};
	auto& all_encode_classes = language::type_db::instance().get_class_with_pred(_class_with_encode_prop);
	std::unordered_map<std::string, std::string> result;
	auto encode_func_mustache_file = std::ifstream("../mustache/encode_func.mustache");
	std::string template_str = std::string(std::istreambuf_iterator<char>(encode_func_mustache_file), std::istreambuf_iterator<char>());
	mustache::mustache encode_func_mustache_tempalte(template_str);
	for (auto one_class : all_encode_classes)
	{
		auto cur_file_path_str = one_class->file();
		the_logger.info("get class {} with annotation prop encode location {}", one_class->unqualified_name(), cur_file_path_str);
		std::filesystem::path file_path(cur_file_path_str);
		auto _cur_parent_path = file_path.parent_path();
		auto generated_h_file_name = one_class->unqualified_name() + "_generated.h";
		auto new_h_file_path = _cur_parent_path / generated_h_file_name;
		// 首先encode父类 按照父类的名称排序
		auto _bases = one_class->bases();
		std::sort(_bases.begin(), _bases.end(), [](const language::type_info* a, const language::type_info* b)
			{
				if (a->name() < b->name())
				{
					return true;
				}
				else
				{
					return false;
				}
			});
		mustache::data base_list{mustache::data::type::list};
		for (auto one_base : _bases)
		{
			base_list << mustache::data{"base_type", one_base->name()};
		}
		// 然后encode自己的变量
		auto encode_fields = one_class->query_fields_with_pred(_field_with_encode_prop);
		std::sort(encode_fields.begin(), encode_fields.end(), [](const language::variable_node* a, const language::variable_node* b)
			{
				if (a->unqualified_name() < b->unqualified_name())
				{
					return true;
				}
				else
				{
					return false;
				}
			});
		mustache::data field_list{ mustache::data::type::list };
		for (auto one_field :encode_fields)
		{
			field_list << mustache::data{ "field_name", one_field->unqualified_name() };
		}
		mustache::data render_args;
		render_args.set("fields", field_list);
		render_args.set("bases", base_list);
		auto encode_str = encode_func_mustache_tempalte.render(render_args);
		utils::append_output_to_stream(result, new_h_file_path.string(), encode_str);
	}
	return result;
}
int main()
{
	auto& the_logger = utils::get_logger();
	CXIndex m_index;
	CXTranslationUnit m_translationUnit;
	std::vector<std::string> arguments;
	arguments.push_back("-x");
	arguments.push_back("c++");
	arguments.push_back("-std=c++17");
	arguments.push_back("-D__meta_parse__");
	arguments.push_back("-ID:/usr/include/");
	arguments.push_back("-I../include/");
	the_logger.info("arguments is {}", utils::join(arguments, ","));
	std::vector<const char *> cstr_arguments;

	for (const auto& i : arguments)
	{
		cstr_arguments.push_back(i.c_str());
	}
	
	bool display_diag = true;
	m_index = clang_createIndex(true, display_diag);
	std::string file_path = "../example/generate_encode/test_class.cpp";
	//std::string file_path = "sima.cpp";
	m_translationUnit = clang_createTranslationUnitFromSourceFile(m_index, file_path.c_str(), static_cast<int>(cstr_arguments.size()), cstr_arguments.data(), 0, nullptr);
	auto cursor = clang_getTranslationUnitCursor(m_translationUnit);
	the_logger.info("the root cursor is {}", utils::to_string(cursor));
	auto& cur_type_db = language::type_db::instance();
	cur_type_db.create_from_translate_unit(cursor);
	//recursive_print_decl_under_namespace("A");
	cur_type_db.build_class_under_namespace("std");
	cur_type_db.build_class_under_namespace("test");
	//recursive_print_func_under_namespace("A");
	//recursive_print_class_under_namespace("A");
	json result = language::type_db::instance().to_json();
	ofstream json_out("type_info.json");
	json_out << setw(4) << result << endl;
	std::unordered_map<std::string, std::string> file_content;
	utils::merge_file_content(file_content, generate_encode());
	utils::write_content_to_file(file_content);
	clang_disposeTranslationUnit(m_translationUnit);

	return 0;
    
}