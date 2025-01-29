#include "bot.hpp"


Bot::Bot()
{
        _toCensor["fuck"] = "fork";
        _toCensor["shit"] = "sheet";
        _toCensor["bastard"] = "basket";
        _toCensor["asshole"] = "ashhole";
        _toCensor["prick"] = "brick";
        _toCensor["retard"] = "restart";
        _toCensor["scumbag"] = "scam-bag";
        _toCensor["jackass"] = "jacket";
        _toCensor["dipshit"] = "dipstick";


        _toKick.insert("communist party");
        _toKick.insert("chinese government");
        _toKick.insert("xi jinping");
        _toKick.insert("human rights");
        _toKick.insert("freedom of speech");
        _toKick.insert("winnie the pooh");
        _toKick.insert("press freedom");
}

std::string		Bot::censor( std::vector<std::string> &cmds ){
	std::map<std::string, std::string>::iterator	it;
	std::string										respond = "";
	
	if (_toCensor.find(cmds[2].substr(1)) != _toCensor.end())
		respond += " :" + _toCensor[cmds[2].substr(1)];
	else
		respond += " " + cmds[2];
	
	for (size_t i = 3; i < cmds.size(); i++){
		if (_toCensor.find(cmds[i]) != _toCensor.end())
			respond += " " + _toCensor[cmds[i]];
		else
			respond += " " + cmds[i];
	}
	return respond;
}

bool	Bot::shallKick( std::string& message ){
	std::set<std::string>::iterator	it;

	for (it = _toKick.begin(); it != _toKick.end(); ++it){
		if (message.find(*it) != std::string::npos)
			return true;
	}
	return false;
}

Bot::~Bot( )
{
}
