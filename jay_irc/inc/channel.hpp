//
// Created by jaewkim on 2022/09/26.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

#include "./main.h"

class Client;

enum ch_opt
{
    opt_o = 1 << 0,
    opt_k = 1 << 1,
    opt_err = 1 << 7
};

class Channel
{
  private:
    std::string __name;
    std::string __topic;
    std::set<Client *> __operator_list;
    std::set<Client *> __active_clients;
	Client &__hostClient;
    unsigned char __mode;
    std::string __key;

    Channel();
    Channel &operator=(const Channel &rhs);

  public:
    // Member functions
    Channel(const std::string &name, Client *client);
    Channel(const std::string &name);
    ~Channel();

    // Element access
    const std::string &getName() const;
    const std::set<Client *> &getActiveClients() const;

    // Modifiers
    void setMode(unsigned char mode);

    void addClient(Client *client);
    void eraseClient(Client *client);

    // Lookups
    Client      *findClient(std::string nick);
    Client      *findClient(Client *client);
    bool        findOperator(const Client &client) const;
	std::string	listingActiveClient(Client &client) const;

    // Identifiers
    bool isClientInChannel(const Client &client);

    void addOperator(Client *client);
    void delOperator(Client *client);
	const std::string	getTopic();
	void	setTopic(std::string topic);
};

#endif // FT_IRC_CHANNEL_HPP
