#include <algorithm>
#include <iterator>
#include "CollisionGrid.h"
#include "Vector2d.h"
#include "Graphics.h"
#include "Scene.h"

// Public members

CollisionGrid::CollisionGrid(const Size& cell_size) : 
	__cell_size(cell_size) {}
CollisionGrid::CollisionGrid(float cell_width, float cell_height) : 
	CollisionGrid(Size(cell_width, cell_height)) {}

// Inherited from IBroadphase

void CollisionGrid::Add(ICollidable* collider) {

	__colliders.push_back(collider);

}
void CollisionGrid::Remove(ICollidable* collider) {

	__colliders.remove(collider);

}
void CollisionGrid::Clear() {

	__grid.clear();

}
void CollisionGrid::Update() {

	// Clear existing map contents.
	__grid.clear();

	// Map all colliders to grid spaces.
	for (auto it = __colliders.begin(); it != __colliders.end(); ++it)
		MapToCells(*it);

}

const std::vector<std::pair<ICollidable*, ICollidable*>>& CollisionGrid::FindPairs() {

	// Create a set for storing pairs (to prevent duplicates).
	static std::unordered_set<std::pair<ICollidable*, ICollidable*>, PairHasher> pairs;
	pairs.clear();

	// Interate over all cells, and for all other colliders in the same cell, generate a pair.
	for (auto i = __grid.begin(); i != __grid.end();) {

		// Determine the beginning and ending boundaries for the next key.
		auto starting_it = i;
		auto ending_it = i;
		while (ending_it != __grid.end() && ending_it->first == starting_it->first)
			++ending_it;

		// Generate colliding pairs for each combination of colliders with this key.
		for (auto j = starting_it; j != ending_it; ++j)
			for (auto k = std::next(j, 1); k != ending_it; ++k)
				pairs.insert(std::pair<ICollidable*, ICollidable*>(j->second, k->second));

		// Continue the loop for the next key.
		i = ending_it;

	}

	// Copy contents to vector.
	__pairs.clear();
	std::copy(pairs.begin(), pairs.end(), std::back_inserter(__pairs));

	// Return the result.
	return __pairs;

}
void CollisionGrid::QueryRegion(const Rectangle& region, std::vector<ICollidable*>& output, int filter) const {

	// get the cells that are occupied by the region.
	static std::vector<Point> cells;
	cells.clear();
	GetIntersectedCells(region, cells);

	// Use a set to store the collider list (to prevent duplicates).
	static std::unordered_set<ICollidable*> colliders;
	colliders.clear();

	// Add all colliders in those cells to the output list.
	for (size_t i = 0; i < cells.size(); ++i) {
		auto r = __grid.equal_range(cells[i]);
		for (auto j = r.first; j != r.second; ++j)
			if (filter == 0 || j->second->CollisionId() & filter)
				colliders.insert(j->second);
	}

	// Copy contents to vector.
	std::copy(colliders.begin(), colliders.end(), std::back_inserter(output));

}
ICollidable* CollisionGrid::QueryNearest(const Point& point, int filter) const {

	// The algorithm will check a region surrounding the the given Point, and if no relevant colliders are found, the region will be expanded according by the cell size.
	// To potentially improve efficiency, the difference of the cells of the two regions could be checked instead.
	
	//ICollidable* closest = nullptr;
	//std::vector<Point> cells;
	//Rectangle region(point.X() - __cell_size.Width() / 2.0f, point.Y() - __cell_size.Height() / 2.0f, __cell_size.Width(), __cell_size.Height());
	//do {
	//	float shortest_sq = -1.0f;
	//	GetIntersectedCells(region, cells);
	//	for (size_t i = 0; i < cells.size(); ++i) {
	//		auto r = __grid.equal_range(cells[i]);
	//		for (auto j = r.first; j != r.second; ++j)
	//			if (filter == 0 || j->second->CollisionId() & filter) {
	//				float dist = PointDistanceSquared(point, j->second->AABB());
	//				if (dist < shortest_sq) {
	//					shortest_sq = dist;
	//					closest = j->second;
	//				}
	//			}
	//	}
	//	if (closest) 
	//		return closest;
	//	cells.clear();
	//} while (region.Size() < Game::Scene().Dimensions());
	return nullptr;
}

