#include <iostream>
#include <sstream>
#include <fstream>

class Tintin_reporter
{
	public:
		std::string name;
		std::ofstream file;
		Tintin_reporter();
		Tintin_reporter(Tintin_reporter const &toto);
		~Tintin_reporter();
		void	log(std::string toto);
		void	info(std::string toto);
		void	error(std::string toto);
	
	private:
		std::string getdate();
};
