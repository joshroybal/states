// #include <iostream>
#include <fstream>
#include <cstring>

int main()
{
   std::ifstream ifstr;
   ifstr.open("states.csv", std::ios::in);
   std::ofstream ofstr;
   ofstr.open("states.dat", std::ios::out | std::ios::binary);
   char buf[120];
   memset(buf, '\0', sizeof(buf));
   ifstr.getline(buf, sizeof(buf));
   while ( (ifstr.getline(buf, sizeof(buf))) != 0) {
      ofstr.write(buf, sizeof(buf));
      memset(buf, '\0', sizeof(buf));
   }
   ifstr.close();
   ofstr.close();
   return 0;
}
