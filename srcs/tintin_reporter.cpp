#include "../includes/tintin_reporter.hpp"

Tintin_reporter::Tintin_reporter(void){
	this->name = "Matt_daemon: ";
	system("mkdir -p /var/log/Matt_daemon");
}
Tintin_reporter::Tintin_reporter(Tintin_reporter const &toto){
}
Tintin_reporter::~Tintin_reporter(void){}

void	Tintin_reporter::log(std::string toto){
	this->file.open("/var/log/Matt_daemon/Matt_daemon.log", std::ios_base::app);
	this->file << this->getdate() << " [ LOG ]"<< this->name << toto << '\n';
	this->file.close();
}
void	Tintin_reporter::info(std::string toto){
	this->file.open("/var/log/Matt_daemon/Matt_daemon.log", std::ios_base::app);
	this->file << this->getdate() << " [ INFO ]" << this->name << toto << '\n';
	this->file.close();
}
void	Tintin_reporter::error(std::string toto){
	this->file.open("/var/log/Matt_daemon/Matt_daemon.log", std::ios_base::app);
	this->file << this->getdate() << " [ ERROR ]" << this->name << toto << '\n';
	this->file.close();
}

std::string Tintin_reporter::getdate(){
	time_t t = time(0);
	tm* now = localtime(&t);
	std::stringstream os;
	os << '[' <<  std::to_string(now->tm_mday) << '/' <<std::to_string(now->tm_mon + 1) << '/'
		<< std::to_string(now->tm_year + 1900) << '-' << 
		std::to_string(now->tm_hour) << ':' << std::to_string(now->tm_min) << ':' <<std::to_string(now->tm_sec)
		<< ']';
	return os.str();
}
