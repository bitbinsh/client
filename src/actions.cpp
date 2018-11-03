#include <iostream>

#include "actions.h"

using namespace std;
using namespace boost::program_options;

int do_upload(variables_map &m, std::unique_ptr<URI> uri) {
    cout << "Upload" << endl;
    return 0;
}

int do_download(variables_map &m, std::unique_ptr<URI> uri) {
    cout << "Download" << endl;
    return 0;
}
