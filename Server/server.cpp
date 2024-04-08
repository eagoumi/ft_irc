#include "server.hpp"
#include "../Commands/Commands.hpp"
#include <unistd.h>
#include "../Logger/logger.hpp"


void Server::Quit(std::string reason)
{
	std::cout << "Nick : " << _User->getNickName() << std::endl;
	if (_User->getNickName().empty())
		_logger.IRCPrint(":*@localhost.IRC QUIT :Quit:" + reason);
	else if (reason.at(0) == ':')
		_logger.IRCPrint(":" + _User->getNickName() + "@" + "localhost.IRC " + "QUIT :Quit" + reason);
	else
		_logger.IRCPrint(":" + _User->getNickName() + "@" + "localhost.IRC " + "QUIT :Quit:" + reason);
	_logger.IRCPrint("ERROR: Quit:" + reason);
	//send Message to all channels
	std::cout << "Client DISCONNECTED." << std::endl;
}


void Server::CheckForConnectionClients()
{
	char buffer[1024];
	Commands cmdObj;
	cmdData data;
	for (size_t i = 1; i < _Storeusersfd.size(); i++)
	{
		if (_Storeusersfd.at(i).revents & POLLIN)
		{
			bzero(buffer, sizeof(buffer));
			int recive = recv(_Storeusersfd.at(i).fd, buffer, sizeof(buffer), 0);
			std::cout << recive << std::endl;
			std::string cmdquit; // PROBLEM HERE
			std::istringstream QUITCMD(buffer); // PROBLEM HERE
        	User *client = _db->getUser(_Storeusersfd[i].fd);
			client->appendToCmdLine(buffer);
			std::string cmdLine = client->getCmdLine();
			// std::cout << "cmdLine (" + client->getNickName() + ") : [\n\t" << cmdLine << "\n]" << std::endl; // PROBLEM HERE
			if (recive > 0)
			{
				if (cmdLine.find('\n') == std::string::npos)
					continue;
				//data Received
				data.fd = _Storeusersfd[i].fd;
				data.line = buffer; // PROBLEM HERE
				data.serverPass = _Password;
				_logger.setCurrUser(client);
				QUITCMD >> cmdquit; // PROBLEM HERE
				if (cmdquit == "QUIT" || cmdquit == "quit")
				{
					std::string reason = buffer;
					size_t C = reason.find(" ");
					if (C != std::string::npos)
						reason = reason.substr(C + 1, reason.length());
					QUITCMD >> reason;
					Quit(reason);
					//================================Send Notice To All Channel Joined =========//
					std::map<std::string, Channel *> JoinedCh = client->getJoinedChannels();
					if (!JoinedCh.empty())
					{
						std::map<std::string, Channel *>::iterator itJC = JoinedCh.begin();
						_logger.SendJoinedMembers(itJC->second , "QUIT :Quit" + reason);
					}
					//===========================================================================//

					_db->deleteUser(_Storeusersfd.at(i).fd);
					close(_Storeusersfd.at(i).fd);
					_Storeusersfd.erase(_Storeusersfd.begin() + i);
					i--;
				}
				else 
				{
					std::string line;
					std::istringstream bufferStream(cmdLine);
					while (std::getline(bufferStream, line, '\n')) 
					{
						size_t pos;
						if ((pos = line.find('\r')) != std::string::npos)
							line.erase(pos);
						data.line = line;
						std::cout << data.line << std::endl;
						cmdObj.CommandMapinit(data);
						line.clear();
					}
					client->clearCmdLine();
				}
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
			}
		}
	}
}

Server::Server(const int &port, const std::string &password) : _logger(Logger::GetInstance()), _Port(port), _Password(password)
{
	_db = Database::GetInstance();
}

std::string Server::HostIPADress()
{

	std::string iphost;
	std::system("ifconfig | grep 'inet ' | awk 'NR==2 {print $2}' > .log");
	std::fstream ipfile;
	ipfile.open(".log");
	std::getline(ipfile, iphost);
	std::system("rm -rf .log");

	return iphost;
}

void Server::ServerStarting()
{
	struct pollfd srvpollfd;

	_IPHostAdress = HostIPADress(); // take IP Host of Machine
	_logger.setServerIp(_IPHostAdress);
	std::cout << "dd: " << _IPHostAdress << std::endl;
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
		int ret = poll(&_Storeusersfd[0], _Storeusersfd.size(), -1);
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
			CheckForConnectionClients();
			//check for if the connection was lost or some error for connection from the clients
	   _db->debug();
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
	size_t lensockadd = sizeof(_Sockaddclient);
	// memset(&_Sockaddclient, 0, lensockadd);
	int newSocketfd = accept(_Socketsfd, (sockaddr *)&_Sockaddclient, (socklen_t *)&lensockadd);
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
		_User = new User(newSocketfd);
		_db->addNewUser(_User);
		// _db->getUser(_pollfds.fd);
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