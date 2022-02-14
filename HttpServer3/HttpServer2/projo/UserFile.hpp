#pragma once
#include <string>
#include <algorithm>

class UserFile {

public:
	UserFile() {};

	UserFile(const std::string& fcode):fileCode(fcode){}

	UserFile(int ID, long long size, const std::string& code, const std::string& name, const std::string& path, const std::string& time) :
		fileID(ID), fileSize(size), fileCode(code), fileName(name), filePath(path), uploadTime(time) {}

	UserFile(long long size, const std::string& code, const std::string& name, const std::string& path) :
		fileSize(size), fileCode(code), fileName(name), filePath(path){}

	~UserFile() {}

public:
	int getFileID() { return fileID; }
	long long getFileSize()const { return fileSize; }
	std::string getFileCode()const { return fileCode; }
	std::string getFileName()const { return fileName; }
	std::string getFilePath()const { return filePath; }
	std::string getUploadTime()const { return uploadTime; }

public:
	void setFileID(int ID) { this->fileID = ID; }
	void setFileSize(long long size) { this->fileSize = size; }
	void setFileCode(const std::string& code) { this->fileCode = code; }
	void setFileName(const std::string& name) { this->fileName = name; }
	void setFilePath(const std::string& path) { this->filePath = path; }
	void setUploadTime(const std::string& time) { this->uploadTime = time; }

public:
	std::string toString(){
		using std::to_string;
		std::string mess="fileID:"+to_string(fileID)+",";
		mess+="fileSize:"+to_string(fileSize)+",";
		mess+="fileCode:"+fileCode+",";
		mess+="fileName:"+fileName+",";
		mess+="filePath:"+filePath+",";
		mess+="uploadTime:"+uploadTime;
		return mess;
	}

private:
	int fileID=-1;
	long long fileSize;
	std::string fileCode;
	std::string fileName;
	std::string filePath;
	std::string uploadTime;
};