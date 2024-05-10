#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int amountInserted = 0;
int heapDownActions = 0;
int userInserts = 0;
int userRemoves = 0;
int userReturnMin = 0;
int userPrint = 0;
int totalHeapUser = 0;
int totalHeapUp = 0;
int totalHeapDown = 0;
bool capacityDoubled = false;

//Given struct by professor
struct priorityData {
	string dataValue;
	int priorityValue;
};

class ArrayHeap {
private:
	int capacity;
	priorityData* arrayHeap;
public:
	ArrayHeap() {
		capacity = 10;
		arrayHeap = new priorityData[capacity];
	}
	//Overload constructor
	ArrayHeap(int c) {
		capacity = c;
		arrayHeap = new priorityData[capacity];
	}
	void addElement(priorityData elem) {
		int position = 0;
		int parentIndex = 0;
		capacityDoubled = false;
		//Deals with the array being filled
		if (arrayHeap[capacity - 1].priorityValue > 0) {
			priorityData* largerArrayHeap = new priorityData[capacity * 2];
			for (int i = 1; i < capacity; i++) {
				largerArrayHeap[i] = arrayHeap[i];
			}
			//Delete the original heap to save on memory, no need to keep the old one
			delete[] arrayHeap;

			//Update the pointer to the new array
			arrayHeap = largerArrayHeap;
			capacity *= 2;
			capacityDoubled = true;
		}

		//Add element at the end of the queue
		for (int i = 1; i < capacity; i++) {
			if (arrayHeap[i].priorityValue < 0) {
				arrayHeap[i] = elem;
				position = i;
				break;
			}
		}

		parentIndex = position / 2;

		while (parentIndex != 0) {
			if (arrayHeap[parentIndex].priorityValue > arrayHeap[position].priorityValue) {
				swap(arrayHeap[parentIndex].priorityValue, arrayHeap[position].priorityValue);
				swap(arrayHeap[parentIndex].dataValue, arrayHeap[position].dataValue);
				position = parentIndex;
				heapDownActions++;
				totalHeapUp++;
				totalHeapDown++;
			}
			else {
				break;
			}
			//Check the next parent
			parentIndex = position / 2;
		}

	}
	priorityData removeMin() {
		priorityData minValue;
		minValue.dataValue = arrayHeap[1].dataValue;
		minValue.priorityValue = arrayHeap[1].priorityValue;
		int position = 0;
		int parentIndex = 0;
		int lastIndex = 0;

		if (arrayHeap[1].priorityValue < 0) {
			cout << "Attempt to remove element from empty heap" << endl;
			minValue.dataValue = "Invalid";
			minValue.priorityValue = -1;
		}
		else {

			for (int i = 1; i < capacity; i++) {
				if (arrayHeap[i].priorityValue > 0) {
					lastIndex = i;
				}
				else {
					break;
				}
			}

			arrayHeap[1].priorityValue = -1;
			swap(arrayHeap[1].priorityValue, arrayHeap[lastIndex].priorityValue);
			swap(arrayHeap[1].dataValue, arrayHeap[lastIndex].dataValue);

			for (int i = 2; i < lastIndex; i++) {
				swap(arrayHeap[i - 1].priorityValue, arrayHeap[i].priorityValue);
				swap(arrayHeap[i - 1].dataValue, arrayHeap[i].dataValue);
			}

			for (int i = 2; i < lastIndex; i++) {
				position = i;

				parentIndex = position / 2;

				while (parentIndex != 0) {
					if (arrayHeap[parentIndex].priorityValue > arrayHeap[position].priorityValue) {
						swap(arrayHeap[parentIndex].priorityValue, arrayHeap[position].priorityValue);
						swap(arrayHeap[parentIndex].dataValue, arrayHeap[position].dataValue);
						position = parentIndex;
						totalHeapUp++;
						totalHeapDown++;
					}
					else {
						break;
					}
					//Check the next parent
					parentIndex = position / 2;
				}
			}
		}

		return minValue;
	}

