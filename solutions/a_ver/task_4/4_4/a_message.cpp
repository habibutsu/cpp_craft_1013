#include "a_message.h"

#include <stdexcept>
#include <sstream>

task4_4::a_message::a_message( std::istream& inp )
{
	inp.read( content_, content_size );
	if ( inp.eof() )
		throw std::logic_error("bad input stream, a_message cannot be readed");
}

task4_4::message_ptr task4_4::a_message::create_message( std::istream& inp )
{
	return message_ptr( new a_message( inp ) );
}

task4_4::a_message::~a_message()
{
}
//
const char task4_4::a_message::type() const
{
	return 'A';
}
const std::string task4_4::a_message::str() const
{
    std::stringstream stream_str;

    stream_str << "a_message";
    stream_str << "(";
    stream_str.write(content_, content_size);
    stream_str << ")";

	return stream_str.str();
}
