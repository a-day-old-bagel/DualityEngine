/* 
 * File:   FileIO.h
 * Author: adayoldbagel
 *
 * Created on February 18, 2015, 4:25 PM
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include <string>
#include <fstream>

bool ReadFile(const char* pFileName, std::string& outFile);

#endif	/* FILEIO_H */