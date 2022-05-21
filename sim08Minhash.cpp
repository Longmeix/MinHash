# include <iostream>
# include <algorithm>
# include <set>
# include <math.h>
# include <fstream>
#include <string.h>

# define K 8  // k-shingles
# define DOC_NUM 2
# define DOC_LEN 367
# define REPLACE_LEN 5
# define HASH_NUM 30
# define EPOCH 1000
# define MAXROW 4294967296
using namespace std;

unsigned int shingleMat[DOC_LEN-K+1][DOC_NUM];  // 360 * 2
unsigned int signatureMat[HASH_NUM][DOC_NUM] = {UINT_MAX};
int BAND[6] ={2, 3, 5, 6, 10, 15};


string geneRandDoc(const int len) {
	string doc;
	char c;
	
	for(int i = 0; i < len; i++) {
		int is_upper = rand() % 2;
		if(is_upper == 1) {
			c = 'A' + rand() % 26;
		} else {
			c = 'a' + rand() % 26;
		}
		doc.push_back(c);
	}
	
	return doc;
}

// replace character to genetare similar document
string geneSim08Doc(string base_doc) {
	char c;
	for(int i = 0; i < REPLACE_LEN; i++) {  
		int idx = rand() % base_doc.length();  // replace index
		int is_upper = rand() % 2;
		if(is_upper == 1) {
			c = 'A' + rand() % 26;
		} else {
			c = 'a' + rand() % 26;
		}
		base_doc[idx] = c;
//		cout << idx << " " << c << " ";
	}

	return base_doc;
}

unsigned int uiCRC32_Table[256] = {
	0x00000000L, 0x77073096L, 0xEE0E612CL,
	0x990951BAL, 0x076DC419L, 0x706AF48FL,
	0xE963A535L, 0x9E6495A3L, 0x0EDB8832L,
	0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
	0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L,
	0x90BF1D91L, 0x1DB71064L, 0x6AB020F2L,
	0xF3B97148L, 0x84BE41DEL, 0x1ADAD47DL,
	0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
	0x136C9856L, 0x646BA8C0L, 0xFD62F97AL,
	0x8A65C9ECL, 0x14015C4FL, 0x63066CD9L,
	0xFA0F3D63L, 0x8D080DF5L, 0x3B6E20C8L,
	0x4C69105EL, 0xD56041E4L, 0xA2677172L,
	0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL,
	0xA50AB56BL, 0x35B5A8FAL, 0x42B2986CL,
	0xDBBBC9D6L, 0xACBCF940L, 0x32D86CE3L,
	0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
	0x26D930ACL, 0x51DE003AL, 0xC8D75180L,
	0xBFD06116L, 0x21B4F4B5L, 0x56B3C423L,
	0xCFBA9599L, 0xB8BDA50FL, 0x2802B89EL,
	0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
	0x2F6F7C87L, 0x58684C11L, 0xC1611DABL,
	0xB6662D3DL, 0x76DC4190L, 0x01DB7106L,
	0x98D220BCL, 0xEFD5102AL, 0x71B18589L,
	0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
	0x7807C9A2L, 0x0F00F934L, 0x9609A88EL,
	0xE10E9818L, 0x7F6A0DBBL, 0x086D3D2DL,
	0x91646C97L, 0xE6635C01L, 0x6B6B51F4L,
	0x1C6C6162L, 0x856530D8L, 0xF262004EL,
	0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L,
	0xF50FC457L, 0x65B0D9C6L, 0x12B7E950L,
	0x8BBEB8EAL, 0xFCB9887CL, 0x62DD1DDFL,
	0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
	0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L,
	0xD4BB30E2L, 0x4ADFA541L, 0x3DD895D7L,
	0xA4D1C46DL, 0xD3D6F4FBL, 0x4369E96AL,
	0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
	0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL,
	0xDD0D7CC9L, 0x5005713CL, 0x270241AAL,
	0xBE0B1010L, 0xC90C2086L, 0x5768B525L,
	0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
	0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L,
	0xC7D7A8B4L, 0x59B33D17L, 0x2EB40D81L,
	0xB7BD5C3BL, 0xC0BA6CADL, 0xEDB88320L,
	0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
	0xEAD54739L, 0x9DD277AFL, 0x04DB2615L,
	0x73DC1683L, 0xE3630B12L, 0x94643B84L,
	0x0D6D6A3EL, 0x7A6A5AA8L, 0xE40ECF0BL,
	0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
	0xF00F9344L, 0x8708A3D2L, 0x1E01F268L,
	0x6906C2FEL, 0xF762575DL, 0x806567CBL,
	0x196C3671L, 0x6E6B06E7L, 0xFED41B76L,
	0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
	0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L,
	0x60B08ED5L, 0xD6D6A3E8L, 0xA1D1937EL,
	0x38D8C2C4L, 0x4FDFF252L, 0xD1BB67F1L,
	0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
	0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L,
	0x41047A60L, 0xDF60EFC3L, 0xA867DF55L,
	0x316E8EEFL, 0x4669BE79L, 0xCB61B38CL,
	0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
	0xCC0C7795L, 0xBB0B4703L, 0x220216B9L,
	0x5505262FL, 0xC5BA3BBEL, 0xB2BD0B28L,
	0x2BB45A92L, 0x5CB36A04L, 0xC2D7FFA7L,
	0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
	0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL,
	0x026D930AL, 0x9C0906A9L, 0xEB0E363FL,
	0x72076785L, 0x05005713L, 0x95BF4A82L,
	0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
	0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L,
	0x0BDBDF21L, 0x86D3D2D4L, 0xF1D4E242L,
	0x68DDB3F8L, 0x1FDA836EL, 0x81BE16CDL,
	0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
	0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL,
	0x11010B5CL, 0x8F659EFFL, 0xF862AE69L,
	0x616BFFD3L, 0x166CCF45L, 0xA00AE278L,
	0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
	0xA7672661L, 0xD06016F7L, 0x4969474DL,
	0x3E6E77DBL, 0xAED16A4AL, 0xD9D65ADCL,
	0x40DF0B66L, 0x37D83BF0L, 0xA9BCAE53L,
	0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
	0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L,
	0x24B4A3A6L, 0xBAD03605L, 0xCDD70693L,
	0x54DE5729L, 0x23D967BFL, 0xB3667A2EL,
	0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
	0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL,
	0x2D02EF8DL };

