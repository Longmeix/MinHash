# include <iostream>
# include <fstream>
# include <time.h>

# define DOC_NUM 10
# define DOC_LEN 500
using namespace std;

string geneRandDoc(const int len) {
//	srand(time(NULL));
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

string geneOtherDoc(string base_doc) {
	int idx = rand() % base_doc.length();  // replace index
//	cout << idx << endl;
	char c;
	int is_upper = rand() % 2;
	if(is_upper == 1) {
		c = 'A' + rand() % 26;
	} else {
		c = 'a' + rand() % 26;
	}
	base_doc[idx] = c;
	return base_doc;
}

int main(){
	srand(2022);
	string doc_path = "doc.txt";
	ofstream doc_ofs(doc_path);  // write stream
	
//	generate base document
	string doc0 = geneRandDoc(DOC_LEN);
	doc_ofs << doc0 << endl;  // write base_doc t to stream doc_ofs
	
//	generate the rest doc
	string base_doc = doc0;
	for(int i = 1; i < DOC_NUM; i++) {
		string doc = geneOtherDoc(base_doc);  // base_doc changes
		doc_ofs << doc << endl;
	}
	
	doc_ofs.close();
	return 0;
}