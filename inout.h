std::string read_file(const char* path) {
	auto len = std::filesystem::file_size(path);
	std::string buf(len,'\0');	
	std::ifstream input(path);	
	input.read(&buf[0],len);
	return buf;
}
