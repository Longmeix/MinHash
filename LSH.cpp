# include <iostream>
# include <algorithm>
# include <set>
# include <math.h>

# define K 8  // k-shingles
# define MAXROW 4294967296
# define DOC_NUM 10
# define DOC_LEN 500
# define HASH_NUM 6
# define EPOCH 10
# define BAND 3
using namespace std;

unsigned int shingleMat[DOC_LEN-K+1][DOC_NUM];  // 493 * 1000, #singles * #ducument
unsigned int signatureMat[HASH_NUM][DOC_NUM] = {UINT_MAX};
int collision[DOC_NUM][DOC_NUM] = {0};

int ROWS = HASH_NUM / BAND;

class HashFunc {
	private:
		int a;
		int b;
		long long prime = 4294967311;
		
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

void geneSignatureMat() {
	for(int hi = 0; hi < HASH_NUM; hi++) {  // disorder row by hashing
		HashFunc hFunc;
		for(int d = 0; d < DOC_NUM; d++) {  // each document
			unsigned int min_hash_row = UINT_MAX;
//			find the minimum row index from 1'rows
			for(int sr = 0; sr < DOC_LEN-K+1; sr++) {
				unsigned int old_row = shingleMat[sr][d];
				unsigned int new_row = hFunc.getNewRow(old_row);
				min_hash_row = new_row < min_hash_row ? new_row : min_hash_row;
			}
			signatureMat[hi][d] = min_hash_row;
//			cout << min_hash_row << " ";
		}
//		cout << endl;
	}
}

bool hasEqualBand(int d0, int d1, int start, int end) {
	for(int r = start; r < end; r++) {
		if(signatureMat[r][d0] != signatureMat[r][d1])
			return false;
	}
	return true;
}

bool isCollide(int d0, int d1) {
	for(int b = 0; b < BAND - 1; b++) {
		bool same = hasEqualBand(d0, d1, b*ROWS, (b+1)*ROWS);
		if(same == true) {
			return true;
		}
	}
	return false;
}

double JaccardSimilarity(int d0, int d1) {
	int and_cnt = 0;
	int or_cnt = 0;
	set<unsigned int> d0_shg;
	set<unsigned int> d1_shg;
	set<unsigned int> andset;
	set<unsigned int> orset;

//	remove duplicate shingle integer
	for(int i = 0; i < sizeof(shingleMat) / sizeof(shingleMat[0]); i++) {
		d0_shg.insert(shingleMat[i][d0]);
		d1_shg.insert(shingleMat[i][d1]);
	}
//	calculate jaccard
	set_intersection(d0_shg.begin(), d0_shg.end(), d1_shg.begin(), d1_shg.end(),inserter(andset, andset.begin()));
	set_union(d0_shg.begin(), d0_shg.end(), d1_shg.begin(), d1_shg.end(),inserter(orset, orset.begin()));
	double similarity = 1.0 * andset.size() / orset.size();
	
	return similarity;
}

int main() {
	srand(2022);
//	read shingling integer matrix from file
	FILE* mfp = fopen("shingleMat", "rb");
	fread(shingleMat, sizeof(shingleMat), 1, mfp);
	fclose(mfp);
	
//	Local sensitive hashing
	for(int e = 0; e < EPOCH; e++) {
		//	minhash: hashing shingles to get signature matrix
		geneSignatureMat();
		
		for(int d0 = 0; d0 < DOC_NUM; d0++) {
			for(int d1 = d0 + 1; d1 < DOC_NUM; d1++) {
				if(isCollide(d0, d1) == true) {
					collision[d0][d1] += 1;
				}
			}
		}
		
	}
	
//	results
	for(int d0 = 0; d0 < DOC_NUM; d0++) {
		for(int d1 = d0 + 1; d1 < DOC_NUM; d1++) {
			double exper_prob = 1.0 * collision[d0][d1] / EPOCH;
//			cout << "here" << endl;
			double jaccard = JaccardSimilarity(d0, d1);
			
			double theory_prob = 1 - pow((1 - pow(jaccard, ROWS)), BAND);
			
			if(true)
				{
					cout << "(" <<   d0 << "," << d1 << ")";
					cout << " probability = " <<  100 * exper_prob  << "%  ";
					cout << "similarity = " << 100 * jaccard << "%  ";
					cout << "prediction = " << 100 * theory_prob << "%";
					cout << endl;
				}
		}
	}
	
	
}
