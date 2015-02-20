#include "Hexapod.h"
#include "LegCtrl.h"
#include <string>

#include <iostream>
#include "little_helpers/gnublin.h"
int main() {

   try {
      LegCtrl lc("/dev/servodrive0");


      Hexapod h(50000,&lc);
	   h.Run();
    } catch(std::string const &e) {
        std::cout << "Exception caught: " << e << std::endl;
    } catch(char *e) {
        std::cout << "Exception caught: " << e << std::endl;
    }
    catch (char const* msg)
    {
        std::cout << msg;
    }


	return 0;
}