unsigned int CRC32(void *pData, size_t iLen) {
	unsigned int uiCRC32 = 0xFFFFFFFF;
	unsigned char *pszData = (unsigned char*)pData;

	for (size_t i = 0; i<iLen; ++i)
		uiCRC32 = ((uiCRC32 >> 8) & 0x00FFFFFF) ^ uiCRC32_Table[(uiCRC32 ^ (unsigned int)*pszData++) & 0xFF];

	return (uiCRC32 ^ 0xFFFFFFFF);
}

class HashFunc {
	public:
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
//		cout << hFunc.a << " " << hFunc.b << endl;
		for(int d = 0; d < DOC_NUM; d++) {  // each document
			unsigned int min_hash_row = UINT_MAX;
//			find the minimum row index from 1'rows
			for(int sr = 0; sr < DOC_LEN-K+1; sr++) {
				unsigned int old_row = shingleMat[sr][d];
				unsigned int new_row = hFunc.getNewRow(old_row);
				min_hash_row = (new_row < min_hash_row) ? new_row : min_hash_row;
			}
			signatureMat[hi][d] = min_hash_row;
		}
	}
}

bool hasEqualBand(int d0, int d1, int start, int end) {
	for(int r = start; r < end; r++) {
		if(signatureMat[r][d0] != signatureMat[r][d1])
			return false;
	}
	return true;
}

bool isCollide(int d0, int d1, int band, int row) {
	for(int b = 0; b < band; b++) {
		bool same = hasEqualBand(d0, d1, b*row, (b+1)*row);
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

int main(){
	srand(2022);
//	1. generate documents
	string doc_path = "doc0.8.txt";
	ofstream doc_ofs(doc_path);  // write stream
//	generate base document
	string doc0 = geneRandDoc(DOC_LEN);
	doc_ofs << doc0 << endl;  // write base_doc t to stream doc_ofs
//	generate a document with jaccard similarity 0.8
	string doc = geneSim08Doc(doc0); 
	doc_ofs << doc << endl;
	doc_ofs.close();
	
//	2. shingle
	ifstream doc_read(doc_path);
//	string doc;
	for(int d = 0; d < DOC_NUM; d++) {  // for each ducument
		getline(doc_read, doc);
		cout << d << endl;
		for(int i = 0; i <= doc.length() - K; i++) {
//			string subDoc = doc.substr(i, K);  // sub string with length K=8
//			cout << i << endl;
//			cout << subDoc << endl;
			char c_subDoc[K];
//			strcpy(c_subDoc, subDoc.c_str());
			c_subDoc = doc.substr(i, K);
//			unsigned int intShingle = CRC32(c_subDoc, sizeof(c_subDoc) / sizeof(char));
//			shingleMat[i][d] = intShingle;
//			cout << shingleMat[i][d] << endl;
		}	
	}
	doc_read.close();

	
//	3. signature matrix and calculate results
	int d0 = 0;
	int d1 = 1;
	for(int bi = 0; bi < sizeof(BAND) / sizeof(int); bi++) {
		int band = BAND[bi];
		int row = HASH_NUM / band;
		int collision = 0;
		for(int e = 0; e < EPOCH; e++) {
	//		min-hash, transfer shingles to signature matrix
			geneSignatureMat();
			if(isCollide(d0, d1, band, row) == true) {
				collision += 1;
			}		
		}
		
//		cout << "a" << endl;
		
		double exper_prob = 1.0 * collision / EPOCH;
		double jaccard = JaccardSimilarity(d0, d1);
		double theory_prob = 1 - pow(1 - pow(jaccard, row), band);
		
		cout << "(" <<   d0 << "," << d1 << ") ";
		cout << "probability = " <<  100 * exper_prob  << "% \t";
		cout << "similarity = " << 100 * jaccard << "%\t";
		cout << "prediction = " << 100 * theory_prob << "%\n";
	}
	
	
	
	return 0;
}