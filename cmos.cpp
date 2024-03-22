/**
 *        @file: cmos.cpp
 *      @author: Drew VanAtta and Jake Bailey
 *        @date: Feb 26, 2024
 *       @brief: Add Description
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <set> //helped us with duplicate k-grams & comparing fingerprints with intersection
#include <algorithm>
using namespace std;

size_t KGRAMSIZE = 11;
size_t WINDOWSIZE = 21;
/// function prototypes

/**
 * @brief Takes the intersection of two k-gram sets
 * 
 * @param hashedkGrams The vector of k-gram sets
 * @param i The index of the first k-gram set
 * @param j The index of the second k-gram set
 * @return The intersection of the two k-gram sets
 */
set<int> intersectionKGrams(vector<set<int>> hashedkGrams, int i, int j)
{
  set<int> intersectionSet;
  set_intersection(hashedkGrams[i].begin(), hashedkGrams[i].end(), hashedkGrams[j].begin(), hashedkGrams[j].end(), inserter(intersectionSet, intersectionSet.begin()));
  return intersectionSet;
}

/** 
* @brief GENERATES OUR KGRAMS AND DOES THE FINGERPRINT ANALYSIS
* @param text the string of tokens
* @param k the size of each kgram
* @return a set of hashed kgrams
*/
set<int> generateKGrams(const string &text, int k)
{
  vector<int> kGrams;

  for (size_t i = 0; i <= text.length() - k; ++i) //text.length() - k for bounding the kgrams
  {

    string kGram = text.substr(i, k);

    // hash the kgram
    int hashedGram = 0;
    for (size_t j = 0; j < kGram.length(); ++j)
    {
      hashedGram += kGram[j];
      hashedGram *= (j + 1);
    }
    hashedGram %= 1007; //seemed like a fun number

    // cout << kGram << ": " << hashedGram << endl;
    kGrams.push_back(hashedGram);
  }
  // FINGERPRINT + WINDOW ANAYLSIS
  set<int> fingerprintSet;
  int smallest;
  for (size_t j = 0; j <= kGrams.size() - WINDOWSIZE; j++) //
  {
    smallest = kGrams[j]; //initialize smallest variable
    for (size_t i = 0; i < WINDOWSIZE; i++)
    {
      if (kGrams[i + j] < smallest)
      {
        smallest = kGrams[i + j];
      }
    }
    fingerprintSet.insert(smallest); //add the smallest to the k-gram set
  }

  return fingerprintSet;
}

int main(int argc, char const *argv[])
{

  ifstream inFile;
  inFile.open("tokens.txt");
  if (inFile.fail())
  {
    cout << "File failed to open" << endl;
    return 0;
  }

  vector<set<int>> hashedkGrams;
  for (int i = 0; i < 54; i++)
  {
    string temp;
    // inFile.ignore(' ');
    getline(inFile, temp);
    hashedkGrams.push_back(generateKGrams(temp, KGRAMSIZE));
  }

  set<int> intersection;
  for (int a = 0; a < 54; a++) // amount of files
  {

    for (int b = 0; b < 54; b++) //matches them with amount of files
    {
      set<int> intersection = intersectionKGrams(hashedkGrams, a, b);
      float percentShared = (float)intersection.size() / (float)hashedkGrams[a].size() * 100.0f;
      if (percentShared > 75.0f && b != a) //ONLY KEEP TOP 75% OF MATCHES --- MOST LIKELY TO BE PLARAGARISM && avoids self match
      {
        cout << "File " << a + 1 << " is " << percentShared << "% similar to File " << b + 1 << endl;
      }
    }
  }

  inFile.close();
  return 0;
} // main