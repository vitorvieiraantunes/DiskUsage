/*!	\file		du_Functions.cpp
	\author		Vitor Antunes
	\date		2020-04-17
	\Purpose	Holds the functions for du.cpp


*/
#include "du.hpp"
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

/*			sortingBySize
*Purpose:	compare size of the second elements in the pairs
*Accepts:	pair<string, dataType> , pair<string, dataType>
*Returns:	bool
*/
bool sortingBySize(const pair<string, dataType>& lhs, const pair<string, dataType>& rhs)
{
	return lhs.second < rhs.second;
}
/*			sortingByName
*Purpose:	compare name of the first elements in the pairs
*Accepts:	pair<string, dataType> , pair<string, dataType>
*Returns:	bool
*/
bool sortingByName(const pair<string, dataType>& lhs, const pair<string, dataType>& rhs)
{
	return lhs.first < rhs.first;
}
/*			makeHumanReadable
*Purpose:	Takes the size in bytes, prints the value in the right unit and return the unit symbol
*Accepts:	dataType
*Returns:	string
*/
string makeHumanReadable(dataType size)
{
	//hold the size in the new unit
	double tempSize = 0;
	//holds the unit symbol
	string tempUnit = "";
	//determine the size of the number
	if (size > 0) {//bytes
		if ((size) > pow(2, 10)) {//KiB
			if ((size) > pow(2, 20)) {//MiB
				if ((size) > pow(2, 30)) {//GiB
					if ((size) > pow(2, 40)) {//TiB
						tempUnit = "T";
						tempSize = pow(2, -40) * (size);//calculates the size in the new unit
					}
					else {
						tempUnit = "G";
						tempSize = pow(2, -30) * (size);//calculates the size in the new unit
					}
				}
				else {
					tempUnit = "M";
					tempSize = pow(2, -20) * (size);//calculates the size in the new unit
				}
			}
			else {
				tempUnit = "K";
				tempSize = pow(2, -10) * (size);//calculates the size in the new unit
			}
		}
		else {
			tempUnit = "B";
			tempSize = (double)size;//size doesn't change
		}
	}
	if (tempSize >= 10)//Rounding for number >=  10
	{//round to 0 decimals
		tempSize = (int)(((tempSize * 10) + 5) / 10);
		cout << tempSize;//print number
	}
	else if (tempSize > 0)//Rounding for number <  10
	{//round to 1 decimal
		tempSize = ((int)((tempSize * 10) + .5)) / 10.0;
		cout << fixed << setprecision(1) << tempSize;// print the number with fixed precision
		cout << fixed << setprecision(0);//set precision back
	}
	else//no rounding or unit for 0
		cout << 0;
	return  tempUnit;//return the unit if any
}

/*			rscan2
*Purpose:	scan the current folder and all sub folders to adds the size of all the files
*Accepts:	path, vector<pair<string, dataType>>, dataType, dataType
*Returns:	void
*/
//
void rscan2(path const& f, vector <pair<string, dataType>>& foldersVec, dataType clusterSize, dataType& totalSize) {
	
	//Loops the item in the main folder
	for ( auto d : directory_iterator(f))
	{
		//Hold the size of the current item
		dataType size = 0;
		//If item is a directory
		if (is_directory(d.status()))
		{
			//Recursive iterator loops everything inside the folder
			for (auto d2 : recursive_directory_iterator(d.path()))
			{

				//if the current item is not a rirectory
				if (!is_directory(d2.status()))
				{
					//add file size to the current folder size
					dataType filesize = d2.file_size();
					size += filesize / clusterSize;
					//if file ocupies a partial cluster this add the complete cluster
					if (filesize % clusterSize != 0)
					{
						size += 1;
					}
				}
			}
			//cout << size << "\t" << d->path().string() << endl;
			foldersVec.push_back(make_pair(d.path().string(), size));
		}
		//If its not a directory, get the size of the file in the main folder
		else
		{
			//add file size to the current item size
			dataType filesize = d.file_size();
			size += filesize / clusterSize;
			//if file ocupies a partial cluster this add the complete cluster
			if (filesize % clusterSize != 0)
			{
				size += 1;
			}
		}
		
		//add the size of the file or folder to the total size of the main folder
		totalSize += size;
	}
	foldersVec.push_back(make_pair(f.string(), totalSize));

}
/*			printResults
*Purpose:	Checks the switches and prints the results in proper formating
*Accepts:	bool, bool, dataType, dataType, string, int, int
*Returns:	void
*/
void printResults(bool boolh, bool boolb, dataType size, dataType clusterSize, string folderpath, int nDigitsBytes, int nDigits)
{
	//print in human readable format
	if (boolh) cout << makeHumanReadable(size * clusterSize) << "\t" << folderpath << endl;
	//prints in bytes format
	else if (boolb)cout << left << setw(nDigitsBytes) << (size * clusterSize) << "   " << folderpath << endl;
	//prints in clusters
	else cout << left << setw(nDigits) << size << "   " << folderpath << endl;
}