	priorityData returnMin() {

		priorityData minValue;
		//Min value should be in first array index
		minValue.dataValue = arrayHeap[1].dataValue;
		minValue.priorityValue = arrayHeap[1].priorityValue;

		if (arrayHeap[1].priorityValue < 0) {
			cout << "Attempt to return element from empty heap" << endl;
		}
		else {
			cout << "Return top element: ";
			cout << "(\"" + minValue.dataValue + "\", " + to_string(minValue.priorityValue) + ") " << endl;
		}


		return minValue;
	}

	string toString() {
		int lastIndex = 0;
		for(int i = 1; i < capacity; i++) {
			if (arrayHeap[i].priorityValue > 0) {
				lastIndex = i;
			}
			else {
				break;
			}
		}
		string result = "[";
		if (arrayHeap[1].priorityValue < 0) {
			result += "The heap is empty]";
			return result;
		}

		for (int i = 1; i < capacity; i++) {
			if (arrayHeap[i].priorityValue > 0) { 
				result += "(\"" + arrayHeap[i].dataValue + "\", " + to_string(arrayHeap[i].priorityValue) + ")";
				if (i != lastIndex) {
					result += "\n";
				}
			}
		}
		result += "]";
		result += '\n';
		return result;
	}
};

class PriorityQueue : public ArrayHeap {
private:
	int capacity = 10;
	priorityData* priorityQueue;
public:
	PriorityQueue() : ArrayHeap() {

	}

	//Overload constructor
	PriorityQueue(int c) : ArrayHeap(c) {

	}

	void insert(priorityData& elem){
		addElement(elem);
	}

	priorityData remove() {
		return removeMin();
	}

	priorityData returnPriorityMin() {
		return returnMin();
	}

	string toPriorityString() {
		return toString();
	}

};

