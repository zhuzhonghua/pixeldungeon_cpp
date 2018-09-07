#include "glog.h"
#include "util.h"

#include <stdarg.h>

const std::string GLog::TAG = "GAME";

const std::string GLog::POSITIVE = "++ ";
const std::string GLog::NEGATIVE = "-- ";
const std::string GLog::WARNING = "** ";
const std::string GLog::HIGHLIGHT = "@@ ";

Signal<std::string> GLog::update;

void GLog::i(const std::string& text, ...)
{
	va_list args;
	va_start(args, text);
	_i(text, args);
	va_end(args);
}

void GLog::p(const std::string& text, ...)
{
	va_list args;
	va_start(args, text);
	_i(POSITIVE + text, args);
	va_end(args);
}
void GLog::n(const std::string& text, ...)
{
	va_list args;
	va_start(args, text);
	_i(NEGATIVE + text, args);
	va_end(args);
}
void GLog::w(const std::string& text, ...)
{
	va_list args;
	va_start(args, text);
	_i(WARNING + text, args);
	va_end(args);
}
void GLog::h(const std::string& text, ...)
{
	va_list args;
	va_start(args, text);
	_i(HIGHLIGHT + text, args);
	va_end(args);
}

void GLog::_i(const std::string& text, va_list args)
{
	char buf[256] = { 0 };

	vsprintf(buf, text.c_str(), args);

	std::string sbuf(buf);
	update.dispatch(&sbuf);
}