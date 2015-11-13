#include "types.hpp"

#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <vector>

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


void save_inverse_data(inverse_t const& inv, std::string const& name) {
   std::ofstream file(name);
   for (auto const& t : inv) {
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


void do_in_thread(int tid, std::vector<std::pair<std::string, int>>* score_v,
      appax_set* en_doc, appax_file* fr) {
   int max = 0;
   std::string save_doc;
   //appax_set useit(*en_doc);
   for (ite_appax_f it = fr->begin(); it != fr->end(); ++it) {
      int score = nb_common(&((*it).second), en_doc);
      if (score > 0 && score > max) {
         (*score_v)[tid] = std::make_pair((*it).first, score);
         max = score;
      }
   }
}


int main(int argc, char** argv) {   
   // read files
   std::ifstream file_fr(argv[1]);
   std::ifstream file_en(argv[2]);

   appax_file fr_1;
   appax_file fr_2;
   appax_file fr_3;
   appax_file fr_4;
   appax_file fr_5;
   appax_file fr_6;
   appax_file en;

   appariement_map appariement;
   inverse_t inverse;

   const int nb_thread = 6;

   std::cout << "fr" << std::endl;
   // {{{
   // init an empty string
   std::string line;
   std::getline(file_fr, line);
   int nb_file_fr = std::atoi(line.c_str());
   int buff_size = 
      round(static_cast<double>(nb_file_fr)/static_cast<double>(nb_thread));
   int nb = 1;
   while (std::getline(file_fr, line)) {
      // get filename
      int pos = line.find_first_of(";");
      std::string doc = line.substr(0, pos);
      // get other
      std::string other = line.substr(pos+1, line.size());
      std::istringstream iss(other, std::istringstream::in);

      std::string w;
      while (iss >> w) {
         if (nb < buff_size)
            fr_1[doc].insert(w);
         else if (nb < 2*buff_size)
            fr_2[doc].insert(w);
         else if (nb < 3*buff_size)
            fr_3[doc].insert(w);
         else if (nb < 4*buff_size)
            fr_4[doc].insert(w);
         else if (nb < 5*buff_size)
            fr_5[doc].insert(w);
         else
            fr_6[doc].insert(w);
      }
      nb++;
   }
   file_fr.close();
   // }}}

   std::cout << "en" << std::endl;
   int i = 1;
   std::thread t0;
   std::thread t1;
   std::thread t2;
   std::thread t3;
   std::thread t4;
   std::vector<std::pair<std::string, int>> v = {
      std::make_pair("", 0), 
      std::make_pair("", 0), 
      std::make_pair("", 0), 
      std::make_pair("", 0),  
      std::make_pair("", 0),  
      std::make_pair("", 0)};
   while ((!fr_1.empty() 
            || !fr_2.empty() 
            || !fr_3.empty() 
            || !fr_4.empty() 
            || !fr_5.empty() 
            || !fr_6.empty()) 
         && std::getline(file_en, line)) {
      // get filename
      int pos = line.find_first_of(";");
      std::string doc = line.substr(0, pos);
      // get other
      std::string other = line.substr(pos+1, line.size());
      std::istringstream iss(other, std::istringstream::in);

      std::string w;
      while (iss >> w) {
         en[doc].insert(w);
      }
      appax_set* tmp = &en[doc];

      // make appariement
      std::cout << i << std::endl;
      t0 = std::thread(do_in_thread, 0, &v, tmp, &fr_1);
      t1 = std::thread(do_in_thread, 1, &v, tmp, &fr_2);
      t2 = std::thread(do_in_thread, 2, &v, tmp, &fr_3);
      t3 = std::thread(do_in_thread, 3, &v, tmp, &fr_4);
      t4 = std::thread(do_in_thread, 4, &v, tmp, &fr_5);
      // for main thread
      do_in_thread(5, &v, tmp, &fr_6);

      t0.join();
      t1.join();
      t2.join();
      t3.join();
      t4.join();

      //*/
      auto max = std::find_if(v.begin(), v.end(), 
            [&](std::pair<std::string, int> v1) {
               return v1.second == std::max(std::max(
                              std::max(std::max(std::max(v[0].second, 
                                       v[1].second),v[2].second),
                                 v[3].second), v[4].second), 
                           v[5].second);
            });

      //inverse[max->second].insert(std::make_pair(max->first, doc));
      appariement[max->first][doc] = max->second;
      //*/
      i++;
   }
   file_en.close();

   std::cout << "save appariement" << std::endl;
   save_data(appariement, "save/appariements_fr-en.txt");

   //std::cout << "save inverse" << std::endl;
   //save_inverse_data(inverse, "save/inverse_fr-en.txt");

   return EXIT_SUCCESS;
}
