//course: CS216-005
//Project: Programming Assignment 1
//Date: 02/24/2017
//Purpose: Reads and stores a database of actors and their affiliated movies into a map list
//		   which then asks the user to search for actors in movies by comparrision/contrast, in 3 diffiernt ways
//		   or ask the user to search for the co-actors or an actor
//Author: Jared Rigdon
// --------------------------------------------------

//Note: This was made with the intention of using Linux terminal, so it requires a command line prompt. This can be avoided by assigned one in the visual studio options

/*
Basics: Reads from a list of actors and what movies they are in
		Then assigns a map to assign actors in the same movie together
		The user then chooses the options to compare/contrast the actors

*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>   // transform() function
#include <cctype>
#include <set>
#include <map>

using namespace std;

// declare global variable (constant only)
const int LEN = 2;  // to store one character and one terminator

void choiceA(map<string, set<string> > &actors_db, string mov1, string mov2);
void choiceC(map<string, set<string> > &actors_db, string mov1, string mov2);
void choiceO(map<string, set<string> > &actors_db, string mov1, string mov2);

int main(int argc, char* argv[])
{
	// Check whether the number of command line arguments is exactly one 
	if (argc != 2)
	{
		cout << "Warning: need exactly one command line argument." << endl;
		cout << "Usage: " << argv[0] << " <inputfile_name>" << endl;
		return 1;
	}

	ifstream in_file;
	in_file.open(argv[1]);
	// Check whether the input file can be open successfully or not
	if (!in_file.good())
	{
		cout << "Warning: cannot open file named " << argv[1] << "!" << endl;
		return 2;
	}
	/*
	apparently you can alter the movie set list and assgin it to a new key value and it acts as a new set associated with a new key value
	thats useful
	*/
	// Read data from the input file, and store into a map object: actors_db
	// the key is the actor name, the value is the set of movies which the actor(key) is in
	map<string, set<string> > actors_db;

	while (!in_file.eof())
	{
		string line;
		getline(in_file, line);
		string name, movie;
		istringstream iss(line);
		getline(iss, name, ',');

		// Extract extra white space
		iss >> ws;

		// Create a new set of movies assocatiated with name
		set<string> movies;

		while (getline(iss, movie, ','))
		{
			movies.insert(movie);
			// extract white space 
			iss >> ws;
		}
		// If the key does not exist, add the key to the map actors_db
		// If the key exists, then do nothing
		if (actors_db.find(name) == actors_db.end())
			actors_db.insert(pair<string, set<string> >(name, movies));
	}
	// close the input file
	in_file.close();

	int choice;

	while (true) {
		//ask the user to choose to search for either actors in movies or actors and co actors
		cout << "This application stores information about Actors and their Movies,\n please choose your option (Enter Q or q to quit):" << endl;
		cout << "1. Actors in Movies\n2. Actors and co-actors" << endl;
		cin >> choice;

		//checks to see if the input is valid
		if (cin.fail())
		{
			cin.clear();
			string input_to_check;
			cin >> input_to_check;
			if (input_to_check == "Q" || input_to_check == "q")
				break;
			cout << "Invalid number!" << endl;
			continue;
		}
		else if (choice <= 0 || choice > 2) {
			cin.clear();
			cin.ignore(265, '\n');
			cout << "Invalid number!" << endl;
		}
		//asks the user for 2 movies
		else if (choice == 1) {

			string mov1, mov2;
			cin.ignore();
			cout << "Please input the first movie title: ";
			getline(cin, mov1);
			cout << "Please input the second movie title : ";
			getline(cin, mov2);

			int x = 0;
			int y = 0;
			//iterate the map list and the set list to check for the movies
			for (auto it = actors_db.cbegin(); it != actors_db.cend(); ++it) {
				for (auto i = it->second.cbegin(); i != it->second.cend(); ++i) {
					if (*i == mov1) {
						if (x < 1) {
							x++;
						}
					}
					if (*i == mov2) {
						if (y < 1) {
							y++;
						}
					}
				}
			}

			//determines if either of the movies are valid
			if ((x + y) == 2) {
				cout << "Both movies are in the database, please continue..." << endl;
			}
			else {
				cout << "One or both of the movies are not in the database." << endl;
				continue;
			}
			//asks the user for valid input
			string ten_ish;

			while (true) {
				cout << "\nPlease input your menu option (enter Q or q to quit)\nA -- to print all the actors in either of the two movies.\nC -- to print all the common actors in both of the movies.\nO -- to print all the actors who are in one movie, but not in both." << endl;
				getline(cin, ten_ish);

				if (cin.fail())
				{
					cin.clear();
					cin.ignore(265, '\n');
					cout << "Invalid option!" << endl;
					continue;
				}
				//goes to the proper choices
				else if (ten_ish == "A" || ten_ish == "a") {

					choiceA(actors_db, mov1, mov2);
					continue;

				}
				else if (ten_ish == "C" || ten_ish == "c") {

					choiceC(actors_db, mov1, mov2);
					continue;
				}
				else if (ten_ish == "O" || ten_ish == "o") {

					choiceO(actors_db, mov1, mov2);
					continue;
				}
				else if (ten_ish == "Q" || ten_ish == "q") {

					break;
				}
				else {
					cin.clear();
					cin.ignore(265, '\n');
					cout << "Invalid option!" << endl;
					continue;
				}
			}

			//clears any errors with any strings
			ten_ish.clear();
			mov1.clear();
			mov2.clear();
		}

		else {
			//asks the user for the name of an actor
			string name;
			cout << "Finding the co-actors of the actor by typing his/her name: ";
			cin.ignore();
			getline(cin, name);
			auto pos = actors_db.find(name);
			if (pos != actors_db.end())
			{
				//then search for the co_actors
				for (auto it_mov = pos->second.begin(); it_mov != pos->second.end(); it_mov++) {
					//assign the current movie to a string for comparrison
					string movie = *it_mov;
					cout << "The co-actors of " << name << " in the movie \"" << movie << "\" are: " << endl;
					//iterates over the other actors in the list
					for (auto it = actors_db.cbegin(); it != actors_db.cend(); ++it) {
						//checks to skip the current actor we are looking at
						if (it->first == name) {
							continue;
						}
						//compares the movies to see if there exist a co-actor
						for (auto i = it->second.cbegin(); i != it->second.cend(); ++i) {
							if (*i == movie) {
								cout << it->first << endl;
							}

						}
					}
					cout << "***********************************" << endl;
				}
			}

			else
			{
				cout << "The actor name you entered is not in the database." << endl;
			}
			name.clear();
		}
	}
	return 0;
}

