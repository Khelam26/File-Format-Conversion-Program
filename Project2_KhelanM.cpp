// File Format Conversion Program
// C++ program that will convert a "pipe-delimited" text file to a file in csv format
// Programmer: Khelan Modi
// June 21st, 2019
// Due Date July 2nd, 2019

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

// Global Constant for the size of the string array
const int SIZE = 1000;

// Function prototype
void printHelpMenu(); // prints the help menu
void getInputFile(string& inputFilename, ifstream& inFile); // gets the name of the input file from the user
void getOuputFile(string& outputFile, ofstream& outFile); // gets the name of the output file from the user. has a fail member function to get a valid name
void setDelimiter(string& delimiter); // asks the user to enter delimiter, can be called by the d command
void processInputFile(string& inputFilename, string& outputFile, ifstream& infile, ofstream& outFile, string delimiter = "|");
// process the input file to print the data to a file as well as on the monitor
int splitLineToArray(string dataField, string delimiter, string dataArray[], int MAX_SIZE);
// stores the data into the array that is between the delimiters. 

int main()
{
	string command, inputFilename, outputFilename;
	ifstream inFile;
	string delimiter = "|";
	ofstream outFile;

	system("title File Format Conversion by K. Modi");
	system("color 3f");

	printHelpMenu();

	while (true) {
		cout << "\nCommand: ";
		getline(cin, command);

		if (command == "i" || command == "I") {
			getInputFile(inputFilename, inFile);// prompt that asks the user for a valid file name
		}

		else if (command == "o" || command == "O") {
			getOuputFile(outputFilename, outFile); // prompt that asks the user for a valid file name
		}
		else if (command == "h" || command == "H") {
			printHelpMenu(); // prints the help menu, which has a list of commands
		}
		else if (command == "Q" || command == "q") { // to exit the program
			cout << "Program is Exitting with status = 0\n";
			exit(0);
			system("pause");
		}
		else if (command == "CI" || command == "ci") {
			inFile.close(); // to close the input file
		}
		else if (command == "CO" || command == "co") {
			outFile.close(); // to close the output file
		}
		else if (command == "P" || command == "p") {
			processInputFile(inputFilename, outputFilename, inFile, outFile, delimiter); // processes the file to print the data to a file as well as on the monitor
		}
		else if (command == "d") {
			setDelimiter(delimiter); // changing a delimiter from its default value 
		}
		else {
			cout << "Invalid Command. \nTry another "; // prompt when an invalid command has been entered 
		}
	}
	system("pause");
	return 0;
} // end of the main function.

//****************************************************************************************************
//Prints the list of commands of the monitor, to help the user
// has no arguments nor does it return anything
//****************************************************************************************************
void printHelpMenu() {
	cout << "Supported Commands: "
		<< "\n\t CI \t Close Input File."
		<< "\n\t CO \t Close Output File."
		<< "\n\t d  \t Set the Delimiter."
		<< "\n\t h  \t Print this help text."
		<< "\n\t i  \t Open file for INPUT"
		<< "\n\t o  \t Open file for OUTPUT"
		<< "\n\t p  \t Process Data File"
		<< "\n\t q  \t Quit (End the program)\n";
} // end of printHelpMenu

//*****************************************************************************************************
// Function that asks the user to enter a valid file name, if it fails to open
// displays a error message and asking the user to enter a valid file name again
// does not return anything, takes two reference variables so that it can passed to a process array function
//*****************************************************************************************************
void getInputFile(string& inputFilename, ifstream& inFile) {

	cout << "Enter name of the input file: ";
	getline(cin, inputFilename);
	inFile.open(inputFilename);
	int choice;

	while (inFile.fail())
	{
		cout << "\a\n\t Cannot Open File. Enter name of a VALID input file or press H for help: ";
		getline(cin, inputFilename);

		inFile.open(inputFilename);
	}
}

//*****************************************************************************************************
// Function that asks the user to enter a valid file name, if it fails to open
// displays a error message and asking the user to enter a valid file name again
// does not return anything, takes two reference variables so that it can passed to a process array function
//*****************************************************************************************************
void getOuputFile(string& outputFilename, ofstream& outFile) {

	cout << "Enter name of the output file: ";
	getline(cin, outputFilename);
	outFile.open(outputFilename);
	while (outFile.fail())
	{
		cout << "\a\n\t Cannot Open File. Enter name of a VALID Output file: ";
		getline(cin, outputFilename);
		outFile.open(outputFilename);
	}
}

// =========================================================================================================
// *Function: splitLineToArray - Scan through one line of text, output fields to array.
// *Inputs: inputText =  string containing one line of text.
//          inputDelim = string containing the delimiter character (separates one data field from the next).
//          maxFields =  Maximum number of fields
// *Outputs: returns the number of fields found.
//           outputFieldArray[] = array of strings containing output fields
// *Desciption:
//      Starting at the beginning of the inputText string, scan for each
//      occurrence of the inputDelimiter character/string.  The first data field begins
//      at the starting position and ends at the position before the delimiter
//      character.
//      The SECOND data field begins at the position AFTER the delimiter, and
//      extends to the position before the NEXT ocurrence of the delimiter.
//      If no delimiter is found, the the current data field extends to the
//      end of the line.
// =========================================================================================================

