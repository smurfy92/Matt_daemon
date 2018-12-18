#include "../includes/matt_daemon.hpp"

Tintin_reporter		logger;
int			connections[3] = {-1, -1, -1};

void	init_daemon(t_daemon *daemon)
{
	daemon->lock_file = 0;
	daemon->sock = 0;
}

t_daemon		*get_daemon(void)
{
	static t_daemon *daemon = NULL;

	if (!daemon)
	{
		daemon = (t_daemon*)malloc(sizeof(t_daemon));
		init_daemon(daemon);		
	}
	return (daemon);
}

void	ft_free_mem(t_mem *mem)
{
	ft_strdel(&mem->data);
	free(mem);
}

t_mem	*ft_memjoin(t_mem *dest, t_mem *src)
{
	t_mem *ret;

	ret = (t_mem *)malloc(sizeof(t_mem));
	if (!dest)
	{
		ret->data = ft_strnew(src->len);
		ft_memcpy(ret->data, src->data, src->len);
		ret->len = src->len;
		return (ret);
	}
	ret->len = dest->len + src->len;
	ret->data = ft_strnew(ret->len);
	ft_memcpy((void*)ret->data, dest->data, dest->len);
	ft_memcpy((void*)&ret->data[dest->len], src->data, src->len);
	ft_free_mem(dest);
	return (ret);
}

t_mem	*read_fd(int fd)
{
	t_mem	*mem;
	t_mem	*buf;

	mem = NULL;
	buf = (t_mem *)malloc(sizeof(t_mem));
	buf->data = ft_strnew(BUFFER + 1);
	buf->len = 0;
	while ((buf->len = read(fd, buf->data, BUFFER)))
	{
		mem = ft_memjoin(mem, buf);
		ft_bzero(buf->data, buf->len);
		if (buf->len < BUFFER)
			break ;
		buf->len = 0;
	}
	if (!mem)
		return (buf);
	ft_free_mem(buf);
	return (mem);
}

int		ft_create_serveur(t_daemon *daemon)
{
	int					sock;
	struct protoent		*p;
	struct sockaddr_in6	sin;
	int toto = 1;

	p = getprotobyname("tcp");
	logger.info("Creating server");
	if (p == 0)
	{
		logger.error("proto error");
		ft_exit(daemon, -1);
	}
	sock = socket(AF_INET6, SOCK_STREAM, p->p_proto);
	if (setsockopt(daemon->sock, SOL_SOCKET, SO_REUSEADDR, &toto, sizeof(int)) < 0)
	{
		logger.error("setsockopt failed");
		ft_exit(daemon, -1);
	}
	sin.sin6_family = AF_INET6;
	sin.sin6_port = htons(4242);
	sin.sin6_addr = in6addr_any;
	if (bind(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		logger.error("bind error");
		ft_exit(daemon, -1);
	}
	logger.info("Server created");
	listen(sock, 42);
	return (sock);
}

void	handle_sigs(int signum)
{
	t_daemon *daemon;

	daemon = get_daemon();
	std::stringstream s;
	s << "received signal :" << std::to_string(signum);
	logger.log(s.str());
	ft_exit(daemon, -1);
}

void	toto(int signum)
{
	t_daemon *daemon;

	daemon = get_daemon();
	logger.info("received KILL signal");
	ft_exit(daemon, -1);
}

void	set_sigs()
{
	int i;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = handle_sigs;
	signal(SIGKILL, toto);

	i = -1;
	while (++i < 32)
		sigaction(i,  &act, 0);
}

void	ft_exit(t_daemon *daemon, int status)
{
	if (daemon->lock_file > 0)
		flock(daemon->lock_file, LOCK_UN);
	unlink("/var/lock/matt_daemon.lock");
	logger.info("quitting");
	close(daemon->sock);
	exit(status);
}

void	handle_connection(t_daemon *daemon, int cs)
{
	t_mem *mem;
	std::stringstream s;

	logger.log("got connection");
	while (42)
	{
		mem = NULL;
		mem = read_fd(cs);
		if (mem == NULL || mem->len == 0)
		{
			connections[cs] = -1;
			close(cs);
		}
		mem->data[mem->len - 1] = '\0';
		s << "User input : " << mem->data;
		logger.log(s.str());
		s << "User input len : " << mem->len;
		logger.log(s.str());
		if (ft_strequ(mem->data, "quit") == 1)
		{
			close(cs);
			connections[cs] = -1;
			ft_exit(daemon, -1);
		}
		ft_free_mem(mem);
		s.str("");
	}
}

void	open_lock(t_daemon *daemon)
{
	daemon->lock_file = open("/var/lock/matt_daemon.lock", O_CREAT | O_EXCL);
	if (daemon->lock_file == -1)
	{
		logger.error("Error file locked");
		std::cerr << "Can't open :/var/lock/matt_daemon.lock\n";
		exit(-1);
	}
	flock(daemon->lock_file, LOCK_EX);
}

int	check_connections()
{
	int i = -1;

	while (++i < 3)
		if (connections[i] == -1)
			return (i);
	return (-1);
}


int	main(void)
{
	pid_t child;
	int fd;
	struct sockaddr_in6		sin;
	unsigned int			sizesin;
	int cs;
	int socket;
	t_daemon		*daemon;
	int tmp;

	logger.info("Started.");

	daemon= get_daemon();
	open_lock(daemon);
	child = fork();
	set_sigs();
	if (child == 0)
	{
		setsid();
		child = fork();
		if (child == 0)
		{
			chdir("/");
			close(0);
			close(1);
			close(2);
			daemon->sock = ft_create_serveur(daemon);
			std::stringstream s;
			s << "started. PID:" <<std::to_string(getpid());
			logger.info(s.str());
			logger.info("Entering daemon mode");
			while (42)
			{
				cs = accept(daemon->sock, (struct sockaddr*)&sin, &sizesin);
				tmp = check_connections();
				if (tmp != -1)
				{
					connections[tmp] = cs;
					child = fork();
					if (child == 0)
					{
						s << "accepting connections number " << tmp;
						logger.info(s.str());
						logger.info("accepted connection");
						handle_connection(daemon, cs);
					}
				}
				else
				{
					ft_putstr_fd("Sorry to many connections\n", cs);
					logger.error("too many connections");
					close(cs);
				}
			}
		}
		else
			exit(-1);
	}
	else
		exit(-1);
	return (0);
}
