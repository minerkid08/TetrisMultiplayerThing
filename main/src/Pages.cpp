#include "Pages.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> pages = {};

bool hasPage(std::string name){
  return pages.find(name) != pages.end();
}

std::string getPage(std::string name){
  return pages[name];
}

void loadPages(){
	for(std::filesystem::path path : std::filesystem::directory_iterator("pages")){
		std::ifstream ifstream(path);
		std::stringstream sstream;
		sstream << ifstream.rdbuf();
		std::string ext = path.extension().string();
		std::string filename = path.filename().string();
		pages[filename.substr(0, filename.size() - ext.size())] = sstream.str();
	}
}

