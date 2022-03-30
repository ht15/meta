#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <optional>
#include <functional>
#include "forward_decl.h"
#include <core/macro.h>
#include <nlohmann/json.hpp>
#include <core/serialize/decode.h>
using json = nlohmann::json;
namespace test
{

	class entity_base
	{
	public:
		entity_type type() const
		{
			return _type;
		}
		static entity_type static_type()
		{
			return entity_type::invalid;
		}
		template <typename T>
		std::optional<std::reference_wrapper<T>> get_component();
		template <typename T>
		bool has_component()const;
		entity_base(const std::string& _in_id, entity_type cur_type):
		_type(cur_type),
		_id(_in_id)
		{

		}
	private:

		const entity_type _type;
		std::unordered_map<component_type, component_base*> _component_address;
	protected:
		void add_component(component_base* _cur_component);
		const std::string& _id;
	};
	template <typename T>
	T* create_entity(const std::string& _in_id, const json& args)
	{
		auto new_entity = new T(_in_id, args);
		new_entity->register_components();
		new_entity->on_created();
		return new_entity;
	}
}
#include "entity_base_impl.h"

