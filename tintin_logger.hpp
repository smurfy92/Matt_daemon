#include <iostream>
#include <sstream>
class Tintin_logger
{
	public:
		std::string name;
//		ofstream file;
		Tintin_logger();
		Tintin_logger(Tintin_logger const &toto);
		~Tintin_logger();
		void	log(std::string toto);
		void	info(std::string toto);
	
	private:
		std::string getdate();
};
