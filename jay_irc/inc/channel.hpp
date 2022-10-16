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
    // std::vector<std::string &> __banned;
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
    void setKey(std::string const &key);

    void addClient(Client *client);
    void eraseClient(Client *client);
    //void addBanned(const std::string &nick);
    //void eraseBanned(const std::string &nick);

    // Lookups
    Client      *findClient(std::string nick);
    Client      *findClient(Client *client);
    bool        findOperator(const Client &client) const;
	std::string	listingActiveClient(Client &client) const;

    // Identifiers
    bool isClientInChannel(const Client &client);
    // bool isBanned(const std::string &nick);

    // Others
    //unsigned char get_permissions(const std::string &nick);

    //void setPermissions(Client *client, unsigned char perm);

    //void printChannel();
    // std::string	sendUserList(std::string serverip, std::string nick);

    void addOperator(Client *client);
    void delOperator(Client *client);
	const std::string	getTopic();
	void	setTopic(std::string topic);
};

#endif // FT_IRC_CHANNEL_HPP
