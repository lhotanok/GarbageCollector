#pragma once

#include <functional>

class gc_object {
private:
	bool is_reachable;

	gc_object* next_ptr;
	gc_object* previous_ptr;
public:
	gc_object(); // constructor
	gc_object(const gc_object& other); // copy-constructor
	gc_object(gc_object&& other) noexcept; // move-constructor
	virtual ~gc_object(); // virtual destructor

	virtual void get_ptrs(std::function<void(gc_object*)> callback);

	gc_object& operator=(const gc_object& other); // copy-assignment
	gc_object& operator=(gc_object&& other) noexcept; // move-assignment

	gc_object* get_next() const;
	gc_object* get_previous() const;

	void set_next(gc_object* next_node);
	void set_previous(gc_object* previous_node);

	bool get_reach_flag() const;

	void set_reach_flag(bool value);
	void set_subtree_reachability(bool value);
};

class base_gc_root_ptr {
private:
	base_gc_root_ptr* next_ptr;
	base_gc_root_ptr* previous_ptr;

public:
	base_gc_root_ptr();
	virtual ~base_gc_root_ptr();

	base_gc_root_ptr* get_next() const;
	base_gc_root_ptr* get_previous() const;

	void set_next(base_gc_root_ptr* next_node);
	void set_previous(base_gc_root_ptr* previous_node);

	virtual void set_objects_reachability(bool value);
};

template <typename T>
class gc_root_ptr : public base_gc_root_ptr {
private:
	T* value_ptr;
public:
	gc_root_ptr(); // default constructor
	gc_root_ptr(T* instance); // parametric constructor
	gc_root_ptr(const gc_root_ptr<T>& other); // copy-constructor
	gc_root_ptr(gc_root_ptr<T>&& other) noexcept; // move-constructor
	virtual ~gc_root_ptr();

	gc_root_ptr<T>& operator=(const gc_root_ptr<T>& other); // copy-assignment
	gc_root_ptr<T>& operator=(gc_root_ptr<T>&& other) noexcept; // move-assignment

	T& operator*() const;
	T* operator->() const;
	operator bool() const;
	T* get();
	T& operator=(T* instance);

	void reset(T* ptr = nullptr);

	void set_objects_reachability(bool value) override;
};

template <typename T>
class linked_list {
private:
	T* first;
	T* last;

	size_t count;
	void clear();

public:
	linked_list();

	size_t get_count();
	T* get_first();
	T* get_last();

	void set_first(T* new_first);
	void set_last(T* new_last);

	void push_back(T* object_ptr);
	void delete_node(T* object_ptr);
};

class gc {
private:
	static void mark();
	static void sweep();
public:
	static void collect();
};

#include "gc_impl.h"
