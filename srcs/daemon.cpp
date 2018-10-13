#include "../includes/matt_daemon.hpp"

Tintin_logger logger;

int		ft_create_serveur()
{
	int					sock;
	struct protoent		*p;
	struct sockaddr_in6	sin;

	p = getprotobyname("tcp");
	logger.info("Creating server");
	if (p == 0)
	{
		logger.error("proto error");
		exit(-1);
	}
	sock = socket(AF_INET6, SOCK_STREAM, p->p_proto);
	sin.sin6_family = AF_INET6;
	sin.sin6_port = htons(4242);
	sin.sin6_addr = in6addr_any;
	if (bind(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		logger.error("bind error");
		exit(-1);
	}
	logger.info("Server created");
	listen(sock, 42);
	return (sock);
}

void	handle_sigs(int signum)
{

	std::stringstream s;
	s << "received signal :" << std::to_string(signum);
	std::cout << "coucou";
	logger.log(s.str());
}

void	set_sigs()
{
	int i;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = handle_sigs;

	i = -1;
	while (++i < 32)
		sigaction(i,  &act, 0);
}


int	main(void)
{
	pid_t child;

	logger.info("Started.");
	child = fork();
	set_sigs();
	if (child == 0)
	{
		chdir("/");
		setsid();
		close(0);
		close(1);
		ft_create_serveur();
		std::stringstream s;
		s << "started. PID:" <<std::to_string(getpid());
		logger.log(s.str());
		while (42);
	}
	else
		exit(-1);
	return (0);
}
