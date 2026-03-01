#ifndef MOVIES_H
#define MOVIES_H

#include <string>

struct Movie {
    std::string name;
    double rating;

    Movie() : name(""), rating(0.0) {}
    Movie(const std::string& n, double r) : name(n), rating(r) {}
};

#endif