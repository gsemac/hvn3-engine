#ifndef __DATAGRID_H
#define __DATAGRID_H
#include <vector>
#include "Grid.h"

template <typename T>
class DataGrid : public Grid {

private:
	std::vector<std::vector<T>> __data;

private:
	T __default_value;

public:
	DataGrid() {}
	DataGrid(int rows, int columns, T def_value)
		: Grid(rows, columns), __data(rows, std::vector<T>(columns, T def_value)) {}
	void InsertRow(std::vector<T>& row) {

		// Add the row to the list of data.
		__data.push_back(row);

		// Adjust the row/column size if necessary.
		__rows++;
		if (row.size() > __columns) __columns = row.size();

	}
	T& At(int x, int y) {
		return __data[y][x];
	}

};

#endif
