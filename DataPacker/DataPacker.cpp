//   CSC237 Project1:  Text Packing / Unpacking Operations 
//   Student:   Arti Kuhrmeier
//   Due Date:  10/16/24
//   Description: 
//      This program reads a text document, “packs” the ASCII characters  
//      from that document into unsigned int variables, and outputs those variables  
//      to another text file as integers. 
//      This program also reverses the process, converting the unsigned int numbers 
//      back into a copy of the original text document.

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

void packData();
void unpackData();
void displayHelpText();
bool quitCommand();
bool confirmYN(string promptText);

int main()
{
	string command;
	bool keepRunning = true;
	cout << "Supported commands: \n"
		<< "     p    Build Packed Data File.\n"
		<< "     u    Create unpacked (text) data from packed data.\n"
		<< "     h    Print this help text.\n"
		<< "     q    Quit (exit) the program." << endl;
	cout << endl;
	//Write our command loop, essentially our main menu
	while (keepRunning == true)
	{
		cout << "Command: ";
		getline(cin, command);

		if (command == "p")
		{
			packData();
		}
		else if (command == "u")
		{
			unpackData();
		}
		else if (command == "h")
		{
			displayHelpText();
		}
		else if (command == "q")
		{
			keepRunning = !quitCommand();
		}
	}
}

/*packData() reads from an existing file, and will write to a new one. This function reads a file, line-by-line
and converts the file string into bits. Afterwards, it outputs the bits into a new, specified file.*/
void packData()
{
	int inputTextLength = 0;
	ifstream inputFile;
	ofstream outputFile;
	string inputFileName, outputFileName, inputText, outputText;

	cout << "Enter the input filename: ";
	cin >> inputFileName;
	inputFile.open(inputFileName.c_str());

	if (inputFile.fail())
	{
		cout << "(line " << __LINE__ << ") Error opening file:  "
			<< inputFileName << endl;

		cin.ignore(10, '\n');
		return;
	}

	cout << "Enter the output filename: ";
	cin >> outputFileName;
	outputFile.open(outputFileName.c_str());

	/*Begin our loop of reading the specified file.*/
	while (getline(inputFile, inputText))
	{
		int charCount = 0;
		unsigned int mergeValue = 0;

		inputTextLength = inputText.length();
		outputText = inputText + '\n';
		cout << "Input text (length = " << inputTextLength << "): " << outputText;

		/*This inner loop converts the string into bits, and packs them away.*/
		for (int position = 0; position < outputText.length(); position++)
		{
			unsigned int shiftedValue = outputText[position];
			int offset = position % 4;
			if (offset == 0)
			{
				mergeValue |= shiftedValue << 24;
			}
			else if (offset == 1)
			{
				mergeValue |= shiftedValue << 16;
			}
			else if (offset == 2)
			{
				mergeValue |= shiftedValue << 8;
			}
			else if (offset == 3)
			{
				mergeValue |= shiftedValue;
			}
			/*Increase the character count until it hits 4. Once it does, pack the full mergeValue into the specified file.*/
			charCount++;
			if (charCount == 4)
			{
				outputFile << mergeValue << endl;
				charCount = 0;
				mergeValue = 0;
			}
		}
		/*Pack away any leftovers that aren't divisible by 4*/
		if (charCount > 0)
		{
			outputFile << mergeValue << endl;
			outputFile << endl;
		}
	}
	cout << endl;
	cin.ignore(10, '\n');

	inputFile.close();
	outputFile.close();
}

/*unpackData() reverses the operation of packData. Here, it reads the packed file, and assigns characters to specific bits.
After, these characters are appended onto a string, and finally outputted into a specified Output file.*/
void unpackData()
{
	int inputTextLength = 0;
	unsigned int inputNumber;
	ifstream inputFile;
	ofstream outputFile;
	string inputFileName, outputFileName, outputText;

	cout << "Enter the input filename: ";
	cin >> inputFileName;
	inputFile.open(inputFileName.c_str());

	if (inputFile.fail())
	{
		cout << "(line " << __LINE__ << ") Error opening file:  "
			<< inputFileName << endl;

		cin.ignore(10, '\n');
		return;
	}

	cout << "Enter the output filename: ";
	cin >> outputFileName;
	outputFile.open(outputFileName.c_str());

	/*Begin our loop of reading the specified file.
	Here, we don't need an inner loop. Simply create the characters, and append them to the text.*/
	while (inputFile >> inputNumber)
	{
		char character1 = (inputNumber >> 24) & 0xFF;
		char character2 = (inputNumber >> 16) & 0xFF;
		char character3 = (inputNumber >> 8) & 0xFF;
		char character4 = inputNumber & 0xFF;

		if (character1 != '\0')
		{
			outputText += character1;
		}
		if (character2 != '\0')
		{
			outputText += character2;
		}
		if (character3 != '\0')
		{
			outputText += character3;
		}
		if (character4 != '\0')
		{
			outputText += character4;
		}
	}
	outputFile << outputText;
	cout << endl;
	cin.ignore(10, '\n');

	inputFile.close();
	outputFile.close();
}

/*Here we show a reference table for commands. This is also shown as soon as the program starts.*/
void displayHelpText()
{
	cout << "Supported commands: \n"
		<< "     p    Build Packed Data File.\n"
		<< "     u    Create unpacked (text) data from packed data.\n"
		<< "     h    Print this help text.\n"
		<< "     q    Quit (exit) the program." << endl;
	cout << endl;
}

/*This quit command confirms whether or not you want to exit the program.*/
bool quitCommand()
{
	return confirmYN("Are you sure that you want to exit the program? ");
}

/*This block of code sets various flags to confirm whether or not the user's input was deliberate.
In this case, it's used to confirm that the user wishes to exit the program.*/
bool confirmYN(string promptText)
{
	string inputString;
	bool confirmationValue = false;
	bool inputIsOK = false;

	do
	{
		// input from user 
		cout << promptText;
		getline(cin, inputString);

		if (inputString == "y" || inputString == "Y")
		{
			confirmationValue = true;
			inputIsOK = true;
		}
		else if (inputString == "n" || inputString == "N")
		{
			confirmationValue = false;
			inputIsOK = true;
		}
		else
		{
			cout << "Invalid input:  " << inputString << ".  Please input y or n." << endl;
		}
	} while (inputIsOK == false);
	return confirmationValue;
}