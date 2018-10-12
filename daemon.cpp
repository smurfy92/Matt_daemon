#include "matt_daemon.hpp"

int		ft_create_serveur()
{
	int					sock;
	struct protoent		*p;
	struct sockaddr_in6	sin;

	p = getprotobyname("tcp");
	if (p == 0)
	{
		std::cout << "connection error";
		exit(-1);
	}
	sock = socket(AF_INET6, SOCK_STREAM, p->p_proto);
	sin.sin6_family = AF_INET6;
	sin.sin6_port = htons(4242);
	sin.sin6_addr = in6addr_any;
	if (bind(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		std::cout << "port already in use";
		exit(-1);
	}
	listen(sock, 42);
	return (sock);
}


int	main(void)
{
	pid_t child;
	Tintin_logger logger;

	child = fork();
	if (child == 0)
	{
		std::stringstream s;
		s << "started. PID:" <<std::to_string(getpid());
		logger.log(s.str());
		ft_create_serveur();
		while (42);
	}
	else
		exit(-1);
	return (0);
}
