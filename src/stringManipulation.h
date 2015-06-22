#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

typedef vector<string> stringList;
typedef vector<int> numList;

//split function to split line with desired deliminator
stringList split(const string &s, char delim) 
{
	stringList result;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) 
	{
		result.push_back(item);
	}
	return result;
}

//extract field info
string extractField(string field)
{
    stringList factors = split(field,':');
    return factors[factors.size()-1];
}

string concatString(stringList strings)
{
	int i,size = strings.size();
	string result ;
	for (i = 0; i < size; i++)
	{
		result += strings[i] ;
	}
	return result;
}
