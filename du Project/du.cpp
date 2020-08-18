/*!	\file		du.cpp
	\author		Vitor Antunes
	\date		2020-04-15
	\Purpose	Program that calculate the disk usage of one or more specific folders and all subfolders inside them,
				with a few different input and output options
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

int main(int argc, char* argv[])
{
	
	//Declares Bools for the switches
	bool bools = false;
	bool boolk = false;
	bool boolh = false;
	bool boolz = false;
	bool booln = false;
	bool boolr = false;
	bool boolb = false;
	bool boolHelp = false;
	bool boolVersion = false;
	bool boolBlock = false;
	bool boolFolder = false;
	//Variable to hold the size of the cluster
	dataType clusterSize = 0;
	//Variable to hold path a input folder
	path folderPath;
	//vector to hold all the folder paths
	vector<path> pathVec;
	//String to hold program version
	string version = "1.0.0";
	
	

	//For loop for reading the arguments
	for (size_t i = 1; i < (size_t)argc; i++)
	{
		//place the argument in a stringstream
		stringstream tempstream(argv[i]);
		//char to hold chars from the stream
		char tempChar;
		//gets the first char in the argument
		tempstream.get(tempChar);
		//checks if is switch
		if (tempChar == '-')
		{
			//gets the next char
			while (tempstream.get(tempChar))
			{
				//if else for all the switches
				if (tempChar == 's') { if (bools) { cout << "Error: duplicated switch <" << tempChar << ">\n"; return EXIT_SUCCESS; } bools = true; }
				else if (tempChar == 'k') { if (boolk) { cout << "Error: duplicated switch <" << tempChar << ">\n"; return EXIT_SUCCESS; } boolk = true; }
				else if (tempChar == 'h') { if (boolh) { cout << "Error: duplicated switch <" << tempChar << ">\n"; return EXIT_SUCCESS; } boolh = true; }
				else if (tempChar == 'z') { if (boolz) { cout << "Error: duplicated switch <" << tempChar << ">\n"; return EXIT_SUCCESS; }boolz = true; }
				else if (tempChar == 'n') { if (booln) { cout << "Error: duplicated switch <" << tempChar << ">\n"; return EXIT_SUCCESS; }booln = true; }
				else if (tempChar == 'r') { if (boolr) { cout << "Error: duplicated switch <" << tempChar << ">\n"; return EXIT_SUCCESS; }boolr = true; }
				else if (tempChar == 'b') { if (boolb) { cout << "Error: duplicated switch <" << tempChar << ">\n"; return EXIT_SUCCESS; }boolb = true; }
				//if is an -- switch
				else if (tempChar == '-')
				{
					//temp string to hold the witch
					string word;
					//gets all char in the argument but stops at any '='
					while (tempstream.get(tempChar) && tempChar != '=')
						word += tempChar;
					//if else for the  -- switches
						// Help Switch
					if (word == "help") 
					{
						//cout << "du (c++) 2020, Vitor Antunes\n===========================================================\nVersion " <<version <<"\n\nA disk usage utility inspired by the UNIX du command.\n\nUsage: du [-skhb] [--help] [--version] [--block-size=dddd] [folder]*\n\nExamples:\n  du\n    > display the sum of the cluster sizes of each directory\n      starting the cwd\n\n  du folder\n    > display the sum of the cluster sizes of each directory\n      starting with 'folder'\n\n  du -h\n    > display the results in a human readable form\n\n  du -s\n    > display only the final summary\n\n  du -b\n    > display in bytes\n\n  du -k\n    > cluster size is 1024\n\n  du -z\n    > display the list sorted by size\n\n  du -n\n    > display the list sorted by name\n\n  du -r\n    > display the list in reverse order\n\n  du --block-size=dddd\n    > set the cluster size to the specified integer > 0\n\n  du --help\n    > displays the help\n\n  du --version\n    > displays version number in the format d.d.d" << endl;
						cout << "du (c) 2020, Vitor Antunes\n" <<
							"===========================================================\n" <<
							"Version 1.0.0\n\nA disk usage utility inspired by the UNIX du command.\n" <<
							"\nUsage: du [-skhb] [--help] [--version] [--block-size=dddd] [folder]*\n" <<
							"\nExamples:\n  du\n" <<
							"    > display the sum of the cluster sizes of each directory\n" <<
							"      starting the cwd\n" <<
							"\n  du folder\n" <<
							"    > display the sum of the cluster sizes of each directory\n" <<
							"      starting with 'folder'\n" <<
							"\n  du -h\n" <<
							"    > display the results in a human readable form\n" <<
							"\n  du -s\n" <<
							"    > display only the final summary\n" <<
							"\n  du -b\n" <<
							"    > display in bytes\n" <<
							"\n  du -k\n" <<
							"    > cluster size is 1024\n" <<
							"\n  du -z\n" <<
							"    > display the list sorted by size\n" <<
							"\n  du -n\n" <<
							"    > display the list sorted by name\n" <<
							"\n  du -r\n" <<
							"    > display the list in reverse order\n" <<
							"\n  du --block-size=dddd\n" <<
							"    > set the cluster size to the specified integer > 0\n" <<
							"\n  du --help\n" <<
							"    > displays the help\n" <<
							"\n  du --version\n" <<
							"    > displays version number in the format d.d.d" <<endl;
						return EXIT_SUCCESS;
					}
						//version Suitch
					else if (word == "version") 
					{
						cout << version << endl;
						return EXIT_SUCCESS;
					}
						//blocksize switch
					else if (word == "block-size" && tempChar == '=')
					{
						//gets the Block size value into a string
						string number;
						boolBlock = true;						
						tempstream >> number;
						//try catch to see if its a valid number
						try
						{
							clusterSize = stoi(number);
							//checks if number is 0 or negative
							if (int test = stoi(number) <= 0)
							{
								cout << "Error: block-size value is invalid <" << number << ">\n";
								return EXIT_FAILURE;
							}
						}
						catch (const std::exception&)
						{
							//if any errors occur trying to convert to int
							cout << "Error: block-size value is invalid <" << number << ">\n";
							return EXIT_FAILURE;
						}
					}
					else
					{
						//-- switch invalid
						cout << "Bad switch <--" << word << ">\n";
						return EXIT_FAILURE;
					}
					}
					
				else
				{
					// - switch invalid
					cout << "Error: unknown switches: <" << tempChar << ">\n";
					return EXIT_FAILURE;
				}
			}
		}
		else 
		{
			boolFolder = true;// signals that a input for folder path has been provided
			tempstream.unget();// puts the extracted char back in the stream
			//Gets the folder path in the folderPath variable
			while (tempstream.get(tempChar))
				folderPath += tempChar;
			//add folder path to the array
			pathVec.push_back(folderPath);
			//clean the folder path for the next input
			folderPath = "";
		}
	}
	
	//Checks if path needs to be determined
	if (!boolFolder)pathVec.push_back(".");
	//invalid switch combinations
	if (boolb && boolh) {
		cout << "Error: cannot use both -b and -h" << endl;
		return EXIT_FAILURE; }
	if (boolk && boolBlock) {
		cout << "Error: -k and --block-size are incompatible." << endl;
		return EXIT_FAILURE;
	}
	if (boolz && booln) {
		cout << "Error: -n and -z switches are incompatible." << endl;
		return EXIT_FAILURE;
	}
	//checks if the path is valid
	for (size_t i = 0; i < pathVec.size(); i++)
	{
		if (exists(pathVec[i]) && is_directory(pathVec[i])) /*cout << "All OK - folder exists\n"*/;
		else {//invalid path
			cout << "Error: folder <" << pathVec[i].string() << "> doesn't exist.";
			return EXIT_FAILURE;
		}
	}	
	
	//check if block size needs to be determined
	if (boolk) clusterSize = 1024;
	else if (!boolBlock)
	{
		// Determine cluster size
		auto drive = std::filesystem::absolute("c:\\").root_name().string() + '\\';
		DWORD sectorsPerCluster, bytesPerSector, numberFreeClusters, totalNumberOfClusters;
		if (GetDiskFreeSpaceA(drive.c_str(), &sectorsPerCluster, &bytesPerSector, &numberFreeClusters, &totalNumberOfClusters))
		{
			clusterSize = dataType(sectorsPerCluster) * bytesPerSector;
		}
	}


	//print the report
	for (size_t i = 0; i < pathVec.size(); i++)
	{
		//Size of the rootfolder
		dataType totalSize = 0;
		//Vector of the folders in the rootfolder
		vector <pair<string, dataType>> foldersVec;
		//Try catch in case of error in reading the files
		try
		{
			//Recursive function to get all the directories sizes
			rscan2(pathVec[i], foldersVec, clusterSize, totalSize);
		}
		catch (const std::exception&)
		{
			cout << "something went wrong:" << endl;
			return EXIT_FAILURE;
		}
		

		//define how many digits the biggest number to be display has in cluster and in Bytes
		dataType largestN = totalSize;
		int nDigits = 0;
		int nDigitsBytes = 0;
		dataType largestNBytes = largestN * clusterSize;
		//define the number of digits by checking how many times it can be divided by 10
		while (largestN > 0) {
			nDigits++;
			largestN /= 10;
		}
		while (largestNBytes > 0) {
			nDigitsBytes++;
			largestNBytes /= 10;
		}

		//Print summary result
		if (bools)
		{
			printResults(boolh, boolb, totalSize, clusterSize, pathVec[i].string(), nDigitsBytes, nDigits);
		}
		else
		{
			
			//Order by size
			if (boolz){
				if (boolr){// print in reverse order by size
					sort(foldersVec.rbegin(), foldersVec.rend(), sortingBySize);
					for (size_t i = 0; i < foldersVec.size(); i++)
						printResults(boolh, boolb, foldersVec[i].second, clusterSize, foldersVec[i].first, nDigitsBytes, nDigits);
				}
				else{//print order by size
					sort(foldersVec.begin(), foldersVec.end(), sortingBySize);
					for (size_t i = 0; i < foldersVec.size(); i++)
						printResults(boolh, boolb, foldersVec[i].second, clusterSize, foldersVec[i].first, nDigitsBytes, nDigits);
				}
			}
			//Order by name
			else if (booln){
				if (boolr){// print in reverse order by name
					sort(foldersVec.rbegin(), foldersVec.rend(), sortingByName);
					for (size_t i = 0; i < foldersVec.size(); i++)
						printResults(boolh, boolb, foldersVec[i].second, clusterSize, foldersVec[i].first, nDigitsBytes, nDigits);
				}
				else{//print order by name
					sort(foldersVec.begin(), foldersVec.end(), sortingByName);
					for (size_t i = 0; i < foldersVec.size(); i++)
						printResults(boolh, boolb, foldersVec[i].second, clusterSize, foldersVec[i].first, nDigitsBytes, nDigits);
				}
			}
			//Don't order
			else{
				if (boolr){// print without ordering from the end to the begining
					for (int i = foldersVec.size() - 1; i >= 0; i--)
						printResults(boolh, boolb, foldersVec[i].second, clusterSize, foldersVec[i].first, nDigitsBytes, nDigits);
				}
				else{//print without ordering
					for (size_t i = 0; i < foldersVec.size(); i++)
						printResults(boolh, boolb, foldersVec[i].second, clusterSize, foldersVec[i].first, nDigitsBytes, nDigits);
				}
			}
		}//end of if-else bools
	}// end of path for loop
}//End of Main