#include "types.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <thread>

#include <cmath>


void save_data(appariement_map const& appar, std::string const& name) {
   std::ofstream file(name);
   for (auto const& t : appar) {
      for (auto const& tt : t.second) {
         file << t.first << " " << tt.first << " " << tt.second << std::endl;
      }
      file << "::end:: ::end:: ::end::" << std::endl;
   }
   file.close();
}


int nb_common(appax_set const* s1, appax_set const* s2) {
   int common = 0;
   if (s1->size() <= s2->size()) {
      for (auto const& s : *s1) {
         if (s2->find(s) != s2->end())
            common++;
      }
   }
   else {
      for (auto const& s : *s2) {
         if (s1->find(s) != s1->end())
            common++;
      }
   }
   return common;
}


void do_in_thread(appax_file* en, appax_file* fr, appariement_map* appariement
      , str_v_t::iterator ite_b, str_v_t::iterator ite_e) {
   int cpt = 1;
   int max = 0;
   std::string save_fr = "";
   for (auto it = ite_b; it != ite_e; ++it) {
      if(cpt % 100 == 0)
        std::cout << "from: " << std::this_thread::get_id() << " -> " << cpt++ << std::endl;
      max = 0;
      save_fr = "";
      for (auto const& v : *fr) {
         int score = nb_common(&(v.second), &((*en)[*it]));
         if (score > 0 && score > max) {
            save_fr = v.first;
            max = score;
         }
      }
      (*appariement)[save_fr][*it] = max;
   }
}


int main(int argc, char** argv) {   
   // read files

   appax_file fr;
   appax_file en;

   str_v_t en_filename;

   appariement_map appariement;
   inverse_t inverse;


   std::cout << "fr" << std::endl;
   // {{{ FR
   // init an empty string
   std::ifstream file_fr(argv[1]);
   std::string line;
   std::getline(file_fr, line);
   while (std::getline(file_fr, line)) {
      // get filename
      int pos = line.find_first_of(";");
      std::string doc = line.substr(0, pos);
      // get other
      std::string other = line.substr(pos+1, line.size());
      std::istringstream iss(other, std::istringstream::in);

      std::string w;
      while (iss >> w) {
         fr[doc].insert(w);
      }
   }
   file_fr.close();
   // }}}

   std::cout << "en" << std::endl;
   // {{{ EN
   std::ifstream file_en(argv[2]);
   std::getline(file_en, line);
   int nb_file_en = std::atoi(line.c_str());
   while (std::getline(file_en, line)) {
      // get filename
      int pos = line.find_first_of(";");
      std::string doc = line.substr(0, pos);

      // save doc name
      en_filename.push_back(doc);

      // get other
      std::string other = line.substr(pos+1, line.size());
      std::istringstream iss(other, std::istringstream::in);

      std::string w;
      while (iss >> w) {
         en[doc].insert(w);
      }
   }
   file_en.close();
   // }}}

   int nb_thread = std::atoi(argv[3]);
   int per_parts = nb_file_en / nb_thread;
   std::vector<std::thread> threads;

   // make appariement
   std::cout << "start scoring" << std::endl;
   for (auto k = 0; k < nb_thread; ++k) {
   threads.push_back(std::thread(do_in_thread, &en, &fr, &appariement
         , en_filename.begin()+(k*per_parts)
         ,(k == nb_thread-1 ? en_filename.end() 
            : en_filename.begin()+((k+1)*per_parts))));
   }

   for (auto k = 0; k < nb_thread; ++k)
      threads.at(k).join();

   std::cout << "save appariement" << std::endl;
   save_data(appariement, "save/appariements_fr-en.txt");

   return EXIT_SUCCESS;
}

