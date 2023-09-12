//Symbol table implement.
//Symbol table is a class that is test run in the main function.
//All input need to be capitalized before get put in the table! This is not implement in the table class, it's will be in the scanner
//Student: Thu Luu, Bri Binder, Tony Sunagawa
//CSI 465 - Compiler design
//Pokorny, Kian

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

//Error list
const string INVALID = "Invalid input!!!";
const string NOT_FOUND = "Not found!!!";
const string EMPTY_TABLE = "Table is empty!!!";

//data type size list in bytes!!!!
const int CHAR = 1;//x
const int REAL = 4;//x
const int STR = 80;//max size is 80, this also include the end of the string \0
//real life string address will be determined by the length of the input/litteral string.

const int PTR = 2;//x
const int YN = 1;//x
const int HEX = 4;//x
const int NONE = 0;//x
const int INT = 4;//x

//Prototype list
struct Symbol;
class SymbolTable;
void print_line();
void menu();
void uppercase(string&);	//This should come with scanner class

//this is menu enum for this program only
enum CHOICES 
{
	add = 1,
	lookup = 2,
	del = 3,
	print = 4,
	quit = 5
};

//this struct come with the class SymbolTable
struct Symbol
{
	string name;
	string type;
	string kind;
	int scope = 0 ;
	int address = 0;
};

class SymbolTable
{
private:
	Symbol table[100];
	int current_size = 0;			//current_size is how many element existed int the table, so counter = current_size - 1
	int current_scope = 0;
	int current_address[50];		//current_address of scope 0 is current_address[0]
public:
	SymbolTable();

	void add_symbol(Symbol);
	void del_current_scope();
	
	static Symbol create_symbol(string, string, string);	//need the namespace SymbolTable::create_symbol
	int find_symbol(string) const;
	void print_symbol(int) const;
	void printTable() const;
};

int main()
{
	int choice = 0;			//choice from the menu
	int pos = 0;			//position of a symbol in the table if found
	Symbol sym;
	SymbolTable table;
	string nm, tp, kd;

	//intro
	cout << "This program is to implement a symbol table!" << endl;
	print_line();

	/*TESTING CODE
	Symbol a;
	a.name = "A";
	a.type = "INT";
	a.kind = "VAR";

	Symbol b;
	b.name = "B";
	b.type = "REAL";
	b.kind = "VAR";

	Symbol c;
	c.name = "C";
	c.type = "INT";
	c.kind = "FUN";

	table.add_symbol(a);
	table.add_symbol(b);
	
	table.add_symbol(c);
	table.add_symbol(a);
	table.add_symbol(a);
	
	table.add_symbol(c);
	table.add_symbol(a);
	table.add_symbol(a);

	table.add_symbol(c);
	table.add_symbol(a);
	table.add_symbol(a);

	table.printTable();

	table.del_current_scope();
	table.printTable();
	
	table.add_symbol(a);
	table.add_symbol(a);
	table.printTable();

	table.del_current_scope();
	table.printTable();
	*/

	do 
	{
		menu();
		cin >> choice;

		switch (choice)
		{
		case add:
			//get name
			cout << "What is the name of the symbol? ";
			cin >> nm;
			uppercase(nm);

			//get type
			cout << "What is the type of the symbol? (char, int, real, hex, none, str, ptr, YN) ";
			cin >> tp;
			uppercase(tp);

			//get kind
			cout << "What is the kind of symbol? (var, fun, param) ";
			cin >> kd;
			uppercase(kd);

			sym = SymbolTable::create_symbol(nm, tp, kd);
			cout << sym.name << endl << sym.type << endl << sym.kind << endl;
			table.add_symbol(sym);
			table.printTable();
			print_line();
			break;
		case lookup:
			cout << "Please enter the name of the symbol you're searching for: " << endl;
			cin >> nm;
			uppercase(nm);

			pos = table.find_symbol(nm);
			if (pos != -1)
			{
				cout << endl;
				table.print_symbol(pos);
			}
			else cout << NOT_FOUND;

			print_line();
			break;
		case del:
			table.del_current_scope();
			print_line();
			break;
		case print:
			table.printTable();
			print_line();
			break;
		case quit:
			//do nothing
			print_line();
			break;
		default:
			cout << INVALID << endl;
			print_line();
		}

	} while (choice != quit);

	return 0;
}

