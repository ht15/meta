﻿#include "node.h"
#include "name_space.h"
#include "utils.h"
#include <iostream>
#include <queue>
#include "nodes/type_info.h"

using namespace std;
using namespace meta;
bool interested_kind(CXCursorKind _cur_kind)
{
	switch (_cur_kind)
	{
	case CXCursor_ClassDecl:
	case CXCursor_StructDecl:
	case CXCursor_EnumDecl:
	case CXCursor_FunctionDecl:
	case CXCursor_Namespace:
	case CXCursor_VarDecl:
	case CXCursor_FunctionTemplate:
	case CXCursor_CXXMethod:
	case CXCursor_Constructor:
		return true;
	default:
		return false;
	}
}
void recursive_print_decl_under_namespace(const std::string& ns_name)
{
	std::queue<language::node*> tasks;
	auto& all_ns_nodes = language::name_space::get_synonymous_name_spaces(ns_name);
	for (const auto& one_node : all_ns_nodes)
	{
		tasks.push(one_node);
	}
	auto cur_visitor = [&ns_name](const language::node* temp_node)
	{
		if (interested_kind(temp_node->get_kind()))
		{
			utils::get_logger().debug("node {} is {} under namespace {}", temp_node->get_qualified_name(), utils::cursor_kind_to_string(temp_node->get_kind()), ns_name);
		}
		return language::node_visit_result::visit_recurse;

	};
	for (const auto& i : all_ns_nodes)
	{
		language::bfs_visit_nodes(i, cur_visitor);
	}
}
void print_func_decl_info(const language::node* _node)
{
	if (!_node)
	{
		return;
	}
	auto _cur_cursor = _node->get_cursor();
	int num_args = clang_Cursor_getNumArguments(_cur_cursor);
	
	auto & the_logger = utils::get_logger();
	
	auto _cur_type = clang_getCursorType(_cur_cursor);
	auto func_name = utils::to_string(clang_getCursorSpelling(_cur_cursor));
	auto func_type_name = utils::to_string(clang_getTypeSpelling(_cur_type));
	the_logger.info("find func {} with type {}", func_name, func_type_name);
	if (num_args < 0)
	{
		return;
	}
	auto return_type = utils::to_string(clang_getTypeSpelling(clang_getResultType(_cur_type)));
	
	the_logger.info("func {} args begin", func_name);
	for (int i = 0; i < num_args; i++)
	{
		auto arg_cursor = clang_Cursor_getArgument(_cur_cursor, i);
		auto arg_name = utils::to_string(clang_getCursorSpelling(arg_cursor));
		if (arg_name.empty())
		{
			arg_name = "no_name";
		}
		auto arg_data_type = utils::to_string(clang_getTypeSpelling(clang_getArgType(_cur_type, i)));
		the_logger.info("arg idx {} has name {} type {}", i, arg_name, arg_data_type);
	}
	the_logger.info("func {} args end", func_name);
}
void recursive_print_func_under_namespace(const std::string& ns_name)
{
	std::queue<language::node*> tasks;
	auto& all_ns_nodes = language::name_space::get_synonymous_name_spaces(ns_name);

	auto cur_visitor = [&ns_name](const language::node* temp_node)
	{
		print_func_decl_info(temp_node);

		return language::node_visit_result::visit_recurse;

	};
	for (const auto& i : all_ns_nodes)
	{
		language::bfs_visit_nodes(i, cur_visitor);
	}
}
int main(int argc, char* argv[])
{
	CXIndex m_index;
	CXTranslationUnit m_translationUnit;
	std::vector<std::string> arguments;
	arguments.push_back("-x");
	arguments.push_back("c++");
	arguments.push_back("-std=c++17");

	std::vector<const char *> cstr_arguments;

	for (const auto& i : arguments)
	{
		cstr_arguments.push_back(i.c_str());
	}
	bool display_diag = true;
	m_index = clang_createIndex(true, display_diag);
	std::string file_path = "../test/test.cpp";
	m_translationUnit = clang_createTranslationUnitFromSourceFile(m_index, file_path.c_str(), static_cast<int>(cstr_arguments.size()), cstr_arguments.data(), 0, nullptr);
	auto cursor = clang_getTranslationUnitCursor(m_translationUnit);
	auto visitor = [](CXCursor cur, CXCursor parent, CXClientData data)
	{
		// cout << "cur_cursor kind " << cur.kind << endl;
		if (cur.kind == CXCursor_LastPreprocessing)
			return CXChildVisit_Break;
		// if (clang_isCursorDefinition(cur) && interested_kind(cur.kind))
		if (interested_kind(cur.kind))
		{
			language::node_db::get_instance().create_node(cur);
		}
		return CXChildVisit_Recurse;
	};
	clang_visitChildren(cursor, visitor, nullptr);
	recursive_print_decl_under_namespace("A");

	recursive_print_func_under_namespace("A");
	clang_disposeTranslationUnit(m_translationUnit);
	return 0;
}