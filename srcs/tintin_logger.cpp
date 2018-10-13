#include "../includes/tintin_logger.hpp"

Tintin_logger::Tintin_logger(void){
	this->name = "Matt_daemon: ";
	system("mkdir -p /var/log/Matt_daemon");
}
Tintin_logger::Tintin_logger(Tintin_logger const &toto){
}
Tintin_logger::~Tintin_logger(void){}

void	Tintin_logger::log(std::string toto){
	this->file.open("/var/log/Matt_daemon/Matt_daemon.log", std::ios_base::app);
	this->file << this->getdate() << " [ LOG ] - "<< this->name << toto << '\n';
	this->file.close();
}
void	Tintin_logger::info(std::string toto){
	this->file.open("/var/log/Matt_daemon/Matt_daemon.log", std::ios_base::app);
	this->file << this->getdate() << " [ INFO ]" << this->name << toto << '\n';
	this->file.close();
}
void	Tintin_logger::error(std::string toto){
	this->file.open("/var/log/Matt_daemon/Matt_daemon.log", std::ios_base::app);
	this->file << this->getdate() << " [ ERROR ]" << this->name << toto << '\n';
	this->file.close();
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
