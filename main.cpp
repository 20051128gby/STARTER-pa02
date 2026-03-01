#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

static void printMovieLine(const Movie& mv) {
    cout << mv.name << ", " << fixed << setprecision(1) << mv.rating << endl;
}

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);

    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }

    vector<Movie> movies;

    string line, movieName;
    double movieRating;

    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        movies.push_back(Movie(movieName, movieRating));
    }

    movieFile.close();

    if (argc == 2){
        sort(movies.begin(), movies.end(),
             [](const Movie& a, const Movie& b){
                 return a.name < b.name;
             });

        for (const auto& mv : movies) {
            printMovieLine(mv);
        }
        return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        line = rtrim_copy(line);
        if (!line.empty()) prefixes.push_back(line);
    }
    prefixFile.close();

    PrefixTrie trie;
    for (int i = 0; i < (int)prefixes.size(); i++) {
        trie.insert(prefixes[i], i);
    }

    vector<vector<Movie>> buckets(prefixes.size());
    for (const auto& mv : movies) {
        trie.dispatchMovieToBuckets(mv, buckets);
    }

    vector<bool> has(prefixes.size(), false);
    vector<Movie> best(prefixes.size());

    for (int i = 0; i < (int)prefixes.size(); i++) {
        auto& vec = buckets[i];
        if (vec.empty()) continue;

        sort(vec.begin(), vec.end(),
             [](const Movie& a, const Movie& b){
                 if (a.rating != b.rating) return a.rating > b.rating;
                 return a.name < b.name;
             });

        has[i] = true;
        best[i] = vec.front();
    }

    for (int i = 0; i < (int)prefixes.size(); i++) {
        if (!has[i]) {
            cout << "No movies found with prefix " << prefixes[i] << endl;
            continue;
        }
        for (const auto& mv : buckets[i]) {
            printMovieLine(mv);
        }
        cout << endl;
    }

    for (int i = 0; i < (int)prefixes.size(); i++) {
        if (!has[i]) continue;
        cout << "Best movie with prefix " << prefixes[i]
             << " is: " << best[i].name
             << " with rating " << fixed << setprecision(1) << best[i].rating
             << endl;
    }

    return 0;
}

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (!movieName.empty() && movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}