# Webserv
fonctions:

execve : Exécute un programme spécifié par un chemin d'accès.

dup et dup2 : Dupliquent un descripteur de fichier (copie un fichier ouvert).

pipe : Crée un tube (pipe) pour la communication entre processus.

strerror : Retourne une chaîne de caractères décrivant l'erreur associée à un code d'erreur (errno).

gai_strerror : Convertit un code d'erreur retourné par getaddrinfo en une chaîne de caractères lisible.

errno : Variable globale contenant le code d' la dernière opération qui a échoué.

fork : Crée un nouveau processus (clone) à partir du processus parent.

socketpair : Crée une paire de sockets connectées pour la communication entre processus.

hton et ntoh : Convertissent les valeurs d'entiers entre l'ordre d'octets de l'hôte et l'ordre d'octets du réseau.

select, poll, epoll (avec epoll_create, epoll_ctl, epoll_wait) et kqueue (avec kqueue et kevent) : Mécanismes de gestion des entrées/sorties multiplexées, permettant de gérer efficacement de multiples descripteurs de fichiers.

socket : Crée un socket pour la communication réseau.

accept : Accepte une connexion entrante sur un socket.

listen : Met un socket en mode écoute pour les connexions entrantes.

send et recv : Envoient et reçoivent des données sur un socket.

chdir : Change le répertoire de travail courant.

bind : Associe un socket à une adresse IP et un port.

connect : Établit une connexion à une adresse distante à l'aide d'un socket.

getaddrinfo et freeaddrinfo : Résolvent des noms d'hôte en adresses IP et vice versa.

setsockopt : Configure des options pour un socket.

getsockname : Récupère l'adresse locale associée à un socket.

getprotobyname : Récupère les informations sur un protocole réseau par son nom.

fcntl : Contrôle des opérations sur les descripteurs de fichiers, y compris la non-blocage.

close : Ferme un descripteur de fichier ou un socket.

read et write : Lisent et écrivent des données depuis et vers un descripteur de fichier.

waitpid : Attends qu'un processus enfant se termine et récupère son statut.

kill : Envoie un signal à un processus.

signal : Associe une fonction de gestionnaire de signal à un signal donné.

access : Vérifie l'accès à un fichier en fonction des permissions.

stat : Obtient des informations sur un fichier, comme sa taille, son propriétaire, etc.

opendir, readdir et closedir : Ouvrent un répertoire, lisent ses entrées et le ferment.

