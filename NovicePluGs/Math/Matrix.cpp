#include "Matrix.h"

Matrix::Matrix() {
	row = 0;
	column = 0;
	_matrix.clear();
}

Matrix::Matrix(std::size_t row, std::size_t column) {
	this->row = row;
	this->column = column;
	this->_matrix.assign(this->row, std::vector<float>(this->column, 0));
}

Matrix::Matrix(std::vector<std::vector<float>>&& vec) {
	assert(!vec.empty());
	this->_matrix = vec;
	row = _matrix.size();
	column = _matrix[0].size();
}

Matrix::Matrix(const std::vector<std::vector<float>>& vec) {
	assert(!vec.empty());
	this->_matrix = vec;
	row = _matrix.size();
	column = _matrix[0].size();
}

Matrix::Matrix(std::vector<float>& vec) {
	assert(!vec.empty());
	_matrix.assign(1, vec);
	row = _matrix.size();
	column = _matrix[0].size();
}

const Matrix Matrix::Multiply(const Matrix& matrix, const float& times) {
	Matrix result{ matrix.row, matrix.column };
	for (int i = 0; i < result.row; i++) {
		for (int j = 0; j < result.column; j++) {
			result[i][j] = matrix[i][j] * times;
		}
	}
	return result;
}

Matrix::Matrix(std::vector<float> vec) {
	assert(!vec.empty());
	_matrix.assign(1, vec);
	row = _matrix.size();
	column = _matrix[0].size();
}

Matrix2x2::Matrix2x2() {
	row = 2;
	column = 2;
	_matrix.assign(2, std::vector<float>(2, 0));
}

Matrix2x2::Matrix2x2(std::vector<std::vector<float>> vec) {
	assert(vec.size() == 2 && vec.front().size() == 2);
	row = 2;
	column = 2;
	_matrix = vec;
}

Matrix2x2 Matrix2x2::Transpose(const Matrix2x2& matrix) {
	Matrix2x2 result;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			result._matrix[i][j] = matrix._matrix[j][i];
		}
	}
	return result;
}

Matrix3x3::Matrix3x3() :
	row(__matrix.row),
	column(__matrix.column) {
	__matrix = { 3, 3 };
}

Matrix3x3::Matrix3x3(std::vector<std::vector<float>> vec) :
	row(__matrix.row),
	column(__matrix.column) {
	assert(vec.size() == 3 && vec.front().size() == 3);
	__matrix = vec;
}

Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& matrix, const float& times) {
	return Matrix::Multiply(matrix.__matrix,times)._matrix;
}

Matrix3x3 Matrix3x3::Inverse(const Matrix3x3& matrix) {
	return matrix.inverse();
}

Matrix3x3 Matrix3x3::Transpose(const Matrix3x3& matrix) {
	Matrix3x3 result;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result.__matrix[i][j] = matrix.__matrix[j][i];
		}
	}
	return result;
}

const Matrix3x3 Matrix3x3::inverse() const {
	float length =
		__matrix[0][0] * __matrix[1][1] * __matrix[2][2] +
		__matrix[0][1] * __matrix[1][2] * __matrix[2][0] +
		__matrix[0][2] * __matrix[1][0] * __matrix[2][1] -

		__matrix[0][2] * __matrix[1][1] * __matrix[2][0] -
		__matrix[0][1] * __matrix[1][0] * __matrix[2][2] -
		__matrix[0][0] * __matrix[1][2] * __matrix[2][1];
	assert(length != 0);
	Matrix3x3 result;
	result.__matrix._matrix = {
		{ (__matrix[1][1] * __matrix[2][2] - __matrix[1][2] * __matrix[2][1]) / length, -(__matrix[0][1] * __matrix[2][2] - __matrix[0][2] * __matrix[2][1]) / length, (__matrix[0][1] * __matrix[1][2] - __matrix[0][2] * __matrix[1][1]) / length },
		{ -(__matrix[1][0] * __matrix[2][2] - __matrix[1][2] * __matrix[2][0]) / length, (__matrix[0][0] * __matrix[2][2] - __matrix[0][2] * __matrix[2][0]) / length, -(__matrix[0][0] * __matrix[1][2] - __matrix[0][2] * __matrix[1][0]) / length },
		{ (__matrix[1][0] * __matrix[2][1] - __matrix[1][1] * __matrix[2][0]) / length, -(__matrix[0][0] * __matrix[2][1] - __matrix[0][1] * __matrix[2][0]) / length, (__matrix[0][0] * __matrix[1][1] - __matrix[0][1] * __matrix[1][0]) / length },
	};
	return result;
}
