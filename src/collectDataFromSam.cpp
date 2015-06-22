#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <numeric>
#include "stringManipulation.h"
#include "mismatch.h"


//fix sequence, MDline, cigarLine
stringList insertionAndMismatch(string cigarLine, string MDline, string sequence, stringList &insertion, 
                                string &softclippedHead, string &softclippedTail, string id, string numOfMis)
{
    stringList mismatchList(atoi(numOfMis.c_str()));
    string fixedCigarLine, fixedSeq, mismatch, ref, read;
    int mis, i = 0, size = cigarLine.length();
    while( i < size)
    {
        if (cigarLine.at(i) == 'S')
        {
            if ( i == 0)
            {
                while ( cigarLine.at(i) == 'S')
                {
                    i ++ ;
                }
                softclippedHead = sequence.substr(0,i);
            }
            else
            {
                softclippedTail = sequence.substr(i,size-i);
                i += size;
            }
        }
        else if (cigarLine.at(i) == 'I')
        {
            insertion.push_back(string(1,sequence[i]));
            i ++;
        }
        else
        {
            fixedCigarLine.push_back(cigarLine[i]);
            fixedSeq.push_back(sequence[i]);
            i ++;
        }
    }
//    //debugging    
//    cout << fixedCigarLine << endl;
//    cout << MDline << endl;
//    cout << fixedSeq  <<endl;
//    cout << sequence << endl ;
//    cout << cigarLine << endl;
//    cout << softclippedHead << '\t' << softclippedTail << endl;

    //assertion
    assert(fixedCigarLine.length() == MDline.length());
    assert(fixedSeq.length() == MDline.length());
    
    //extract misincorporations
    mis = 0;
    for (int i = 0; i < fixedCigarLine.length(); i++)
    {
        if (fixedCigarLine.at(i) == 'M' && MDline.at(i) != '=')
        {
            ref = MDline.substr(i,1) ;
            read = fixedSeq.substr(i,1);
            assert (ref != read);
            mismatch = ref +"to"+ read;
            mismatchList[mis] = mismatch;
            mis ++;
        }
    }
    return mismatchList;
}

//process cigar string to remove D
string processCigar(string cigarString, int &headClipped, int &tailClipped)
{
    stringList cigarLetter;
    numList cigarNum;
    string cigarLine;
    regexSeparate(cigarString,cigarNum,cigarLetter);
    int operators = cigarNum.size();
    for (int i=0 ; i < operators; i++)
    {
        if (cigarLetter[i] != "D")
        {
            if (i == 0 && cigarLetter[i] == "S")
            {
                headClipped = cigarNum[0];
            }
            else if (cigarLetter[i] == "S")
            {
                tailClipped = cigarNum[i];
            }
            for (int j = 0; j < cigarNum[i]; j++)
            {
                cigarLine += cigarLetter[i];
            }
        }
    }
    return cigarLine;
}

// create sequence-like MD string
string creatMDstring(numList correctedMDnum, stringList correctedMDletter)
{
    string MDstring;
    for (int i = 0; i < correctedMDletter.size(); i++)
    {
        if (correctedMDletter[i][0] != '^' && correctedMDletter[i] != "N")
        {
            MDstring.append(string(correctedMDnum[i],'='));
            MDstring.append(correctedMDletter[i]);
        }
        else
        {
            MDstring.append(string(correctedMDnum[i],'='));
        }
    }
    return MDstring;
}

// correct MD sepearte list
string correctMDstring(numList MDnum, stringList MDLetter, stringList &deletions)
{
    stringList correctedMDletter;
    numList correctedMDnum;
    string MDList;
    int i = 0;
    while (i < MDnum.size())
    {
        if (MDLetter[i] == "^")
        {
            correctedMDnum.push_back(MDnum[i]);
        }
        else if (MDLetter[i][0] == '^')
        {
            if (MDLetter[i+1][0]!='^')
            {
                correctedMDletter.push_back(MDLetter[i]);
            }
            else if (MDLetter[i+1].length() < MDLetter[i].length())
            {
                correctedMDletter.push_back(MDLetter[i]);
            }
        }
        else if (MDLetter[i][0] != '^')
        {
            correctedMDnum.push_back(MDnum[i]);
            correctedMDletter.push_back(MDLetter[i]);
        }
        i++;
    }

    //make deletion list
    for (int i = 0; i < correctedMDnum.size(); i ++)
    {
        if (correctedMDletter[i][0] == '^')
        {
            deletions.push_back(correctedMDletter[i]);
        }
    }
    MDList  = creatMDstring(correctedMDnum,correctedMDletter);
    return MDList;
}

//function for processing MD strings
string processMD(string MDfield, stringList &deletions)
{
    string MD, MDstring;
    MD = extractField(MDfield);
    numList MDnum;
    stringList MDLetter;
    regexSeparate(MD, MDnum, MDLetter);
    MDstring = correctMDstring(MDnum,MDLetter, deletions);
    return MDstring;
}

//getting average quality given a quality string
double averageQual(string quality)
{
    int i, qualscore, qual = 0 , size = quality.length();
    double avgQual;

    for (i = 0; i < size; i ++ )
    {
        qualscore = quality.at(i);
        qual += (qualscore-33);
    }
    avgQual = qual/(double)size;
    return avgQual;
}

//define extra field
int findField(stringList columns,string &XGfield, 
                string &NMfield, string &MDfield)
{
    int i;
    string field;
    for (i = 10 ; i < columns.size(); i++)
    {
        field = columns[i];
        if (field.substr(0,2) == "XG")
        {
            XGfield = field;
        }
        else if (field.substr(0,2) == "NM")
        {
            NMfield = field;
        }
        else if (field.substr(0,2) == "MD")
        {
            MDfield = field;
        }
    }
    return 0;
}

