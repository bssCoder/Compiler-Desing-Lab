#include<bits/stdc++.h>
using namespace std;
#define OUT 0
#define IN 1
string removeComments(string prgm)
{
	
	int n = prgm.length();
	string res;
	bool s_cmt = false;
	bool m_cmt = false;
	for (int i=0; i<n; i++)
	{
		if (s_cmt == true && prgm[i] == '\n')
			s_cmt = false;
		else if (m_cmt == true && prgm[i] == '*' && prgm[i+1] == '/')
			m_cmt = false, i++;
		else if (s_cmt || m_cmt)
			continue;
		else if (prgm[i] == '/' && prgm[i+1] == '/')
			s_cmt = true, i++;
		else if (prgm[i] == '/' && prgm[i+1] == '*')
			m_cmt = true, i++;
		else res += prgm[i];
	}
	return res;
}
string readFileToString(const string& filename) {
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << '\n';
        return "";
    }
    string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
    file.close();
    return content;
}
int main() {
    string filename = "C:\\Users\\students\\Desktop\\lab3_temp.cpp";
    string fileContent = readFileToString(filename);
    if (!fileContent.empty()) {
        cout << "File content:" << "\n";
        cout << fileContent << "\n";
    }
	cout << "Given Program \n";
	cout << filename << "\n";
	cout << "\nModified Program \n";
	cout << removeComments(fileContent)<<'\n';
}

