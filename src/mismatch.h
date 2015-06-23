#include <iostream>

using namespace std;

typedef vector<int> numList;

numList getBaseCount(string seq)
{
    //baseCount (A count, C count, T count, G count)
    numList baseCount(5);
    for (int i = 0 ; i < seq.length() ; i ++)
    {
        if (seq[i] == 'A')
        {
            baseCount[0] += 1;
        }
        else if (seq[i] == 'C')
        {
            baseCount[1] += 1;
        }
        else if (seq[i] == 'T')
        {
            baseCount[2] += 1;
        }
        else if (seq[i] == 'G')
        {
            baseCount[3] += 1;
        }
        else if (seq[i] == 'N')
        {
            baseCount[4] += 1;
        }
    }
    return baseCount;
}

//count mismatch 
//output:
// AtoC, AtoT, AtoG, CtoA, CtoT, CtoG, GtoA, GtoT, GtoC, TtoA,TtoC, TtoG
numList getMismatchCount(stringList mismatchList)
{
    numList mismatchCount(12);
    for (int i = 0 ; i < mismatchList.size(); i++)
    {
        if (mismatchList[i] == "AtoC")
        {
            mismatchCount[0] += 1;
//			cerr << "AtoC added"  << endl;
        }
        else if (mismatchList[i] == "AtoT")
        {
            mismatchCount[1] += 1;
//			cerr << "AtoT added" << endl;
        }
        else if (mismatchList[i] == "AtoG")
        {
            mismatchCount[2] += 1;
//			cerr << "AtoG added" << endl;
        }
        else if (mismatchList[i] == "CtoA")
        {
            mismatchCount[3] += 1 ;
//			cerr << "CtoA added" << endl;
        }
        else if (mismatchList[i] == "CtoT")
        {
            mismatchCount[4] += 1;
//			cerr << "CtoT added" << endl;
        }
        else if (mismatchList[i] == "CtoG")
        {
            mismatchCount[5] += 1;
//			cerr << "CtoG added" << endl;
        }
        else if (mismatchList[i] == "GtoA")
        {
            mismatchCount[6] += 1;
        }
        else if (mismatchList[i] == "GtoT")
        {
            mismatchCount[7] += 1;
        }
        else if (mismatchList[i] == "GtoC")
        {
            mismatchCount[8] += 1;
        }
        else if (mismatchList[i] == "TtoA")
        {
            mismatchCount[9] += 1;
        }
        else if (mismatchList[i] == "TtoC")
        {
            mismatchCount[10] += 1;
        }
        else if (mismatchList[i] == "TtoG")
        {
            mismatchCount[11] += 1;
        }
    }
    return mismatchCount;
}

