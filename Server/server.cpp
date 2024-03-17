#include "server.hpp"


void Server::CommandMapinit()
{
	//commands 
	//than loop to insert commands on the map
}

Server::Server(const int &port, const std::string &password) : _Port(port), _Password(password)
{
	// std::cout << "Here is Agoumi Before :" << std::endl;
	// std::cout << _Port << std::endl;
	// std::cout << _Password << std::endl;
	// std::cout << _Sockadd.sin_addr.s_addr << std::endl;
	// std::cout << _Sockadd.sin_port << std::endl;
	// std::cout << "ip : " << INADDR_ANY << std::endl;
	// createSockets();
	// setSocketsopt();
	// listtenSock();
	// std::cout << _Port << std::endl;
	// std::cout << _Password << std::endl;
	// std::cout << _Sockadd.sin_addr.s_addr << std::endl;
	// std::cout << _Sockadd.sin_port << std::endl;
	// std::cout << "ip : " << INADDR_ANY << std::endl;
	// std::cout << "Here is Agoumi" << std::endl;
	CommandMapinit();
}

#define MAX_CLIENTS _Storeusersfd.size() // Assume MAX_CLIENTS + 1 for the server socket

void Server::ServerStarting()
{
	struct pollfd srvpollfd;

	_IPHostAdress = std::system("ifconfig | grep 'inet ' | awk 'NR==2 {print $2}'"); // take IP Host of Machine
	createSockets();
	setSocketsopt();
	listtenSock();
	srvpollfd.fd = _Socketsfd;
	srvpollfd.events = POLLIN;
	srvpollfd.revents = 0;
	_Storeusersfd.push_back(srvpollfd);
	while(1)
	{
		// Wait indefinitely for an event
	   int ret = poll(&_Storeusersfd[0], MAX_CLIENTS + 1, -1); // Assume MAX_CLIENTS + 1 for the server socket
	   if (ret < 0)
	   {
			std::cerr << "Error: Poll() Failed to Call System!!!!!!!!" << std::endl;
			return ;
	   }
	   if (_Storeusersfd[0].revents == POLLIN)
	   {
			std::cout << "Enjoy Dear New Client You just Now Connected, Welcome aboard!" << std::endl;
			accept_connection();
	   }
	   else
	   {
			std::cout << "waaaa laaaaa" << std::endl;
			//check for if the connection was lost or some error for connection from the clients
	   }
	}
}

void Server::createSockets()
{
	// AF_INET/AF_INET6 = Domain = integer type, communication domain, example = AF_INET6/AF_INET (IPv6/IPv4 protocol)
	// SOCK_STREAM/SOCK_DGRAM: UDP/TCP(unreliable, connectionless) 
	// 0 = Protocol = Protocol value for Internet Protocol(IP), which is 0. 
	//          This is the same number which appears on the protocol field in the IP header of a packet. (man protocols for more details)
	_Socketsfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_Socketsfd < 0)
	{
		std::cerr << "Error: Failed to create Sockets" << std::endl;
		exit (EXIT_FAILURE);
	}
}


// int setsockopt(int s, int level, int optname, char *optval, int optlen)
// s = The socket descriptor
// level = The level for which the option is being set <we use SOL_SOCKET>

void    Server::setSocketsopt()
{
	// INADDR_ANY = can take any IP address first was 0.0.0.0 than 127.0.0.1 localhost
	int enable_SO_REU = 1;
	if (setsockopt(_Socketsfd, SOL_SOCKET, SO_REUSEADDR, &enable_SO_REU, sizeof(int)) < 0)
	{
		std::cerr << "Error: Failed to Set Sockets opt" << std::endl;
		exit (EXIT_FAILURE);
	}
	_Sockadd.sin_family = AF_INET;
	_Sockadd.sin_port = htons(_Port);
	//need for connetre le du serveur port
	// htons return a number of 16bits dans l'ordre octet utilise dans les reseau TCP/IP
	//htons()  s = Short= La fonction htons peut être utilisée pour convertir 
		// un numéro de port IP dans l’ordre d’octet hôte en numéro de port IP dans l’ordre d’octet réseau.
	_Sockadd.sin_addr.s_addr =  INADDR_ANY; //or htonl(127.0.0.1)
	// htonl() = convert 32bits dans un ordre utiliser dans le reseau TCP/IP
	// htonl() = l (long) utiliser pour connetre IP (localhost)
	if (bind(_Socketsfd, (sockaddr *)&_Sockadd, sizeof(_Sockadd)))
	{
		std::cerr << "Error: failed to bind Sockets, Maybe Port was already used" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::listtenSock()
{
	if (listen(_Socketsfd, 32) < 0)
	{
		std::cerr << "Error: faild to listen Sockets" << std::endl;
		exit(EXIT_FAILURE);
	}
	// to set a socket to non-blocking mode
	if (fcntl(_Socketsfd, F_SETFD, O_NONBLOCK) < 0)
	{
		std::cerr << "Error: Failed to  Manipulating File Descriptors." << std::endl;
		exit (EXIT_FAILURE);
	}
}
void Server::accept_connection()
{

	//Accept Request Connection
	size_t lensockadd = sizeof(_Sockadd);
	int newSocketfd = accept(_Socketsfd, (sockaddr *)&_Sockadd, (socklen_t *)lensockadd);
	if (newSocketfd >= 0)
	{
		//inet_ntoa() only supports IPv4 addresses. 
		// For software that needs to be compatible with IPv6, inet_ntop() is the preferred alternative.
		std::string convertlocalhost(inet_ntoa(_Sockadd.sin_addr)); //It converts an Internet host address, given in network byte order (which is typically a numeric IP address in binary form), 
								// into a standard dot-decimal notation string (the familiar IPv4 address format, such as "192.168.1.1").
		struct pollfd pollfds;

		pollfds.fd = newSocketfd; // File descriptor to monitor (fd li ghanra9bo)
		pollfds.events = POLLIN; // Events to monitor for this fd (Les events li khasna nra9boha l had fd)
		// POLLIN = That There's data to read.
		pollfds.revents = 0; // Events that occurred on this fd (Les events li traw fe had fd)
		_Storeusersfd.push_back(pollfds);
		// _ConnectedUser.insert(std::pair<newSocketfd, (Name)>);
	}
	else if (newSocketfd < 0)
	{
		std::cerr << "Error: Failed to Connection Request. " << std::endl;
		exit(EXIT_FAILURE);
	}


}

Server::~Server()
{

}