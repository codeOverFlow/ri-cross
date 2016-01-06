#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "types.hpp"

#include <stdlib.h>
#include <dirent.h>
#include <cstring>

void save_map(appax_file& map, char* resname) {
   std::ofstream save_file (std::string(resname)
         + std::string(".txt"));

   save_file << map.size() << std::endl;
   for (auto const& kv : map) {
      save_file << kv.first << ";";
      for (auto const& s : kv.second)
         save_file << s << " ";
      save_file << std::endl;
   }
   save_file.close();
}

void read_dir(char* filename, char* resname) {
   // create the map
   appax_file m;

   // open the dir


    // load the file
    std::string name = std::string(filename);
    //std::cout << name << std::endl;
    //std::cout << "\tRead the file: " << name << std::endl;
    //m[name] = appax_set();
    std::ifstream file(name);
    std::string a;

    // create the trash
    std::set<std::string> trash;

    // store the map of the file in a pointer
    //appax_set* ptmp = &m[name];
    
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
      // get filename
      int pos = line.find_first_of(";");
      std::string doc = line.substr(0, pos);

      // get other
      std::string other = line.substr(pos+1, line.size());
      std::istringstream iss(other, std::istringstream::in);

      while (iss >> a) {

         // erase  and save in trash if occurs at least twice
	      if (a.size() > 0) {
	          if (m.find(a) == m.end())
	            m[a] = appax_set();
	          
	          appax_set* ptmp = &m[a];
           	if (ptmp->find(doc) != ptmp->end()) {
              	ptmp->erase(doc);
              	trash.insert(doc);
           	}
           	else {
             		ptmp->insert(doc);
           	}
	      }
      }
    

   }

   // save the map
   std::cout << std::endl << "Save the map" << std::endl;
   save_map(m, resname);
}

int main(int argc, char** argv) {
   if (argc < 3) {
      std::cerr << "[ERR] Give the data file path and the res file path" << std::endl;
      return EXIT_FAILURE;
   }
   
   read_dir(argv[1], argv[2]);

   return EXIT_SUCCESS;
}
