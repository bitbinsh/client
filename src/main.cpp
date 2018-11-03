#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "actions.h"

using namespace boost::program_options;

using namespace std;

void print_help(options_description &desc, basic_ostream<char> &s) {
    s << "bitbin is bitbin's client." << endl <<
         "" << endl <<
         "Usage: bitbin [options] <file/url>" << endl;
    s << desc << endl;
}

bool parse_input_uri(vector<string> &m, unique_ptr<network::uri> &uri) {
    auto firstArg = m[0];
    try {
        uri = make_unique<network::uri>(firstArg);
        return true;
    } catch (network::uri_syntax_error&) {
        return false;
    }
}

int main(int argc, char** argv) {
    variables_map vm;

    options_description desc;

    desc.add_options()
            ("help,h", "help message")
            ("input", value<vector<string>>()->multitoken()->required(), "input, whether a url or local file path")
            ("endpoint", value<string>()->default_value("https://bitbin.sh"), "http endpoint")
            ("upload,u", value<bool>()->default_value(false)->implicit_value(true)->zero_tokens(), "force bitbin to upload the arguments")
            ("download,d", value<bool>()->default_value(false)->implicit_value(true)->zero_tokens(), "force bitbin to download the argument")
            ;

    map<string, string> env_map {
        pair<string, string>(string("BITBIN_ENDPOINT"), string("endpoint"))
    };

    positional_options_description pd;
    pd.add("input", -1);

    try {
        store(command_line_parser(argc, argv)
                      .options(desc).positional(pd).run(), vm);

        store(parse_environment(desc, [env_map](string env_var) -> string {
            auto p = env_map.find(env_var);
            if (p == env_map.end())
                return "";
            else
                return p->second;
        }), vm);

        notify(vm);
    } catch (boost::program_options::error &o) {
        cerr << o.what() << endl;
        print_help(desc, cerr);
        return 1;
    }

    if (vm.count("help")) {
        print_help(desc, cout);
        return 0;
    }

    // bitbin can be invoked in two separate ways - either to upload or download. lets figure out which one it is
    bool upload = vm["upload"].as<bool>();
    bool download = vm["download"].as<bool>();

    auto input = vm["input"].as<vector<string>>();

    action todo = nullptr;
    unique_ptr<network::uri> uri;

    if (upload && download) {
        cerr << "Both upload and download are specified, desired action is ambiguous" << endl;
        return 1;
    } else if (upload) {
        todo = &do_upload;
    } else if (download) {
        // so in order to 'do' a download, we should be able to parse the URI
        if (parse_input_uri(input, uri)) {
            todo = &do_download;
        } else {
            cerr << "Download flag set, yet the argument doesn't look like a URI..." << endl;
            return 1;
        }
    } else if (input.size() > 1) {
        todo = &do_upload;
    } else if (parse_input_uri(input, uri)) {
        // we parsed it! looks like a URI, we should download it.
        todo = &do_download;
    } else {
        todo = &do_upload; // I guess we need to upload!
    }

    return todo(vm, move(uri));
}

