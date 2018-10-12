#include <iostream>
#include <sstream>
#include <fstream>
class Tintin_logger
{
	public:
		std::string name;
		std::ofstream file;
		Tintin_logger();
		Tintin_logger(Tintin_logger const &toto);
		~Tintin_logger();
		void	log(std::string toto);
		void	info(std::string toto);
	
	private:
		std::string getdate();
};
