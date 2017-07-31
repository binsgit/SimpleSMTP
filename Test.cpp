//
// Created by root on 17-7-26.
//

#include "SimpleSMTP.hpp"

#include <string>

int debug(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr){

	std::string parsed_data;


	const char loghdr[] = "libReimu::SimpleSMTP: ";

	switch (type) {
		case CURLINFO_TEXT:
			parsed_data = std::string(data, data+size);
			fprintf(stderr, "%s%p: %s", loghdr, userptr, parsed_data.c_str());
			break;
		case CURLINFO_HEADER_IN:
			parsed_data = std::string(data, data+size);
			fprintf(stderr, "%s%p: <- %s", loghdr, userptr, parsed_data.c_str());
			break;
		case CURLINFO_HEADER_OUT:
			parsed_data = std::string(data, data+size);
			fprintf(stderr, "%s%p: -> %s", loghdr, userptr, parsed_data.c_str());
			break;
		case CURLINFO_DATA_OUT:
//			fprintf(stderr, "%s%p: Sent %zu bytes data\n", loghdr, userptr, size);
			break;
		case CURLINFO_SSL_DATA_OUT:
//			fprintf(stderr, "%s%p: Sent %zu bytes SSL data\n", loghdr, userptr, size);
			break;
		case CURLINFO_DATA_IN:
//			fprintf(stderr, "%s%p: Received %zu bytes data\n", loghdr, userptr, size);
			break;
		case CURLINFO_SSL_DATA_IN:
//			fprintf(stderr, "%s%p: Received %zu bytes SSL data\n", loghdr, userptr, size);
			break;
		default:
			fprintf(stderr, "%s%p: 喵喵喵？？ %d %p %zu\n", loghdr, userptr, type, data, size);
			break;
	}

	return 0;
}


int main(int argc, char **argv) {
	if (argc < 6) {
		printf("Usage: test smtps://smtp.gmail.com reimu r31mu000 reimu@gensokyo.moe marisa@gensokyo.moe\n");
		exit(1);
	}

	Reimu::SimpleSMTP ssm;
	ssm.Flag = Reimu::SimpleSMTP::Flags::Verbose;
	ssm.DebugCallback = &debug;
	ssm.SMTP_Server = argv[1];
	ssm.SMTP_User = argv[2];
	ssm.SMTP_Password = argv[3];
	ssm.Sender = argv[4];

	ssm.Subject = "Reimu SimpleSMTP Test";
	ssm.Recipients.push_back(argv[5]);
	ssm.Body = "蛤";

	return ssm.Send();
}