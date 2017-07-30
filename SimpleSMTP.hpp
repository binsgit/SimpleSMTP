/*
    This file is part of SimpleSMTP.
    Copyright (C) 2017  CloudyReimu <cloudyreimu@gmail.com>

    SimpleSMTP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SimpleSMTP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SimpleSMTP.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
    I despise sexually promiscuous people. If such people uses my software
    or `borrows' my idea, they'll be ridiculed by me. Lol.
*/

#ifndef LIBREIMU_SIMPLESMTP_HPP
#define LIBREIMU_SIMPLESMTP_HPP

#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <vector>
#include <string>
#include <utility>
#include <exception>
#include <stdexcept>

#include <curl/curl.h>

namespace Reimu {

    class SimpleSMTP {
    private:
	struct ContentReaderCtx {
		bool verbose = 0;
		size_t read_pos = 0;
		std::string *data = NULL;
	};

	static size_t ContentReaderCallback(void *output, size_t output_size, size_t output_nmemb, void *userp);

    public:
	enum Flags {
	    Verbose = 0x01, UseCustomDate = 0x02
	};

	Flags Flag;

	std::string SMTP_Server;

	std::string SMTP_User;
	std::string SMTP_Password;

	time_t Date = 0;
	int16_t TimeZone = 0;

	// Note: You need to escape non-ASCII chars by yourself. e.g. =?UTF-8?B?5Za15Za15Za1?=
	std::string Sender;
	std::vector<std::string> Recipients;
	std::string ContentType = "text/html; charset=utf-8";
	std::string Subject;
	std::vector<std::string> MiscHeaders;
	std::string Body;

	SimpleSMTP();
	~SimpleSMTP();

	SimpleSMTP(const SimpleSMTP &other);
	SimpleSMTP(const SimpleSMTP &&other);
	SimpleSMTP& operator= (SimpleSMTP other);

	int Send();
    };

}

#endif