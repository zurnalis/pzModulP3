// P3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream> 


using namespace std;

struct dataRow{
	int rowNumber;
	vector<int> columnsToCover;
	int columnsToCoverCount;
	vector<int> coveredColumns;
};

struct minor{
	vector<int> rows;
	vector<int> columns;
};

//parsing/pringing methods
vector<string> loadFileLines(string filePath);
void print(vector<string> v);
vector<dataRow> parseDataTableFromLines(vector<string> v);
vector<int> parseColumnValues(string column);
void printTable(vector<dataRow> v);
string dataRowToString(dataRow dr);
string columnValuesToString(vector<int> v);
vector<minor> loadMinorsFromFile(); //loading from file by default name: minors
minor parseMinor(string minorStr);

//algorithm logic methods
vector<dataRow> findBiggestColumnsCountToCover(vector<dataRow> table); //finding column/columns with the biggest columnsToCover parameter
vector<dataRow> removeUnchamtchingRows(vector<dataRow> rows); //remove the columns with the unmatching pattern of columnsToCover
minor createMinorFromRows(vector<dataRow> rows);//creating minor from the coulns passed to method
vector<dataRow> updateTableWithMinors(vector<dataRow> tmpTable, vector<minor> minors);
vector<dataRow> updateTableWithMinor(vector<dataRow> tmpTable, minor m);//updating the table with found minor, gonna use it for actualization of table from file
dataRow updateDataRowWithMinor(dataRow row, minor m);
vector<int> substract_vector(std::vector<int> a, std::vector<int> b); //vectors substraction

//some more parsing methods and saving minor to file
string minorToString(minor m);
void saveTestMinorToFile(minor m);

int _tmain(int argc, _TCHAR* argv[])
{
	string s;
	cout << "Enter file path with the table \n";
	getline(cin, s);

	vector<string> fileLines = loadFileLines(s);
	vector<dataRow> table = parseDataTableFromLines(fileLines);

	cout << "Table loaded \n";
	printTable(table);
	cout << "\n";

	vector<minor> minors = loadMinorsFromFile();

	if (minors.size() > 0){
		cout << "Minors loaded from file, updated table: \n \n";

		table = updateTableWithMinors(table, minors);

		printTable(table);
		cout << "\n";
	}
	else{
		cout << "No correct minors found \n \n";
	}

	cout << "Starting the search for minor \n \n";

	vector<dataRow> biggestColumnsCountToCover = findBiggestColumnsCountToCover(table);
	cout << "Rows with the biggest count of columns to cover: \n \n";
	printTable(biggestColumnsCountToCover);
	cout << "\n";

	vector<dataRow> matchingColumns = removeUnchamtchingRows(biggestColumnsCountToCover);
	cout << "Matching rows with biggest count of columns to cover: \n \n";
	printTable(matchingColumns);
	cout << "\n";

	minor foundMinor = createMinorFromRows(matchingColumns);
	cout << "Found minor: \n \n";
	cout << minorToString(foundMinor) << "\n";
	cout << "\n";

	saveTestMinorToFile(foundMinor);

	cout << "Saved minor to check in file \"minorToCheck.txt\" \n \n";

	cout << "Press any key to exit";
	system("pause");

	return 0;
}

//logic methods operating on the acctual data
vector<dataRow> findBiggestColumnsCountToCover(vector<dataRow> table){
	vector<dataRow> foundColumns;
	int biggestColumnsToCover = 0;

	for (vector<dataRow>::iterator row = table.begin(); row != table.end(); ++row) {
		if ((*row).columnsToCoverCount == biggestColumnsToCover){
			foundColumns.push_back(*row);
		}else if ((*row).columnsToCoverCount > biggestColumnsToCover){
			biggestColumnsToCover = (*row).columnsToCoverCount;
			foundColumns.clear();
			foundColumns.push_back(*row);
		}
	}

	if (biggestColumnsToCover == 0){
		foundColumns.clear();
		return foundColumns;
	}

	return foundColumns;
}

vector<dataRow> removeUnchamtchingRows(vector<dataRow> rows){
	vector<dataRow> foundColumns;
	vector<dataRow> currentMatchingColumns;
	int matchingVectors = 0;

	for (vector<dataRow>::iterator row = rows.begin(); row != rows.end(); ++row) {
		for (vector<dataRow>::iterator row2 = rows.begin(); row2 != rows.end(); ++row2) {
			if ((*row).columnsToCover == (*row2).columnsToCover){
				currentMatchingColumns.push_back(*row2);
			}
		}

		if (foundColumns.size() < currentMatchingColumns.size()){
			foundColumns = currentMatchingColumns;
		}

		currentMatchingColumns.clear();
	}

	return foundColumns;
}

minor createMinorFromRows(vector<dataRow> rows){
	minor m;

	for (vector<dataRow>::iterator row = rows.begin(); row != rows.end(); ++row) {
		m.rows.push_back((*row).rowNumber);
		if (m.columns.size() == 0){
			m.columns = (*row).columnsToCover;
		}
	}

	return m;
}

