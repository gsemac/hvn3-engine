#pragma once
#include <vector>

namespace hvn3 {

	template<typename T>
	//typedef int T;
	class Matrix {

	public:
		Matrix() {}
		
	private:
		std::vector<T> _mat;

	};

}