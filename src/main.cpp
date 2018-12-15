#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <cgicc/Cgicc.h>
#include "states.hpp"

void printform(std::string&);

int main()
{
   char buf[RECORD_LENGTH];
   const std::string header = "\t\t\t<tr><th>State</th><th>Capital</th>"
   "<th>Flag</th><th>Population</th><th>Total Area km \u00B2</th>"
   "<th>Postal Code</th><th>Date of Admission</th><th>Electoral Votes</th>"
   "<th>Largest City</th></tr>\n";
   const std::string headline = "C++ U. S. States Data Retrieval";

   // pointer to function prototype
   Record* (*search)(const std::string&);

   std::cout << "Content-type: text/html\n\n";
   std::cout << "<!DOCTYPE html>\n";
   std::cout << "<html>\n";
   std::cout << "\t<head>\n";
   std::cout << "\t\t<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
   std::cout << "\t\t<link rel='stylesheet' media='all' href='/includes/gradienttable.css'>\n";
   std::cout << "\t\t<title>" << headline << "</title>\n";
   std::cout << "\t</head>\n";
   std::cout << "\t<body>\n";
   std::cout << "\t\t<header><p>" << headline << "</p></header>\n";
   std::cout << "\t\t<p><a href='/index.html'>Home</a> | <a href='/states.html'>Back</a></p>\n";
   std::cout << "\t\t<h1>" << headline << "</h1>\n";

   try {
      std::string target("");
      std::string process("");
      cgicc::Cgicc cgi;
      
      cgicc::form_iterator pt = cgi.getElement("target");
      if (!pt->isEmpty() && pt != cgi.getElements().end()) {
         target = **pt;
      }
      
      cgicc::form_iterator option = cgi.getElement("option");
      if (!option->isEmpty() && option != cgi.getElements().end()) {
         process = **option;
         printform(process);  // print form with button selection preserved
      }
            
      if (process.length() > 0) {
         // main processing block
         if (process == "state" || process == "capital") {
            if (process == "state") search = &binarysearch;
            if (process == "capital") search = &linearsearch;
            Record* record = search(target);
            if (record != 0) {
               std::cout << "<table class='gradienttable'>\n";
               std::cout << header;
               std::cout << record->getTableRow();
               std::cout << "</table>\n";
            } else {
               std::cout << "<p>target not found</p>\n";
            }
         } else if (process == "state_list") {
            std::cout << "<table class='gradienttable'>\n";
            std::cout << header;
            std::ifstream ifstr;
            ifstr.open("states.dat", std::ios::in | std::ios::binary);
            Record record;
            while ( ( ifstr.read((char *)&record, sizeof(Record)) ) != 0 ) {
               std::cout << record.getTableRow();
            }            
            ifstr.close();
            std::cout << "</table>\n";
         } else if (process == "capital_list") {
            unsigned n = 0;
            std::ifstream ifstr;
            ifstr.open("states.dat", std::ios::in | std::ios::binary);
            Record record;
            std::map<std::string, unsigned> table_map;
            while ( ( ifstr.read((char *)&record, sizeof(Record)) ) != 0 ) {
               std::string keyfield = record.getCapital();
               std::pair<std::string, unsigned> p(keyfield, ++n);
               table_map.insert(p);
            }            

            // reset file pointer and dump the mapped data back out
            ifstr.clear();
            ifstr.seekg(0, std::ios::beg);
            std::cout << "\t\t<table class='gradienttable'>\n";
            std::cout << header;
            std::map<std::string, unsigned>::iterator it = table_map.begin();
            for (it=table_map.begin(); it!=table_map.end(); ++it) {
               unsigned scalar = it->second - 1;
               ifstr.seekg( scalar * sizeof(Record), ifstr.beg  );
               ifstr.read((char *)&record, sizeof(Record));
               std::cout << record.getTableRow();
            }
            std::cout << "\t\t</table>\n";
            ifstr.close();

         } else if (process == "pop" || process == "pop_reverse"
               || process == "area" || process == "area_reverse") {
            unsigned n = 0;
            std::ifstream ifstr;
            ifstr.open("states.dat", std::ios::in | std::ios::binary);
            Record record;
            std::map<unsigned, unsigned> table;
            while ( ( ifstr.read((char *)&record, sizeof(Record)) ) != 0 ) {
               std::string key;
               if (process == "pop" || process == "pop_reverse") 
                  key = record.getPop();
               else
                  key = record.getArea();
               key.erase(std::remove(key.begin(), key.end(), ','), key.end());
               std::istringstream iss(key);
               unsigned z;
               iss >> z;
               std::pair<unsigned, unsigned> p(z, ++n);
               table.insert(p);
            }            

            // reset file pointer and dump the mapped data back out
            ifstr.clear();
            ifstr.seekg(0, std::ios::beg);
            std::cout << "\t\t<table class='gradienttable'>\n";
            std::cout << header;
            if (process == "pop" || process == "area") {
               std::map<unsigned, unsigned>::iterator it = table.begin();
               for (it=table.begin(); it!=table.end(); ++it) {
                  unsigned scalar = it->second - 1;
                  ifstr.seekg( scalar * sizeof(Record), ifstr.beg  );
                  ifstr.read((char *)&record, sizeof(Record));
                  std::cout << record.getTableRow();
               }
            } else {
               std::map<unsigned, unsigned>::reverse_iterator rit = table.rbegin();
               for (rit=table.rbegin(); rit!=table.rend(); ++rit) {
                  unsigned scalar = rit->second - 1;
                  ifstr.seekg( scalar * sizeof(Record), ifstr.beg  );
                  ifstr.read((char *)&record, sizeof(Record));
                  std::cout << record.getTableRow();
               }
            }
            std::cout << "\t\t</table>\n";
            ifstr.close();
         }

      } else {
         std::cout << "<p>no process selected</p>\n";
      }

         
   } catch (std::exception& e) {
      // handle any errors - omitted for brevity
   }
   std::cout << "\t\t<p><a href='/index.html'>Home</a> | <a href='/states.html'>Back</a></p>\n";
   std::cout << "\t\t<footer><p>Copyright \u00A9 2018 Josh Roybal</p></footer>\n";
   std::cout << "\t</body>\n";
   std::cout << "</html>\n";
   return 0;
}

