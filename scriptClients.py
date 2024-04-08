# import socket
# import threading
# import signal


# # Informations de connexion au serveur IRC
# server_ip = "10.13.4.6"
# # server_ip = "localhost"
# server_port = 9090
# # server_password = "2023"
# server_password = "pp"

# # Liste de pseudos disponibles
# available_nicks = []


# for i in range(0, 5000):
#     nick = "client_bot{}".format(i)
#     available_nicks.append(nick)

# def connect_irc(nick):
#     irc_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#     irc_socket.connect((server_ip, server_port))

#     irc_socket.send("PASS {}\r\n".format(server_password).encode())

#     irc_socket.send("NICK {}\r\n".format(nick).encode())
#     irc_socket.send("USER {} 0 * :Client Bot\r\n".format(nick).encode())
#     # irc_socket.send("JOIN #teee\r\n".format(nick).encode())

#     while True:
#         # signal.signal(signal.SIGPIPE, signal.SIG_IGN)
#         message = irc_socket.recv(2048).decode()
#         print(message)

# for nick in available_nicks:
#     thread = threading.Thread(target=connect_irc, args=(nick,))
#     thread.start()

import irc.client
import irc.events
import threading
import time

SERVER = '10.13.4.6'
PORT = 9090
PASSWORD = 'pp'
CHANNELS = ['#channel1', '#channel2', '#channel11', '#channel22','#channel111', '#channel222','#channel1111', '#channel2222']  # Channels to join
NICKNAME_BASE = 'Bot'  # Base nickname; will append numbers to this
USERNAME_BASE = 'Ag'  # Base username; will append numbers to this
CLIENT_COUNT = 1000  # Number of clients to connect

def on_welcome(connection, event):
    for channel in CHANNELS:
        connection.join(channel)

def on_join(connection, event):
    channel = event.target
    connection.mode(channel, "+o {}".format(connection.get_nickname()))

def connect_client(client_id):
    nickname = "{}{}".format(NICKNAME_BASE, client_id)
    username = "{}{}".format(USERNAME_BASE, client_id)

    reactor = irc.client.Reactor()
    try:
        connection = reactor.server().connect(SERVER, PORT, nickname, password=PASSWORD, username=username)
    except irc.client.ServerConnectionError as exc:
        print("Connection failed: ", exc)
        return

    connection.add_global_handler("welcome", on_welcome)
    connection.add_global_handler("join", on_join)

    reactor.process_forever()

if __name__ == "__main__":
    for i in range(CLIENT_COUNT):
        threading.Thread(target=connect_client, args=(i+1,)).start()
        time.sleep(1)  # Stagger connections slightly