/*
Auuthor: Chalet Shelton
Date: 9/9/18
Instructor: Dr. Bryce
Description: This program attempts to run the AETG algorithm. 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>

using namespace std;

class node{
	public:
		bool covered;
		int x, y;
		node();
};

/***
set it to the size of pairs after we calculate pairs
and when ever we mark an entry in the pairs map we deacrement SIZE
****/
int SIZE = 0;
int SIZECOPY;

map<string,bool> pairs; //will contain key pair and bool if it's covered
int const totalRuns =100;

void getPairs(int factors, int levels);
vector<int> randomFactions(vector<int> &candiate, int factors, int levels);
vector<int> assignPairs(vector<int> &candiate, int factors, int levels, int &boolcounter);
void swap(int &big, int &little);

int main()
{
	int factors = 0, levels =0, counter =0, temp=0, left=0, right =0, worstTotal =0, bestTotal=0, total=0, candiateSum=0;
	int boolcounter = 0;
	string key;
	clock_t start =0.0, end =0.0;

	ofstream output;
 
	output.open("output.txt"); //open file
	
	cout <<"Enter number of factors: ";
	cin  >> factors;
	cout <<"Enter number of levels: ";
	cin  >> levels;
	
	vector<int> randInd(factors,-1); //temp for canadaite
	vector<int> candiate(factors,-1); //holds actual row for candidate
	vector<int> cpy_candiate(factors, -1);
	vector<int> bestCandiate(factors, -1); // holds the best Candiate
	vector<vector<int> > testSuite, finalSuite; // 2D arrays that contain the temporary test suite and the final testsuite 
	
	
	//get the first pair
	getPairs(factors,levels);
		SIZECOPY = pairs.size();

	int size = 0, bestsize=50000; //ints that are used to determine the best and worst test suite


	for(int n =0; n < totalRuns; n++) //run 100 times
	{
		cout << "running... \n";	
		// reset the map
		for(auto& it : pairs)
			it.second = false;

		// reset the size	
		SIZE = SIZECOPY;

		// reset the testSuite
		testSuite.clear();

		//start the time
		start = clock();
		while(true) //for the 50 runs for the best suite
		{
			/** reset variables***/
			counter = 0;
			temp = 0;			
			
			fill(candiate.begin(), candiate.end(), -1); //set the candiate to -1, basically making it empty
			
			// after getting getting the best candidate and adding it to the testSuite, pick a new unused pair for
			// the next candidate to try
			randomFactions(candiate, factors, levels);
			cpy_candiate = candiate;

			//getting how many empty factors we have left
			for(int i =0; i <candiate.size(); i++)
			{
				if(candiate[i] == -1)
					counter++;
			}
			
			//loop through 50 times with the orginal pair to find best canidate 
			for(int j =0; j < 50; j++)
			{
				//resetting candiate row
				candiate = cpy_candiate;
				//get a candiate
				for(int i = counter; i > 0; i--)
					assignPairs(candiate, factors, levels, boolcounter);
				
				//using the boolcounter to determine which is the best candiate
				if(boolcounter > temp)
				{
					bestCandiate = candiate;
					temp = boolcounter; //temp has largest number of pair
				}
				boolcounter = 0;
			}
					
			// after picking the best candidate from 50, update the map
			for(int i =0; i < factors-1; i++)
			{
				for(int k = i+1; k < factors; k++)
				{
					//look at adjacent columns
					left = bestCandiate[i];
					right = bestCandiate[k];
					//make the adjacent columns into strings so that we're able to seach the map
					key = to_string(left) + to_string(right);
					
					//if the key pair is marked false, mark it as true.
					if(pairs[key] == false)
					{				
						pairs[key] = true; //mark as covered
						--SIZE;
					}
				}
			}

			testSuite.push_back(bestCandiate); //this creates the test suite
			
			if(SIZE == 0 || SIZE < 0)
				break;
			//for(auto itr : pairs)
			//	cout << itr.first << " " << itr.second << " " << endl;

			//for(int i =0; i < candiate.size(); i++)
			//	cout << "bestCandiate[i]: " << bestCandiate[i] << endl;
				
		}
		//determines the best test suite	
		size = testSuite.size();
		//if we're on first run, make that test suite our final suite
		if(n == 0) finalSuite = testSuite;		
		if(size < bestsize)
		{
			bestsize = size;
			finalSuite = testSuite;
		}
		if(size > worstTotal) worstTotal = size;
		
		total = size;
		candiateSum += size;
	}
	
	end = clock();
	double avgTime = double(end - start) / CLOCKS_PER_SEC; //get the time
			
	cout << "Total time to run: " << avgTime << endl;
	cout << "Best result:       " << finalSuite.size() << endl;
	cout << "Worst result:      " << worstTotal << endl;
	cout << "Average result:    " << candiateSum / totalRuns << endl;
	
	output << finalSuite.size() << endl;
	output << endl;
	output << "Test Suite: \n"; 
	for(int i = 0; i < finalSuite.size(); i++)
	{
		for(int j = 0; j < factors; j++)
		{
			output << finalSuite[i][j] << " ";
		}
		
		output << endl;
	}
	output << endl;

	output.close();

	return 0;
}
/*
constructor for the class node
*/
node::node() {
	x = -1;
	y = -1;
	covered = false;
}

