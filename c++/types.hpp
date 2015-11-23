#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>
#include <utility>

typedef std::set<std::string> appax_set;
typedef std::map<std::string, appax_set> appax_file;
typedef appax_file::iterator ite_appax_f;

typedef std::map<std::string, int> appar_score;
typedef std::map<std::string, appar_score> appariement_map;

typedef std::map<int, std::set<std::pair<std::string, std::string>>> inverse_t;

typedef std::vector<std::string> str_v_t;