int splitLineToArray(string inputText, string inputDelim,
	string outputFieldArray[], int maxFields)
{
	//	Local Variables:
	int nextFieldPosition = 0;		//	Start position of data field.
	int foundPosition;				//  Position where delimiter was found,
	int fieldCount = 0;		        //  Number of data fields found so far,
	bool endOfLine = false;			//	Flag -- end of input line detected,
	string dataField;				//  Copy of the data field.


	while (!endOfLine)
	{
		foundPosition = inputText.find(inputDelim, nextFieldPosition);
		if (foundPosition == -1)
		{
			// (Here if the string 'find' function reached the end of the input string.)
			endOfLine = true;

			foundPosition = inputText.length();

			if (foundPosition > nextFieldPosition)
			{
				dataField = inputText.substr(nextFieldPosition,
					foundPosition - nextFieldPosition);

				fieldCount++;
			}
		}
		else
		{
			// (Here if 'inputDelim' was found in the input string.)
			// Copy the data field contents from the input string to the
			// 'dataField' local variable.
			dataField = inputText.substr(nextFieldPosition, foundPosition - nextFieldPosition);
			fieldCount++;

			// Adjust 'nextFieldPosition' as preparation for finding NEXT delimiter.
			nextFieldPosition = foundPosition + 1;
		}

		if (fieldCount > 0)
		{
			if (fieldCount > maxFields)
			{
				cout << "ERROR at line __LINE__: too many input fields, fieldCount="
					<< fieldCount << endl;
			}
			else
			{
				outputFieldArray[fieldCount - 1] = dataField;
			}
		}
	} // (end while)
	return fieldCount;
} // (end function splitLineToArray )

//************************************************************************************************************************
// The function reads each line from the file and
// prints them on the monitor 
// also separates the contents between the delimiter and saves them to a file
// Does not return anyting to the main function
// Finds if there are quotes or commas in each of the array of strings.
// Inserts quotes around where the quotes are found as well as in the start 
// and end of the string. 
// Inserts quotes at the start and end of string for strings with commas.
// Inserts commas at the each of each string. 
// Arguements are passed as reference to make changes to the orginal data
//************************************************************************************************************************
void processInputFile(string& inputFilename, string& outputFile, ifstream& infile, ofstream& outFile, string delimiter) {

	int  foundPosition,		// position of the double quote(opening quote)
		foundComma,			// position found of the comma in the string array
		fieldCount = 0,		// Number of data fields found so far,
		stringLength = 0,	// The length of the string, or of each data between the delimiter
		findQuotes = 0;		// The new position of the double quote(closing quote), 
	string dataArray[SIZE], // string array that stores the data returned from each data Field
		dataField,			// The string between the delimiters is stored
		myStr = "\"";		// separating each data Field with a double quote

	while (getline(infile, dataField)) // Runs until the end of the file has been encountered
	{
		fieldCount = splitLineToArray(dataField, delimiter, dataArray, SIZE); // the value of field count is returned from the function
		// This function also separates each data field.
		cout << "Input Data  = " << dataField << endl; // prints the data as it is on the monitor.

		for (int x = 0; x < fieldCount; x++)
		{
			// Finds the first position is which a double quote is found in string array
			foundPosition = dataArray[x].find('\"', 0);

			// statement happens if .find function returns a position in which a double quote was found
			if (foundPosition != string::npos) // npos checks if the foundposition is a valid location
			{
				// Insert a double quote where the opening double quote was found
				dataArray[x].insert(foundPosition, myStr);

				// Finds the second position is which a closing double quote is found in string array
				findQuotes = dataArray[x].find('\"', foundPosition + 2); // location is updated to find the new double quote

				// Insert a double qoute where the closing double quote was found in the string array
				dataArray[x].insert(findQuotes, myStr);

				// Insert another double quote in front of the already existed double quote 
				dataArray[x].insert(0, myStr);

				// Finding the length of string to find the end of the string
				stringLength = dataArray[x].length();

				// Insert double quote at the end of the already existed double quote 
				dataArray[x].insert(stringLength, myStr);
			} // code for the DOUBLE QUOTES section

			//--------------------------------------------------------------------------------------------------------------------
			// Finding the location of a COMMA
			// Find the initial postition in which a comma is found 
			foundComma = dataArray[x].find(',', 0);

			// statements happen if the comma is found
			if (foundComma != string::npos) // npos is to check if the location is a valid or not
			{
				// Insert a double quote at the beginning of the string, where the comma was found
				dataArray[x].insert(0, myStr);

				// Find the length of string to find the end of string
				stringLength = dataArray[x].length();

				// Insert a double quote at the end of the string in which the comma was found
				dataArray[x].insert(stringLength, myStr);
			}

			// Find the lenght of the final string 
			stringLength = dataArray[x].length();

			// Add a commo after each string array 
			dataArray[x].insert(stringLength, ",");
			// Print out to file
			outFile << dataArray[x];
		}
		outFile << endl;
	}

	// End of file
	cout << "End of file encountered." << endl;

} // (end processDataFile function)


//************************************************************************************************************************
// Prompts the user to set a new delimiter
// the default value of the delimiter is pipe "|"
// prints the new delimter on the monitor
//************************************************************************************************************************
void setDelimiter(string& delimiter) {
	cout << "Existing delimiter = \"" << delimiter << "\".  Enter new delimiter: ";
	getline(cin, delimiter);
	cout << "New delimiter = \"" << delimiter << "\"." << endl;
}
