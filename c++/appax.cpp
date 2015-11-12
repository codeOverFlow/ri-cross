#include <iostream>
#include <fstream>
#include <algorithm>
#include "types.hpp"

#include <stdlib.h>
#include <dirent.h>
#include <cstring>

/**
 * print the content of an appax_file
 */
void print_map(appax_file const& map) {
   for (auto const& t_s_m : map) {
      std::cout << "key: " << t_s_m.first << std::endl;
      for (auto const& s : t_s_m.second) {
         std::cout << "\t" << s << " -> " << s << std::endl;
      }
   }
   std::cout << std::endl << std::endl;
}

void save_map(appax_file& map, char* dirname) {
   std::string dn(dirname);
   dn = dn.substr(dn.size() - 3, 2);
   std::ofstream save_file (std::string("save/appax_per_file_") + dn 
         + std::string(".txt"));

   for (auto const& kv : map) {
      save_file << kv.first << ";";
      for (auto const& s : kv.second)
         save_file << s << " ";
      save_file << std::endl;
   }
   save_file.close();


   // serialize it
   std::string name(std::string("save/") + dn + std::string(".bin"));
   std::cout << "Serialize: " << name << "..." 
      << std::endl;
   std::ofstream of(name, std::ios::binary);

   of.write((char*)&map, sizeof(map));
   of.close();
   std::cout << "Done." << std::endl;
}

void read_dir(char* dirname) {
   // create the map
   appax_file m;

   // open the dir
   DIR* dir;
   dirent* pdir;

   std::cout << "Open the dir: " << dirname << std::endl << std::endl;
   dir = opendir(dirname);

   int i = 1;
   int k = 1;


   while ((pdir = readdir(dir))) {
      // take care of '..' and '.'
      if (pdir->d_type == DT_DIR) continue;
      
      // load the file
      std::string name = std::string(pdir->d_name);
      //std::cout << "\tRead the file: " << name << std::endl;
      m[name] = appax_set();
      std::ifstream file(dirname + name);
      std::string a;

      // create the trash
      std::set<std::string> trash;

      // store the map of the file in a pointer
      appax_set* ptmp = &m[name];

      if (k == 1000) {
         std::cout << "\r" << k*i;
         std::cout.flush();
         k = 1;
         i++;
      }

      while (file >> a) {
         // make it lowercase
         std::transform(a.begin(), a.end(), a.begin(), ::tolower);

         // erase  and save in trash if occurs at least twice
         if (ptmp->find(a) != ptmp->end()) {
            ptmp->erase(a);
            trash.insert(a);
         }
         else {
            ptmp->insert(a);
         }
      }
      k++;
   }

   // save the map
   std::cout << std::endl << "Save the map" << std::endl;
   save_map(m, dirname);
}

int main(int argc, char** argv) {
   if (argc < 2) {
      std::cerr << "[ERR] Give the data dir path" << std::endl;
      return EXIT_FAILURE;
   }
   else if (argv[1][std::strlen(argv[1])-1] != '/') {
      std::cerr << "[ERR] The data dir path must end with '/'" << std::endl;
      return EXIT_FAILURE;
   }

   read_dir(argv[1]);

   return EXIT_SUCCESS;
}
