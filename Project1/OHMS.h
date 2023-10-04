/*
*    PseuArknights
*
*    Copyright (C) 2023  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string>

namespace OHMS {
	inline int filescanf(const std::string& filepath, const char* const format, ...) {
		FILE* fp{ 0 };
		fopen_s(&fp, filepath.c_str(), "r");
		if (!fp)
			return 0;
		int result{ 0 };
		va_list arglist;
		va_start(arglist, format);
		result = vfscanf_s(fp, format, arglist);
		va_end(arglist);
		return result;
	}

	static std::string getRandOperator(unsigned int maxN, unsigned int nN) {
		unsigned int res = rand() % maxN;
		char ch[8];
		ch[nN] = '\0';
		for (int i = nN - 1; i >= 0; i--) {
			ch[i] = res % 10 + '0';
			res /= 10;
		}
		std::string str;
		str.assign(ch);
		return str;
	}

	static std::string getOperatorStr(unsigned int nN) {
		char ch[8];
		ch[4] = '\0';
		for (int i = 3; i >= 0; i--) {
			ch[i] = nN % 10 + '0';
			nN /= 10;
		}
		std::string str;
		str.assign(ch);
		return str;
	}
}
