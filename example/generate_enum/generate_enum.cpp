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

#include <core/parser/nodes/class.h>
#include <core/parser/clang_utils.h>

//#include <core/serialize/decode.h>
#include <core/parser/generator.h>

using namespace std;
using namespace spiritsaway::meta;




std::unordered_map<std::string, std::string> generate_enum()
{
	auto& the_logger = utils::get_logger();
	std::unordered_map<std::string, std::string> _annotation_value = { };
	auto all_property_enums = language::type_db::instance().get_enum_with_pred([&_annotation_value](const language::enum_node& _cur_node)
		{
			return language::filter_with_annotation_value<language::enum_node>("enum", _annotation_value, _cur_node);
		});
	std::unordered_map<std::string, std::string> result;
	mustache::mustache enum_helper_mustache_tempalte = generator::load_mustache_from_file("../mustache/enum_helper.mustache");


	for (auto one_enum : all_property_enums)
	{
		auto cur_file_path_str = one_enum->file();
		std::filesystem::path file_path(cur_file_path_str);
		auto _cur_parent_path = file_path.parent_path();
		auto generated_h_file_name = one_enum->unqualified_name() + ".generated_h";
		auto new_h_file_path = _cur_parent_path / generated_h_file_name;
		mustache::data render_args;
		render_args.set("enum_items", generator::generate_helper_for_enum(one_enum));
		render_args.set("class_name", one_enum->unqualified_name());
		render_args.set("class_full_name", one_enum->name());
		generator::append_output_to_stream(result, new_h_file_path.string(), enum_helper_mustache_tempalte.render(render_args));
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
	std::vector<const char *> cstr_arguments;

	for (const auto& i : arguments)
	{
		cstr_arguments.push_back(i.c_str());
	}

	bool display_diag = true;
	m_index = clang_createIndex(true, display_diag);
	std::string file_path = "../example/generate_enum/test_class.cpp";
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
	//utils::merge_file_content(file_content, generate_encode_decode());
	generator::merge_file_content(file_content, generate_enum());
	generator::write_content_to_file(file_content);
	clang_disposeTranslationUnit(m_translationUnit);

	return 0;

}