#pragma once

#include <cassert>
#include <vector>

class Matrix {
	friend class Matrix2x2;
	friend class Matrix3x3;
public:
	// ------------------デフォルトコンストラクタ------------------
	Matrix(void);
	~Matrix(void) = default;

public:
	// ------------------コンストラクタ------------------
	Matrix(std::size_t row, std::size_t column);
	Matrix(std::vector<std::vector<float>>&& vec);
	Matrix(const std::vector<std::vector<float>>& vec);
	Matrix(std::vector<float> vec);
	Matrix(std::vector<float>& vec);

protected:
	// ------------------行列------------------
	std::vector<std::vector<float>> _matrix;

public:
	// ------------------行列数保存用------------------
	/// <summary>
	/// 行数
	/// </summary>
	std::size_t row;

	/// <summary>
	/// 列数
	/// </summary>
	std::size_t column;

public:
	static const Matrix Multiply(const Matrix& matrix, const float& times);

public:
	// ------------------演算子オーバーロード------------------
	inline const std::vector<float>& operator[](std::size_t index) const {
		return _matrix[index];
	}

	inline std::vector<float>& operator[](std::size_t index) {
		return _matrix[index];
	}

	inline const Matrix operator+(const Matrix& opr) const {
		assert(row == opr.row && column == opr.column);
		Matrix result{ row, column };
		for (std::size_t i = 0; i < row; i++) {
			for (std::size_t j = 0; j < column; j++) {
				result[i][j] = _matrix[i][j] + opr[i][j];
			}
		}
		return result;
	}

	inline Matrix operator*(const Matrix& opr) const {
		assert(column == opr.row);
		Matrix result{ row, column };
		for (std::size_t i = 0; i < row; i++) {
			for (std::size_t k = 0; k < column; k++) {
				for (std::size_t j = 0; j < column; j++) {
					result[i][j] += _matrix[i][k] * opr[k][j];
				}
			}
		}
		return result;
	}

	inline Matrix& operator+=(const Matrix& opr) {
		assert(row == opr.row && column == opr.column);
		for (std::size_t i = 0; i < row; i++) {
			for (std::size_t j = 0; j < column; j++) {
				_matrix[i][j] += opr[i][j];
			}
		}
		return *this;
	}

	inline Matrix& operator*=(const Matrix& opr) {
		assert(column == opr.row);
		*this = (*this) * opr;
		return *this;
	}
};

// ------------------2x2の特殊行列を継承定義------------------
class Matrix2x2 : public Matrix {
public:
	Matrix2x2();
	Matrix2x2(std::vector<std::vector<float>> vec);

	// 固定長のためdelete
	Matrix2x2(std::size_t row, std::size_t column) = delete;
	Matrix2x2(std::vector<float> vec) = delete;

public:
	static Matrix2x2 Transpose(const Matrix2x2& matrix);
};

// ------------------3x3の特殊行列を継承定義------------------
class Matrix3x3 final {
public: // コンストラクタ
	Matrix3x3();
	Matrix3x3(std::vector<std::vector<float>> vec);

public: // static関数
	static Matrix3x3 Multiply(const Matrix3x3& matrix, const float& times);

	static Matrix3x3 Inverse(const Matrix3x3& matrix);

	static Matrix3x3 Transpose(const Matrix3x3& matrix);

public: // メンバ関数
	const Matrix3x3 inverse() const;

private:
	Matrix __matrix; // has継承

public:
	std::size_t& row; // 軽量化のため参照渡し
	std::size_t& column; // 軽量化のため参照渡し

public:
	// ------------------演算子オーバーロード------------------
	inline void operator=(const Matrix3x3& opr) {
		__matrix = opr.__matrix;
		row = __matrix.row;
		column = __matrix.column;
	}

	inline const std::vector<float>& operator[](std::size_t index) const {
		return __matrix._matrix[index];
	}
	
	inline std::vector<float>& operator[](std::size_t index) {
		return __matrix._matrix[index];
	}

	inline const Matrix3x3 operator+(const Matrix3x3& opr) const {
		return (__matrix + opr.__matrix)._matrix;
	}

	inline Matrix3x3 operator*(const Matrix3x3& opr) const {
		return (__matrix * opr.__matrix)._matrix;
	}

	inline Matrix3x3& operator+=(const Matrix3x3& opr) {
		*this = (__matrix + opr.__matrix)._matrix;
		return *this;
	}

	inline Matrix3x3& operator*=(const Matrix3x3& opr) {
		*this = (__matrix * opr.__matrix)._matrix;
		return *this;
	}
};