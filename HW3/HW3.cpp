// HW3.cpp : Defines the entry point for the application.
//

#include "HW3.h"
#include <string>
#include <iostream>
#include <sstream>
//#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>

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
		// throw std::invalid_argument("Matrix dimensions are not compatible for addition");
	}
	Matrix result(matrix1.rows, matrix1.cols);
	for (int i = 0; i < matrix1.rows; i++) {
		for (int j = 0; j < matrix1.cols; j++) {
			result.elements[i][j] = matrix1.elements[i][j] + matrix2.elements[i][j];
		}
	}
	return result;
}


float sumOfAbsoluteValues(const Matrix& m) {
	float sum = 0.0f;
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			sum += std::abs(m.elements[i][j]);
		}
	}
	return sum;
}

Matrix randomSearch(Matrix& w1, Matrix& b1, Matrix& w2, Matrix& b2, Matrix& w3, Matrix& b3, int num_attempts) {
	Matrix best_x(w1.cols, 1);
	best_x.fillFromString("[[1.0], [1.0], [1.0], [1.0]]");
	Matrix x(w1.cols, 1);
	x.fillFromString("[[1.0], [1.0], [1.0], [1.0]]");

	float best_sum = std::numeric_limits<float>::infinity();

	for (int attempt = 0; attempt < num_attempts; ++attempt) {
		
		// Generate random input change
		for (int i = 0; i < x.rows; i++) {
			for (int j = 0; j < x.cols; j++) {
				x.elements[i][j] += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f; //random between -1 and 1
			}
		}

		// Evaluate the network with the new input
		Matrix l1 = add(multiply(w1, x), b1);
		l1.relu();
		Matrix l2 = add(multiply(w2, l1), b2);
		l2.relu();
		Matrix answer = add(multiply(w3, l2), b3);

		// Check if the new input gives a sum of outputs closer to zero
		float current_sum = sumOfAbsoluteValues(answer);
		if (current_sum < best_sum) {	//x is new best x
			std::cout << sumOfAbsoluteValues(answer) << std::endl;
			best_sum = current_sum;
			for (int i = 0; i < best_x.rows; i++) {
				for (int j = 0; j < best_x.cols; j++) {
					best_x.elements[i][j] = x.elements[i][j];
				}
			}
		}
		else{
			//bring x back to best_x
			for (int i = 0; i < best_x.rows; i++) {
				for (int j = 0; j < best_x.cols; j++) {
					x.elements[i][j] = best_x.elements[i][j];
				}
			}
		}
	}
		return best_x;
}

