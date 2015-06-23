#include <string>
#include <sstream>
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

void regexSeparate(string xstr, numList &nums, stringList &strs)
{
	int size = xstr.length();
	int j, current, i = 0;
	char x, y;
	while (i < size)
	{
		x = xstr.at(i);
		if (isdigit(x))
		{
			current = x - '0';
			j = i + 1;
			if (j < size){
				while (j < size && isdigit(xstr.at(j)))
				{
					y = xstr.at(j);
					current = current * 10 + (y - '0');
					j++;
					i++;
				}
			}
//			cout << "pushed current:" << current << endl;
			nums.push_back(current);
		}
		else 
		{
			string curString;
			curString = x;
			j = i + 1;
			if (j < size)
			{
				while (j < size && !isdigit(xstr.at(j)))
				{
					curString += xstr[j];
					j ++;
					i ++ ;
				}
			}
//			cout << "pushed string:" << curString << endl;
			strs.push_back(string(curString));
		}
		i ++;
	}
	strs.push_back("END");
}
