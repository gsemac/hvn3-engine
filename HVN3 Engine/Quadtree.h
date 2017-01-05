#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include "Rectangle.h"

// todo: Create improved Quadtree using methods described here: http://lspiroengine.com/?p=530

template <typename T>
class Quadtree {

public:
	Quadtree(const Rectangle& bounds) : __bounds(bounds), __nodes() {

		__objects_per_node = 5;
		__max_levels = 5;
		__level = 0;

	}
	~Quadtree() {

		// Free memory used by nodes.
		Clear();

	}

	// Clears the tree contents.
	void Clear() {

		__objects.clear();

		for (int i = 0; i < 4; ++i)
			if (__nodes[i]) {
				__nodes[i]->Clear();
				delete __nodes[i];
				__nodes[i] = nullptr;
			}

	}

	// Inserts a new object into the tree.
	void Insert(const T& value, const Rectangle& bounds) {

		// If the tree has child nodes (it either has 4 or none), see if the bounds belong in one of them.
		if (__nodes[0]) {

			int index = GetIndex(bounds);
			if (index != -1) {
				__nodes[index]->Insert(value, bounds);
				return;
			}

		}

		// Add the value to the list objects for the node.
		__objects.push_back(std::pair<T, Rectangle>(value, bounds));

		// If we've reached the max object limit but not the depth limit, split the node.
		if (!__nodes[0] && __objects.size() > __objects_per_node && ( __level < __max_levels || __max_levels == 0)) {

			Split();

			for (size_t i = 0; i < __objects.size(); ++i) {
				int index = GetIndex(__objects[i].second);
				if (index != -1) {
					// Insert the object into a child node.
					__nodes[index]->Insert(__objects[i].first, __objects[i].second);
					// Remove the object from this node (swap & pop).
					std::swap(__objects[i], __objects[__objects.size() - 1]);
					__objects.pop_back();
				}
			}

		}

	}

	// Returns all objects whose bounding boxes intersect with the given rectangle.
	void Query(const Rectangle& region, std::vector<T>& output) {

		// Get the node index that the rectangle resides in.
		int index = GetIndex(region);

		// If it belongs in a sub-node, query that node.
		if (index != -1 && __nodes[0])
			__nodes[index]->Query(region, output);

		// Add all objects that intersect with the given region.
		for (size_t i = 0; i < __objects.size(); ++i)
			if (Intersects(region, __objects[i].second))
				output.push_back(__objects[i].first);

	}

	void Draw() {

		Graphics::DrawRectangle(__bounds, Color::Red(), 1);

	}

private:
	int __objects_per_node;
	int __max_levels;
	int __level;
	Rectangle __bounds;
	std::vector<std::pair<T, Rectangle>> __objects;
	Quadtree<T>* __nodes[4];

	// Splits the node into 4 subnodes.
	void Split() {

		float sw = __bounds.Width() / 2.0f;
		float sh = __bounds.Height() / 2.0f;
		float x = __bounds.X;
		float y = __bounds.Y;

		__nodes[0] = new Quadtree<T>(Rectangle(x + sw, y, sw, sh));
		__nodes[1] = new Quadtree<T>(Rectangle(x, y, sw, sh));
		__nodes[2] = new Quadtree<T>(Rectangle(x, y + sh, sw, sh));
		__nodes[3] = new Quadtree<T>(Rectangle(x + sw, y + sh, sw, sh));

		for (int i = 0; i < 4; ++i) {
			__nodes[i]->__level = __level + 1;
			__nodes[i]->__objects_per_node = __objects_per_node;
			__nodes[i]->__max_levels = __max_levels;
		}

	}

	// Determines which child node that the object in. Returns -1 if part of the parent node.
	int GetIndex(const Rectangle& rect) {

		int index = -1;
		Point midpoint = __bounds.Midpoint();

		// Determine what quadrant the rectangle fits inside of.
		bool top_quadrant = (rect.Y < midpoint.Y &&  rect.Y2() < midpoint.Y); // Can fit in top quadrant
		bool bottom_quadrant = (rect.Y > midpoint.Y); // Can fit in bottom quadrant
		if (rect.X < midpoint.X && rect.X2() < midpoint.X) // Can fit in left quadrant
			index = top_quadrant ? 1 : 2;
		else if (rect.X > midpoint.X) // Can fit in right quadrant
			index = top_quadrant ? 0 : 3;

		// If it didn't fit in any quadrants, we'll assign it to the parent node.
		return index;

	}

	// Prints the tree to standard output.
	void Print() {

		// Print the contents of the current node.
		for (size_t i = 0; i < __objects.size(); ++i) {
			std::cout << __objects[i].first;
			if (i < __objects.size() - 1)
				std::cout << ' ';
		}

		// Print contents of child nodes (encased in parentheses).
		if (__nodes[0]) {
			std::cout << " (";
			__nodes[0]->Print();
			std::cout << "), ";
			std::cout << '(';
			__nodes[1]->Print();
			std::cout << "), ";
			std::cout << '(';
			__nodes[2]->Print();
			std::cout << "), ";
			std::cout << '(';
			__nodes[3]->Print();
			std::cout << ")";
		}

	}

};