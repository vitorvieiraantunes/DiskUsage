#pragma once
/*!	\file		du.cpp
	\author		Vitor Antunes
	\date		2020-04-17
	\Purpose	header file for du Project
*/
#include <vector>
#include <algorithm>
#include<locale>
#include<iostream>
#include<fstream>
#include<Windows.h>
#include<sstream>
using namespace std;
#include <filesystem>
using namespace std::filesystem;

typedef uintmax_t dataType;

bool sortingBySize(const pair<string, dataType>& lhs, const pair<string, dataType>& rhs);

bool sortingByName(const pair<string, dataType>& lhs, const pair<string, dataType>& rhs);

void rscan2(path const& f, vector <pair<string, dataType>>& foldersVec, dataType clusterSize, dataType& totalSize);

string makeHumanReadable(dataType size);

void printResults(bool boolh, bool boolb, dataType size, dataType clusterSize, string folderpath, int nDigitsBytes, int nDigits);
