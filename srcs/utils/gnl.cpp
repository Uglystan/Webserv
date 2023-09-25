#include "gnl.hpp"

int    ft_strchr(char *temp, char c)
{
    int    i;

    i = 0;
    while (temp[i] != '\0')
    {
        if (temp[i] == c)
            return (1);
        i++;
    }
    return (0);
}

int    ft_strlen(char *str)
{
    int    i;
    
    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

int    size_no_nl(char *temp, char c)
{
    int    i;
    int    j;

    i = 0;
    j = 0;
    while (temp[i] == -1)
        i++;
    while (temp[i] != c)
    {
        i++;
        j++;
    }
    return (j);
}

void    init_temp(char * temp)
{
    int    i;

     i = 0;
     while (temp[i] != '\n' && temp[i] != '\0')
    {
        temp[i] = -1;
        i++;
    }
    if (temp[i] == '\n')
        temp[i] = -1;
}

char    *join(char *buff, char *temp)
{
    int    i;
    int    j;
    char    *new_buff;

    i = 0;
    j = 0;
    new_buff = static_cast<char*>(malloc(sizeof(char) * (ft_strlen(buff) + size_no_nl(temp, '\0') + 1)));
    while (buff[j] != '\0')
    {
        new_buff[j] = buff[j];
        j++;
    }
    while (temp[i] == -1)
        i++;
    while (temp[i] != '\0' && temp[i] != -1)
    {
        new_buff[j] = temp[i];
        i++;
        j++;
    }
    new_buff[j] = '\0';
    free(buff);
    init_temp(temp);
    return (new_buff);
    
}

char    *newline(char *buff, char *temp)
{
    int    i;
    int    j;
    char    *new_buff;

    i = 0;
    j = 0;
    new_buff = static_cast<char*>(malloc(sizeof(char) * (ft_strlen(buff) + size_no_nl(temp, '\n') + 2)));

    while (buff[j] != '\0')
    {
        new_buff[j] = buff[j];
        j++;
    }
    while (temp[i] == -1)
        i++;
    while (temp[i] != '\n')
    {
        new_buff[j] = temp[i];
        i++;
        j++;
    }
    new_buff[j] = '\n';
    j++;
    new_buff[j] = '\0';
    free(buff);
    init_temp(temp);
    return (new_buff);
}

int    only_one(char *temp)
{
    int    i;

    i = 0;
    while (temp[i] != '\0')
    {
        if (temp[i] != -1)
            return (1);
        i++;
    }
    return (0);
}

char    *get_next_line(int fd)//temp premier cara /n pb
{
    static char    temp[BUFFER_SIZE + 1];
    char    *line;
    
    if (fd == -1 || BUFFER_SIZE <= 0)
        return (NULL);
    line = static_cast<char*>(malloc(sizeof(char) * 1));
    line[0] = '\0';
    while (1)
    {
        if (only_one(temp) == 0)
            if (read(fd, temp, BUFFER_SIZE) == 0)
                break ;
        if (ft_strchr(temp, '\n') == 0)//pas de \n
            line = join(line, temp);
        else if (ft_strchr(temp, '\n') == 1)//\n
        {
            line = newline(line, temp);
            break;
        }
    }
    if (line[0] == '\0')
    {
        free(line);
        return (NULL);
    }
    return (line);
}
