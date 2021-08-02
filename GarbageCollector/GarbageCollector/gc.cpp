#include "gc.h"

linked_list<gc_object> objects;
linked_list<base_gc_root_ptr> root_ptrs;

gc_object::gc_object()
	: is_reachable(false), next_ptr(nullptr), previous_ptr(nullptr)
{
	objects.push_back(this);
}

gc_object::gc_object(const gc_object& other)
	: is_reachable(false), next_ptr(nullptr), previous_ptr(nullptr)
{
	objects.push_back(this);
}

gc_object::gc_object(gc_object&& other) noexcept
	: is_reachable(false), next_ptr(other.get_next()), previous_ptr(other.get_previous())
{
	if (previous_ptr != nullptr)
	{
		previous_ptr->set_next(this);
	}
	else
	{
		objects.set_first(this);
	}

	if (next_ptr != nullptr)
	{
		next_ptr->set_previous(this);
	}
	else
	{
		objects.set_last(this);
	}

	other.set_reach_flag(false);
	other.set_next(nullptr);
	other.set_previous(nullptr);
}

gc_object::~gc_object()
{
	objects.delete_node(this);
}

void gc_object::get_ptrs(std::function<void(gc_object*)> callback) {}

gc_object& gc_object::operator=(const gc_object& other)
{
	return *this;
}

gc_object& gc_object::operator=(gc_object&& other) noexcept
{
	other.is_reachable = false;
	return *this;
}

gc_object* gc_object::get_next() const
{
	return next_ptr;
}

gc_object* gc_object::get_previous() const
{
	return previous_ptr;
}

void gc_object::set_next(gc_object* next_node)
{
	next_ptr = next_node;
}

void gc_object::set_previous(gc_object* previous_node)
{
	previous_ptr = previous_node;
}

bool gc_object::get_reach_flag() const
{
	return is_reachable;
}

void gc_object::set_reach_flag(bool value)
{
	is_reachable = value;
}

void gc_object::set_subtree_reachability(bool value)
{
	set_reach_flag(value);
	get_ptrs([&](gc_object* obj) {
		if (obj != nullptr && obj->get_reach_flag() != value)
		{
			// no cycle detected
			obj->set_subtree_reachability(value);
		}
		});
}

void gc::mark()
{
	auto root_ptr = root_ptrs.get_first();

	while (root_ptr != nullptr)
	{
		const bool reachable = true;
		root_ptr->set_objects_reachability(reachable);
		root_ptr = root_ptr->get_next();
	}
}

void gc::sweep()
{
	auto object_ptr = objects.get_first();

	while (object_ptr != nullptr)
	{
		if (!(object_ptr->get_reach_flag()))
		{
			// sweep unreachable object
			auto next_ptr = object_ptr->get_next();
			delete object_ptr;
			object_ptr = next_ptr;
		}
		else
		{
			object_ptr = object_ptr->get_next();
		}
	}

	object_ptr = objects.get_first();

	while (object_ptr != nullptr)
	{
		object_ptr->set_reach_flag(false); // reset reachability flag for next GC run
		object_ptr = object_ptr->get_next();
	}
}

void gc::collect()
{
	gc::mark();
	gc::sweep();
}

base_gc_root_ptr::base_gc_root_ptr() : next_ptr(nullptr), previous_ptr(nullptr) {}

base_gc_root_ptr::~base_gc_root_ptr() {}

base_gc_root_ptr* base_gc_root_ptr::get_next() const
{
	return next_ptr;
}

base_gc_root_ptr* base_gc_root_ptr::get_previous() const
{
	return previous_ptr;
}

void base_gc_root_ptr::set_next(base_gc_root_ptr* next_node)
{
	next_ptr = next_node;
}

void base_gc_root_ptr::set_previous(base_gc_root_ptr* previous_node)
{
	previous_ptr = previous_node;
}

void base_gc_root_ptr::set_objects_reachability(bool value) {}
