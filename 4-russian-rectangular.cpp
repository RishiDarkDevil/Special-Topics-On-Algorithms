#include <bits/stdc++.h>
using namespace std;

// Takes matrix input
void matrix_input(vector<vector<bool>> &A, int n, int r) {
	for (int i = 0; i < n; ++i) {
		vector<bool> row;
		for (int j = 0; j < r; ++j) {
			bool x;
			cin >> x;
			row.push_back(x);
		}
		A.push_back(row);
	}
}

// Prints matrix
void print_matrix(vector<vector<bool>> &A) {
	for (auto &row: A) {
		for (auto ele: row) {
			cout << (int)ele << " ";
		}
		cout << endl;
	}
}

// Converts binary vector to decimal
int conv_dec(vector<bool> arr) {
	int res = 0;
	for (auto ele: arr) {
		res <<= 1;
		res |= ele;
	}
	return res;
}

void pad_matrices(vector<vector<bool>> &A, vector<vector<bool>> &B, int n, int c1, int c2, int pad_length) {

	// Pad left matrix columns
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < pad_length; ++j) {
			A[i].push_back(0);
		}
	}

	// Pad right matrix rows 
	vector<bool> zero_rows;
	for (int i = 0; i < c2; ++i)
		zero_rows.push_back(0);

	for (int i = 0; i < pad_length; ++i) {
		B.push_back(zero_rows);
	}

}

int bin_exp(int a, int b) {
	int res = 1;
	while (b) {
		if (b & 1)
			res *= a;
		a *= a;
		b >>= 1;
	}
	return res;
}

vector<bool> add_rows(vector<bool> &row1, vector<bool> &row2) {
	vector<bool> summ;
	for (int i = 0; i < row1.size(); ++i)
		summ.push_back(row1[i] || row2[i]);
	return summ;
}

// Create partition matrices
void partition(
	vector<vector<vector<bool>>> &partition_A, 
	vector<vector<vector<bool>>> &partition_B, 
	vector<vector<bool>> &A, 
	vector<vector<bool>> &B, 
	int m, 
	int k) {

	for (int i = 0; i < k; ++i) {

		vector<vector<bool>> part_A, part_B;


		for (auto &row_A: A) {
			vector<bool> row;
			for (int p = i*m; p < (i+1)*m; p++) {
				row.push_back(row_A[p]);
			}
			part_A.push_back(row);
		}
		partition_A.push_back(part_A);


		for (int p = i*m; p < (i+1)*m; p++) {
			part_B.push_back(B[p]);
		}
		partition_B.push_back(part_B);
	}

}

// Adds two matrices
void add_matrices(vector<vector<bool>> &A, vector<vector<bool>> &B) {
	for (int i = 0; i < A.size(); ++i) {
		for (int j = 0; j < A[0].size(); ++j) {
			A[i][j] = A[i][j] || B[i][j];
		}
	}
}

int main() {

	// Size of the input matrices, Two input matrices
	int n, c1, c2;
	vector<vector<bool>> A, B;

	// Taking input
	cin >> n >> c1 >> c2;
	matrix_input(A, n, c1);
	matrix_input(B, c1, c2);

	int m = floor(log2(c1)); // Size of a partition of matrix1
	int k = ceil(c1/(float)m); // Number of partitions
	int fill_rows = bin_exp(2, m); // The precomputation number of rows
	vector<bool> zero_rows; // A row of all zeroes
	for (int i = 0; i < c2; ++i)
		zero_rows.push_back(0);

	// Pad matrices
	int pad_length = m*k - c1;
	pad_matrices(A, B, n, c1, c2, pad_length);

	// Partition matrices
	vector<vector<vector<bool>>> partition_A, partition_B;
	partition(partition_A, partition_B, A, B, m, k);

	// Output matrix
	vector<vector<bool>> C;
	for (int i = 0; i < n; ++i) {
		vector<bool> row;
		for (int i = 0; i < c2; ++i) {
			row.push_back(0);
		}
		C.push_back(row);
	}

	// Calculating matrix product using sum of outer products
	for (int i = 0; i < k; ++i) {

		// precompute sum of all possible combinations of rows of B_i
		vector<vector<bool>> precompute_B;
		precompute_B.push_back(zero_rows);

		// Number of rows in precomputed sums of rows of B_i table
		int p = 0, bp = 1;


		for (int j = 1; j < fill_rows; ++j) {
			vector<bool> row_B = add_rows(precompute_B[j - bin_exp(2, p)], partition_B[i][m - (p+1)]);
			precompute_B.push_back(row_B);

			// Why this works? Because if we reach 2^p then we need to cover 2^p more steps to reach 2^(p+1)
			// which is the next power of 2 after 2^p.
			// So, whenever we reach 2^p - 1
			// We use the recursive pattern i.e. (000, 001),
			// (010, 011 -- see the second bit repeats the same pattern as before set),
			// (100, 101, 110, 111 -- see the second and third bit repeats the same pattern as all the previous two sets)
			if (bp == 1) {
				bp = j + 1;
				p++;
			} else {
				bp--;
			}
		}

		// Calculate Ai x Bi
		vector<vector<bool>> part_prod;
		for (auto &row: partition_A[i]) {
			part_prod.push_back(precompute_B[conv_dec(row)]); 
		}

		// Adding the outer products
		add_matrices(C, part_prod);
	}

	// Display Product
	print_matrix(C);

}