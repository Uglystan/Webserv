#include "CGIparsing.hpp"

std::string extractContentType(std::string &request)
{
	size_t contentTypePos = request.find("Content-Type:");
	if (contentTypePos != std::string::npos)
	{
		size_t lineEndPos = request.find("\r\n", contentTypePos);
		if (lineEndPos != std::string::npos)
		{
			std::string contentTypeLine = request.substr(contentTypePos, lineEndPos - contentTypePos);
			size_t colonPos = contentTypeLine.find(':');
			if (colonPos != std::string::npos)
			{
				std::string contentType = contentTypeLine.substr(colonPos + 1);
				size_t firstNonSpace = contentType.find_first_not_of(" \t");
				size_t lastNonSpace = contentType.find_last_not_of(" \t");
				if (firstNonSpace != std::string::npos && lastNonSpace != std::string::npos)
				{
					contentType = contentType.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
					return (contentType);
				}
			}
		}
	}
	return ("");
}

std::string extractContentLength(std::string &request, std::string postData)
{
	std::string contentLength;
	size_t contentLengthPos = request.find("Content-Length: ");

	if (contentLengthPos != std::string::npos)
	{
		size_t lineEnd = request.find("\r\n", contentLengthPos);
		if (lineEnd != std::string::npos)
			contentLength = request.substr(contentLengthPos + 16, lineEnd - contentLengthPos - 16);
		else
			return ("");
	}
	else
	{
		std::stringstream ss;
		ss << postData.size();
		contentLength = ss.str();
	}
	return (contentLength);
}

static std::string	extract_text(std::string &request)
{
	size_t headerEnd = request.find("\r\n\r\n");
	if (headerEnd != std::string::npos)
	{
		std::string postData = request.substr(headerEnd + 4);  // 4 caractères pour sauter "\r\n\r\n"
		return (postData);
	}
	return ("");
}

static std::string	extract_file(std::string &request, std::string &boundary)
{
	if (boundary == "")
		return ("");
	std::string	delimiter = "--" + boundary;
	size_t delimiterPos = request.find(delimiter);
	if (delimiterPos != std::string::npos)
	{
		std::string withoutheader = request.substr(delimiterPos , request.size() - delimiterPos); 
		size_t	newdelimiterPos = withoutheader.find(delimiter + "--");
		if (newdelimiterPos != std::string::npos)
		{
			size_t	newwdelimiterPos = withoutheader.find("\r\n\r\n", newdelimiterPos);
			return (withoutheader.substr(0, newwdelimiterPos));
		}
		// else
		// {
		// 	withoutheader += "\r\n" + delimiter + "--";
		// 	return (withoutheader);
		// }
	}
	return ("");
}

static std::string	extract_boundary(std::string &type)
{
	size_t boundaryPos = type.find("boundary=");
	if (boundaryPos != std::string::npos)
	{
		boundaryPos += 9;
		return (type.substr(boundaryPos, type.size() - boundaryPos));
	}
	return ("");
}

std::string extractPostData(std::string &request, std::string &type)
{
	if (type == "application/x-www-form-urlencoded")
	{
		return (extract_text(request));
	}
	else if (type.find("multipart/form-data") != std::string::npos)
	{
		std::string	boundary = extract_boundary(type);
		return (extract_file(request, boundary));
	}
	return "";
}

std::string extractQueryString(std::string &request)
{
	std::string firstLine;
	std::istringstream iss(request);
	std::getline(iss, firstLine);
	size_t firstQuestionMark = firstLine.find('?');
	if (firstQuestionMark != std::string::npos)
	{
		size_t nextSpacePos = firstLine.find(' ', firstQuestionMark + 1);
		if (nextSpacePos != std::string::npos)
			return (firstLine.substr(firstQuestionMark + 1, nextSpacePos - firstQuestionMark - 1));
	}
	return ("-1");
}

std::string	find_cgi_path(std::string path, std::string ext)
{
	size_t	copysize;
	size_t	find_ext = path.find(ext);
	if (find_ext != std::string::npos)
	{
		size_t	find_start = path.rfind(",", find_ext);
		if (find_start != std::string::npos)
			find_start += 1;
		else
			find_start = 0;
		size_t	find_end = path.find(",", find_ext);
		if (find_end != std::string::npos)
			copysize = find_end - find_start;
		else
			copysize = path.size() - find_start;
		return (path.substr(find_start, copysize));
	}
	return("");
}