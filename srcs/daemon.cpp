#include "../includes/matt_daemon.hpp"

Tintin_logger logger;

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
	listen(sock, 1);
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

void	handle_connection(int cs, int socket)
{
	char buf[1024];
	t_mem *mem;
	std::stringstream s;

	buf[1023] = '\0';
	logger.log("got connection");
	while (42)
	{
		mem = read_fd(cs);
		s << "user input" << mem->data;
		logger.log(s.str());
	}
}


int	main(void)
{
	pid_t child;
	int fd;
	struct sockaddr_in6		sin;
	socklen_t			*sizesin;
	int cs;
	int socket;

	logger.info("Started.");

	fd = open("/var/lock/matt_daemon.lock", O_CREAT | O_EXCL);
	if (fd == -1)
	{
		logger.error("Error file locked");
		std::cerr << "Can't open :/var/lock/matt_daemon.lock\n";
		exit(-1);
	}
	flock(fd, LOCK_EX);
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
			socket = ft_create_serveur();
			std::stringstream s;
			s << "started. PID:" <<std::to_string(getpid());
			logger.info(s.str());
			logger.info("Entering daemon mode");
			while (42)
			{
				cs = accept(socket, (struct sockaddr*)&sin, sizesin);
				logger.info("accepted connection");
				handle_connection(cs, socket);
			}
		}
		else
			exit(-1);
	}
	else
		exit(-1);
	return (0);
}
