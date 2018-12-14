#ifndef STATES_H
#define STATES_H

const unsigned RECORD_LENGTH = 120;

enum { dummy, state, capital, flag, population, area, postal, admittance, 
   electoral_votes, largest_city };

class Record {
   public:
      std::string getState();
      std::string getCapital();
      std::string getFlag();
      std::string getPop();
      std::string getArea();
      std::string getPostal();
      std::string getDOA();
      std::string getEV();
      std::string getCity();
      std::string getTableRow();
   private:
      char buf[RECORD_LENGTH];
      std::string getfield(unsigned);
};

Record* binarysearch(const std::string&);
Record* linearsearch(const std::string&);

#endif
