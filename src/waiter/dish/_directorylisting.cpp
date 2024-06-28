/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _directorylisting.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 19:37:30 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/06/28 19:37:30 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "customer.hpp"
#include "dish.hpp"
#include <dirent.h>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>

std::string formatFileSize(off_t size) {
	std::ostringstream oss;
	if (size >= 1024 * 1024) {
		oss << std::fixed << std::setprecision(1) << static_cast<double>(size) / (1024 * 1024) << "M";
	} else if (size >= 1024) {
		oss << std::fixed << std::setprecision(1) << static_cast<double>(size) / 1024 << "K";
	} else {
		oss << size << "B";
	}
	return oss.str();
}

// Function to format the file modification time
std::string formatModTime(time_t modTime) {
	struct tm tm;
	localtime_r(&modTime, &tm);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &tm);
	return buffer;
}

// Function to create the HTML string for directory listing
std::string Dish::_generateDirectoryListing() {
	std::ostringstream html;
	html << "<html>\n<head>\n<title>Index of " << this->finalPage.substr(this->recipe.root.size()) << "</title>\n";
	html << "<style>\n";
	html << "table { width: 100%; border-collapse: collapse; }\n";
	html << "th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }\n";
	html << "th { background-color: #f2f2f2; }\n";
	html << "</style>\n";
	html << "</head>\n<body>\n";
	html << "<h1>Index of " << this->finalPage.substr(this->recipe.root.size()) << "</h1>\n";
	html << "<table>\n<tr><th>Name</th><th>Last Modified</th><th>Size</th></tr>\n";


	DIR* dir = opendir(this->finalPage.c_str());
	if (dir == nullptr) {
		this->_handleFileError(errno);
		return "";
	}
	struct dirent* entry;
	errno = 0;
	while ((entry = readdir(dir)) != nullptr) {
		std::string name = entry->d_name;
		if (name == ".") continue;

		std::string fullPath = this->finalPage + name;
		struct stat fileStat;
		if (stat(fullPath.c_str(), &fileStat) == -1) {
			this->_handleFileError(errno);
			closedir(dir);
			return "";
		}

		html << "<tr>";
		html << "<td><a href=\"" << this->finalPage.substr(this->recipe.root.size()) + name << "\">" << name << "</a></td>";
		html << "<td>" << formatModTime(fileStat.st_mtime) << "</td>";
		html << "<td>" << (S_ISDIR(fileStat.st_mode) ? "-" : formatFileSize(fileStat.st_size)) << "</td>";
		html << "</tr>\n";
	}
	if (errno != 0) {
		this->status.updateState(INTERNALERR);
		closedir(dir);
		return "";
	}
	closedir(dir);

	html << "</table>\n</body>\n</html>\n";
	return html.str();
}

void Dish::_autoindex() {
	std::string html = this->_generateDirectoryListing();
	if (this->status.getState() != OK) {
		return ;
	}
	this->body = html;
	this->customer.eat();
}
