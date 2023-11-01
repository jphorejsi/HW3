// HW3.cpp : Defines the entry point for the application.
//

#include "HW3.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;


class Matrix {
public:
	int rows;
	int cols;
	float** elements;

	Matrix(int rows, int cols) : rows(rows), cols(cols) {
		// Allocate memory for the 2D array
		elements = new float* [rows];
		for (int i = 0; i < rows; i++) {
			elements[i] = new float[cols];
		}
	}

	void relu() {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (elements[i][j] < 0.0f) {
					elements[i][j] = 0.0f;
				}
			}
		}
	}

	void fillFromString(const std::string& input) {
		std::string cleanInput = input;
		cleanInput.erase(std::remove(cleanInput.begin(), cleanInput.end(), '['), cleanInput.end());
		cleanInput.erase(std::remove(cleanInput.begin(), cleanInput.end(), ']'), cleanInput.end());
		std::stringstream ss(cleanInput);
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				std::string token;
				if (std::getline(ss, token, ',')) {
					elements[i][j] = std::stof(token);
				}
				else {
					std::cerr << "Error: Input string doesn't match matrix dimensions." << std::endl;
					return;
				}
			}
		}
	}

};

Matrix multiply(Matrix m1, Matrix m2) {
	Matrix newMtx = Matrix(m1.rows, m2.cols);
	if (m1.cols != m2.rows) {
		// Matrices cannot be multiplied
		throw std::invalid_argument("Matrix dimensions are not compatible for multiplication");
	}
	for (int i = 0; i < newMtx.rows; i++) {
		for (int j = 0; j < newMtx.cols; j++) {
			float sum = 0.0f;
			for (int k = 0; k < m1.cols; k++) {
				sum += m1.elements[i][k] * m2.elements[k][j];
			}
			newMtx.elements[i][j] = sum;
		}
	}
	return newMtx;
}

Matrix add(const Matrix& matrix1, const Matrix& matrix2) {
	if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
		throw std::invalid_argument("Matrix dimensions are not compatible for addition");
	}
	Matrix result(matrix1.rows, matrix1.cols);
	for (int i = 0; i < matrix1.rows; i++) {
		for (int j = 0; j < matrix1.cols; j++) {
			result.elements[i][j] = matrix1.elements[i][j] + matrix2.elements[i][j];
		}
	}
	return result;
}

Matrix f(Matrix w, Matrix b, Matrix x) {
	return add(multiply(w, x), b);
}

Matrix minMatrix(Matrix m1, Matrix m2) {
	if (m1.rows != m2.rows || m1.cols != m2.cols) {
		throw std::invalid_argument("Matrix dimensions are not compatible for min");
	}
	float sum1 = 0;
	float sum2 = 0;
	for (int i = 0; i < m1.rows; i++) {
		for (int j = 0; j < m1.cols; j++) {
			sum1 += abs(m1.elements[i][j]);
			sum2 += abs(m2.elements[i][j]);
		}
	}
	if (sum1 < sum2) {
		return m1;
	}
	else return m2;
}


int main()
{
	//Test 2
	Matrix w1 = Matrix(5, 4);
	Matrix b1 = Matrix(w1.rows, 1);
	w1.fillFromString("[[-4.0, -5.0, 1.0, -2.0], [-5.0, 3.0, 0.0, -9.0], [-4.0, 9.0, -4.0, 5.0], [3.0, 5.0, 9.0, -2.0], [-8.0, -6.0, 1.0, 6.0]]");
	b1.fillFromString("[[-3.0], [-7.0], [6.0], [-9.0], [9.0]]");
	Matrix w2 = Matrix(5, 5);
	Matrix b2 = Matrix(w2.rows, 1);
	w2.fillFromString("[[9.0, 0.0, 3.0, 6.0, 7.0], [4.0, -9.0, 8.0, 3.0, -3.0], [-6.0, -4.0, -3.0, -8.0, 0.0], [4.0, 9.0, -4.0, 4.0, -8.0], [-4.0, 0.0, 0.0, -3.0, 6.0]]");
	b2.fillFromString("[[-2.0], [7.0], [0.0], [-5.0], [6.0]]");
	Matrix w3 = Matrix(1, 5);
	Matrix b3 = Matrix(w3.rows, 1);
	w3.fillFromString("[[-4.0, -2.0, 6.0, 0.0, 4.0]]");
	b3.fillFromString("[[0.0]]");

	Matrix x = Matrix(w1.cols, 1);
	x.fillFromString("[[1.0], [1.0], [1.0], [1.0]]");

	Matrix l1 = add(multiply(w1, x), b1);
	l1.relu();
	Matrix l2 = add(multiply(w2, l1), b2);
	l2.relu();
	Matrix answer = add(multiply(w3, l2), b3);

	for (int i = 0; i < answer.rows; i++) {
		for (int j = 0; j < answer.cols; j++) {
			std::cout << answer.elements[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
