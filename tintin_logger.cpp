#include "tintin_logger.hpp"

Tintin_logger::Tintin_logger(void){
	this->name = "Matt_daemon: ";
	system("mkdir -p /var/log/Matt_daemon");
	this->file.open("/var/log/Matt_daemon/Matt_daemon.log");
}
Tintin_logger::Tintin_logger(Tintin_logger const &toto){
}
Tintin_logger::~Tintin_logger(void){}

void	Tintin_logger::log(std::string toto){
	std::cout << this->getdate() << " [ LOG ] - "<< this->name << toto << '\n';
}
void	Tintin_logger::info(std::string toto){
	std::cout << this->getdate() << " [ INFO ]" << this->name << toto << '\n';
}

std::string Tintin_logger::getdate(){
	time_t t = time(0);
	tm* now = localtime(&t);
	std::stringstream os;
	os << '[' <<  std::to_string(now->tm_mday) << '/' <<std::to_string(now->tm_mon + 1) << '/'
		<< std::to_string(now->tm_year + 1900) << '-' << 
		std::to_string(now->tm_hour) << ':' << std::to_string(now->tm_min) << ':' <<std::to_string(now->tm_sec)
		<< ']';
	return os.str();
}
