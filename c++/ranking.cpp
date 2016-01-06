#include "types.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <list>

#include <cmath>

std::mutex sher;

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


void do_in_thread(appax_file* en, appax_file* inverse_fr, std::ofstream* file
      , str_v_t::iterator ite_b, str_v_t::iterator ite_e) {
   int cpt = 1;
   std::list<int> max(10, 0);
   std::list<std::string> save_fr(10, "");
   for (str_v_t::iterator it = ite_b; it < ite_e; ++it) {
      if((++cpt) % 100 == 0)
         std::cout << "from: " << std::this_thread::get_id() << " -> " << cpt++ << std::endl;
      std::map<std::string, int> scorePerFileFr;
      max = std::list<int>(10, 0);
      save_fr = std::list<std::string>(10, "");
      for (auto const& v : (*en)[*it]) {
         for (auto const& ffr : (*inverse_fr)[v])
         {
            scorePerFileFr[ffr] += 1;
            int score = scorePerFileFr[ffr];
	    if(score > 0)
	    {
             std::list<std::string>::iterator itSave = save_fr.begin();
    	     std::list<int>::iterator itMin = max.begin();
	     std::list<int>::iterator itParcoursMax = max.begin();

	     bool find = false;
             for(auto it = save_fr.begin(); it != save_fr.end(); ++it)
             {
		if(*it == ffr)
		{
		  (*itParcoursMax)++;
                  find = true;
		  break;
		}

                if(*itMin > *itParcoursMax)
                {
		   itMin = itParcoursMax;
		   itSave = it;
		}

		++itParcoursMax;

	     }

	     if(!find && score > *itMin)
	     {
		*itMin = score;
		*itSave = ffr;
             }
	   }

         }
      }
      sher.lock();
      if (it != ite_b) {
         (*en).erase(*it);
      }
      std::list<std::string>::iterator itSave = save_fr.begin();
      for(auto itMax = max.begin(); itMax != max.end(); ++itMax)
      {
         (*file) << *itSave << " " << (*it) << " " << *itMax << std::endl;
         ++itSave;
      }
      sher.unlock();
   }
}


int main(int argc, char** argv) {   
   // read files
   appax_file inverse_fr;
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
      std::string word = line.substr(0, pos);
      // get other
      std::string other = line.substr(pos+1, line.size());
      std::istringstream iss(other, std::istringstream::in);

      std::string w;
      while (iss >> w) {
         inverse_fr[word].insert(w);
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
   int ret = system("mkdir save/");
   std::ofstream file("save/appariements_fr-en.txt");
   for (auto k = 0; k < nb_thread; ++k) {
      threads.push_back(std::thread(do_in_thread, &en, &inverse_fr, &file
               , en_filename.begin()+(k*per_parts)
               ,(k == nb_thread-1 ? en_filename.end() 
                  : en_filename.begin()+((k+1)*per_parts))));
   }

   for (auto k = 0; k < nb_thread; ++k)
      threads.at(k).join();

   return EXIT_SUCCESS;
}

