#pragma once

#ifndef COMPARE_H
#define COMPARE_H

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include "Diff.h"  

std::vector<std::string>read(const std::string& name);
void compare (const std::string& Datei,const std::string& Datei2);

#endif // DEBUG