int printBase(numList bases)
{
    for (int i = 0 ; i < bases.size(); i ++)
    {
        cout << bases[i] << "\t"; 
    }
    return 0;
}

//main processing function
int processline(string line)
{    
    stringList columns, deletions, insertions, mismatchList;
    string softclippedHead, softclippedTail;
    numList baseCounter, deletionBaseCounter, insertionBaseCounter, mismatchBaseCounter;
    numList headClippedBaseCounter, tailClippedBaseCounter;
    string chrom, id, sequence,quality;
    string deletionsString, insertionString, clippedString;
    string  XGfield, NMfield, MDfield, numberOfMismatch, numberOfGapExtention;
    string cigarString, MDline, cigarLine;
    int i, seqlength, headClipped = 0, tailClipped = 0;
    double averageQualityScore, head5Qual, end5Qual;

    columns = split(line,'\t');
    chrom = columns[2];
    if (chrom != "*")
    {
        //define columns
        id = columns[0];
        cigarString = columns[5];
        sequence = columns[9];
        quality = columns[10];
        seqlength = sequence.length();
        
        baseCounter = getBaseCount(sequence);
        averageQualityScore = averageQual(quality);
        head5Qual = averageQual(quality.substr(0,5));
        end5Qual = averageQual(quality.substr(seqlength-5,5));

        //define extra field
        findField(columns, XGfield, NMfield, MDfield);

        //get field numbers
        numberOfMismatch = extractField(NMfield);
        numberOfGapExtention = extractField(XGfield);

        MDline = processMD(MDfield, deletions);

        cigarLine = processCigar(cigarString, headClipped, tailClipped);
        mismatchList = insertionAndMismatch(cigarLine, MDline, sequence, insertions, softclippedHead, softclippedTail, id, numberOfMismatch);
        mismatchBaseCounter = getMismatchCount(mismatchList);
        
        //count deletion and insertion
        deletionsString = concatString(deletions);
        insertionString = concatString(insertions);
        deletionBaseCounter = getBaseCount(deletionsString);
        insertionBaseCounter = getBaseCount(insertionString);
        headClippedBaseCounter = getBaseCount(softclippedHead);
        tailClippedBaseCounter = getBaseCount(softclippedTail);

        //assertions for verifying program
        assert (softclippedTail.length() == tailClipped);
        assert (softclippedHead.length() == headClipped);
        assert(accumulate(baseCounter.begin(),baseCounter.end(),0) == seqlength);

        // print out result
        cout << id << "\t" << seqlength << "\t" ;
        //print out base counts
        printBase(baseCounter) ;
        cout << averageQualityScore << "\t" <<  head5Qual << "\t" << end5Qual << "\t";
        cout << numberOfGapExtention << "\t" << atoi(numberOfMismatch.c_str()) - atoi(numberOfGapExtention.c_str()) << "\t";
        //print mismatch
        // AtoC, AtoT, AtoG, CtoA, CtoT, CtoG, GtoA, GtoT, GtoC, TtoA,TtoC, TtoG
        printBase(mismatchBaseCounter);
        // print out deletion
        printBase(deletionBaseCounter) ;
        printBase(insertionBaseCounter) ;
        cout << headClipped << "\t" << tailClipped << "\t";
        printBase(headClippedBaseCounter);
        printBase(tailClippedBaseCounter);
        cout << endl;
   }
   return 0;
}

int processFile(const char * filename)
{
    ifstream samfile (filename);
    for (string line; getline(samfile, line);)
    {
        if (line[0] != '@')
        {
            processline(line);
        }
    }
    cout << endl;
    return 0;
}

//processing stdin line by line
int processStdin()
{
    for (string line; getline(cin, line) ;)
    {
        if (line[0] != '@')
        {
            processline(line);
        }
    }
    cout << endl;
    return 0;
}

//print headers
int printheader()
{            
    cout << "ID\tseqlength\t";
    cout << "A\tC\tT\tG\tN\t";
    cout << "averageQual\tfirst5Qual\tlast5Qual\t";
    cout << "gapExtention\tmismatch\t";
    cout << "AtoC\tAtoT\tAtoG\t";
    cout << "CtoA\tCtoT\tCtoG\t";
    cout << "GtoA\tGtoT\tGtoC\t";
    cout << "TtoA\tTtoC\tTtoG\t";
    cout << "deleted_A\tdeleted_C\tdeleted_T\tdeleted_G\tdeleted_N\t" ;
    cout << "inserted_A\tinserted_C\tinserted_T\tinserted_G\tinserted_N\t";
    cout << "headClipped\ttailclipped\t";
    cout << "headClipped_A\theadClipped_C\theadClipped_T\theadClipped_G\theadClipped_N\t";
    cout << "tailClipped_A\ttailClipped_C\ttailClipped_T\ttailClipped_G\ttailClipped_N";
    cout <<endl;
    return 0;
}    

//print usage
int usage(string programname)
{
    cout << "usage: " << programname << " <samfile>|<stdin>" << endl;
    cout << "If stdin is used, please provide: -" << endl;
    return 0;
}

//main function
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        string programname = argv[0];
        usage(programname);
        exit(EXIT_FAILURE);
    }
    printheader();
    cerr << argv[1] << endl;
    if (strcmp(argv[1],"-") == 0)
    {
        cerr << "Taking file from stdin!" << endl;
        processStdin();
    }
    else
    {
        cerr << "Taking file from file: "<< argv[1] << "..." << endl;
        const char *filename = argv[1];
        processFile(filename);
    }
    return 0;
}

