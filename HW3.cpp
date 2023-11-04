// HW3.cpp : Defines the entry point for the application.
//

#include "HW3.h"
#include <string>
#include <iostream>
#include <sstream>
//#include <algorithm>
//#include <ctime>
//#include <cstdlib>
#include <random>
#include <limits>

using namespace std;


class Matrix {
public:
	int rows;
	int cols;
	double** elements;

	Matrix(int rows, int cols) : rows(rows), cols(cols) {
		// Allocate memory for the 2D array
		elements = new double* [rows];
		for (int i = 0; i < rows; i++) {
			elements[i] = new double[cols];
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

	void fillFromString(const string& input) {
		string cleanInput = input;
		cleanInput.erase(remove(cleanInput.begin(), cleanInput.end(), '['), cleanInput.end());
		cleanInput.erase(remove(cleanInput.begin(), cleanInput.end(), ']'), cleanInput.end());
		stringstream ss(cleanInput);
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				string token;
				if (getline(ss, token, ',')) {
					elements[i][j] = stod(token);
				}
				else {
					cerr << "Error: Input string doesn't match matrix dimensions." << endl;
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
		throw invalid_argument("Matrix dimensions are not compatible for multiplication");
	}
	for (int i = 0; i < newMtx.rows; i++) {
		for (int j = 0; j < newMtx.cols; j++) {
			double sum = 0.0f;
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
		throw invalid_argument("Matrix dimensions are not compatible for addition");
	}
	Matrix result(matrix1.rows, matrix1.cols);
	for (int i = 0; i < matrix1.rows; i++) {
		for (int j = 0; j < matrix1.cols; j++) {
			result.elements[i][j] = matrix1.elements[i][j] + matrix2.elements[i][j];
		}
	}
	return result;
}


double sumOfAbsoluteValues(const Matrix& m) {
	double sum = 0.0f;
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			sum += abs(m.elements[i][j]);
		}
	}
	return sum;
}

//Matrix randomSearch(Matrix& w1, Matrix& b1, Matrix& w2, Matrix& b2, Matrix& w3, Matrix& b3, int num_attempts) {
//	Matrix best_x(w1.cols, 1);
//	best_x.fillFromString("[[1.0], [1.0], [1.0], [1.0]]");
//	Matrix x(w1.cols, 1);
//	x.fillFromString("[[1.0], [1.0], [1.0], [1.0]]");
//
//	float best_sum = std::numeric_limits<float>::infinity();
//
//	for (int attempt = 0; attempt < num_attempts; ++attempt) {
//		
//		// Generate random input change
//		for (int i = 0; i < x.rows; i++) {
//			for (int j = 0; j < x.cols; j++) {
//				x.elements[i][j] += (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * 2.0f - 1.0f; //random between -1 and 1
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
//			cout << sumOfAbsoluteValues(answer) << endl;
//			best_sum = current_sum;
//			for (int i = 0; i < best_x.rows; i++) {
//				for (int j = 0; j < best_x.cols; j++) {
//					best_x.elements[i][j] = x.elements[i][j];
//				}
//			}
//		}
//		else{
//			//bring x back to best_x
//			for (int i = 0; i < best_x.rows; i++) {
//				for (int j = 0; j < best_x.cols; j++) {
//					x.elements[i][j] = best_x.elements[i][j];
//				}
//			}
//		}
//	}
//		return best_x;
//}

int main() {
	//Test 2 [[2.0], [-2.0], [-3.0], [1.0]]		0
	
	//Test 3
	//0.154787
	//1.38884 0.192694 3.92187 2.66823 4.73965 -0.318552
	
	//Test 4
	//0.000610352 sum
	//-0.297281 input


	//test 5 1.12326 0.36755 0.464334 -0.526764		808.581

	double lower_bound = -1;
	double upper_bound = 1;
	std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
	std::default_random_engine re;


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
		if (attempt % 100000 == 0) {
			cout << attempt << endl;
		}
		// Generate random input change
		for (int i = 0; i < x.rows; i++) {
			for (int j = 0; j < x.cols; j++) {
				x.elements[i][j] += unif(re); //random between -1 and 1
			}
		}

		// Evaluate the network with the new input
		Matrix l1 = add(multiply(w1, x), b1);
		l1.relu();
		Matrix l2 = add(multiply(w2, l1), b2);
		l2.relu();
		Matrix answer = add(multiply(w3, l2), b3);

		// Check if the new input gives a sum of outputs closer to zero
		double current_sum = sumOfAbsoluteValues(answer);
		if (current_sum < best_sum) {	//x is new best x
			cout << sumOfAbsoluteValues(answer) << endl;
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