// Private members

void CollisionGrid::GetIntersectedCells(ICollidable* collider, std::vector<Point>& cells) const {

	// Get the collider's AABB. If the mask type is undefined, do not assign it to any cells.
	if (collider->Mask().Type() == MaskType::Undefined) return;
	Rectangle aabb = collider->AABB();

	// Get cells intersected by the AABB.
	GetIntersectedCells(aabb, cells);

}
void CollisionGrid::GetIntersectedCells(const Rectangle& region, std::vector<Point>& cells) const {

	// Determine the cells intersected by each corner.
	Point tl = CellAt(region.X(), region.Y());
	Point br = CellAt(region.X2() - 1.0f, region.Y2() - 1.0f);

	// Add all cells in-between to the collection.
	int cw = __cell_size.Width();
	int ch = __cell_size.Height();
	for (int i = tl.X() * cw; i <= br.X() * cw; i += cw)
		for (int j = tl.Y() * ch; j <= br.Y() * ch; j += ch)
			cells.push_back(CellAt(i, j));

}
void CollisionGrid::MapToCells(ICollidable* collider) {

	// Initialize vector for holding assigned cells (re-used for every call).
	static std::vector<Point> cells;
	cells.clear();

	// Get cells for the current collider.
	GetIntersectedCells(collider, cells);

	// Add all cells to the map.
	for (size_t i = 0; i < cells.size(); ++i)
		__grid.insert(std::pair<Point, ICollidable*>(cells[i], collider));

}

//Object* CollisionGrid::RayCast(const Line& ray) {
//
//	// Calculate the direction of the line.
//	float dir = PointDirection(ray.First(), ray.Second());
//
//	// Walk the line, checking each new sector.
//	Point cell(-1.0f, -1.0f);
//	Point current_pos = ray.First();
//	do {
//		if (cell == CellAt(current_pos)) {
//			current_pos = PointInDirection(current_pos, dir, 1.0f);
//			continue;
//		}
//		cell = CellAt(current_pos);
//		std::vector<Object*>& objects = ObjectsAt(cell.X(), cell.Y);
//		// Find the best candidate in this cell.
//		for (size_t i = 0; i < objects.size(); ++i) {
//			return objects[0];
//		}
//		current_pos = PointInDirection(current_pos, dir, 1.0f);
//	} while (current_pos.X() < ray.Second().X() && current_pos.Y() < ray.Second().Y);
//
//	return nullptr;
//}

// Private members

Point CollisionGrid::CellAt(const Point& point) const {

	return CellAt(point.X(), point.Y());

}
Point CollisionGrid::CellAt(float x, float y) const {

	return Point((std::floor)(x / __cell_size.Width()), (std::floor)(y / __cell_size.Height()));

}
void CollisionGrid::Draw(int cell_dimensions) const {

	//for (size_t i = 0; i < Columns(); ++i)
	//	for (size_t j = 0; j < Rows(); ++j) {
	//		al_draw_rectangle(i * cell_dimensions, j * cell_dimensions, i * cell_dimensions + cell_dimensions, j * cell_dimensions + cell_dimensions,
	//			al_map_rgba_f(0, 0, 0, 0.3), 2);
	//		if (__grid[i][j].size() > 0)
	//			al_draw_rectangle(i * cell_dimensions + 2, j * cell_dimensions + 2, i * cell_dimensions + cell_dimensions - 2, j * cell_dimensions + cell_dimensions - 2,
	//				al_map_rgba_f(0.8, 0.8, 0.8, 0.8), 2);
	//	}

}