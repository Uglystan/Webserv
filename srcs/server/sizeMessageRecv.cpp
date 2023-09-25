#include "../../include/serveur.hpp"

int	sizeBody(std::string buf)
{
    std::string finder = "Content-Length: ";

    size_t i = buf.find(finder);
    if (i == std::string::npos)
    {
        std::cerr << "La sous-chaîne n'a pas été trouvée." << std::endl;
        return -1; // ou une autre valeur pour indiquer une erreur
    }

    i += finder.length(); // Avancer au-delà de "Content-Length"

    // Concaténer les chiffres pour former la taille
    std::string size;
    while (isdigit(buf[i]))
    {
        size += buf[i];
        i++;
    }

    int taille = std::atoi(size.c_str()); // Convertir la chaîne en entier
    return taille;
}

int	sizeHeader(std::string buf)
{
	std::string finder = "\r\n\r\n";

	size_t i = buf.find(finder);
	i += finder.length();
	if (i == std::string::npos)
   	{
        	std::cerr << "La sous-chaîne n'a pas été trouvée." << std::endl;
        	return -1; // ou une autre valeur pour indiquer une erreur
	}
	return (i);
}

int	sizeMessage(char *buffer)
{
	std::string buf = buffer;
	int	size = 0;

	if (buf.find("POST") != std::string::npos)
	{
		if (sizeBody(buf) > MAX_CLIENT_BODYSIZE)
		{
			/*Error page 413*/
		}
		size = sizeHeader(buf) + sizeBody(buf);
		return (size);
	}
	else
		return (sizeHeader(buf));
}

int	sizeMessageChunk(char *buffer)
{
	std::stringstream stream(buffer);
	std::string	sizeMessage;
	int	i = 0;
	int	decimal;

	while (buffer[i] != '\n')
		i++;
	std::getline(stream, sizeMessage);
	std::stringstream line(sizeMessage);
	line << std::hex;
	line >> std::hex >> decimal;
	return (i + decimal);
}