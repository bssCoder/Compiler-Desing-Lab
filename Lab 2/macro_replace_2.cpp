#include <bits/stdc++.h>
#include <fstream>
using namespace std;
void processDirectives(const string& inputFilename, const string& outputFilename, const set<string>& definedMacros) {
    ifstream inputFile(inputFilename);
    ofstream outputFile(outputFilename);
    string line;
    stack<bool> conditionStack;
    bool currentCondition = true;
    if (!inputFile.is_open()) {
        cerr << "Error opening input file." << endl;
        return;
    }
    if (!outputFile.is_open()) {
        cerr << "Error opening output file." << endl;
        return;
    }
    regex defineRegex("#define\\s+(\\w+)");
    while (getline(inputFile, line)) {
        string trimmedLine = line;
        trimmedLine.erase(trimmedLine.find_last_not_of(" \n\r\t") + 1);
        if (trimmedLine.find("#if") == 0) {
            conditionStack.push(currentCondition);
            string condition = trimmedLine.substr(3);
            if (condition.find("defined(") != string::npos) {
                string macro = condition.substr(condition.find("defined(") + 8);
                macro.erase(macro.find_last_not_of(" )") + 1);
                currentCondition = (definedMacros.find(macro) != definedMacros.end());
            } else {
                currentCondition = true;
            }
        } else if (trimmedLine.find("#ifdef") == 0) {
            conditionStack.push(currentCondition);
            string macro = trimmedLine.substr(6);
            currentCondition = (definedMacros.find(macro) != definedMacros.end());
        } else if (trimmedLine.find("#ifndef") == 0) {
            conditionStack.push(currentCondition);
            string macro = trimmedLine.substr(7);
            currentCondition = (definedMacros.find(macro) == definedMacros.end());
        } else if (trimmedLine.find("#else") == 0) {
            if (!conditionStack.empty()) {
                currentCondition = !conditionStack.top();
            } else {
                cerr << "Error: #else without matching #if" << endl;
                return;
            }
        } else if (trimmedLine.find("#elif") == 0) {
            if (!conditionStack.empty()) {
                string condition = trimmedLine.substr(5);
                if (condition.find("defined(") != string::npos) {
                    string macro = condition.substr(condition.find("defined(") + 8);
                    macro.erase(macro.find_last_not_of(" )") + 1);
                    currentCondition = (definedMacros.find(macro) != definedMacros.end());
                } else {
                    currentCondition = true;
                }
            } else {
                cerr << "Error: #elif without matching #if" << endl;
                return;
            }
        } else if (trimmedLine.find("#endif") == 0) {
            if (!conditionStack.empty()) {
                currentCondition = conditionStack.top();
                conditionStack.pop();
            } else {
                cerr << "Error: #endif without matching #if" << endl;
                return;
            }
        } else if (regex_search(trimmedLine, defineRegex)) {
            smatch match;
            regex_search(trimmedLine, match, defineRegex);
            string macro = match[1];
            if (definedMacros.find(macro) == definedMacros.end()) {
                if (currentCondition) {
                    outputFile << line << endl;
                    cout << line << endl;
                }
            }
        } else {
            if (currentCondition) {
                outputFile << line << endl;
                cout << line << endl;
            }
        }
    }
    inputFile.close();
    outputFile.close();
}
void readMacroDefinitions(const string &filename, set<string> &definedMacros) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening the file to read macro definitions." << endl;
        return;
    }
    string line;
    while (getline(inputFile, line)) {
        if (line.find("#define ENABLE_FEATURE") != string::npos) {
            definedMacros.insert("ENABLE_FEATURE");
        } else if (line.find("#define DISABLE_FEATURE") != string::npos) {
            definedMacros.insert("DISABLE_FEATURE");
        }
    }
    inputFile.close();
}
string getModifiedFileName(const string &filename) {
    int extensionPointer = filename.find_last_of(".");
    return filename.substr(0, extensionPointer) + "_modified.cpp";
}
int main() {
    set<string> definedMacros;
    string filename;


    cout << "Enter name of the file to open: ";
    cin >> filename;
    readMacroDefinitions(filename, definedMacros);
    string outputFilename = getModifiedFileName(filename);
    processDirectives(filename, outputFilename, definedMacros);
    return 0;
}