//list the actors in both movies
void choiceA(map<string, set<string> > &actors_db, string mov1, string mov2) {
	cout << "All the actors in either of the two movies:" << endl;

	//iterates through the map list
	for (auto it = actors_db.cbegin(); it != actors_db.cend(); ++it) {
		for (auto i = it->second.cbegin(); i != it->second.cend(); ++i) {
			if (*i == mov1 || *i == mov2) {
				cout << it->first << endl;
				break;
			}
		}
	}
}

//list the actors that are in both movies
void choiceC(map<string, set<string> > &actors_db, string mov1, string mov2) {
	cout << "Common actors in both movies:" << endl;

	//iterates through the map list
	int act_ct;
	for (auto it = actors_db.cbegin(); it != actors_db.cend(); ++it) {
		for (auto i = it->second.cbegin(); i != it->second.cend(); ++i) {
			if (*i == mov1 || *i == mov2) {
				act_ct++;
			}
		}
		if (act_ct == 2) {
			cout << it->first << endl;
		}
		act_ct = 0;
	}
}

//list the actors that are in one movie but not the other
void choiceO(map<string, set<string> > &actors_db, string mov1, string mov2) {
	cout << "Actors only in one of two movies:" << endl;

	//iterates through the map list
	int act_ct;
	for (auto it = actors_db.cbegin(); it != actors_db.cend(); ++it) {
		for (auto i = it->second.cbegin(); i != it->second.cend(); ++i) {
			if (*i == mov1 || *i == mov2) {
				act_ct++;
			}
		}
		if (act_ct == 1) {
			cout << it->first << endl;
		}
		act_ct = 0;
	}
}