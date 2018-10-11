#include <vector>
#include <cstdlib>
#include <iostream>

using Matrix = std::vector<std::vector<double>>;

// initializes a square identity matrix of size n x n
Matrix id(unsigned n) {
	Matrix res;
	res.resize(n);
	for(unsigned i=0; i<n; i++) {
		res[i].resize(n);
		for(unsigned j=0; j<n; j++) {
			res[i][j] = (i == j) ? 1 : 0;
		}
	}
	return res;
}

//initializes a matrix with 0
Matrix init(unsigned n) {
	Matrix res;
	res.resize(n);
	for(unsigned i=0; i<n; i++) {
		res[i].resize(n);
		for(unsigned j=0; j<n; j++) {
			res[i][j] = 0;
		}
	}
	return res;
}

// computes the product of two matrices
//added seq1 together with change of inner loops for memory locality(improved cache hits)
Matrix operator*(const Matrix& a, const Matrix& b) {
	unsigned n = a.size();
	Matrix c = init(n);
	for(unsigned i=0; i<n; ++i) {
		for(unsigned k=0; k<n; ++k) {
			for(unsigned j=0; j<n; ++j) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return c;
}


int main(int argc, char** argv) {
	
	if(argc!=2) return EXIT_FAILURE;
	unsigned n = atoi(argv[1]);
	if(n==0) return EXIT_FAILURE;

	// create two matrices
	auto a = id(n);
	a[0][0] = 42;
	auto b = id(n);

	// compute the product
	auto c = a * b;

	// check that the result is correct
	if(c!=a){
		std::cout<<"FAIL";	
	}
	return (c == a) ? EXIT_SUCCESS : EXIT_FAILURE;
}
