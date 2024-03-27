#include "server.hpp"
#include "../Commands/Commands.hpp"
#include <unistd.h>


// void Server::CommandMapinit()
// {
	//commands 
	//than loop to insert commands on the map
// }

// void Server::SetClientNickName(int fd, std::string &nickname)
// {

// }



void Server::CheckForConnectionClients()
{
	char buffer[1024];

	for (size_t i = 1; i < _Storeusersfd.size(); i++)
	{
		if (_Storeusersfd.at(i).revents & POLLIN)
		{
			bzero(buffer, sizeof(buffer));
			std::cout << _Storeusersfd.at(i).fd << std::endl;
			int recive = recv(_Storeusersfd.at(i).fd, buffer, sizeof(buffer), 0);
			std::cout << recive << std::endl;
			// puts(buffer);
			if (recive > 0)
			{
				//data Received
				// std::cout << "goooo gooo gooo" << std::endl;
				std::cout << "user to create : " << _Storeusersfd[i].fd << std::endl;
        		User *User = _db->getUser(_Storeusersfd[i].fd);
				if (User->isAuthenticated() == false)
					Authentication(i, buffer);
				// Authentication(i, buffer, false, false, false, false);
					// std::cout << "Wrong Password" << std::endl;
			}
			else
			{
				if (recive == 0)
				{
					std::cout << "Client DISCONNECTED." << std::endl;
				}
				else
				{
					std::cout << "Unexpected Error Of losing connection." << std::endl;
				}
				_db->deleteUser(_Storeusersfd.at(i).fd);
				close(_Storeusersfd.at(i).fd); // Close Socket
				_Storeusersfd.erase(_Storeusersfd.begin() + i); // Remove Poll Set on Vector
				i--; //Cerrection index ater Removal
				// puts("jj");
			}
		}
	}
}



Server::Server(const int &port, const std::string &password) : _Port(port), _Password(password)//, _IsAuth(false), _correct_pass(false), _NickCheck(false), _UserCheck(false)
{
	_db = Database::GetInstance();
}

// #define MAX_CLIENTS _Storeusersfd.size() // Assume MAX_CLIENTS + 1 for the server socket
std::string Server::HostIPADress()
{

	std::string iphost;
	std::istringstream string(std::system("ifconfig | grep 'inet ' | awk 'NR==2 {print $2}' > .log"));
	std::fstream ipfile;
	ipfile.open(".log");
	std::getline(ipfile, iphost);
	std::system("rm -rf .log");
	// string >> iphost;
	return iphost;
}

void Server::ServerStarting()
{
	struct pollfd srvpollfd;

	_IPHostAdress = HostIPADress(); // take IP Host of Machine
	std::cout << "dd: " << _IPHostAdress << std::endl;
	createSockets();
	setSocketsopt();
	listtenSock();
	srvpollfd.fd = _Socketsfd;
	srvpollfd.events = POLLIN;
	srvpollfd.revents = 0;
	_Storeusersfd.push_back(srvpollfd);
	// std::cout << &_Socketsfd << std::endl;
	
	while(1)
	{
		// std::cout << _Storeusersfd[0].fd << std::endl;
		// Wait indefinitely for an event
		int ret = poll(&_Storeusersfd[0], _Storeusersfd.size(), -1); // Assume MAX_CLIENTS + 1 for the server socket
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
			// std::cout << "waaaa laaaaa" << std::endl;
			CheckForConnectionClients();
			// puts("here");
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
	// to set a socket to non-blocking mode
	if (fcntl(_Socketsfd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error: Failed to  Manipulating File Descriptors." << std::endl;
		exit (EXIT_FAILURE);
	}
	_Sockaddsrv.sin_family = AF_INET;
	_Sockaddsrv.sin_port = htons(_Port);
	//need for connaitre le du serveur port
	// htons return a number of 16bits dans l'ordre octet utilise dans les reseau TCP/IP
	//htons()  s = Short= La fonction htons peut être utilisée pour convertir 
		// un numéro de port IP dans l’ordre d’octet hôte en numéro de port IP dans l’ordre d’octet réseau.
	_Sockaddsrv.sin_addr.s_addr =  INADDR_ANY; //or htonl(127.0.0.1)
	// htonl() = convert 32bits dans un ordre utiliser dans le reseau TCP/IP
	// htonl() = l (long) utiliser pour connetre IP (localhost)
	if (bind(_Socketsfd, (sockaddr *)&_Sockaddsrv, sizeof(_Sockaddsrv)))
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
}
void Server::accept_connection()
{
	//Accept Request Connection
	// std::cout << _Socketsfd << std::endl;
	size_t lensockadd = sizeof(_Sockaddclient);
	// memset(&_Sockaddclient, 0, lensockadd);
	int newSocketfd = accept(_Socketsfd, (sockaddr *)&_Sockaddclient, (socklen_t *)&lensockadd);
	std::cout << newSocketfd << std::endl;
	if (newSocketfd >= 0)
	{
		//inet_ntoa() only supports IPv4 addresses. 
		// For software that needs to be compatible with IPv6, inet_ntop() is the preferred alternative.
		std::string convertlocalhost(inet_ntoa(_Sockaddclient.sin_addr)); //It converts an Internet host address, given in network byte order (which is typically a numeric IP address in binary form), 
								// into a standard dot-decimal notation string (the familiar IPv4 address format, such as "192.168.1.1").

		_pollfds.fd = newSocketfd; // File descriptor to monitor (fd li ghanra9bo)
		_pollfds.events = POLLIN; // Events to monitor for this fd (Les events li khasna nra9boha l had fd)
		// POLLIN = That There's data to read.
		_pollfds.revents = 0; // Events that occurred on this fd (Les events li traw fe had fd)
		_Storeusersfd.push_back(_pollfds);
		// _ConnectedUser.insert(std::pair<newSocketfd, (Name)>);
		User *user = new User(_pollfds.fd);
		_db->addNewUser(user);
		// _db->getUser(_pollfds.fd);
		// std::cout << _db << std::endl;

		// _db = Database::GetInstance();
		// std::cout << &_db << std::endl;
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