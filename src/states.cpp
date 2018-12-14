#include <fstream>
#include <cstring>
#include "states.hpp"

// Record class public method definitions
std::string Record::getState()
{
   return getfield(state);
}

std::string Record::getCapital()
{
   return getfield(capital);
}

std::string Record::getFlag()
{
   return getfield(flag);
}

std::string Record::getPop()
{
   return getfield(population);
}

std::string Record::getArea()
{
   return getfield(area);
}

std::string Record::getPostal()
{
   return getfield(postal);
}

std::string Record::getDOA()
{
   return getfield(admittance);
}

std::string Record::getEV()
{
   return getfield(electoral_votes);
}

std::string Record::getCity()
{
   return getfield(largest_city);
}

// two very different ways to do it
std::string Record::getTableRow()
{
   return "\t\t\t<tr><td>" + getState() + "</td><td>" + getCapital() 
      + "</td><td>" + getFlag() + "</td><td>" + getPop() + "</td><td>" 
      + getArea() + "</td><td>" + getPostal() + "</td><td>" + getDOA() 
      + "</td><td>" + getEV() + "</td><td>" + getCity() + "</td></tr>\n";
   /*
   std::string row = "\t\t\t<tr>";
   for (int fldno = 1; fldno <= 9; ++fldno) {
      row += "<td>";
      row += getfield(fldno);
      row += "</td>";
   }
   row += "</tr>\n";
   return row;
   */
}

// Record class private method definitions
std::string Record::getfield(unsigned n)
{
   unsigned i = 0, j = 0, buflen = strlen(buf);
   std::string field;

   while (i < buflen) {
      while (buf[i] != ',' && i < buflen)
         if (buf[i] != '\"')
            if (j + 1 == n)
               field += buf[i++];
            else
               i++;
         else {
            i++;
            while (buf[i] != '\"') {
               if (j + 1 == n)
                  field += buf[i++];
               else
                  i++;
            }
            i++;
         }
      if (j + 1 == n) return field;
      i++;
      j++;
   }
   field.clear();
   return field;
}

// auxiliary utility function definitions
Record* binarysearch(const std::string& target)
{
   static Record record;
   std::ifstream ifstr;
   ifstr.open("states.dat", std::ios::in | std::ios::binary);
   ifstr.seekg(0, std::ios::end);
   int n = ifstr.tellg() / sizeof(Record);
   int m, l = 0, r = n - 1;

   ifstr.seekg(0, std::ios::beg);   // reset file pointer
   while (l <= r) {
      m = (l + r) / 2;
      ifstr.seekg(m * sizeof(Record), std::ios::beg);
      ifstr.read((char *)&record, sizeof(Record));
      std::string field = record.getState();
      if (target < field)
         r = m - 1;
      else if (target > field)
         l = m + 1;       
      else {
         ifstr.close();         
         return &record;
      }
   }
   return 0;
}

Record* linearsearch(const std::string& target)
{
   static Record record;
   std::ifstream ifstr;
   ifstr.open("states.dat", std::ios::in | std::ios::binary);
   while ( ( ifstr.read((char *)&record, sizeof(Record)) ) != 0 ) {
      std::string field = record.getCapital();
      if (target == field) return &record;
   }
   ifstr.close();   
   return 0;
}
