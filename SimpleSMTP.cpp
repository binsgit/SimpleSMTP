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

#include "SimpleSMTP.hpp"

Reimu::SimpleSMTP::SimpleSMTP() {

}

Reimu::SimpleSMTP::~SimpleSMTP() {

}

Reimu::SimpleSMTP::SimpleSMTP(const Reimu::SimpleSMTP &other) {
	Flag = other.Flag;
	SMTP_Server = other.SMTP_Server;
	SMTP_User = other.SMTP_User;
	SMTP_Password = other.SMTP_Password;
	Date = other.Date;
	Sender = other.Sender;
	Recipients = other.Recipients;
	Subject = other.Subject;
	MiscHeaders = other.MiscHeaders;
	Body = other.Body;
}

Reimu::SimpleSMTP::SimpleSMTP(const Reimu::SimpleSMTP &&other) {
	Flag = std::move(other.Flag);
	SMTP_Server = std::move(other.SMTP_Server);
	SMTP_User = std::move(other.SMTP_User);
	SMTP_Password = std::move(other.SMTP_Password);
	Date = std::move(other.Date);
	Sender = std::move(other.Sender);
	Recipients = std::move(other.Recipients);
	Subject = std::move(other.Subject);
	MiscHeaders = std::move(other.MiscHeaders);
	Body = std::move(other.Body);
}

Reimu::SimpleSMTP &Reimu::SimpleSMTP::operator=(Reimu::SimpleSMTP other) {
	Flag = other.Flag;
	SMTP_Server = other.SMTP_Server;
	SMTP_User = other.SMTP_User;
	SMTP_Password = other.SMTP_Password;
	Date = other.Date;
	Sender = other.Sender;
	Recipients = other.Recipients;
	Subject = other.Subject;
	MiscHeaders = other.MiscHeaders;
	Body = other.Body;

	return *this;
}

size_t Reimu::SimpleSMTP::ContentReaderCallback(void *output, size_t output_size, size_t output_nmemb, void *userp) {
	struct ContentReaderCtx *ctx = (struct ContentReaderCtx *)userp;

	if ((output_size == 0) || (output_nmemb == 0) || ((output_size*output_nmemb) < 1)) {
		return 0;
	}

	size_t leftsize = ctx->data->size() - ctx->read_pos;

	if (leftsize == 0)
		return 0;

	size_t writesize;

	if (leftsize < output_size)
		writesize = leftsize;
	else
		writesize = output_size;

	if (ctx->verbose) {
		fprintf(stderr, "SimpleSMTP::ContentReaderCallback: Writing %zu bytes to %p\n", writesize, output);
	}

	memcpy(output, ctx->data->c_str()+ctx->read_pos, writesize);

	ctx->read_pos += writesize;

	return writesize;
}


int Reimu::SimpleSMTP::Send() {
	if (SMTP_Server.empty())
		throw std::logic_error("SMTP server not specified");

	if (Recipients.empty())
		throw std::logic_error("Recipients not specified");

	if (!(Flag & Flags::UseCustomDate))
		Date = time(NULL);

	CURL *curl_ctx = NULL;
	CURLcode curl_rc = CURLE_OK;
	struct tm parsed_time;
	char datebuf[48];

	curl_ctx = curl_easy_init();

	if (!curl_ctx)
		throw std::runtime_error("Failed to initialize curl context");


	curl_easy_setopt(curl_ctx, CURLOPT_URL, SMTP_Server.c_str());
	curl_easy_setopt(curl_ctx, CURLOPT_USERNAME, SMTP_User.c_str());
	curl_easy_setopt(curl_ctx, CURLOPT_PASSWORD, SMTP_Password.c_str());
	curl_easy_setopt(curl_ctx, CURLOPT_MAIL_FROM, Sender.c_str());

	gmtime_r(&Date, &parsed_time);
	strftime(datebuf, 46, "%a, %d %b %Y %X", &parsed_time);

	std::string RawData;

	RawData += "Date: " + std::string(datebuf) + " +0000\r\n";
	RawData += "To: ";

	struct curl_slist *recipients = NULL;
	for (auto &rcpt : Recipients) {
		recipients = curl_slist_append(recipients, rcpt.c_str());
		RawData += rcpt + ",";
	}
	RawData.pop_back();
	curl_easy_setopt(curl_ctx, CURLOPT_MAIL_RCPT, recipients);

	RawData += "\r\nFrom: ";
	if (SenderName.size())
		RawData += SenderName + " <" + Sender + ">";
	else
		RawData += Sender;

	RawData += "\r\nContent-Type: " + ContentType + "\r\nSubject: " + Subject + "\r\n";

	for (auto &hdr : MiscHeaders) {
		RawData += hdr + "\r\n";
	}

	RawData += "\r\n\r\n" + Body + "\r\n\r\n";

	struct ContentReaderCtx ctx;
	ctx.data = &RawData;
	ctx.read_pos = 0;
	ctx.verbose = 0;

	curl_easy_setopt(curl_ctx, CURLOPT_READFUNCTION, ContentReaderCallback);
	curl_easy_setopt(curl_ctx, CURLOPT_READDATA, &ctx);
	curl_easy_setopt(curl_ctx, CURLOPT_UPLOAD, 1L);

	if (Flag & Flags::Verbose) {
		fprintf(stderr, "Reimu::SimpleSMTP:::Send: Raw data:\n%s\n", RawData.c_str());
		curl_easy_setopt(curl_ctx, CURLOPT_VERBOSE, 1L);
		ctx.verbose = 1;
	}

	curl_rc = curl_easy_perform(curl_ctx);

	if (curl_rc != CURLE_OK)
		throw std::runtime_error("curl_easy_perform() failed: " + std::string(curl_easy_strerror(curl_rc)));

	curl_slist_free_all(recipients);

	curl_easy_cleanup(curl_ctx);

	return curl_rc;
}