vector<dataRow> updateTableWithMinors(vector<dataRow> tmpTable, vector<minor> minors){

	for (vector<minor>::iterator min = minors.begin(); min != minors.end(); ++min) {
		tmpTable = updateTableWithMinor(tmpTable, (*min));
	}

	return tmpTable;
}

vector<dataRow> updateTableWithMinor(vector<dataRow> tmpTable, minor m){
	//vector<dataRow> updatedTable;

	for (vector<int>::iterator rowNumber = m.rows.begin(); rowNumber != m.rows.end(); ++rowNumber) {
		for (vector<dataRow>::iterator row = tmpTable.begin(); row != tmpTable.end(); ++row) {
			if ((*rowNumber) == (*row).rowNumber){ //updating row if equal
				(*row) = updateDataRowWithMinor((*row), m);
			}
		}
	}

	return tmpTable;
}

dataRow updateDataRowWithMinor(dataRow row, minor m){
	row.columnsToCover = substract_vector(row.columnsToCover, m.columns);
	row.columnsToCoverCount = row.columnsToCover.size();
	row.coveredColumns = m.columns;

	return row;
}

vector<int> substract_vector(vector<int> a, vector<int> b) //substract all values in b from a
{ 
	for (vector<int>::iterator colValue = b.begin(); colValue != b.end(); ++colValue) {

		vector<int>::iterator position = find(a.begin(), a.end(), (*colValue));
		if (position != a.end()) // == vector.end() means the element was not found
			a.erase(position);
	} 

	return a;
}

//loading and parsing methods
vector<string> loadFileLines(string filePath){
	vector<string> fileLines;
	ifstream in_stream;
	string line;
	
	in_stream.open(filePath);

	while (getline(in_stream, line))
	{
		fileLines.push_back(line);
	}

	in_stream.close();

	return fileLines;
}

vector<dataRow> parseDataTableFromLines(vector<string> v){
	int parsingRow = 0;
	vector<dataRow> localTable;

	for (vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
		stringstream ss(*it);
		string column;
		int columnNumber = 0;
		dataRow dr;

		while (getline(ss, column, '|')) {
			switch (columnNumber)
			{
				case 0:{ 
					dr.rowNumber = atoi(column.c_str());
					break;
				}
				case 1:{ 
					dr.columnsToCover = parseColumnValues(column);
					break; 
				}
				case 2:{ 
					dr.columnsToCoverCount = atoi(column.c_str());
					break; 
				}
				case 3:{ 
					dr.coveredColumns = parseColumnValues(column);
					break; 
				}
				default:{ 
					cout << column << "ERROR: incorrect column count in file, ignoring column\n";
					break; 
				}
			}

			columnNumber++;
		}

		localTable.push_back(dr);
		parsingRow++;
	}

	return localTable;
}

vector<int> parseColumnValues(string column){
	vector<int> columnValues;
	stringstream ss(column);
	string val;

	while (getline(ss, val, ',')) {
		columnValues.push_back(atoi(val.c_str()));
	}

	return columnValues;
}

void printTable(vector<dataRow> v){
	for (vector<dataRow>::iterator it = v.begin(); it != v.end(); ++it) {
		cout<< dataRowToString(*it) << "\n";
	}
}

string dataRowToString(dataRow dr){
	string rowToString = to_string(dr.rowNumber) + "|" + columnValuesToString(dr.columnsToCover) + "|" + to_string(dr.columnsToCoverCount) + "|" + columnValuesToString(dr.coveredColumns);
	return rowToString;
}

string columnValuesToString(vector<int> v){
	string columnValues = "";

	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		columnValues += to_string(*it) + ",";
	}

	if (columnValues.length() > 0){
		columnValues = columnValues.substr(0, columnValues.length()-1);
	}

	return columnValues;
}

void print(vector<string> v){
	for (vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
		cout << *it << "\n";
	}
}

string minorToString(minor m){
	string minorString = "";

	minorString = columnValuesToString(m.rows) + "|" + columnValuesToString(m.columns);

	return minorString;
}

void saveTestMinorToFile(minor m){
	ofstream outputFile("minorToCheck.txt");

	outputFile.clear();

	outputFile << minorToString(m);

	outputFile.close();
}

vector<minor> loadMinorsFromFile(){
	vector<string> minorsStr = loadFileLines("minors.txt");
	vector<minor> minors;

	for (vector<string>::iterator it = minorsStr.begin(); it != minorsStr.end(); ++it) {
		minors.push_back(parseMinor(*it));
	}
	
	return minors;
}

minor parseMinor(string minorStr){
	stringstream ss(minorStr);
	string column;
	int columnNumber = 0;
	minor m;

	while (getline(ss, column, '|')) {

		switch (columnNumber)
		{
			case 0:{
					   m.rows = parseColumnValues(column);
					   break;
			}
			case 1:{
					   m.columns = parseColumnValues(column);
					   break;
			}
			default:{
						cout << column << "ERROR: incorrect column count in minors file, ignoring column\n";
						break;
			}
		}

		columnNumber++;
	}

	return m;
}