int main() {
	srand(static_cast<unsigned int>(time(nullptr)));
	//Test 2 [[2.0], [-2.0], [-3.0], [1.0]]		0
	//Matrix w1 = Matrix(5, 4);
	//Matrix b1 = Matrix(w1.rows, 1);
	//w1.fillFromString("[[-4.0, -5.0, 1.0, -2.0], [-5.0, 3.0, 0.0, -9.0], [-4.0, 9.0, -4.0, 5.0], [3.0, 5.0, 9.0, -2.0], [-8.0, -6.0, 1.0, 6.0]]");
	//b1.fillFromString("[[-3.0], [-7.0], [6.0], [-9.0], [9.0]]");
	//Matrix w2 = Matrix(5, 5);
	//Matrix b2 = Matrix(w2.rows, 1);
	//w2.fillFromString("[[9.0, 0.0, 3.0, 6.0, 7.0], [4.0, -9.0, 8.0, 3.0, -3.0], [-6.0, -4.0, -3.0, -8.0, 0.0], [4.0, 9.0, -4.0, 4.0, -8.0], [-4.0, 0.0, 0.0, -3.0, 6.0]]");
	//b2.fillFromString("[[-2.0], [7.0], [0.0], [-5.0], [6.0]]");
	//Matrix w3 = Matrix(1, 5);
	//Matrix b3 = Matrix(w3.rows, 1);
	//w3.fillFromString("[[-4.0, -2.0, 6.0, 0.0, 4.0]]");
	//b3.fillFromString("[[0.0]]");
	////Matrix x = Matrix(w1.cols, 1);
	////x.fillFromString("[[2.0], [-2.0], [-3.0], [1.0]]");
	///*Matrix l1 = add(multiply(w1, x), b1);
	//l1.relu();
	//Matrix l2 = add(multiply(w2, l1), b2);
	//l2.relu();
	//Matrix answer = add(multiply(w3, l2), b3);*/
	////std::cout << sumOfAbsoluteValues(answer);
	//int num_attempts = 1000;
	//Matrix best_input = randomSearch(w1, b1, w2, b2, w3, b3, num_attempts);
	//std::cout << "Best input vector that minimizes the output towards zero:" << std::endl;
	//for (int i = 0; i < best_input.rows; i++) {
	//	std::cout << best_input.elements[i][0] << " ";
	//}
	//std::cout << std::endl;


	//Test 3
	//0.154787
	//1.38884 0.192694 3.92187 2.66823 4.73965 -0.318552
	//Matrix w1 = Matrix(5, 6);
	//w1.fillFromString("[[2.0, 8.0, -5.0, -4.0, -6.0, 2.0], [7.0, -8.0, 3.0, -1.0, -7.0, 7.0], [-2.0, 0.0, 1.0, 8.0, -1.0, 6.0], [-7.0, 4.0, -2.0, 2.0, 4.0, -2.0], [-1.0, 3.0, 0.0, -9.0, 8.0, 6.0]]");
	//Matrix b1 = Matrix(w1.rows, 1);
	//b1.fillFromString("[[7.0], [6.0], [-9.0], [0.0], [-2.0]]");
	//Matrix w2 = Matrix(3, 5);
	//w2.fillFromString("[[0.0, 0.0, 0.0, 4.0, -3.0], [-9.0, -9.0, 8.0, -6.0, 0.0], [-2.0, 1.0, 0.0, -5.0, 4.0]]");
	//Matrix b2 = Matrix(w2.rows, 1);
	//b2.fillFromString("[[-5.0], [-6.0], [4.0]]");
	//Matrix best_x(w1.cols, 1);
	//best_x.fillFromString("[[1.0], [1.0], [1.0], [1.0], [1.0], [1.0]]");
	//Matrix x(w1.cols, 1);
	//x.fillFromString("[[1.0], [1.0], [1.0], [1.0], [1.0], [1.0]]");
	//float best_sum = std::numeric_limits<float>::infinity();
	//for (int attempt = 0; attempt < 10000000; ++attempt) {
	//	// Generate random input change
	//	for (int i = 0; i < x.rows; i++) {
	//		for (int j = 0; j < x.cols; j++) {
	//			//val = rand() % 3 - 1;
	//			(static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
	//			x.elements[i][j] += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;; //random between -1 and 1
	//		}
	//	}
	//	// Evaluate the network with the new input
	//	Matrix l1 = add(multiply(w1, x), b1);
	//	l1.relu();
	//	Matrix answer = add(multiply(w2, l1), b2);
	//	// Check if the new input gives a sum of outputs closer to zero
	//	float current_sum = sumOfAbsoluteValues(answer);
	//	if (current_sum < best_sum) {	//x is new best x
	//		std::cout << sumOfAbsoluteValues(answer) << std::endl;
	//		best_sum = current_sum;
	//		for (int i = 0; i < best_x.rows; i++) {
	//			for (int j = 0; j < best_x.cols; j++) {
	//				best_x.elements[i][j] = x.elements[i][j];
	//			}
	//		}
	//	}
	//	else {
	//		//bring x back to best_x
	//		for (int i = 0; i < best_x.rows; i++) {
	//			for (int j = 0; j < best_x.cols; j++) {
	//				x.elements[i][j] = best_x.elements[i][j];
	//			}
	//		}
	//	}
	//}
	//for (int i = 0; i < best_x.rows; i++) {
	//	for (int j = 0; j < best_x.cols; j++) {
	//		cout << best_x.elements[i][j] << " ";
	//	}
	//}
	/*Matrix x = Matrix(w1.cols, 1);
	x.fillFromString("[[1.0], [1.0], [1.0], [1.0], [1.0], [1.0]]");

	Matrix l1 = add(multiply(w1, x), b1);
	l1.relu();
	Matrix l2 = add(multiply(w2, l1), b2);*/
	//for (int i = 0; i < l2.rows; i++) {
	//	for (int j = 0; j < l2.cols; j++) {
	//		cout << l2.elements[i][j] << " ";
	//	}
	//}


	//Test 4
	//0.000610352 sum
	//-0.297281 input
//	Matrix w1 = Matrix(10, 1);
//	w1.fillFromString("[[-1.0], [-4.0], [-8.0], [-2.0], [-6.0], [4.0], [1.0], [-1.0], [7.0], [0.0]]");
//	Matrix b1 = Matrix(w1.rows, 1);
//	b1.fillFromString("[[3.0], [-9.0], [4.0], [6.0], [-4.0], [-9.0], [-9.0], [-9.0], [-8.0], [-8.0]]");
//
//	Matrix w2 = Matrix(5, 10);
//	w2.fillFromString("[[-1.0, -4.0, -4.0, -9.0, -7.0, 9.0, -8.0, -2.0, -6.0, -6.0], [1.0, -8.0, -1.0, 4.0, 2.0, -5.0, -8.0, -6.0, -5.0, 7.0], [0.0, -4.0, -7.0, -1.0, 0.0, -7.0, 6.0, -9.0, 9.0, 4.0], [3.0, -7.0, -7.0, 0.0, 3.0, 6.0, 9.0, 9.0, -9.0, 3.0], [-6.0, 1.0, 0.0, 7.0, 8.0, 3.0, -9.0, -5.0, -9.0, -5.0]]");
//	Matrix b2 = Matrix(w2.rows, 1);
//	b2.fillFromString("[[1.0], [8.0], [0.0], [-1.0], [-7.0]]");
//
//	Matrix w3 = Matrix(1, 5);
//	w3.fillFromString("[[-7.0, -3.0, 6.0, -2.0, 5.0]]");
//	Matrix b3 = Matrix(w3.rows, 1);
//	b3.fillFromString("[[-3.0]]");
//
//	Matrix best_x(w1.cols, 1);
//	best_x.fillFromString("[[1.0]]");
//	Matrix x(w1.cols, 1);
//	x.fillFromString("[[1.0]]");
//
//	float best_sum = std::numeric_limits<float>::infinity();
//
//	for (int attempt = 0; attempt < 100000; ++attempt) {
//
//		// Generate random input change
//		for (int i = 0; i < x.rows; i++) {
//			for (int j = 0; j < x.cols; j++) {
//				x.elements[i][j] += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f; //random between -1 and 1
//			}
//		}
//
//		// Evaluate the network with the new input
//		Matrix l1 = add(multiply(w1, x), b1);
//		l1.relu();
//		Matrix l2 = add(multiply(w2, l1), b2);
//		l2.relu();
//		Matrix answer = add(multiply(w3, l2), b3);
//
//		// Check if the new input gives a sum of outputs closer to zero
//		float current_sum = sumOfAbsoluteValues(answer);
//		if (current_sum < best_sum) {	//x is new best x
//			std::cout << sumOfAbsoluteValues(answer) << std::endl;
//			best_sum = current_sum;
//			for (int i = 0; i < best_x.rows; i++) {
//				for (int j = 0; j < best_x.cols; j++) {
//					best_x.elements[i][j] = x.elements[i][j];
//				}
//			}
//		}
//		else {
//			//bring x back to best_x
//			for (int i = 0; i < best_x.rows; i++) {
//				for (int j = 0; j < best_x.cols; j++) {
//					x.elements[i][j] = best_x.elements[i][j];
//				}
//			}
//		}
//	}
//
//	for (int i = 0; i < best_x.rows; i++) {
//		for (int j = 0; j < best_x.cols; j++) {
//			cout << best_x.elements[i][j] << " ";
//		}
//	}
//


	//test 5
	Matrix w1 = Matrix(10, 4);
	w1.fillFromString("[[0.0, -4.0, 8.0, 8.0], [-2.0, 6.0, 2.0, -8.0], [-8.0, 5.0,8.0, -5.0], [-6.0, -7.0, -6.0, 0.0], [-4.0, 4.0, 0.0, -9.0], [-9.0, 9.0, 9.0, 7.0], [-4.0, -5.0, -4.0, -6.0], [-8.0, 7.0, -4.0, -3.0], [0.0, 6.0, -2.0, -4.0], [8.0, 0.0, -9.0, 1.0]]");
	Matrix b1 = Matrix(w1.rows, 1);
	b1.fillFromString("[[2.0], [-5.0], [0.0], [1.0], [4.0], [5.0], [5.0], [-2.0],[9.0], [-1.0]]");

	Matrix w2 = Matrix(10, 10);
	w2.fillFromString("[9.0, 6.0, -6.0, -3.0, 7.0, -3.0, 8.0, -4.0, 0.0, -8.0], [-1.0, -2.0, -7.0, 0.0, 2.0, -9.0, -5.0, 5.0, 0.0, 9.0], [-5.0, -4.0, 9.0, 8.0, -7.0, 2.0, 7.0, 7.0, 0.0, 0.0], [-4.0, 5.0, 1.0, -9.0, 1.0, 0.0, -9.0, 2.0, -1.0, -2.0], [-3.0, -9.0, 8.0, 3.0, 0.0, 9.0, 4.0, 3.0, -8.0, 8.0], [-7.0, -4.0, -9.0, 3.0, 0.0, 1.0, 2.0, -9.0, -5.0, 1.0], [0.0, 3.0, 6.0, -7.0, -8.0, -4.0, 1.0, -7.0, 2.0, 8.0], [-2.0, 5.0, 0.0, -4.0, 5.0, 7.0, 0.0, 6.0, -8.0, 1.0], [6.0, 5.0, 3.0, -5.0, -8.0, 3.0, 4.0, 2.0, 6.0, 5.0], [0.0, -3.0, 9.0, 1.0, -4.0, -5.0, 4.0, 4.0, 2.0, -2.0]]");
	Matrix b2 = Matrix(w2.rows, 1);
	b2.fillFromString("[-1.0], [-4.0], [-1.0], [6.0], [5.0], [3.0], [-9.0], [-6.0], [8.0], [9.0]]");

	Matrix w3 = Matrix(4, 10);
	w3.fillFromString("[[5.0, -1.0, -8.0, 9.0, -6.0, -1.0, -6.0, 2.0, -7.0, -4.0], [9.0, -2.0, 6.0, 4.0, 7.0, -6.0, 5.0, 5.0, 0.0, -8.0], [0.0, -4.0, 9.0, -3.0, -7.0, -2.0, 6.0, 2.0, 7.0, -5.0], [5.0, -5.0, 7.0, -8.0, 9.0, -6.0, 2.0, -8.0, 6.0, 4.0]]");
	Matrix b3 = Matrix(w3.rows, 1);
	b3.fillFromString("[[8.0], [-3.0], [2.0], [9.0]]");

	Matrix best_x(w1.cols, 1);
	best_x.fillFromString("[[1.0], [1.0], [1.0], [1.0]]");
	Matrix x(w1.cols, 1);
	x.fillFromString("[[1.0], [1.0], [1.0], [1.0]]");

	float best_sum = std::numeric_limits<float>::infinity();

	for (int attempt = 0; attempt < 10000000; ++attempt) {

		// Generate random input change
		for (int i = 0; i < x.rows; i++) {
			for (int j = 0; j < x.cols; j++) {
				x.elements[i][j] += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f; //random between -1 and 1
			}
		}

		// Evaluate the network with the new input
		Matrix l1 = add(multiply(w1, x), b1);
		l1.relu();
		Matrix l2 = add(multiply(w2, l1), b2);
		l2.relu();
		Matrix answer = add(multiply(w3, l2), b3);

		// Check if the new input gives a sum of outputs closer to zero
		float current_sum = sumOfAbsoluteValues(answer);
		if (current_sum < best_sum) {	//x is new best x
			std::cout << sumOfAbsoluteValues(answer) << std::endl;
			best_sum = current_sum;
			for (int i = 0; i < best_x.rows; i++) {
				for (int j = 0; j < best_x.cols; j++) {
					best_x.elements[i][j] = x.elements[i][j];
				}
			}
		}
		else {
			//bring x back to best_x
			for (int i = 0; i < best_x.rows; i++) {
				for (int j = 0; j < best_x.cols; j++) {
					x.elements[i][j] = best_x.elements[i][j];
				}
			}
		}
	}

	for (int i = 0; i < best_x.rows; i++) {
		for (int j = 0; j < best_x.cols; j++) {
			cout << best_x.elements[i][j] << " ";
		}
	}


	return 0;
}
