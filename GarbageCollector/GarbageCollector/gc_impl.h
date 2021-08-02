#pragma once

#include "gc.h"

extern linked_list<gc_object> objects;
extern linked_list<base_gc_root_ptr> root_ptrs;

template<typename T>
inline gc_root_ptr<T>::gc_root_ptr()
	: value_ptr(nullptr)
{
	root_ptrs.push_back(this);
}

template<typename T>
inline gc_root_ptr<T>::gc_root_ptr(T* instance)
	: value_ptr(instance)
{
	root_ptrs.push_back(this);
}

template<typename T>
inline gc_root_ptr<T>::gc_root_ptr(const gc_root_ptr<T>& other)
	: value_ptr(other.value_ptr)
{
	root_ptrs.push_back(this);
}

template<typename T>
inline gc_root_ptr<T>::gc_root_ptr(gc_root_ptr<T>&& other) noexcept
	: value_ptr(other.value_ptr)
{
	other.value_ptr = nullptr;
}

template<typename T>
inline gc_root_ptr<T>::~gc_root_ptr()
{
	if (root_ptrs.get_count() != 0)
	{
		root_ptrs.delete_node(this);
	}
}

template<typename T>
inline gc_root_ptr<T>& gc_root_ptr<T>::operator=(const gc_root_ptr<T>& other)
{
	value_ptr = other.value_ptr;
	return *this;
}

template<typename T>
inline gc_root_ptr<T>& gc_root_ptr<T>::operator=(gc_root_ptr<T>&& other) noexcept
{
	value_ptr = other.value_ptr;
	other.value_ptr = nullptr;
	return *this;
}

template<typename T>
inline T& gc_root_ptr<T>::operator*() const
{
	return *value_ptr;
}

template<typename T>
inline T* gc_root_ptr<T>::operator->() const
{
	return value_ptr;
}

template<typename T>
inline gc_root_ptr<T>::operator bool() const
{
	return value_ptr != nullptr;
}

template<typename T>
inline T* gc_root_ptr<T>::get()
{
	return value_ptr;
}

template<typename T>
inline T& gc_root_ptr<T>::operator=(T* instance)
{
	value_ptr = instance;
	return *value_ptr;
}

template<typename T>
inline void gc_root_ptr<T>::reset(T* ptr)
{
	value_ptr = ptr;
}

template<typename T>
inline void gc_root_ptr<T>::set_objects_reachability(bool value)
{
	if (get() != nullptr)
	{
		get()->set_subtree_reachability(value);
	}
}

template<typename T>
inline void linked_list<T>::clear()
{
	first = nullptr;
	last = nullptr;
	count = 0;
}

template<typename T>
inline linked_list<T>::linked_list()
{
	count = 0;
	first = nullptr;
	last = nullptr;
}

template<typename T>
inline T* linked_list<T>::get_first()
{
	return first;
}

template<typename T>
inline T* linked_list<T>::get_last()
{
	return last;
}

template<typename T>
inline void linked_list<T>::set_first(T* new_first)
{
	first = new_first;
}

template<typename T>
inline void linked_list<T>::set_last(T* new_last)
{
	last = new_last;
}

template<typename T>
inline size_t linked_list<T>::get_count()
{
	return count;
}

template<typename T>
inline void linked_list<T>::push_back(T* object_ptr)
{
	if (count == 0)
	{
		first = object_ptr;
	}
	else if (count == 1)
	{
		first->set_next(object_ptr);
	}
	else
	{
		last->set_next(object_ptr);
	}

	object_ptr->set_previous(last);
	last = object_ptr;

	count++;
}

template<typename T>
inline void linked_list<T>::delete_node(T* object_ptr)
{
	auto previous = object_ptr->get_previous();
	auto next = object_ptr->get_next();

	if (previous == nullptr && next == nullptr)
	{
		clear();
	}
	else
	{
		if (previous != nullptr)
		{
			previous->set_next(next);
			if (previous->get_previous() == nullptr)
			{
				set_first(previous);
			}
			if (previous->get_next() == nullptr)
			{
				set_last(previous);
			}
		}

		if (next != nullptr)
		{
			next->set_previous(previous);
			if (next->get_next() == nullptr)
			{
				set_last(next);
			}
			if (next->get_previous() == nullptr)
			{
				set_first(next);
			}
		}

		count--;
	}
}