int main() {
	int cap = 0;
	int userSelection;
	string fileName;
	string actionFileName;
	string outputFileName;
	string strElements;
	string strActionElements;
	priorityData element;

	cout << "Select 1 for default capacity." << endl;
	cout << "Select 2 to input a specific capacity." << endl;
	cin >> userSelection;
	if (userSelection == 1) {
		cap = 10;
	}
	else if (userSelection == 2) {
		cout << "What is the capacity?: ";
		cin >> cap;
	}
	else {
		cout << "Invalid Selection";
		return 1;
	}
	cout << endl;


	ArrayHeap arrayHeap(cap);
	PriorityQueue priorityQueue(cap);

	cout << "Input file name: ";
	cin >> fileName;
	cout << endl;

	cout << "Output file name: ";
	cin >> outputFileName;
	cout << endl;

	ifstream inputFile(fileName);
	ofstream outputFile(outputFileName);

	cout << "Heap created with size " << cap << endl;
	outputFile << "Heap created with size " << cap << endl;




	//Checks if file exists
	if (!inputFile) {
		cerr << "File does not exist." << endl;
		outputFile << "File does not exist." << endl;
		exit(1);
	}
	//Checks if file is empty
	else if (inputFile.peek() == ifstream::traits_type::eof()) {
		cerr << "The file you have chosen is empty" << endl;
		outputFile << "The file you have chosen is empty." << endl;
	}
	cout << "Array Heap Initialization " << endl;
	outputFile << "Array Heap Initialization " << endl;

	int checkIncrement = 1;
	while (inputFile >> strElements) {
		if (checkIncrement % 2 == 0) {
			//Checks for strings, will skip over non-numbers
			try {
				element.priorityValue = stoi(strElements);
				arrayHeap.addElement(element);
				priorityQueue.insert(element);
				if (capacityDoubled) {
					cout << "Heap table size double to " << (cap * 2) << endl;
					outputFile << "Heap table size double to " << (cap * 2) << endl;
				}
				amountInserted++;
			}
			catch (invalid_argument) {
				continue;
			}
		}
		else {
			element.dataValue = strElements;
		}
		checkIncrement++;
		

	}

	cout << amountInserted << " elements inserted" << endl;
	outputFile << amountInserted << " elements inserted" << endl;
	cout << heapDownActions << " number of heap-down actions to create heap order" << endl;
	outputFile << heapDownActions << " number of heap-down actions to create heap order" << endl;

	if (amountInserted <= 20) {
		cout << "From ArrayHeap parent class: " << endl;
		outputFile << "From ArrayHeap parent class: " << endl;
		cout << arrayHeap.toString() << endl;
		outputFile << arrayHeap.toString() << endl;
		cout << endl;
		outputFile << endl;
		cout << "From PriorityQueue child class: " << endl;
		outputFile << "From PriorityQueue child class: " << endl;
		cout << priorityQueue.toPriorityString() << endl;
		outputFile << arrayHeap.toString() << endl;
	}

	cout << "Array Heap Initialization Complete" << endl;
	outputFile << "Array Heap Initialization Complete" << endl;


	int userSelection2;
	string actionSelect;
	string userExit;

	cout << endl;
	cout << "Enter 1 for actions to be user entered only." << endl;
	cout << "Enter 2 for actions to be file based only." << endl;
	cout << "Enter 3 for actions to be both." << endl;
	cin >> userSelection2;

	totalHeapUp = 0;
	totalHeapDown = 0;
	if (userSelection2 == 1) {
		cout << "User actions interface" << endl;
		outputFile << "User actions interface" << endl;
		outputFile << endl;
		cout << endl;
		while (userExit != "Q") {
			cout << "Select I to Insert, R to remove top element, S to see top element, and A to view all elements, and Q to Quit: ";
			cin >> userExit;
			cout << endl;
			if (userExit == "I") {
				cout << "Enter string data value: " << endl;
				cin >> element.dataValue;
				userInserts++;
				cout << "Enter priority value: " << endl;
				cin >> element.priorityValue;
				cout << "Insert: value " << element.dataValue << " priority " << element.priorityValue << endl;
				outputFile << "Insert: value " << element.dataValue << " priority " << element.priorityValue << endl;
				arrayHeap.addElement(element);
				priorityQueue.insert(element);
				if (capacityDoubled) {
					cout << "Heap table size double to " << (cap * 2) << endl;
					outputFile << "Heap table size double to " << (cap * 2) << endl;
				}
			}
			else if (userExit == "R") {
				element = arrayHeap.removeMin();
				priorityQueue.remove();
				userRemoves++;
				if (element.priorityValue > 0) {
					outputFile << "Remove: ";
					outputFile << "value " + element.dataValue + ", priority " + to_string(element.priorityValue) << endl;
				}
				else {
					outputFile << "Attempt to remove element from empty heap" << endl;
				}
			}
			else if (userExit == "S") {
				element = arrayHeap.returnMin();
				priorityQueue.returnPriorityMin();
				userReturnMin++;
				if (element.priorityValue > 0) {
					outputFile << "Return top element: ";
					outputFile << "value " + element.dataValue + ", priority " + to_string(element.priorityValue) << endl;
				}
				else {
					outputFile << "Attempt to return element from empty heap" << endl;
				}
			}
			else if (userExit == "A") {
				cout << "Print of elements: " << endl;
				outputFile << "Print of elements: " << endl;
				cout << endl;
				outputFile << endl;
				userPrint++;
				cout << "From ArrayHeap parent class: " << endl;
				outputFile << "From ArrayHeap parent class: " << endl;
				cout << arrayHeap.toString();
				outputFile << arrayHeap.toString();
				cout << endl;
				outputFile << endl;
				cout << "From PriorityQueue child class: " << endl;
				outputFile << "From PriorityQueue child class: " << endl;
				cout << priorityQueue.toString();
				outputFile << arrayHeap.toString();
			}
			else if (userExit != "Q") {
				cout << "Please select a valid option: " << endl;
			}

		}
		cout << "User Actions Interface Complete" << endl;
		outputFile << "User Actions Interface Complete" << endl;
	}
	else if (userSelection2 == 2 || userSelection2 == 3) {
		cout << "User Actions File: " << endl;
		outputFile << "User Actions File: " << endl;
		cout << endl;
		outputFile << endl;
		cout << "Enter name of file: " << endl;
		cin >> actionFileName;
		ifstream actionFile(actionFileName);
		checkIncrement = 1;

		//Checks if file exists
		if (!actionFile) {
			cerr << "File does not exist." << endl;
			outputFile << "File does not exist." << endl;
		}
		//Checks if file is empty
		else if (actionFile.peek() == ifstream::traits_type::eof()) {
			cerr << "The file you have chosen is empty" << endl;
			outputFile << "The file you have chosen is empty." << endl;
		}
		while (actionFile >> strActionElements) {
			if (strActionElements == "I" || checkIncrement == 2 || checkIncrement == 3) {
				if (checkIncrement == 3) {
					//Checks for strings, will skip over non-numbers
					try {
						element.priorityValue = stoi(strActionElements);
						cout << "Insert: value " << element.dataValue << ", priority " << element.priorityValue << endl;
						outputFile << "Insert: value " << element.dataValue << ", priority " << element.priorityValue << endl;
						arrayHeap.addElement(element);
						priorityQueue.insert(element);
						if (capacityDoubled) {
							cout << "Heap table size double to " << (cap * 2) << endl;
							outputFile << "Heap table size double to " << (cap * 2) << endl;
						}
						userInserts++;
						checkIncrement = 0;
					}
					catch (invalid_argument) {
						continue;
					}
				}
				else if (checkIncrement == 2) {
					element.dataValue = strActionElements;
				}
				checkIncrement++;
			}
			else if (strActionElements == "R") {
				element = arrayHeap.removeMin();
				element = priorityQueue.remove();
				userRemoves++;
				if (element.priorityValue > 0) {
					outputFile << "Remove: ";
					outputFile << "value " + element.dataValue + ", priority " + to_string(element.priorityValue) << endl;
				}
				else {
					outputFile << "Attempt to remove element from empty heap" << endl;
				}
			}
			else if (strActionElements == "S") {
				element = arrayHeap.returnMin();
				element = priorityQueue.returnPriorityMin();
				userReturnMin++;
				if (element.priorityValue > 0) {
					outputFile << "Return top element: ";
					outputFile << "value " + element.dataValue + ", priority " + to_string(element.priorityValue) << endl;
				}
				else {
					outputFile << "Attempt to return element from empty heap" << endl;
				}
			}
			else if (strActionElements == "A") {
				cout << "Print of elements: " << endl;
				outputFile << "Print of elements: " << endl;
				userPrint++;
				cout << endl;
				outputFile << endl;
				cout << "From ArrayHeap parent class: " << endl;
				outputFile << "From ArrayHeap parent class: " << endl;
				cout << arrayHeap.toString();
				outputFile << arrayHeap.toString();
				cout << endl;
				outputFile << endl;
				cout << "From PriorityQueue child class: " << endl;
				outputFile << "From PriorityQueue child class: " << endl;
				cout << priorityQueue.toPriorityString();
				outputFile << priorityQueue.toPriorityString();
			}

		}
		if (userSelection2 == 3) {
			cout << "User Actions File Complete" << endl;
			outputFile << "User Actions File Complete" << endl;
			cout << endl;
			outputFile << endl;
			while (userExit != "Q") {
				cout << "Select I to Insert, R to remove top element, S to see top element, and A to view all elements, and Q to Quit: ";
				cin >> userExit;
				cout << endl;
				if (userExit == "I") {
					cout << "Enter string data value: " << endl;
					cin >> element.dataValue;
					userInserts++;

					cout << "Enter priority value: " << endl;
					cin >> element.priorityValue;
					cout << "Insert: value " << element.dataValue << ", priority " << element.priorityValue << endl;
					outputFile << "Insert: value " << element.dataValue << ", priority " << element.priorityValue << endl;
					arrayHeap.addElement(element);
					priorityQueue.insert(element);
					if (capacityDoubled) {
						cout << "Heap table size double to " << (cap * 2) << endl;
						outputFile << "Heap table size double to " << (cap * 2) << endl;
					}

				}
				else if (userExit == "R") {
					element = arrayHeap.removeMin();
					element = priorityQueue.remove();
					userRemoves++;
					if (element.priorityValue > 0) {
						outputFile << "Remove: ";
						outputFile << "value " + element.dataValue + ", priority " + to_string(element.priorityValue) << endl;
					}
					else {
						outputFile << "Attempt to remove element from empty heap" << endl;
					}
				}
				else if (userExit == "S") {
					element = arrayHeap.returnMin();
					element = priorityQueue.returnPriorityMin();
					userReturnMin++;
					if (element.priorityValue > 0) {
						outputFile << "Return top element: ";
						outputFile << "value " + element.dataValue + ", priority " + to_string(element.priorityValue) << endl;
					}
					else {
						outputFile << "Attempt to return element from empty heap" << endl;
					}

				}
				else if (userExit == "A") {
					cout << "Print of elements: " << endl;
					cout << endl;
					outputFile << endl;
					outputFile << "Print of elements: " << endl;
					userPrint++;
					cout << "From ArrayHeap parent class: " << endl;
					outputFile << "From ArrayHeap parent class: " << endl;
					cout << arrayHeap.toString() << endl;
					outputFile << arrayHeap.toString() << endl;
					cout << endl;
					outputFile << endl;
					cout << "From PriorityQueue child class: " << endl;
					outputFile << "From PriorityQueue child class: " << endl;
					cout << priorityQueue.toPriorityString() << endl;
					outputFile << priorityQueue.toPriorityString() << endl;
				}
				else if (userExit != "Q") {
					cout << "Please select a valid option: " << endl;
				}

			}
			cout << "User Actions Interface Complete" << endl;
			outputFile << "User Actions Interface Complete" << endl;

		}

	}


	totalHeapUser = totalHeapUp + totalHeapDown;

	//Print metrics
	cout << "Total number of Inserts - Initialization: " << amountInserted << endl;
	outputFile << "Total number of Inserts - Initialization: " << amountInserted << endl;

	cout << "Total number of heap-down actions - Initialization: " << heapDownActions << endl;
	outputFile << "Total number of heap-down actions - Initialization: " << heapDownActions << endl;

	cout << "Total number of user requested Inserts: " << userInserts << endl;
	outputFile << "Total number of user requested Inserts: " << userInserts << endl;

	cout << "Total number of user requested Removes: " << userRemoves << endl;
	outputFile << "Total number of user requested Removes: " << userRemoves << endl;
	
	cout << "Total number of user requested Return top: " << userReturnMin << endl;
	outputFile << "Total number of user requested Return top: " << userReturnMin << endl;

	cout << "Total number of user requested Print: " << userPrint << endl;
	outputFile << "Total number of user requested Print: " << userPrint << endl;

	cout << "Total number of heap actions for user actions: " << totalHeapUser << endl;
	outputFile << "Total number of heap actions for user actions: " << totalHeapUser << endl;

	cout << "Total number of heap-up actions: " << totalHeapUp << endl;
	outputFile << "Total number of heap-up actions: " << totalHeapUp << endl;

	cout << "Total number of heap-down actions: " << totalHeapDown << endl;
	outputFile << "Total number of heap-down actions: " << totalHeapDown << endl;

	return 0;
}