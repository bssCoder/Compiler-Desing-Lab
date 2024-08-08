#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
using namespace std;
struct Macro {
    vector<string> args;
    string definition;
};
unordered_map<string, string> simpleMacros;
unordered_map<string, Macro> functionMacros;
void extractMacros(ifstream &inputFile) {
    string line;
    while (getline(inputFile, line)) {
        if (line.substr(0, 7) == "#define") {
            stringstream ss(line);
            string macroName;
            ss.ignore(7);
            ss >> macroName;
            size_t openParen = macroName.find('(');
            if (openParen != string::npos) { 
                string argStr = macroName.substr(openParen + 1);
                macroName = macroName.substr(0, openParen);
                size_t closeParen = argStr.find(')');
                argStr = argStr.substr(0, closeParen);
                vector<string> args;
                stringstream argStream(argStr);
                string arg;
                while (getline(argStream, arg, ',')) {
                    args.push_back(arg);
                }
                string definition;
                getline(ss, definition);
                definition = definition.substr(1);
                functionMacros[macroName] = {args, definition};
            } else { 
                string definition;
                getline(ss, definition);
                definition = definition.substr(1);
                simpleMacros[macroName] = definition;
            }
        }
    }
    inputFile.clear();
    inputFile.seekg(0);
}
string replaceFunctionLikeMacro(const string &line, const string &macroName, const Macro &macro) {
    string res = line;
    size_t pos = res.find(macroName + "(");
    while (pos != string::npos) {
        size_t start = pos + macroName.size() + 1;
        size_t end = res.find(')', start);
        if (end == string::npos) break;
        string argStr = res.substr(start, end - start);
        stringstream argStream(argStr);
        vector<string> argValues;
        string argValue;
        while (getline(argStream, argValue, ',')) {
            argValues.push_back(argValue);
        }
        string replacement = macro.definition;
        for (size_t i = 0; i < macro.args.size(); ++i) {
            size_t argPos = 0;
            while ((argPos = replacement.find(macro.args[i], argPos)) != string::npos) {
                replacement.replace(argPos, macro.args[i].length(), argValues[i]);
                argPos += argValues[i].length();
            }
        }
        res = res.replace(pos, end - pos + 1, replacement);
        pos = res.find(macroName + "(", pos + replacement.length());
    }
    return res;
}
string replaceSimpleMacros(const string &line) {
    string res = line;
    for (const auto &pair : simpleMacros) {
        size_t pos = res.find(pair.first);
        while (pos != string::npos) {
            res.replace(pos, pair.first.length(), pair.second);
            pos = res.find(pair.first, pos + pair.second.length());
        }
    }
    return res;
}
string replaceMacros(const string &line) {
    string res = replaceSimpleMacros(line);
    for (const auto &pair : functionMacros) {
        res = replaceFunctionLikeMacro(res, pair.first, pair.second);
    }
    return res;
}
void readAndProcessFile(const string &inputFilename, const string &outputFilename) {
    ifstream inputFile(inputFilename.c_str());
    ofstream outputFile(outputFilename.c_str());


    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }
    extractMacros(inputFile);
    string line;
    cout << "Modified File Content:" << endl;
    while (getline(inputFile, line)) {
        if (line.substr(0, 7) == "#define") continue;
        string modifiedLine = replaceMacros(line);
        outputFile << modifiedLine << endl;
        cout << modifiedLine << endl;
    }
    inputFile.close();
    outputFile.close();
}
string getModifiedFileName(const string &filename) {
    int extensionPointer = filename.find_last_of(".");
    return filename.substr(0, extensionPointer) + "_modified.cpp";
}
int main() {
    string filename;
    cout << "Enter name of the file to open: ";
    cin >> filename;
    string outputFilename = getModifiedFileName(filename);
    readAndProcessFile(filename, outputFilename);
    return 0;
}
