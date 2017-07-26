//
// Created by root on 17-7-26.
//

#include "SimpleSMTP.hpp"

int main(int argc, char **argv) {
	if (argc < 6) {
		printf("Usage: test smtps://smtp.gmail.com reimu r31mu000 reimu@gensokyo.moe marisa@gensokyo.moe\n");
		exit(1);
	}

	Reimu::SimpleSMTP ssm;
	ssm.Flag = Reimu::SimpleSMTP::Flags::Verbose;
	ssm.SMTP_Server = argv[1];
	ssm.SMTP_User = argv[2];
	ssm.SMTP_Password = argv[3];
	ssm.Sender = argv[4];

	ssm.Subject = "Reimu SimpleSMTP Test";
	ssm.Recipients.push_back(argv[5]);
	ssm.Body = "蛤";

	return ssm.Send();
}