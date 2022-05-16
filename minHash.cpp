# include <iostream>

# define K 8  // k-shingles
# define DOC_NUM 10
# define DOC_LEN 500
# define HASH_NUM 2
# define MAXROW 4294967296
using namespace std;

unsigned int shingleMat[DOC_LEN-K+1][DOC_NUM];  // 493 * 1000

class HashFunc {
	private:
		int a;
		int b;
		int prime = 4294967311;
		
	public:
		HashFunc() {
			a = rand();
			b = rand();
		}
		
		unsigned int getNewRow(unsigned int row) {
			unsigned int new_row = ((a * row + b) % prime) % MAXROW;
			return new_row;
		}
};

int main() {
	cout << 2**32 << endl;
	int a;
	cin >> a;
	FILE* mfp = fopen("shingleMat");
	fread(shingleMat, sizeof(shingleMat), 1, mfp);
	fclose();
	
	
}