/*
Description: This function generates all possible pairs, converts them to a string and adds them to a map
*/
void getPairs(int factors, int levels)
{
	node temp;
	pairs.clear(); //clear the list
	int count =0;
	string touple;
	/******** generate all pairs ***********/
	for(int k = 0; k < factors - 1; k++) // for all factors, columns, minus 1
	{
		for(int i = 0; i < levels; i++) // for every value in each column
		{
			for(int j = 0; j < (levels*(factors-k-1)); j++)
			{ // for each values in remining columns
				//get the x value and y value in pair
				temp.x = i+(k*levels); // setting x value
				temp.y = j+((k+1)*levels); //setting y value
				touple = to_string(temp.x) + to_string(temp.y); //convert to strings
				pairs.insert( pair<string,bool> (touple, false) ); // pushing to map and mark them as false
			}
		}
	}

}
/*
Description: This funciton randomizes the indexes, swaps the numbers if the left number is bigger, generates the pair, converts the pair
	to a string, and checks the map to see if they are already covered. If it's not already covered, it adds it to the candiate vector and
	returns it.
*/
//randomize factors
vector<int> randomFactions(vector<int> &candiate, int factors, int levels)
{
	//initialize random seed:
	srand (time(NULL));
	int col1=-1, col2=-1;
	int left, right, temp;
	
	while(true)
	{
		col1 = rand() % factors; //generate a random col
		do{
			col2 = rand() % factors; //generate another random col
		}while(col2 == col1);
		
		
		//if the left is bigger than right swap bc it's not in our map
		if(col1 > col2)
		{
			temp = col2;
			col2 = col1;
			col1 = temp;
		}
		
		//get the pair
		left = (rand() % levels) + (col1 *(factors-1)); //x
		right = (rand() % levels) + (col2 *(factors-1)); //y

		//make into string to compare to map
		string touple = to_string(left) + to_string(right);
		//check map if covered
		if(pairs[touple] == false)
		{
			candiate[col1] = left;
			candiate[col2] = right;
			pairs[touple] = true; //mark the pair as covered
		
			--SIZE;	
		
			return candiate;
		}
	}
}
/*
Description: Swaps numbers if the left is bigger than the right
*/
void swap(int &big, int &little)
{
	//cout <<"BIG: " << big <<endl;
	//cout <<"LITTLE: " << little <<endl;
	int temp;
	if(big > little);
	else{ //if little > big
		temp = big;
		big = little;
		little = temp;
	}
	//cout <<"big: " << big <<endl;
	//cout <<"little: " << little <<endl;
	return;
}
/*
Description: This function first creates 2 vectos filled and unfilled. Filled contains the origianl pair we first picked and unfilled contains
	the indexes that aren't filled. Of the indexes that we're filled I randomly selected one of the indexes and prodcued another pair. I then 
	checked to see if the pair was a good candiate and marks it as covered if it is.
*/
//asign pairs to factions
vector<int> assignPairs(vector<int> &candiate, int factors, int levels, int &boolcounter)
{
	int col1=-1, num=0, numcpy=0, cpy=0, unmatchedPairs=0, best=-1, count=0; //best is number we add to vector
	vector<int> unfilled;
	vector<int> filled;
	
	//push unfilled and filled indexes 
	for(int i=0; i < factors; i++)
	{
		if(candiate[i] != -1)
			filled.push_back(i);
		else
			unfilled.push_back(i);
	}
	//seed time again
	srand(time(NULL));

	col1 = rand() % unfilled.size(); //generate random col
	//get a number from the factor and choose the best number based on most uncovered 
	num = unfilled[col1]*levels; //first number in col1
	
	//check to see if good candiate
	for(int j =0; j < levels; j++)
	{
		unmatchedPairs =0;
		for(int i =0; i < filled.size(); i++)
		{
			string key; //key for map
			numcpy=num; //make copy
			cpy = candiate[filled[i]]; //make copy
			swap(numcpy,cpy); //swap numbers if right is bigger than left to make sure it's in our map
			
			//create a key to mark it as covered on our map
			key = to_string(cpy)+to_string(numcpy);
			
			if(pairs[key] == false)
			{
				unmatchedPairs++;
				boolcounter++
			}
		}
		if(unmatchedPairs > count)
		{
			best = num;
			count = unmatchedPairs;
		}
		num++; //get the second number in factor
	}
	if(best == -1) return candiate;
	
	candiate[best/levels] = best;
	
	//mark pairs as covered
	/*for(int i =0; i < filled.size(); i++)
	{
		string key; //key for map
		numcpy=best; //make copy
		cpy = candiate[filled[i]]; //make copy of
		//cout <<"filled[i]: " << candiate[filled[i]] <<endl;
		if(cpy > numcpy)
		{
			int temp;
			temp = cpy;
			cpy = numcpy;
			numcpy = temp;
		}
		key = to_string(cpy)+to_string(numcpy);

		if(pairs[key] == false)
		{
			boolcounter++;
		}
	}*/
		//used for error checking, prints out the candiate vector
		//for(auto itr : pairs)
		//cout << itr.first << " " << itr.second << " " << endl;
		//cout <<"candiate[col]: " << candiate[unfilled[col1]] << endl;

	return candiate;
}