// print form with selected radio button preserved
void printform(std::string& process)
{
   std::cout << "\t\t<form action = 'states.cgi' method = 'POST' accept-charset='utf-8'>\n";
   std::cout << "\t\t\t<div>target: <input type = 'text' name = 'target' size='50' value = ''></div>\n";
   std::cout << "\t\t\t<br>\n";
   if (process == "state")
      std::cout << "\t\t\t<div><label><input type='radio' name='option' value='state' checked>search by state</label>";
   else
      std::cout << "\t\t\t<div><label><input type='radio' name='option' value='state'>search by state</label>";
  
   if (process == "capital") 
       std::cout << " <label><input type='radio' name='option' value='capital' checked>search by capital</label></div>\n";
   else
      std::cout << " <label><input type='radio' name='option' value='capital'>search by capital</label></div>\n";
   
   if (process == "state_list")
       std::cout << "\t\t\t<div><label><input type='radio' name='option' value='state_list' checked>list alphabetically by state</label>";
   else
      std::cout << "\t\t\t<div><label><input type='radio' name='option' value='state_list'>list alphabetically by state</label>";
   
   if (process == "capital_list")
      std::cout << " <label><input type='radio' name='option' value='capital_list' checked>list alphabetically by capital</label></div>\n";
   else
      std::cout << " <label><input type='radio' name='option' value='capital_list'>list alphabetically by capital</label></div>\n";
  
   if (process == "pop")
      std::cout << "\t\t\t<div><label><input type='radio' name='option' value='pop' checked>list by population ascending</label>";
   else 
      std::cout << "\t\t\t<div><label><input type='radio' name='option' value='pop'>list by population ascending</label>";
   
   if (process == "pop_reverse")
      std::cout << " <label><input type='radio' name='option' value='pop_reverse' checked>list by population descending</label></div>\n";
   else
      std::cout << " <label><input type='radio' name='option' value='pop_reverse'>list by population descending</label></div>\n";
   
   if (process == "area")
      std::cout << "\t\t\t<div><label><input type='radio' name='option' value='area' checked>list by area ascending</label>";
   else
      std::cout << "\t\t\t<div><label><input type='radio' name='option' value='area'>list by area ascending</label>";
   
   if (process == "area_reverse")
      std::cout << " <label><input type='radio' name='option' value='area_reverse' checked>list by area descending</label></div>\n";
   else
      std::cout << " <label><input type='radio' name='option' value='area_reverse'>list by area descending</label></div>\n";
   
   std::cout << "\t\t\t<br>\n";
   std::cout << "\t\t\t<div><input type='submit' name='search' value='submit'></div>\n";
   std::cout << "\t\t</form>\n";
   std::cout << "\t\t<br>\n";
}
