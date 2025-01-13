#ifndef BOT_HPP
# define BOT_HPP

# include "ft_irc.h"

class Bot
{
private:
	std::map<std::string, std::string>	_toCensor;
	std::set<std::string>				_toKick;

public:
	Bot();
	~Bot();

	std::string	censor( std::vector<std::string> &cmds );
	bool		shallKick( std::string& message );
};



#endif