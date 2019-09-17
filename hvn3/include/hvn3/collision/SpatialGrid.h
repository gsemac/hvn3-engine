#pragma once

#include "hvn3/math/MathUtils.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/utility/Size.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace hvn3 {

	template<typename ValueType>
	class SpatialGrid {

	public:
		typedef size_t size_type;
		typedef ValueType value_type;

		typedef PointI cell_type;

		typedef std::pair<value_type*, value_type*> pair_type;
		typedef std::vector<pair_type> pair_result_type;

	private:
		struct BoundsValuePair {
			RectangleF bounds;
			value_type value;
		};

	public:
		SpatialGrid(const SizeI& cellSize) :
			SpatialGrid(cellSize.width, cellSize.height) {
		}
		SpatialGrid(int cellWidth, int cellHeight) :
			_cell_size(cellWidth, cellHeight),
			_recheck_required(false) {
		}

		void Insert(const value_type& value, const RectangleF& bounds) {
			Insert(value, bounds.Position(), bounds.Size());
		}
		void Insert(const value_type& value, const PointF& at, const SizeF& size) {
			Insert(value, at.x, at.y, size.width, size.height);
		}
		void Insert(const value_type& value, float x, float y, float width, float height) {

			// Adjust values in the case of a negative width/height.

			if (width < 0.0f) {

				x -= width;
				width = Math::Abs(width);

			}

			if (height < 0.0f) {

				y -= height;
				height = Math::Abs(height);

			}

			// Determine the cells intersected by each corner.

			float x2 = x + width;
			float y2 = y + height;

			_insert(value, RectangleF(x, y, x2, y2));

			_recheck_required = true;

		}

		size_type Count() const {
			return _values.size();
		}

		void Clear() {

			// Rather than clearing the entire grid, the vectors for each cell are cleared.
			// This allows us to avoid having to allocate new memory as often.

			_clearCells();

			_pair_buffer.clear();
			_values.clear();

		}

		value_type* Pick(const PointF& at) {
			return Pick(at.x, at.y);
		}
		value_type* Pick(float x, float y) {

			cell_type cell = _pointToCell(x, y);

			auto it = _grid.find(cell);

			if (it != _grid.end()) {

				for (auto i = it->second.begin(); i != it->second.end(); ++i)
					if ((*i)->bounds.ContainsPoint(PointF(x, y)))
						return &(*i)->value;

			}

			return nullptr;

		}

		template<typename ContainerType>
		void QueryRegion(ContainerType& outputContainer, const RectangleF& bounds) {
			QueryRegion(outputContainer, bounds.Position(), bounds.Size());
		}
		template<typename ContainerType>
		void QueryRegion(ContainerType& outputContainer, const PointF& at, const SizeF& size) {
			QueryRegion(outputContainer, at.x, at.y, size.width, size.height);
		}
		template<typename ContainerType>
		void QueryRegion(ContainerType& outputContainer, float x, float y, float width, float height) {

			std::unordered_set<value_type*> values;
			RectangleF bounds(x, y, width, height);

			_forEachIntersectedCell(bounds, [&, this](const cell_type& cell) {

				auto it = _grid.find(cell);

				if (it != _grid.end()) {

					for (auto i = it->second.begin(); i != it->second.end(); ++i)
						if (bounds.Intersects((*i)->bounds))
							values.insert(&(*i)->value);

				}

			});

			for (auto i = values.begin(); i != values.end(); ++i)
				outputContainer.push_back(*(*i));

		}

		const pair_result_type& IntersectingPairs() {

			if (_recheck_required) {

				_pair_buffer.clear();

				// A hash set is used for storing pairs in order to prevent duplicates.
				std::unordered_set<std::pair<value_type*, value_type*>, PairHasher> pairs;

				for (auto i = _grid.begin(); i != _grid.end(); ++i) {

					// Nested loop where each value in this cell is tested for intersection with other values.
					// Each comparison only occurs once.

					for (auto j = i->second.begin(); j != i->second.end(); ++j) {

						for (auto k = std::next(j); k != i->second.end(); ++k) {

							if ((*j)->bounds.Intersects((*k)->bounds)) {

								// Make it so the pair values are in the same order for the same values.
								// This makes sure they have identical hashes and aren't added more than once.

								value_type* ptr1 = &(*j)->value;
								value_type* ptr2 = &(*k)->value;

								if (ptr1 > ptr2)
									std::swap(ptr1, ptr2);

								//if (ptr1 != ptr2)
								pairs.insert({ ptr1, ptr2 });

							}

						}

					}

				}

				// Copy the pairs to the result buffer.

				for (auto i = pairs.begin(); i != pairs.end(); ++i)
					_pair_buffer.push_back(pair_type(i->first, i->second));

				_recheck_required = false;

			}

			return _pair_buffer;

		}

	private:
		SizeI _cell_size;
		std::vector<BoundsValuePair> _values;
		std::unordered_map<PointI, std::vector<BoundsValuePair*>> _grid;

		pair_result_type _pair_buffer;
		bool _recheck_required;

		BoundsValuePair* _insert(const value_type& value, const RectangleF& bounds) {

			if (_values.capacity() == _values.size()) {

				// If the vector is about to reallocate, clear all cells since their pointers will be invalidated.
				// Then reinsert all values back into the grid.

				_clearCells();

				_values.push_back({ bounds, value });

				for (auto i = _values.begin(); i != _values.end(); ++i)
					_insertIntoCells(&*i);

			}
			else {

				_values.push_back({ bounds, value });

				_insertIntoCells(&_values.back());

			}

			return &_values.back();

		}
		cell_type _pointToCell(float x, float y) {

			// Round the floating point position down (ex: [32.2, 6.2] should end up in cell [1, 0]).
			x = Math::Floor(x);
			y = Math::Floor(y);

			// Calculate the position of the cell, keeping in mind the potential for negative cell positions.
			PointI cell(
				static_cast<int>(x / 32.0f - (x < 0.0f)),
				static_cast<int>(y / 32.0f - (y < 0.0f))
			);

			return cell;

		}
		template<typename Func>
		void _forEachIntersectedCell(const RectangleF& bounds, const Func& func) {

			PointI top_left = _pointToCell(bounds.X(), bounds.Y());
			PointI bottom_right = _pointToCell(bounds.X2(), bounds.Y2());

			RectangleI rect(top_left, bottom_right);

			for (int i = rect.X(); i <= rect.X2(); ++i)
				for (int j = rect.Y(); j <= rect.Y2(); ++j)
					func(cell_type(i, j));

		}
		void _insertIntoCell(BoundsValuePair* value, const PointI& cell) {

			auto it = _grid.find(cell);

			if (it == _grid.end())
				_grid.insert({ cell, std::vector<BoundsValuePair*>{ value } });
			else
				it->second.push_back(value);

		}
		void _insertIntoCells(BoundsValuePair* value) {

			_forEachIntersectedCell(value->bounds, [&](const cell_type& cell) {
				_insertIntoCell(value, cell);
			});

		}
		void _clearCells() {

			for (auto i = _grid.begin(); i != _grid.end(); ++i)
				i->second.clear();

		}

	};

}