#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

//helper function prototype
void generateWords(
    string& current,
    const string& floating,
    const set<string>& dict,
    set<string>& results,
    int pos);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    string current = in;
    generateWords(current, floating, dict, results, 0);
    return results;
}

//recursive helper function to generate all possible words
void generateWords(
    string& current,
    const string& floating,
    const set<string>& dict,
    set<string>& results,
    int pos)
{
    //base case: if we've filled all positions
    if (pos == current.length())
    {
        //count remaining floating letters that must be used
        string remainingFloat = floating;
        for (char c : current)
        {
            size_t found = remainingFloat.find(c);
            if (found != string::npos)
            {
                remainingFloat.erase(found, 1);
            }
        }
        //only add to results if all floating letters were used and word exists in dictionary
        if (remainingFloat.empty() && dict.find(current) != dict.end())
        {
            results.insert(current);
        }
        return;
    }

    //if current position already has a letter (not '-'), move to next position
    if (current[pos] != '-')
    {
        generateWords(current, floating, dict, results, pos + 1);
        return;
    }

    //try each possible letter in this position
    for (char c = 'a'; c <= 'z'; c++)
    {
        current[pos] = c;
        generateWords(current, floating, dict, results, pos + 1);
    }
    //restore the '-' for backtracking
    current[pos] = '-';
}
