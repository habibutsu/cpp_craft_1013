#include "b_message.h"

#include <stdexcept>
#include <sstream>


task4_4::b_message::b_message( std::istream& inp )
{
	inp >> length_;
    if ( inp.eof() )
        throw std::logic_error("bad input stream");
    inp.ignore(1);

	content_ = new char[ length_ ];
	inp.read( content_, length_ );

    if (inp.fail())
      throw std::logic_error("could be read all");
}

task4_4::message_ptr task4_4::b_message::create_message( std::istream& inp )
{
	return message_ptr( new b_message( inp ) );
}

task4_4::b_message::~b_message()
{
	delete [] content_;
}

const char task4_4::b_message::type() const
{
	return 'B';
}
const std::string task4_4::b_message::str() const
{
    std::stringstream stream_str;

    stream_str << "b_message";
    stream_str << "(";
    stream_str << length_ << "|";
    stream_str.write(content_, length_);
    stream_str << ")";

	return stream_str.str();
}