/*
1. Input a symbol
2. Look up a symbol
3. Print table
4. Quit
*/
void menu()
{
	cout << "1. Add a symbol" << endl;
	cout << "2. Look up a symbol" << endl;
	cout << "3. Delete current scope" << endl;
	cout << "4. Print table" << endl;
	cout << "5. Quit" << endl;
	cout << "Please choose an option: " << endl;
}

//print a seperate line
void print_line()
{
	cout << endl;
	for (int i = 0; i <= 100; i++) cout << "=";
	cout << endl;
}

//turn a string into upercase
void uppercase(string& str)
{
	for (int i = 0; i < str.length(); i++)
	{
		str[i] = (char)toupper(str[i]);
	}
}

/*
print the symbol table
*/
void SymbolTable::printTable() const
{
	int width = 10;
		cout << setw(width*2) << "Name" << setw(width) << "Type" << setw(width) << "Scope" << setw(width) << "Kind" << setw(width) << "Address" << endl;
	for (int i = 0; i < current_size; i++)
	{
		cout << setw(width*2) << table[i].name << setw(width) << table[i].type << setw(width) << table[i].scope;
		cout << setw(width) << table[i].kind << setw(width) << table[i].address << endl;
	}
}

/*
* constructor
* @param s: size of the table
*/
SymbolTable::SymbolTable()
{
	current_size = 0;
	current_scope = 0;
	current_address[0] = 0;
}

/*to add a symbol to the table
* @param input: is the symbol to add
*/
void SymbolTable::add_symbol(Symbol input)
{
	table[current_size] = input;

	//get and scope and address
	table[current_size].scope = current_scope;
	table[current_size].address = current_address[current_scope];

	//update address
	switch (table[current_size].type[0])
	{
	case 'C':
		table[current_size].address = current_address[current_scope];
		current_address[current_scope] += CHAR;
		break;
	case 'I':
		table[current_size].address = current_address[current_scope];
		current_address[current_scope] += INT;
		break;
	case 'R':
		table[current_size].address = current_address[current_scope];
		current_address[current_scope] += REAL;
		break;
	case 'H':
		table[current_size].address = current_address[current_scope];
		current_address[current_scope] += HEX;
		break;
	case 'N':
		table[current_size].address = current_address[current_scope];
		current_address[current_scope] += NONE;
		break;
	case 'S':
		table[current_size].address = current_address[current_scope];
		current_address[current_scope] += STR;
		break;
	case 'P':
		table[current_size].address = current_address[current_scope];
		current_address[current_scope] += PTR;
		break;
	case 'Y':
		table[current_size].address = current_address[current_scope];
		current_address[current_scope] += YN;
		break;
	default:
		cout << "theres a problem!!!" << endl;
		break;
	}

	//update scope
	if (table[current_size].kind == "FUN")
	{
		current_scope++;
		current_address[current_scope] = 0;
	}
	current_size++;
}

//delete the most current scope
void SymbolTable::del_current_scope()
{
	int curr_scope = current_scope;

	if (current_size > 0)
	{
		while (curr_scope == table[current_size - 1].scope)
			current_size--;
		current_scope = table[current_size - 1].scope;
		//current_address[current_scope] = table[current_size - 1].address;
	}
	else cout << EMPTY_TABLE;
}

/*Create a new symbol without scope and address
* @param n: name of the symbol
* @param t: type of the symbol
* @param k: kind of the symbol
* @return a new symbol without scope and address
*/
Symbol SymbolTable::create_symbol(string n, string t, string k)
{
	Symbol sym;
	
	sym.name = n;
	sym.type = t;
	sym.kind = k;
	sym.scope = 0;
	sym.address = 0;

	return sym;
}

/*to find a symbol in the table with symbol's name.

* @param str: name of the symbol
* @return -1 if not found or the position of the symbol if found
*/
int SymbolTable::find_symbol(string str) const
{
	int pos = -1;
	uppercase(str);

	bool found = false;

	for (int i = current_size - 1; i >= 0; i--)
	{
		if (table[i].name == str)
		{
			found = true;
			pos = i;
			break;
		}
	}
	return pos;
}

/*To print a symbol from the table
* @param pos: location of the symbol on the table
*/
void SymbolTable::print_symbol(int pos) const
{
	cout << "Symbol detail:" << endl;
	cout << "\tName:    " << table[pos].name << endl;
	cout << "\tType:    " << table[pos].type << endl;
	cout << "\tKind:    " << table[pos].kind << endl;
	cout << "\tScope:   " << table[pos].scope << endl;
	cout << "\tAddress: " << table[pos].address << endl;
}
