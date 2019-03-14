#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iomanip>
#include <chrono>
#include <pthread.h>

using namespace std;

//structure for passing parameters in thread
struct arg_struct {
	vector<vector<int>> matrixA;
	vector<vector<int>> matrixB;
	int MAX_THREAD;
	int i;
};

//result matrix from A, B
vector<vector<int>> resultMatrix;

//generate random int number between -20 and 20
int randomNumber() {
	return rand() %200 - 100;
}

//generates new matrix of given size with randomNumber
vector<vector<int>> generate_matrix(int row, int col) {
	vector<vector<int>> matrix(row, vector<int>(col));

	for (auto &&rownum : matrix)
		for (int j = 0; j < col; j++)
			rownum[j] = randomNumber();

	return matrix;
}

//print matrix if its lagrger than 10x10
void print_matrix(vector<vector<int>> matrix) {
	if (matrix.size() > 10 || matrix[0].size() > 10)
		return;

	for (auto &&row : matrix) {
		for (auto &&number : row)
			cout << setw(8) << number;
		cout << endl;
	}
	cout << endl;
}

//multiply sequencial
vector<vector<int>> multiply_sequencial(vector<vector<int>> &matrixA, vector<vector<int>> &matrixB) {
	int matrixA_i = matrixA.size();
	int matrixB_k = matrixB.size();
	int matrixB_j = matrixB[0].size();

	cout << "Multiply sequential" << endl;

	vector<vector<int>> result(matrixA_i, vector<int>(matrixB_j, 0));

	auto start = chrono::system_clock::now();

	for (int i = 0; i < matrixA_i; i++)
		for (int k = 0; k < matrixB_k; k++) {
			for (int j = 0; j < matrixB_j; j++)
				result[i][j] += matrixA[i][k] * matrixB[k][j];
		}

	auto end = chrono::system_clock::now();
	chrono::duration<double> final_time = end - start;
	printf("In time: %f \n", final_time.count());

	return result;
}

//multiply parallel
void *multiply_parallel(void *args) {
	struct arg_struct *arguments = (struct arg_struct *)args;
	int start = start = (arguments->matrixA.size() / arguments->MAX_THREAD) * arguments->i;
	int matrixA_i;

	if ((arguments->i + 1 == arguments->MAX_THREAD)
				&& (arguments->matrixA.size() % arguments->MAX_THREAD != 0)) {
		matrixA_i = arguments->matrixA.size();
	} else {
		matrixA_i = (arguments->matrixA.size() / arguments->MAX_THREAD) * (arguments->i + 1);
	}
	int matrixB_k = arguments->matrixB.size();
	int matrixB_j = arguments->matrixB[0].size();

	for (int i = start; i < matrixA_i; i++)
		for (int k = 0; k < matrixB_k; k++) {
			for (int j = 0; j < matrixB_j; j++) {
				resultMatrix[i][j] += arguments->matrixA[i][k] * arguments->matrixB[k][j];
			}
		}

	return NULL;
}

//multiply parallel two matrix using ikj-algorithm into new one
//if number of threads is 0 or size of matrix is lower than threads multiply sequential
vector<vector<int>> multiply(vector<vector<int>> &matrixA, vector<vector<int>> &matrixB,int threadNum) {
	if (threadNum < 1) {
		return multiply_sequencial(matrixA, matrixB);
	}

	if (matrixA.size() < threadNum){
		cout << "Row num of matrix A must be higher than threads(1-8) so calculate sequential" << endl;
		return multiply_sequencial(matrixA, matrixB);
	}

	vector<arg_struct> matrix_struct(threadNum);
	vector<vector<int>> result(matrixA.size(), vector<int>(matrixB[0].size(), 0));
	vector<pthread_t> threads(threadNum);
	resultMatrix = result;

	auto start = chrono::system_clock::now();

	for (int i = 0; i < threadNum; i++) {
		matrix_struct[i].matrixA = matrixA;
		matrix_struct[i].matrixB = matrixB;
		matrix_struct[i].MAX_THREAD = threadNum;
		matrix_struct[i].i = i;
		pthread_create(&threads[i], NULL, multiply_parallel, (void *)&matrix_struct[i]);
	}

	for (int i = 0; i < threadNum; i++){
		pthread_join(threads[i], NULL);
	}

	auto end = chrono::system_clock::now();

	printf("Multiplied parallel in %d threads\n", threadNum);
	chrono::duration<double> final_time = end - start;
	printf("In time: ");
	cout << final_time.count() << endl;

	return resultMatrix;
}

int main(){
	vector<vector<int>> matrixA = generate_matrix(2000, 1000);
	vector<vector<int>> matrixB = generate_matrix(1000, 2000);

	print_matrix(matrixA);
	print_matrix(matrixB);

	for (int i = 0; i < 9; i+=2) {
		vector<vector<int>> result = multiply(matrixA, matrixB, i);
		print_matrix(result);
	}

	getchar();

	return 0;
}