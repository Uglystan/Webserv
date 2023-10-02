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

std::string extractContentLength(std::string &request)
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
	return (contentLength);
}

std::string extractPostData(std::string &request)
{
	size_t headerEnd = request.find("\r\n\r\n");

	if (headerEnd != std::string::npos)
	{
		std::string postData = request.substr(headerEnd + 4);  // 4 caractères pour sauter "\r\n\r\n"
		return (postData);
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
	return ("");
}
