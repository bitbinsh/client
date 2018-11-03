//
// Created by joe on 11/2/18.
//

#ifndef CLIENT_ACTIONS_H
#define CLIENT_ACTIONS_H

#include <boost/program_options.hpp>
#include "network/uri.hpp"

using URI = network::uri;

// variables map, pre-parsed url for downloads
using action = int (*) (boost::program_options::variables_map&, std::unique_ptr<URI>);

int do_upload(boost::program_options::variables_map &, std::unique_ptr<URI>);
int do_download(boost::program_options::variables_map &, std::unique_ptr<URI>);

#endif //CLIENT_ACTIONS_H
